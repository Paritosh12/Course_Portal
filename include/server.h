#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

void handle_add_student(struct message *msg);
void handle_add_faculty(struct message *msg);
void handle_view_student(struct message *msg);
void handle_view_faculty(struct message *msg);
void handle_activate_student(struct message *msg);
void handle_block_student(struct message *msg);
void handle_modify_student(struct message *msg);
void handle_modify_faculty(struct message *msg);
void handle_add_course(struct message *msg);
void handle_remove_course(struct message *msg);
void handle_update_course(struct message *msg);
void handle_view_offering_courses(struct message *msg);
void handle_faculty_password_change(struct message *msg);
void handle_view_all_courses(struct message *msg);
void handle_enroll_course(struct message *msg);
void handle_drop_course(struct message *msg);
void handle_view_enrolled_courses(struct message *msg);
void handle_student_password_change(struct message *msg);
void handle_login(struct message *msg);

#endif