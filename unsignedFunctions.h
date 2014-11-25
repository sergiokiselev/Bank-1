
#ifndef _UNSIGNED_USER_
#define _UNSIGNED_USER_

int openDataBase();
int closeDataBase();

void adminOperation();
void operationistOperation();
void clientOperation();

#define DATABASE_ERROR 1
#define ADMIN_ROLE '1'
#define OPERATOR_ROLE '2'
#define CLIENT_ROLE '3'
#define ROLES_NUMBER 3

int unauthorizedRole();

void menu(int itemNum, char* menuItem[], void*(*menuFunctions[])());

#endif _UNSIGNED_USER_