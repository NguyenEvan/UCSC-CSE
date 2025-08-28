// Asgn 4: A simple HTTP server.
// By:  Andrew Quinn

#include "listener_socket.h"
#include "connection.h"
#include "response.h"
#include "request.h"
#include "queue.h"
#include "rwlock.h"
#include "Dictionary.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <sys/stat.h>

rwlock_t* lock;
pthread_t thread;
sem_t threadCapacity;
hashtable* table;
int NUM_THREADS;
pthread_mutex_t table_lock = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t writing_lock = PTHREAD_MUTEX_INITIALIZER;

void handle_connection(int);
void handle_get(conn_t *);
void handle_put(conn_t *);
void handle_unsupported(conn_t *);
void *worker_function(void *arg);

int main(int argc, char **argv) {
    if (argc < 2) {
        warnx("wrong arguments: %s -t threads port_num", argv[0]);
        fprintf(stderr, "usage: %s -t <threads> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int thread_count = 4;  // Default value
    int opt;
    bool hasThreadOpt = false;
    
    // Parse optional `-t` argument
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't':
                thread_count = atoi(optarg);  // Convert to integer
                if (thread_count <= 0) {
                    fprintf(stderr, "Error: Invalid thread count '%s'\n", optarg);
                    return EXIT_FAILURE;
                }
                hasThreadOpt = true;
                break;
            // case '?':  // Unknown option
            //     fprintf(stderr, "usage: %s -t <threads> <port>\n", argv[0]);
            //     return EXIT_FAILURE;
        }
    }
    
    char *endptr = NULL;
    size_t port;
    if (hasThreadOpt){
        port = (size_t) strtoull(argv[3], &endptr, 10);
    }
    else{
        port = (size_t) strtoull(argv[1], &endptr, 10);
    }
    if (endptr && *endptr != '\0') {
        warnx("invalid port number: %s", argv[1]);
        return EXIT_FAILURE;
    }
    printf("port: %ld\n", port);
    printf("threads: %d\n", thread_count);
    NUM_THREADS = thread_count;
    signal(SIGPIPE, SIG_IGN);
    Listener_Socket_t *sock = ls_new(port);
    if (!sock) {
      warnx("cannot open socket");
      return EXIT_FAILURE;
    }
    table = new_hashtable(1000);
    queue_t *q = queue_new(100);
    lock = rwlock_new(N_WAY, thread_count);
    pthread_t threads[thread_count];
    sem_init(&threadCapacity, 0, thread_count);

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i], NULL, worker_function, (void *)q);
    }


    while (1) {
        int connfd = ls_accept(sock);
        printf("BEFORE CONNECTION FD: %d\n", connfd);
        intptr_t *conn_id = malloc(sizeof(intptr_t));
        *conn_id = connfd;

        queue_push(q, conn_id);
        // intptr_t conn_id = connfd;
        // void *arg = (void *) conn_id;

        // queue_push(q, arg);
        sem_wait(&threadCapacity);
        sem_post(&threadCapacity);  // ✅ Release a slot

    }
    ls_delete(&sock);

    return EXIT_SUCCESS;
}

void *worker_function(void *arg) {
    queue_t *queue = (queue_t *)arg;

    while (1) {
        intptr_t *connfd_ptr;
        queue_pop(queue, (void **)&connfd_ptr);
        // void *connfd_ptr;

        // queue_pop(queue, (void **)&connfd_ptr);  // ✅ Wait for a connection
        // intptr_t intptr = (intptr_t) connfd_ptr;
        // int connfd = (intptr_t) intptr;

        if (connfd_ptr == NULL) continue;  // Ignore invalid connections
        int connfd = (int)(*connfd_ptr);
        free(connfd_ptr);  // Free memory allocated in main loop

        // free(connfd_ptr);  // Free allocated memory for connection ID
        printf("AFtER CONNECTION FD: %d\n", connfd);
        handle_connection(connfd);  // ✅ Process the request

        close(connfd);  // ✅ Close the connection after handling
        
    }
    return NULL;
}

void handle_connection(int connfd) {

    conn_t *conn = conn_new(connfd);

    const Response_t *res = conn_parse(conn);

    if (res != NULL) {
        conn_send_response(conn, res);
    } else {
      printf("%s", conn_str(conn));
        const Request_t *req = conn_get_request(conn);
        if (req == &REQUEST_PUT) {
            handle_put(conn);
        }
        else if (req == &REQUEST_GET) {
            handle_get(conn);
        }
        else{
            res = &RESPONSE_NOT_IMPLEMENTED;
            conn_send_response(conn, res);
        }
	// add cases for other types of requests here
    }

    conn_delete(&conn);
}

void handle_get(conn_t *conn){
    // printf("HERE\n");
    char *uri = conn_get_uri(conn);
    // printf("Uri: %s\n", uri);
    pthread_mutex_lock(&table_lock);
    rwlock_t* retrieved_lock = lookup(table, uri);
    if (retrieved_lock == NULL){
        retrieved_lock = rwlock_new(N_WAY, NUM_THREADS);
        add_urilock(table, uri, retrieved_lock);
    }
    pthread_mutex_unlock(&table_lock);
    reader_lock(retrieved_lock);

    const Response_t *res = NULL;

    int fd = open(uri, O_RDWR);

    if (fd < 0) {
        // printf("%s: %d", uri, errno);
        if (errno == EACCES || errno == EISDIR) {
            res = &RESPONSE_FORBIDDEN;
            goto out;
        } else if (errno == ENOENT) {
            res = &RESPONSE_NOT_FOUND;
            goto out;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
            goto out;
        }
    }

    // char *requestId = conn_get_header(conn, "Request-Id");
    struct stat st;
    stat(uri, &st);
    int count = st.st_size;
    /* fd is valid */
    // send the file
    res = conn_send_file(conn, fd, count);     // conn --> fd, NULL return is good
    if (res == NULL){
        goto out2;
    }
    else{
        goto out;
    }
    // printf("Retrieved URI: %s\n", uri);
    // printf("Retrieved Request ID: %s\n", requestId);
out:
    conn_send_response(conn, res);
out2:
    fprintf(stderr, "GET,/%s,400,%s\n", uri, conn_get_header(conn, "Request-Id"));
    reader_unlock(retrieved_lock);

}

void handle_put(conn_t *conn) {
    char *uri = conn_get_uri(conn);
    const Response_t *res = NULL;
    char temp_path[256];
    bool existed = false;
    int fd = -1;
    rwlock_t* retrieved_lock = NULL;
    bool lock_acquired = false;
    
    // First get the lock for this URI
    pthread_mutex_lock(&table_lock);
    retrieved_lock = lookup(table, uri);
    if (retrieved_lock == NULL) {
        retrieved_lock = rwlock_new(N_WAY, NUM_THREADS);
        add_urilock(table, uri, retrieved_lock);
    }
    pthread_mutex_unlock(&table_lock);
    
    // Create temporary file
    snprintf(temp_path, sizeof(temp_path), "%s.tmp.XXXXXX", uri);
    fd = mkstemp(temp_path);
    
    if (fd < 0) {
        res = &RESPONSE_INTERNAL_SERVER_ERROR;
        goto send_response;
    }
    
    // Receive file content into temporary file
    res = conn_recv_file(conn, fd);
    close(fd);
    
    if (res != NULL) {
        unlink(temp_path);  // Clean up temp file on error
        goto send_response;
    }
    
    // Now acquire the write lock for the final rename operation
    writer_lock(retrieved_lock);
    lock_acquired = true;
    
    // Check if original file exists
    existed = access(uri, F_OK) == 0;
    
    // Atomic rename of temporary file to final destination
    if (rename(temp_path, uri) != 0) {
        if (errno == EACCES || errno == EISDIR) {
            res = &RESPONSE_FORBIDDEN;
        } else {
            res = &RESPONSE_INTERNAL_SERVER_ERROR;
        }
        unlink(temp_path);  // Clean up temp file on error
    } else {
        res = existed ? &RESPONSE_OK : &RESPONSE_CREATED;
    }

send_response:
    
    conn_send_response(conn, res);
    fprintf(stderr, "PUT,/%s,%d,%s\n", uri, 
            res == &RESPONSE_CREATED ? 201 :
            res == &RESPONSE_OK ? 200 :
            res == &RESPONSE_FORBIDDEN ? 403 :
            res == &RESPONSE_INTERNAL_SERVER_ERROR ? 500 : 400,
            conn_get_header(conn, "Request-Id"));
    if (lock_acquired) {
        writer_unlock(retrieved_lock);
    }
}

// // Asgn 4: A simple HTTP server.
// // By:  Andrew Quinn

// #include "listener_socket.h"
// #include "connection.h"
// #include "response.h"
// #include "request.h"

// #include <err.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <signal.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>

// #include <sys/stat.h>

// void handle_connection(int);

// void handle_get(conn_t *);
// void handle_put(conn_t *);
// void handle_unsupported(conn_t *);

// int main(int argc, char **argv) {
//     if (argc < 2) {
//         warnx("wrong arguments: %s port_num", argv[0]);
//         fprintf(stderr, "usage: %s <port>\n", argv[0]);
//         return EXIT_FAILURE;
//     }

//     char *endptr = NULL;
//     size_t port = (size_t) strtoull(argv[1], &endptr, 10);
//     if (endptr && *endptr != '\0') {
//         warnx("invalid port number: %s", argv[1]);
//         return EXIT_FAILURE;
//     }

//     signal(SIGPIPE, SIG_IGN);
//     Listener_Socket_t *sock = ls_new(port);
//     if (!sock) {
//       warnx("cannot open socket");
//       return EXIT_FAILURE;
//     }

//     while (1) {
//         int connfd = ls_accept(sock);
//         handle_connection(connfd);
//         close(connfd);
//     }
//     ls_delete(&sock);

//     return EXIT_SUCCESS;
// }

// void handle_connection(int connfd) {
//     conn_t *conn = conn_new(connfd);
//     const Response_t *res = conn_parse(conn);

//     if (res != NULL) {
//         conn_send_response(conn, res);
//     } else {
//       printf("%s", conn_str(conn));
//         const Request_t *req = conn_get_request(conn);
//         if (req == &REQUEST_PUT) {
//             handle_put(conn);
//         }
//         else if (req == &REQUEST_GET) {
//             handle_get(conn);
//         }
//         else{
//             conn_send_response(conn, res);
//         }
// 	// add cases for other types of requests here
//     }

//     conn_delete(&conn);
// }

// void handle_get(conn_t *conn){
//     char *uri = conn_get_uri(conn);
//     char *requestId = conn_get_header(conn, "Request-Id");
//     printf("Retrieved URI: %s\n", uri);
//     printf("Retrieved Request ID: %s\n", requestId);
// }

// void handle_put(conn_t *conn) {

//     char *uri = conn_get_uri(conn);
//     char *requestId = conn_get_header(conn, "Request-Id");
//     char *contentLen = conn_get_header(conn, "Content-Length");

//     printf("PUT request not implemented. But, we want to put %s", uri);
//     printf("Retrieved Request ID: %s\n", requestId);
//     printf("Retrieved Content Length: %s\n", contentLen);

// }
