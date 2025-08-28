#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#define BUFFER_SIZE 4096

int main(void) {
    char inputBuffer[BUFFER_SIZE];
    int bytesRead = 0;
    int charsRead = 0;
    int fd;
    char *command;
    int bytes = 0;
    do {
        bytes = read(STDIN_FILENO, inputBuffer + bytesRead, sizeof(inputBuffer) - bytesRead - 1);
        if (bytes < 0) {
            fprintf(stderr, "Operation Failed\n");
            return 1;
        }
        bytesRead += bytes;
    } while (bytes > 0);
    inputBuffer[bytesRead] = '\0';
    command = strtok(inputBuffer, "\n");
    charsRead += strlen(command) + 1;
    if (inputBuffer[0] == '\n' || strlen(command) > 3
        || (strcmp(command, "get") != 0 && strcmp(command, "set") != 0)) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }

    char *fileName = strtok(NULL, "\n");
    if (fileName == NULL || strlen(fileName) + 1 > PATH_MAX) {
        fprintf(stderr, "Invalid Command\n");
        return 1;
    }
    charsRead += strlen(fileName) + 1;

    if (strcmp(command, "get") == 0) {
        fd = open(fileName, O_RDWR);
        if (inputBuffer[bytesRead - 1] != '\0' || fd == -1 || strtok(NULL, "\n") != NULL) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return 1;
        }
        int bytes_read = 0;
        char buf[BUFFER_SIZE];
        do {
            bytes_read = read(fd, buf, BUFFER_SIZE);
            if (bytes_read < 0) {
                fprintf(stderr, "Cannot read file\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes
                        = write(STDOUT_FILENO, buf + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                        fprintf(stderr, "Cannot write to stdout\n");
                        return 1;
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        } while (bytes_read > 0);
    } else {
        char *contentLengthStr;
        int contentLength;
        char *fileContent;
        int fileLength;
        char *endptr;
        bool flag = false;
        fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if ((contentLengthStr = strtok(NULL, "\n")) == NULL) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return 1;
        }
        if (fd == -1) {
            fprintf(stderr, "Operation Failed\n");
            close(fd);
            return 1;
        }

        if (strcmp(contentLengthStr, "0") == 0) {
            close(fd);
            printf("OK\n");
            return 0;
        }

        contentLength = strtol(contentLengthStr, &endptr, 10);
        int numbers = 0;
        for (int i = charsRead; i < bytesRead; i++) {
            if (inputBuffer[i] >= '0' && inputBuffer[i] <= '9') {
                numbers += 1;
            } else {
                break;
            }
        }
        if (numbers < (int) strlen(contentLengthStr)) {
            fprintf(stderr, "Invalid Command\n");
            close(fd);
            return 1;
        }
        charsRead += strlen(contentLengthStr) + 1;

        fileContent = inputBuffer + charsRead;
        fileLength = bytesRead - charsRead;
        if ((int) contentLength <= fileLength) {
            fileLength = contentLength;
            flag = true;
        } else {
            contentLength -= fileLength;
        }
        bytes = 0;
        bytesRead = 0;
        do {
            bytes = write(fd, fileContent + bytesRead, fileLength - bytesRead);
            if (bytes < 0) {
                fprintf(stderr, "Operation Failed\n");
                return 1;
            }
            bytesRead += bytes;
        } while (bytesRead < fileLength);
        if (flag) {
            printf("OK\n");
            close(fd);
            return 0;
        }
        int bytes_read = 0;
        char buf2[BUFFER_SIZE];
        while (!flag && (bytes_read = read(STDIN_FILENO, buf2, BUFFER_SIZE)) > 0) {
            if (contentLength <= bytes_read) {
                flag = true;
                bytes_read = contentLength;
            } else if (contentLength > bytes_read) {
                contentLength -= bytes_read;
                if (contentLength < 0) {
                    contentLength = 0;
                }
            }
            if (bytes_read < 0) {
                fprintf(stderr, "Cannot read file\n");
                return 1;
            } else if (bytes_read > 0) {
                int bytes_written = 0;
                do {
                    int bytes = write(fd, buf2 + bytes_written, bytes_read - bytes_written);
                    if (bytes <= 0) {
                        fprintf(stderr, "Cannot write to stdout\n");
                        return 1;
                    }
                    bytes_written += bytes;
                } while (bytes_written < bytes_read);
            }
        }

        printf("OK\n");
    }

    return 0;
}
