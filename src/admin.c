#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/client.h"
#include "../include/utils.h"

void handle_admin(int sock) {
    char password[50];

    // Login prompt
    printf("Enter Admin Password: ");
    scanf("%s", password);

    struct message login_msg;
    login_msg.type = ADMIN_LOGIN;
    strcpy(login_msg.message, password);

    send_message(sock, &login_msg, sizeof(login_msg));
    login_msg = recv_message(sock);

    if (strcmp(login_msg.message, "Login successful") != 0) {
        printf("%s\n", login_msg.message);
        close(sock);
        return;
    }

    while (1) {
        printf("---------------Welcome to Admin Menu---------------\n");
        printf("1. Add Student\n");
        printf("2. Add Faculty\n");
        printf("3. View Student details\n");
        printf("4. View Faculty details\n");
        printf("5. Activate Student\n");
        printf("6. Block Student\n");
        printf("7. Modify Student details\n");
        printf("8. Modify Faculty details\n");
        printf("9. Logout and Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            struct message msg;
            case 1: {
                msg.type = ADD_STUDENT;

                printf("Enter Student Roll No: ");
                scanf("%s", msg.student.roll_no);
                printf("Enter Student Name: ");
                scanf(" %[^\n]", msg.student.name);
                printf("Enter Student Branch: ");
                scanf(" %[^\n]", msg.student.branch);
                printf("Enter Student Password: ");
                scanf(" %[^\n]", msg.student.password);

                msg.student.status = 1;  // 1 means active

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 2: {
                msg.type = ADD_FACULTY;

                printf("Enter Faculty ID: ");
                scanf("%s", msg.faculty.fac_id);
                printf("Enter Faculty Name: ");
                scanf(" %[^\n]", msg.faculty.name);
                printf("Enter Faculty Department: ");
                scanf(" %[^\n]", msg.faculty.department);
                printf("Enter Faculty Password: ");
                scanf(" %[^\n]", msg.faculty.password);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 3: {
                msg.type = VIEW_STUDENT_DETAILS;
                printf("Enter Student Roll No: ");
                scanf("%s", msg.student.roll_no);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                if (msg.student.roll_no[0] == '\0') {
                    printf("No student found with the given roll number\n");
                } else {
                    printf("Student Details:\n");
                    printf("Name: %s\n", msg.student.name);
                    printf("Roll No: %s\n", msg.student.roll_no);
                    printf("Branch: %s\n", msg.student.branch);
                    printf("Password: %s\n", msg.student.password);
                    printf("Status: %s\n",
                           msg.student.status == 1 ? "Active" : "Inactive");
                }
                break;
            }
            case 4: {
                msg.type = VIEW_FACULTY_DETAILS;
                printf("Enter Faculty ID: ");
                scanf("%s", msg.faculty.fac_id);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                if (msg.faculty.fac_id[0] == '\0') {
                    printf("No faculty found with the given ID\n");
                } else {
                    printf("Faculty Details:\n");
                    printf("Name: %s\n", msg.faculty.name);
                    printf("Faculty ID: %s\n", msg.faculty.fac_id);
                    printf("Department: %s\n", msg.faculty.department);
                    printf("Password: %s\n", msg.faculty.password);
                }
                break;
            }
            case 5: {
                msg.type = ACTIVATE_STUDENT;

                printf("Enter Student Roll No to activate: ");
                scanf("%s", msg.student.roll_no);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 6: {
                msg.type = BLOCK_STUDENT;

                printf("Enter Student Roll No to block: ");
                scanf("%s", msg.student.roll_no);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 7: {
                msg.type = MODIFY_STUDENT;

                printf("Enter Student Roll No to modify: ");
                scanf("%s", msg.student.roll_no);

                printf("Enter new name (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.student.name);
                if (strcmp(msg.student.name, "-") == 0) {
                    msg.student.name[0] = '\0';  // Indicate no change
                }

                printf("Enter new branch (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.student.branch);
                if (strcmp(msg.student.branch, "-") == 0) {
                    msg.student.branch[0] = '\0';  // Indicate no change
                }

                printf("Enter new password (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.student.password);
                if (strcmp(msg.student.password, "-") == 0) {
                    msg.student.password[0] = '\0';  // Indicate no change
                }

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 8: {
                msg.type = MODIFY_FACULTY;

                printf("Enter Faculty ID to modify: ");
                scanf("%s", msg.faculty.fac_id);

                printf("Enter new name (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.faculty.name);
                if (strcmp(msg.faculty.name, "-") == 0) {
                    msg.faculty.name[0] = '\0';  // Indicate no change
                }

                printf("Enter new department (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.faculty.department);
                if (strcmp(msg.faculty.department, "-") == 0) {
                    msg.faculty.department[0] = '\0';  // Indicate no change
                }

                printf("Enter new password (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.faculty.password);
                if (strcmp(msg.faculty.password, "-") == 0) {
                    msg.faculty.password[0] = '\0';  // Indicate no change
                }

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 9: {
                msg.type = LOGOUT_EXIT;
                send_message(sock, &msg, sizeof(msg));
                printf("Logging out and exiting...\n");
                close(sock);
                return;
            }
            default:
                printf("Invalid choice. Please try again.\n");
                continue;
        }
    }
}
