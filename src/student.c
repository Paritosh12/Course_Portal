#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/client.h"
#include "../include/utils.h"

void handle_student(int sock) {
    char roll_no[20];
    char password[50];

    // Login prompt
    printf("Enter Roll No: ");
    scanf("%s", roll_no);
    printf("Enter Password: ");
    scanf("%s", password);

    struct message login_msg;
    login_msg.type = STUDENT_LOGIN;
    strcpy(login_msg.student.roll_no, roll_no);
    strcpy(login_msg.student.password, password);

    send_message(sock, &login_msg, sizeof(login_msg));
    login_msg = recv_message(sock);

    if (strcmp(login_msg.message, "Login successful") != 0) {
        printf("%s\n", login_msg.message);
        close(sock);
        return;
    }

    while (1) {
        printf("---------------Welcome to Student Menu---------------\n");
        printf("1. View All Courses\n");
        printf("2. Enroll in a Course\n");
        printf("3. Drop a Course\n");
        printf("4. View Enrolled Courses\n");
        printf("5. Change Password\n");
        printf("6. Logout and Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            struct message msg;
            case 1: {
                msg.type = VIEW_ALL_COURSES;

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                // Display the list of courses
                printf("Available Courses:\n");
                for (int i = 0; i < msg.course_count; i++) {
                    printf("Course Code: %s\n", msg.courseList[i].course_code);
                    printf("Course Name: %s\n", msg.courseList[i].name);
                    printf("Course Description: %s\n",
                           msg.courseList[i].description);
                    printf("Faculty ID: %s\n", msg.courseList[i].faculty_id);
                    printf("\n");
                }
                break;
            }

            case 2: {
                msg.type = ENROLL_COURSE;
                strcpy(msg.student_course.roll_no, roll_no);

                printf("Enter Course Code to Enroll: ");
                scanf("%s", msg.student_course.course_code);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }

            case 3: {
                msg.type = DROP_COURSE;
                strcpy(msg.student_course.roll_no, roll_no);

                printf("Enter Course Code to Drop: ");
                scanf("%s", msg.student_course.course_code);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                printf("%s\n", msg.message);
                break;
            }

            case 4: {
                msg.type = VIEW_ENROLLED_COURSES;
                strcpy(msg.student.roll_no, roll_no);

                send_message(sock, &msg, sizeof(msg));
                msg = recv_message(sock);
                // Display the list of enrolled courses
                printf("Enrolled Courses:\n");
                for (int i = 0; i < msg.course_count; i++) {
                    printf("Course Code: %s\n", msg.courseList[i].course_code);
                    printf("Course Name: %s\n", msg.courseList[i].name);
                    printf("Course Description: %s\n",
                           msg.courseList[i].description);
                    printf("\n");
                }
                break;
            }

            case 5: {
                msg.type = CHANGE_STUDENT_PASSWORD;
                strcpy(msg.student.roll_no, roll_no);

                printf("Enter New Password: ");
                scanf("%s", msg.student.password);

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