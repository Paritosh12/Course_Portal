#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/client.h"
#include "../include/utils.h"

void handle_faculty(int sock) {
    char faculty_id[20];
    char password[50];

    // Login prompt
    printf("Enter Faculty ID: ");
    scanf("%s", faculty_id);
    printf("Enter Password: ");
    scanf("%s", password);

    struct message login_msg;
    login_msg.type = FACULTY_LOGIN;
    strcpy(login_msg.faculty.fac_id, faculty_id);
    strcpy(login_msg.faculty.password, password);

    send_message(sock, &login_msg, sizeof(login_msg));
    login_msg = recv_message(sock);

    if (strcmp(login_msg.message, "Login successful") != 0) {
        printf("%s\n", login_msg.message);
        close(sock);
        return;
    }

    while (1) {
        printf("---------------Welcome to Faculty Menu---------------\n");
        printf("1. View Offering Courses\n");
        printf("2. Add New Course\n");
        printf("3. Remove Course from Catalog\n");
        printf("4. Update Course Details\n");
        printf("5. Change Password\n");
        printf("6. Logout and Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            struct message msg;
            case 1: {
                msg.type = VIEW_OFFERING_COURSES;
                strcpy(msg.course.faculty_id, faculty_id);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                // Display the list of courses
                printf("Offering Courses:\n");
                for (int i = 0; i < msg.course_count; i++) {
                    printf("Course Code: %s\n", msg.courseList[i].course_code);
                    printf("Course Name: %s\n", msg.courseList[i].name);
                    printf("Course Description: %s\n",
                           msg.courseList[i].description);
                    printf("\n");
                }
                break;
            }

            case 2: {
                msg.type = ADD_NEW_COURSE;

                printf("Enter Course Code: ");
                scanf("%s", msg.course.course_code);
                printf("Enter Course Name: ");
                scanf(" %[^\n]", msg.course.name);
                printf("Enter Course Description: ");
                scanf(" %[^\n]", msg.course.description);
                strcpy(msg.course.faculty_id, faculty_id);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }

            case 3: {
                msg.type = REMOVE_COURSE;

                printf("Enter Course Code to remove: ");
                scanf("%s", msg.course.course_code);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }

            case 4: {
                msg.type = UPDATE_COURSE_DETAILS;

                printf("Enter Course Code to update: ");
                scanf("%s", msg.course.course_code);

                printf("Enter new course name (type '-' to keep unchanged): ");
                scanf(" %[^\n]", msg.course.name);
                if (strcmp(msg.course.name, "-") == 0) {
                    msg.course.name[0] = '\0';  // Indicate no change
                }

                printf(
                    "Enter new course description (type '-' to keep "
                    "unchanged): ");
                scanf(" %[^\n]", msg.course.description);
                if (strcmp(msg.course.description, "-") == 0) {
                    msg.course.description[0] = '\0';  // Indicate no change
                }

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 5: {
                msg.type = CHANGE_FACULTY_PASSWORD;

                strcpy(msg.faculty.fac_id, faculty_id);
                printf("Enter new password: ");
                scanf(" %[^\n]", msg.faculty.password);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }
            case 6: {
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
