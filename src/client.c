#include "../include/client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/utils.h"

// Returns a socket descriptor that is connected to the server
int connect_to_server() {
    int sock;
    int r;
    struct sockaddr_in server_addr;

    fillzero(server_addr);

    // Getting an internet socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket():");
        exit(1);
    }

    // Defining the address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SRV_PORT);

    // Connecting to the server
    r = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (r < 0) {
        perror("connect():");
        exit(1);
    }

    return sock;
}

int main() {
    int sock = connect_to_server();

    printf("---------------Welcome to Academia---------------\n");
    printf("Login Type\n");
    printf("Enter your choice {1.Student, 2.Faculty, 3.Admin}: ");
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            handle_student(sock);
            break;
        case 2:
            handle_faculty(sock);
            break;
        case 3:
            handle_admin(sock);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
    }

    close(sock);
}