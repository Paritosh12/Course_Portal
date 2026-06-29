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

#include "../include/server.h"
#include "../include/utils.h"

void handle_add_student(struct message *msg) {
    int fd = open_file_with_lock(STUDENT_FILE, O_RDWR | O_CREAT, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error adding student");
        return;
    }

    // Write student data to the end of the file
    lseek(fd, 0, SEEK_END);
    if (write(fd, &msg->student, sizeof(msg->student)) < 0) {
        perror("write():");
        close_file_with_unlock(fd);  // Unlock and close the file before exiting
        strcpy(msg->message, "Error adding student");
        return;
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Student added successfully");
    return;
}

void handle_add_faculty(struct message *msg) {
    int fd = open_file_with_lock(FACULTY_FILE, O_RDWR | O_CREAT, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error adding faculty");
        return;
    }

    lseek(fd, 0, SEEK_END);
    if (write(fd, &msg->faculty, sizeof(msg->faculty)) < 0) {
        perror("write():");
        close_file_with_unlock(fd);
        strcpy(msg->message, "Error adding faculty");
        return;
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Faculty added successfully");
}

void handle_view_student(struct message *msg) {
    struct student s;
    int fd = open_file_with_lock(STUDENT_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        memset(&msg->student, 0, sizeof(msg->student));
        return;
    }

    while (read(fd, &s, sizeof(s)) > 0) {
        if (strcmp(s.roll_no, msg->student.roll_no) == 0) {
            msg->student = s;
            close_file_with_unlock(fd);
            return;
        }
    }

    close_file_with_unlock(fd);
    memset(&msg->student, 0, sizeof(msg->student));
}

void handle_view_faculty(struct message *msg) {
    struct faculty f;
    int fd = open_file_with_lock(FACULTY_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        memset(&msg->faculty, 0, sizeof(msg->faculty));
        return;
    }

    while (read(fd, &f, sizeof(f)) > 0) {
        if (strcmp(f.fac_id, msg->faculty.fac_id) == 0) {
            msg->faculty = f;
            close_file_with_unlock(fd);
            return;
        }
    }

    close_file_with_unlock(fd);
    memset(&msg->faculty, 0, sizeof(msg->faculty));
}

void handle_activate_student(struct message *msg) {
    struct student s;
    int fd = open_file_with_lock(STUDENT_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error activating student");
        return;
    }

    while (read(fd, &s, sizeof(s)) > 0) {
        if (strcmp(s.roll_no, msg->student.roll_no) == 0) {
            s.status = 1;
            lseek(fd, -sizeof(s), SEEK_CUR);
            if (write(fd, &s, sizeof(s)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error activating student");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Student activated successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Student not found");
}

void handle_block_student(struct message *msg) {
    struct student s;
    int fd = open_file_with_lock(STUDENT_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error blocking student");
        return;
    }

    while (read(fd, &s, sizeof(s)) > 0) {
        if (strcmp(s.roll_no, msg->student.roll_no) == 0) {
            s.status = 0;
            lseek(fd, -sizeof(s), SEEK_CUR);
            if (write(fd, &s, sizeof(s)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error blocking student");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Student blocked successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Student not found");
}

void handle_modify_student(struct message *msg) {
    struct student s;
    int fd = open_file_with_lock(STUDENT_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error modifying student");
        return;
    }

    while (read(fd, &s, sizeof(s)) > 0) {
        if (strcmp(s.roll_no, msg->student.roll_no) == 0) {
            // Update the fields only if they are not empty
            if (strlen(msg->student.name) > 0) {
                strcpy(s.name, msg->student.name);
            }
            if (strlen(msg->student.branch) > 0) {
                strcpy(s.branch, msg->student.branch);
            }
            if (strlen(msg->student.password) > 0) {
                strcpy(s.password, msg->student.password);
            }

            lseek(fd, -sizeof(s), SEEK_CUR);
            if (write(fd, &s, sizeof(s)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error modifying student");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Student modified successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Student not found");
}

void handle_modify_faculty(struct message *msg) {
    struct faculty f;
    int fd = open_file_with_lock(FACULTY_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error modifying faculty");
        return;
    }

    while (read(fd, &f, sizeof(f)) > 0) {
        if (strcmp(f.fac_id, msg->faculty.fac_id) == 0) {
            // Update the fields only if they are not empty
            if (strlen(msg->faculty.name) > 0) {
                strcpy(f.name, msg->faculty.name);
            }
            if (strlen(msg->faculty.department) > 0) {
                strcpy(f.department, msg->faculty.department);
            }
            if (strlen(msg->faculty.password) > 0) {
                strcpy(f.password, msg->faculty.password);
            }

            lseek(fd, -sizeof(f), SEEK_CUR);
            if (write(fd, &f, sizeof(f)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error modifying faculty");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Faculty modified successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Faculty not found");
}

void handle_add_course(struct message *msg) {
    int fd = open_file_with_lock(COURSE_FILE, O_RDWR | O_CREAT, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error adding course");
        return;
    }

    flock(fd, LOCK_EX);
    lseek(fd, 0, SEEK_END);
    if (write(fd, &msg->course, sizeof(msg->course)) < 0) {
        perror("write():");
        close_file_with_unlock(fd);
        strcpy(msg->message, "Error adding course");
        return;
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Course added successfully");
}

void handle_remove_course(struct message *msg) {
    int fd = open_file_with_lock(COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        strcpy(msg->message, "Error removing course");
        return;
    }

    int temp_fd = open("temp_course_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        close_file_with_unlock(fd);
        strcpy(msg->message, "Error creating temporary file");
        return;
    }

    struct course c;
    int found = 0;

    while (read(fd, &c, sizeof(c)) > 0) {
        if (strcmp(c.course_code, msg->course.course_code) == 0) {
            found = 1;  // Skip this course
        } else {
            if (write(temp_fd, &c, sizeof(c)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                close(temp_fd);
                strcpy(msg->message, "Error writing to temporary file");
                return;
            }
        }
    }

    close_file_with_unlock(fd);
    close(temp_fd);

    if (rename("temp_course_file", COURSE_FILE) < 0) {
        perror("rename():");
        strcpy(msg->message, "Error replacing course file");
        return;
    }

    // Remove all students enrolled in the course
    fd = open_file_with_lock(STUDENT_COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        strcpy(msg->message, "Error accessing student-course file");
        return;
    }

    temp_fd =
        open("temp_student_course_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        close_file_with_unlock(fd);
        strcpy(msg->message,
               "Error creating temporary file for student-course");
        return;
    }

    struct student_course sc;

    while (read(fd, &sc, sizeof(sc)) > 0) {
        if (strcmp(sc.course_code, msg->course.course_code) != 0) {
            if (write(temp_fd, &sc, sizeof(sc)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                close(temp_fd);
                strcpy(msg->message,
                       "Error writing to temporary student-course file");
                return;
            }
        }
    }

    close_file_with_unlock(fd);
    close(temp_fd);

    if (rename("temp_student_course_file", STUDENT_COURSE_FILE) < 0) {
        perror("rename():");
        strcpy(msg->message, "Error replacing student-course file");
        return;
    }

    strcpy(msg->message,
           found ? "Course and associated enrollments removed successfully"
                 : "Course not found");
}

void handle_update_course(struct message *msg) {
    int fd = open_file_with_lock(COURSE_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error updating course");
        return;
    }

    flock(fd, LOCK_EX);
    struct course c;
    int found = 0;

    while (read(fd, &c, sizeof(c)) > 0) {
        if (strcmp(c.course_code, msg->course.course_code) == 0) {
            // Update the fields only if they are not empty
            if (strlen(msg->course.name) > 0) {
                strcpy(c.name, msg->course.name);
            }
            if (strlen(msg->course.description) > 0) {
                strcpy(c.description, msg->course.description);
            }

            lseek(fd, -sizeof(c), SEEK_CUR);
            if (write(fd, &c, sizeof(c)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error updating course");
                return;
            }
            found = 1;
            break;
        }
    }

    close_file_with_unlock(fd);

    strcpy(msg->message,
           found ? "Course updated successfully" : "Course not found");
}

void handle_view_offering_courses(struct message *msg) {
    int fd = open_file_with_lock(COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        msg->course_count = 0;
        exit(1);
    }

    flock(fd, LOCK_SH);
    int count = 0;
    struct course temp;
    while (read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.faculty_id, msg->course.faculty_id) == 0) {
            msg->courseList[count++] = temp;
            if (count >= 10) {
                break;  // Limit to 10 courses
            }
        }
    }
    flock(fd, LOCK_UN);
    close(fd);

    msg->course_count = count;
}

void handle_faculty_password_change(struct message *msg) {
    struct faculty temp;
    int fd = open_file_with_lock(FACULTY_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error changing password");
        return;
    }

    flock(fd, LOCK_EX);
    while (read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.fac_id, msg->faculty.fac_id) == 0) {
            strcpy(temp.password, msg->faculty.password);
            lseek(fd, -sizeof(temp), SEEK_CUR);
            if (write(fd, &temp, sizeof(temp)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error changing password");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Password changed successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Faculty not found");
}

void handle_view_all_courses(struct message *msg) {
    int fd = open_file_with_lock(COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        msg->course_count = 0;
        return;
    }

    struct course temp;
    int count = 0;
    while (read(fd, &temp, sizeof(temp)) > 0) {
        msg->courseList[count++] = temp;
    }

    close_file_with_unlock(fd);
    msg->course_count = count;
}

void handle_enroll_course(struct message *msg) {
    int fd =
        open_file_with_lock(STUDENT_COURSE_FILE, O_RDWR | O_CREAT, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error enrolling course");
        return;
    }

    lseek(fd, 0, SEEK_END);
    if (write(fd, &msg->student_course, sizeof(msg->student_course)) < 0) {
        perror("write():");
        close_file_with_unlock(fd);
        strcpy(msg->message, "Error enrolling course");
        return;
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Course enrolled successfully");
}

void handle_drop_course(struct message *msg) {
    int fd = open_file_with_lock(STUDENT_COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        strcpy(msg->message, "Error dropping course");
        return;
    }

    int temp_fd =
        open("temp_student_course_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        close_file_with_unlock(fd);
        strcpy(msg->message, "Error creating temporary file");
        return;
    }

    struct student_course sc;
    int found = 0;

    while (read(fd, &sc, sizeof(sc)) > 0) {
        if (strcmp(sc.roll_no, msg->student_course.roll_no) == 0 &&
            strcmp(sc.course_code, msg->student_course.course_code) == 0) {
            found = 1;
        } else {
            if (write(temp_fd, &sc, sizeof(sc)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                close(temp_fd);
                strcpy(msg->message, "Error writing to temporary file");
                return;
            }
        }
    }

    close_file_with_unlock(fd);
    close(temp_fd);

    if (rename("temp_student_course_file", STUDENT_COURSE_FILE) < 0) {
        perror("rename():");
        strcpy(msg->message, "Error replacing course file");
        return;
    }

    strcpy(msg->message,
           found ? "Course dropped successfully" : "Course not found");
}

void handle_view_enrolled_courses(struct message *msg) {
    int fd = open_file_with_lock(STUDENT_COURSE_FILE, O_RDONLY, LOCK_SH);
    if (fd < 0) {
        msg->course_count = 0;
        strcpy(msg->message, "Error reading enrolled courses");
        return;
    }

    struct student_course sc;
    struct course temp;
    int course_fd = open_file_with_lock(COURSE_FILE, O_RDONLY, LOCK_SH);
    if (course_fd < 0) {
        close_file_with_unlock(fd);
        msg->course_count = 0;
        return;
    }

    int count = 0;
    while (read(fd, &sc, sizeof(sc)) > 0) {
        if (strcmp(sc.roll_no, msg->student.roll_no) == 0) {
            lseek(course_fd, 0, SEEK_SET);
            while (read(course_fd, &temp, sizeof(temp)) > 0) {
                if (strcmp(temp.course_code, sc.course_code) == 0) {
                    msg->courseList[count++] = temp;
                    break;
                }
            }
        }
    }

    close_file_with_unlock(fd);
    close_file_with_unlock(course_fd);

    msg->course_count = count;
}

void handle_student_password_change(struct message *msg) {
    struct student temp;
    int fd = open_file_with_lock(STUDENT_FILE, O_RDWR, LOCK_EX);
    if (fd < 0) {
        strcpy(msg->message, "Error changing password");
        return;
    }

    while (read(fd, &temp, sizeof(temp)) > 0) {
        if (strcmp(temp.roll_no, msg->student.roll_no) == 0) {
            strcpy(temp.password, msg->student.password);
            lseek(fd, -sizeof(temp), SEEK_CUR);
            if (write(fd, &temp, sizeof(temp)) < 0) {
                perror("write():");
                close_file_with_unlock(fd);
                strcpy(msg->message, "Error changing password");
                return;
            }
            close_file_with_unlock(fd);
            strcpy(msg->message, "Password changed successfully");
            return;
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Student not found");
}

void handle_login(struct message *msg) {
    if (msg->type == ADMIN_LOGIN) {
        if (strcmp(msg->message, "admin123") == 0) {
            strcpy(msg->message, "Login successful");
        } else {
            strcpy(msg->message, "Invalid admin password");
        }
        return;
    }

    int fd;
    if (msg->type == FACULTY_LOGIN) {
        fd = open_file_with_lock(FACULTY_FILE, O_RDONLY, LOCK_SH);
    } else if (msg->type == STUDENT_LOGIN) {
        fd = open_file_with_lock(STUDENT_FILE, O_RDONLY, LOCK_SH);
    } else {
        strcpy(msg->message, "Invalid login type");
        return;
    }

    if (fd < 0) {
        strcpy(msg->message, "Error accessing file");
        return;
    }

    if (msg->type == FACULTY_LOGIN) {
        struct faculty f;
        while (read(fd, &f, sizeof(f)) > 0) {
            if (strcmp(f.fac_id, msg->faculty.fac_id) == 0 &&
                strcmp(f.password, msg->faculty.password) == 0) {
                strcpy(msg->message, "Login successful");
                close_file_with_unlock(fd);
                return;
            }
        }
    } else if (msg->type == STUDENT_LOGIN) {
        struct student s;
        while (read(fd, &s, sizeof(s)) > 0) {
            if (strcmp(s.roll_no, msg->student.roll_no) == 0 &&
                strcmp(s.password, msg->student.password) == 0) {
                strcpy(msg->message, "Login successful");
                close_file_with_unlock(fd);
                return;
            }
        }
    }

    close_file_with_unlock(fd);
    strcpy(msg->message, "Invalid credentials");
}
