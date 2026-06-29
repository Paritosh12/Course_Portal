#include "../include/server.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/utils.h"

// Returns a socket descriptor for the server
int start_server() {
    int l_sock;
    int r;
    struct sockaddr_in local;

    fillzero(local);

    // Getting an internet socket
    l_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (l_sock < 0) {
        perror("socket():");
        exit(1);
    }

    // Defining the address and port
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(SRV_PORT);

    // Binding socket to the address and port above
    r = bind(l_sock, (struct sockaddr *)&local, sizeof(local));
    if (r < 0) {
        perror("bind():");
        exit(1);
    }

    // Listening for incoming connections, queueing only 1 connection
    r = listen(l_sock, 1);
    if (r < 0) {
        perror("listen():");
        exit(1);
    }

    return l_sock;
}

void *handle_connection(void *sock) {
    int c_sock = *(int *)sock;
    free(sock);
    struct message msg;
    int bytes_received;

    while (1) {
        // Receiving data from the client
        bytes_received = recv(c_sock, &msg, sizeof(msg), 0);
        if (bytes_received < 0) {
            perror("recv():");
            close(c_sock);
            exit(1);
        } else if (bytes_received == 0) {
            // Connection closed by the client
            printf("Client disconnected\n");
            break;
        }

        // Process the received message
        switch (msg.type) {
            case FACULTY_LOGIN:
            case STUDENT_LOGIN:
            case ADMIN_LOGIN:
                handle_login(&msg);
                break;

            case ADD_STUDENT:
                handle_add_student(&msg);
                break;

            case VIEW_STUDENT_DETAILS:
                handle_view_student(&msg);
                break;

            case ADD_FACULTY:
                handle_add_faculty(&msg);
                break;

            case VIEW_FACULTY_DETAILS:
                handle_view_faculty(&msg);
                break;

            case ACTIVATE_STUDENT:
                handle_activate_student(&msg);
                break;

            case BLOCK_STUDENT:
                handle_block_student(&msg);
                break;

            case MODIFY_STUDENT:
                handle_modify_student(&msg);
                break;

            case MODIFY_FACULTY:
                handle_modify_faculty(&msg);
                break;

            case LOGOUT_EXIT:
                printf("Client logging out and exiting...\n");
                return NULL;

            case VIEW_OFFERING_COURSES:
                handle_view_offering_courses(&msg);
                break;

            case ADD_NEW_COURSE:
                handle_add_course(&msg);
                break;

            case REMOVE_COURSE:
                handle_remove_course(&msg);
                break;

            case UPDATE_COURSE_DETAILS:
                handle_update_course(&msg);
                break;

            case CHANGE_FACULTY_PASSWORD:
                handle_faculty_password_change(&msg);
                break;

            case VIEW_ALL_COURSES:
                handle_view_all_courses(&msg);
                break;

            case ENROLL_COURSE:
                handle_enroll_course(&msg);
                break;

            case DROP_COURSE:
                handle_drop_course(&msg);
                break;

            case VIEW_ENROLLED_COURSES:
                handle_view_enrolled_courses(&msg);
                break;

            case CHANGE_STUDENT_PASSWORD:
                handle_student_password_change(&msg);
                break;
        }
        send_message(c_sock, &msg, sizeof(msg));
    }
}

int main() {
    int l_sock = start_server();

    while (1) {
        struct sockaddr_in remote;
        int addr_size = sizeof(remote);

        fillzero(remote);

        int *c_sock = malloc(sizeof(int));
        *c_sock = accept(l_sock, (struct sockaddr *)&remote, &addr_size);
        if (*c_sock < 0) {
            perror("accept():");
            exit(1);
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(remote.sin_addr),
               ntohs(remote.sin_port));

        pthread_t tid;
        pthread_create(&tid, NULL, handle_connection, (void *)c_sock);
        pthread_detach(tid);
    }
}