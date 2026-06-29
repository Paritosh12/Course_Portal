#ifndef UTILS_H
#define UTILS_H

#define fillzero(X) memset(&X, 0, sizeof(X))
#define SRV_PORT 8000

#define STUDENT_FILE "student.bin"
#define FACULTY_FILE "faculty.bin"
#define COURSE_FILE "course.bin"
#define STUDENT_COURSE_FILE "student_course.bin"

#define ADD_STUDENT 1
#define ADD_FACULTY 2
#define VIEW_STUDENT_DETAILS 3
#define VIEW_FACULTY_DETAILS 4
#define ACTIVATE_STUDENT 5
#define BLOCK_STUDENT 6
#define MODIFY_STUDENT 7
#define MODIFY_FACULTY 8
#define LOGOUT_EXIT 9
#define VIEW_OFFERING_COURSES 10
#define ADD_NEW_COURSE 11
#define REMOVE_COURSE 12
#define UPDATE_COURSE_DETAILS 13
#define CHANGE_FACULTY_PASSWORD 14
#define VIEW_ALL_COURSES 15
#define ENROLL_COURSE 16
#define DROP_COURSE 17
#define VIEW_ENROLLED_COURSES 18
#define CHANGE_STUDENT_PASSWORD 19
#define ADMIN_LOGIN 20
#define FACULTY_LOGIN 21
#define STUDENT_LOGIN 22

// Student struct
struct student {
    char name[50];
    char roll_no[20];
    char branch[50];
    char password[50];
    int status;  // 0 for inactive, 1 for active
};

// Faculty struct
struct faculty {
    char name[50];
    char fac_id[20];
    char password[50];
    char department[50];
};

struct course {
    char course_code[20];
    char name[50];
    char description[100];
    char faculty_id[20];
};

struct student_course {
    char roll_no[20];
    char course_code[20];
};

// Common struct for exchanging messages between client and server
struct message {
    int type;
    char message[100];
    struct student student;
    struct faculty faculty;
    struct course course;
    struct student_course student_course;
    struct course courseList[100];
    int course_count;
};

void send_message(int sock, void *data, int size);

struct message recv_message(int sock);

int open_file_with_lock(const char *filename, int flags, int lock_type);

void close_file_with_unlock(int fd);

#endif