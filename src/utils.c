#include "../include/utils.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void send_message(int sock, void *data, int size) {
    int r = send(sock, data, size, 0);
    if (r < 0) {
        perror("send():");
        close(sock);
        exit(1);
    }
}

struct message recv_message(int sock) {
    struct message msg;
    int bytes_received = recv(sock, &msg, sizeof(msg), 0);
    if (bytes_received < 0) {
        perror("recv():");
        close(sock);
        exit(1);
    } else if (bytes_received == 0) {
        // Connection closed by the server
        printf("Server disconnected\n");
        close(sock);
        exit(1);
    }
    return msg;
}

int open_file_with_lock(const char *filename, int flags, int lock_type) {
    int fd = open(filename, flags, 0644);
    if (fd < 0) {
        perror("open():");
        return -1;
    }
    if (flock(fd, lock_type) < 0) {
        perror("flock():");
        close(fd);
        return -1;
    }
    return fd;
}

void close_file_with_unlock(int fd) {
    if (flock(fd, LOCK_UN) < 0) {
        perror("flock():");
    }
    close(fd);
}