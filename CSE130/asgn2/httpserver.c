#include "iowrapper.h"
#include "listener_socket.h"
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdbool.h>
#include <regex.h>
#include <fcntl.h> // For open()
#include <sys/types.h> // For file types and stat()
#include <sys/stat.h> // For file permissions
#include <errno.h>

#define MAX_METHOD_LEN       8
#define MAX_URI_LEN          64
#define MAX_VERSION_LEN      10
#define MAX_HEADERS          20
#define MAX_HEADER_KEY_LEN   128
#define MAX_HEADER_VALUE_LEN 128

#define REQUEST_LINE_REGEX "^([a-zA-Z]{1,8}) (/[a-zA-Z0-9.-]{1,63}) (HTTP/[0-9]\\.[0-9])\r\n"
#define HEADER_REGEX       "([a-zA-Z0-9.-]{1,128}): ([ -~]{1,128})\r\n"
#define BUFFER_SIZE        8192 * 4
#define HEADER_END         "\r\n\r\n"

typedef struct {
    char method[MAX_METHOD_LEN + 1]; // GET, PUT (null-terminated)
    char uri[MAX_URI_LEN + 1]; // Resource path (null-terminated)
    char http_version[MAX_VERSION_LEN + 1]; // HTTP/1.1 (null-terminated)

    struct {
        char key[MAX_HEADER_KEY_LEN + 1];
        char value[MAX_HEADER_VALUE_LEN + 1];
    } headers[MAX_HEADERS]; // Array of headers (key-value pairs)

    int header_count; // Number of headers parsed
    char *body; // Pointer to body (allocated dynamically for PUT)
    size_t body_length; // Length of the body in bytes
} HttpRequest;

// Function to read the HTTP request until we get "\r\n\r\n" or reach 2048 bytes
ssize_t read_http_request(int client_fd, char *buffer, size_t buffer_size) {
    size_t total_read = 0;
    ssize_t bytes_read;

    while (total_read < buffer_size) { // Leave room for null terminator
        bytes_read = read(client_fd, buffer + total_read, buffer_size - total_read);
        if (bytes_read < 0) {
            perror("Error reading from client");
            return -1; // Error
        }
        if (bytes_read == 0) {
            break; // Client closed connection
        }

        total_read += bytes_read;
        buffer[total_read] = '\0'; // Null-terminate buffer

        // Stop reading if "\r\n\r\n" is found
        if (strstr(buffer, HEADER_END) != NULL) {
            return total_read;
        }
    }

    // If we reach here, the request is too large
    fprintf(stderr, "Request too large\n");
    return -1;
}

// Helper function to extract matches from regex execution
void extract_match(const char *source, regmatch_t match, char *destination, size_t max_len) {
    size_t match_len = match.rm_eo - match.rm_so;
    if (match_len >= max_len)
        match_len = max_len - 1;
    strncpy(destination, source + match.rm_so, match_len);
    destination[match_len] = '\0';
}

// Function to parse an HTTP request
int parse_http_request(const char *request, ssize_t request_size, HttpRequest *parsed_request) {
    regex_t regex;
    regmatch_t matches[4]; // 1 full match + 3 submatches (method, URI, version)

    // Compile the regex for the request-line
    if (regcomp(&regex, REQUEST_LINE_REGEX, REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex for request line\n");
        return -1;
    }

    // Apply regex to find request-line
    if (regexec(&regex, request, 4, matches, 0) == 0) {
        extract_match(request, matches[1], parsed_request->method, MAX_METHOD_LEN);
        extract_match(request, matches[2], parsed_request->uri, MAX_URI_LEN);
        extract_match(request, matches[3], parsed_request->http_version, MAX_VERSION_LEN);
    } else {
        regfree(&regex);
        return -1; // Invalid request format
    }
    regfree(&regex);

    // Parse headers using HEADER_REGEX
    if (regcomp(&regex, HEADER_REGEX, REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex for headers\n");
        return -1;
    }

    parsed_request->header_count = 0;
    const char *header_start = request + matches[0].rm_eo;
    regmatch_t header_matches[3];

    while (parsed_request->header_count < MAX_HEADERS
           && regexec(&regex, header_start, 3, header_matches, 0) == 0) {
        extract_match(header_start, header_matches[1],
            parsed_request->headers[parsed_request->header_count].key, MAX_HEADER_KEY_LEN);
        extract_match(header_start, header_matches[2],
            parsed_request->headers[parsed_request->header_count].value, MAX_HEADER_VALUE_LEN);
        parsed_request->header_count++;
        header_start += header_matches[0].rm_eo; // Move past the matched header
    }
    regfree(&regex);

    // Extract message body if present
    char *body_start = strstr(request, "\r\n\r\n");
    if (body_start != NULL) {
        body_start += 4; // Move past \r\n\r\n
        parsed_request->body_length = parsed_request->body_length
            = request_size - (body_start - request);
        if (parsed_request->body_length > 0) {
            parsed_request->body = malloc(parsed_request->body_length + 1);
            memcpy(parsed_request->body, body_start, parsed_request->body_length);
        } else {
            parsed_request->body = NULL;
        }
    } else {
        parsed_request->body = NULL;
        parsed_request->body_length = 0;
    }

    return 0;
}

void handle_get(int client_fd, const char *uri) {
    const char *fileName = uri + 1;
    int fd = open(fileName, O_RDWR);
    if (fd == -1) {
        if (errno == EISDIR || errno == EACCES) {
            const char *response
                = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        } else if (errno == ENOENT) { // Directory or file not found
            const char *response
                = "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        } else { // General error
            const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                                   "22\r\n\r\nInternal Server Error\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        }
        return;
    }

    // Get the file size using stat()
    struct stat file_stat;
    if (stat(fileName, &file_stat) == -1) {
        close(fd);
        const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                               "22\r\n\r\nInternal Server Error\n";
        write_n_bytes(client_fd, (char *) response, strlen(response));
        return;
    }
    int file_size = file_stat.st_size;

    // Create HTTP response header
    char header[256];
    int header_length = snprintf(
        header, sizeof(header), "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", file_size);

    // Send the header
    write_n_bytes(client_fd, header, header_length);

    // Read and send file contents in chunks
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    do {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read < 0) {
            const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                                         "22\r\n\r\nInternal Server Error\n";
            write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
            close(fd);
            return;
        } else if (bytes_read > 0) {
            int bytes_written = 0;
            do {
                ssize_t bytes
                    = write(client_fd, buffer + bytes_written, bytes_read - bytes_written);
                if (bytes <= 0) {
                    fprintf(stderr, "Cannot write to client\n");
                    close(fd);
                    return;
                }
                bytes_written += bytes;
            } while (bytes_written < bytes_read);
        }
    } while (bytes_read > 0);

    // Check for read errors
    if (bytes_read == -1) {
        const char *error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                                     "22\r\n\r\nInternal Server Error\n";
        write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
    }

    // Close file descriptor
    close(fd);
}

void handle_put(int client_fd, const char *uri, int content_length, char *body, int body_length) {
    // Remove leading '/' from URI to get the filename
    const char *fileName = uri + 1;
    int fileExists = (access(fileName, F_OK) == 0);

    int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        if (errno == EACCES || errno == EISDIR) { // No permission to write/create
            const char *response
                = "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        } else if (errno == ENOENT) { // Directory or file not found
            const char *response
                = "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        } else { // General error
            const char *response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: "
                                   "22\r\n\r\nInternal Server Error\n";
            write_n_bytes(client_fd, (char *) response, strlen(response));
        }
        return;
    }

    // Send appropriate response based on whether the file was created or overwritten
    write_n_bytes(fd, body, body_length);

    // // Read and write the remaining body data
    int bytes_remaining = content_length - body_length;
    pass_n_bytes(client_fd, fd, bytes_remaining);

    close(fd);

    if (fileExists) {
        const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n";
        write_n_bytes(client_fd, (char *) response, strlen(response));
    } else {
        const char *response = "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n";
        write_n_bytes(client_fd, (char *) response, strlen(response));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }
    struct Listener_Socket *listener_socket = ls_new(port);
    if (!listener_socket) {
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }
    while (true) {
        int client_fd = ls_accept(listener_socket);
        if (client_fd == -1) {
            fprintf(stderr, "Failed to accept a connection");
            continue;
        }

        // Read the full HTTP request
        char *request_buffer = calloc(MAX_HEADER_LEN + 1, sizeof(char));

        ssize_t request_size = read_http_request(client_fd, request_buffer, MAX_HEADER_LEN);

        if (request_size > 0) {
            // Initialize HTTP request struct
            HttpRequest parsed_request = { 0 };

            // Parse the request
            if (parse_http_request(request_buffer, request_size, &parsed_request) == 0) {
                // Print headers
                int extractedContentLen = -1;
                for (int i = 0; i < parsed_request.header_count; i++) {
                    if (strcmp(parsed_request.headers[i].key, "Content-Length") == 0) {
                        extractedContentLen = atoi(parsed_request.headers[i].value);
                    }
                }

                if (strcmp(parsed_request.http_version, "HTTP/1.1") != 0) {
                    const char *error_response
                        = "HTTP/1.1 505 Version Not Supported\r\nContent-Length: 22\r\n\r\nVersion "
                          "Not Supported\n";
                    write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
                } else {
                    if (strcmp(parsed_request.method, "GET") == 0) {
                        handle_get(client_fd, parsed_request.uri);
                    } else if (strcmp(parsed_request.method, "PUT") == 0) {
                        if (extractedContentLen == -1) {
                            const char *error_response
                                = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad "
                                  "Request\n";
                            write_n_bytes(
                                client_fd, (char *) error_response, strlen(error_response));
                        } else {
                            handle_put(client_fd, parsed_request.uri, extractedContentLen,
                                parsed_request.body, parsed_request.body_length);
                        }
                    } else {
                        const char *error_response
                            = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot "
                              "Implemented\n";
                        write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
                    }
                }
            } else {
                // Send "400 Bad Request" response if parsing fails
                const char *error_response
                    = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
                write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
            }

            if (parsed_request.body) {
                free(parsed_request.body);
                parsed_request.body = NULL;
            }
        } else {
            // Send "400 Bad Request" response if request is invalid
            const char *error_response
                = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
            write_n_bytes(client_fd, (char *) error_response, strlen(error_response));
        }

        // Close the client connection
        free(request_buffer);

        close(client_fd);
    }

    // Clean up the listener socket
    ls_delete(&listener_socket);

    return 0;
}
