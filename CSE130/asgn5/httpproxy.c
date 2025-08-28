

#include "client_socket.h"
#include "iowrapper.h"
#include "listener_socket.h"
#include "prequest.h"
#include "a5protocol.h"

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/stat.h>
#include <regex.h>

#include "Cache.h"

void handle_connection(uintptr_t connfd, Cache C);

// globals:
Listener_Socket_t *sock = NULL;
Cache C = NULL;

void usage(FILE *stream, char *exec) {
    fprintf(stream, "usage: %s <port> \n", exec);
}

void handle_signal(int sig) {
    fprintf(stderr, "sig: %d\n", sig);
    freeCache(&C);
    ls_delete(&sock);
    exit(0);
}


int main(int argc, char **argv) {
    signal(SIGINT, handle_signal);
    if (argc < 2) {
        usage(stderr, argv[0]); 
        return EXIT_FAILURE;
    }

    // parse port number
    char *endptr = NULL;
    int port = (size_t) strtoull(argv[optind], &endptr, 10);
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[optind]);
        return EXIT_FAILURE;
    }

    if (port < 0 || port > 65535) {
        warnx("Invalid port number (not in range [0, 65535]): %s", argv[1]);
        return EXIT_FAILURE;
    }

    // Parse mode
    char *mode = argv[2];
    if (strcmp(mode, "FIFO") != 0 && strcmp(mode, "LRU") != 0) {
        warnx("Invalid mode: %s. Expected 'FIFO' or 'LRU'.", mode);
        return EXIT_FAILURE;
    }

    // Reset endptr (Good practice but not required)
    endptr = NULL;
    int n = (size_t) strtoull(argv[3], &endptr, 10);
    if (endptr && *endptr != '\0') {
        warnx("Invalid number (not entered or contains invalid characters): %s", argv[3]);
        return EXIT_FAILURE;
    }

    if (n < 0 || n > 1024) {
        warnx("Invalid cache size (not in range [0, 1024]): %s", argv[3]);
        return EXIT_FAILURE;
    }

    // fprintf(stderr, "Port: %d, Mode: %s, N: %d\n", port, mode, n);
    C = newCache(mode, n);
    sock = ls_new(port);
    int i = 0;

    if (sock) {
        while (1) {
            fprintf(stderr, "Iteration: %d\n", i);
            uintptr_t connfd = ls_accept(sock);
            assert(connfd > 0);
            handle_connection(connfd, C);
            i += 1;
        }
        freeCache(&C);
        ls_delete(&sock);
    }

    return EXIT_SUCCESS;
}

void handle_connection(uintptr_t connfd, Cache C) {

    // parse the proxy request from a conn fd.
    Prequest_t *preq = prequest_new(connfd);
    if (preq) {
        char *host = NULL, *uri = NULL;
        int32_t port;
        // fprintf(stderr, "connection for %s", prequest_str(preq));

        host = prequest_get_host(preq);
        uri = prequest_get_uri(preq);
        port = prequest_get_port(preq);
        fprintf(stderr, "host=%s, port=%d, uri=%s\n", host, port, uri);

        // prepare to make a connection to a server using a client socket
        int32_t client_sock = cs_new(host, port);

        if (client_sock < 0) {
            fprintf(stderr, "cannot connect to host %s\n", host);
            goto out;
        }

        // TODO: send GET request use write_n_bytes()
        // "GET /%s HTTP/1.1\r\nConnection: Close\r\n\r\n", uri

        char request[MAX_HEADER_LEN] = {0};
        char buffer[MAX_HEADER_LEN * 10] = {0};
        int reqLen;
        int bytesRead;
        char *response = NULL;
        int responseLen = 0;
        CacheNode N;

        if (getCacheSize(C) > 0){
            if (C == NULL || host == NULL || uri == NULL) {
                fprintf(stderr, "Cache, host, or URI is NULL. Skipping cache lookup.\n");
            } else {
                if ((N = searchCache(C, host, port, uri)) != NULL) {
                    fprintf(stderr, "Cache HIT!\n");
                    write_n_bytes(connfd, getResponse(N), getResponseLen(N));
                    close(client_sock);
                    goto out; // Cache hit, return early
                }
            }
        }
      
        // Send request to the server
        reqLen = snprintf(request, sizeof(request), "GET /%s HTTP/1.1\r\nConnection: Close\r\n\r\n", uri);
    
        write_n_bytes(client_sock, request, reqLen);
        
        while ((bytesRead = read(client_sock, buffer, sizeof(buffer))) > 0) {
            // Resize response buffer to accommodate new data
            char *new_response = realloc(response, responseLen + bytesRead);
            // fprintf(stderr, "buffer:\n%s", buffer);
            if (!new_response) {
                fprintf(stderr, "Memory allocation failed");
                free(response);
                response = NULL;
                goto out;
            }
            response = new_response;
        
            // Copy new data from buffer to response
            memcpy(response + responseLen, buffer, bytesRead);
            responseLen += bytesRead;
        
            // Forward to client
            ssize_t bytesWritten = write_n_bytes(connfd, buffer, bytesRead);
            if (bytesWritten < bytesRead) {
                fprintf(stderr, "Error forwarding to client\n");
                goto out;
            }
        }

        if (bytesRead < 0) {
            fprintf(stderr, "Error reading from server: %s\n", strerror(errno));
            goto out;
        }
        // fprintf(stderr, "responseLen: %d\n", responseLen);
        if (getCacheSize(C) > 0 && responseLen <= MAX_CACHE_ENTRY){
            pushCache(C, response, responseLen, strdup(host), port, strdup(uri));
            // fprintf(stderr, "cache length: %d\n", length(C));
            // fprintf(stderr, "\n\n\nCache:\n");
            // printCache(stderr, C);
        }
        else{
            free(response);
            response = NULL;
        }

        close(client_sock);

    } else {
        fprintf(stderr, "Bad request from %lu\n", connfd);
    }
out:
    if (connfd > 0) close(connfd);
    if (preq) prequest_delete(&preq);
}

