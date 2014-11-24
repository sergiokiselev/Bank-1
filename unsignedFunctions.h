
#ifndef _UNSIGNED_USER_
#define _UNSIGNED_USER_

void adminOperation();

void operationistOperation();

void clientOperation();


char* dataBaseName = "db";
sqlite3* dataBase;

#define DATABASE_ERROR 1
int error_code = 1;
#define ADMIN_ROLE '1'
#define OPERATOR_ROLE '2'
#define CLIENT_ROLE '3'
#define ROLES_NUMBER 3

char roles[ROLES_NUMBER] = { ADMIN_ROLE, OPERATOR_ROLE, CLIENT_ROLE };

char login[32];
char password[32];
char userRole;

char* resultPassword;
char resultRole;


int unauthorizedRole();
int error();

void* rolesFunctions[ROLES_NUMBER] = { *adminOperation, *operationistOperation, *clientOperation };

char authentication(char* login, char* password);
void* authorization();
void* showBankInfo();
void* registerNewUser();
void* logIn();

static int fillResult(void *NotUsed, int argc, char **argv, char **azColName);


#define MAIN_MENU_ITEM_NUM 3

char* mainMenuItem[] = { "LogIn", "Register", "BankInfo" };
void* (*mainMenuFunctions[])() = { *logIn, *registerNewUser, *showBankInfo };


void menu(int itemNum, char* menuItem[], void*(*menuFunctions[])());

#endif _UNSIGNED_USER_