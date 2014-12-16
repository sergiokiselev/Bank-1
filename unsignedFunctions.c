#include "stdio.h"
#include "sqlite3.h"
#include <string.h>
#include <stdlib.h>
#include "unsignedFunctions.h"
#include "util.h"

#ifndef _WIN32
#include <unistd.h>
#endif

extern char* dataBaseName;
extern sqlite3* dataBase;
extern resultId;


#define DATABASE_ERROR 1
#define ADMIN_ROLE '1'
#define OPERATOR_ROLE '2'
#define CLIENT_ROLE '3'
#define AUTHENTICATION_ERROR 'e'
#define NO_SUCH_USER 'u'
#define ROLES_NUMBER 3

#define MAX_LOGIN_LENGTH 32
#define MAX_PASSWORD_LENGTH 32


char* resultPassword;
char resultRole;

char roles[ROLES_NUMBER] = { ADMIN_ROLE, OPERATOR_ROLE, CLIENT_ROLE };
void* rolesFunctions[ROLES_NUMBER] = { *adminOperation, *operationistOperation, *clientOperation };


#define MAIN_MENU_ITEM_NUM 3
char* mainMenuItem[] = { "LogIn", "Register", "BankInfo" };
int (*mainMenuFunctions[])() = { *logIn, *registerNewUser, *showBankInfo };


int unauthorizedRole() {
	menu(MAIN_MENU_ITEM_NUM, mainMenuItem, mainMenuFunctions);
	return 0;
}

int error() {
	printf("loggin error\n");
	return 0;
}

static int fillResult(void *NotUsed, int argc, char **argv, char **azColName) {
	char* resultIdStr = (char*)malloc(sizeof(char)* 10);
	resultPassword = (char*)malloc(sizeof(char)* 32);

	if (argv[0]) {
		sprintf(resultPassword, "%s", argv[0]);
	} else {
		resultPassword = NULL;
	}

	if (argv[1]) {
		resultRole = argv[1][0];
	} else {
		resultPassword = NULL;
	}

	if (argv[2]) {
		resultId = atoi(argv[2]);
	} else {
		resultId = 0;
	}
	return 0;
}

char authentication(char* login, char* password) {
	char getUserSelect[255];
	char* errorMessage = 0;

	if (strlen(login) > MAX_LOGIN_LENGTH && strlen(password) > MAX_PASSWORD_LENGTH) {
		return AUTHENTICATION_ERROR;
	}
	sprintf(getUserSelect, "SELECT password, role, user_id FROM user WHERE login = '%s'", login);
    if (dataBase == NULL) {
        if (openDataBase() == SQLITE_OK) {
            return AUTHENTICATION_ERROR;
        }
    }
	if (sqlite3_exec(dataBase, getUserSelect, fillResult, 0, &errorMessage) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
	}

	if (!strcmp(resultPassword, password)) {
		return resultRole;
	}


//	free(resultPassword);
	resultPassword = " \0";
	return NO_SUCH_USER;
}

void* authorization() {
	char login[MAX_LOGIN_LENGTH];
#ifndef _WIN32
	char* password;
#else
	char password[MAX_PASSWORD_LENGTH];
#endif
	char userRole;
	int i;

	printf("Enter login: ");
	gets(login);



#ifndef _WIN32
	password = getpass("Enter password: ");
#else
    printf("Enter password: ");
	for (i = 0; i < MAX_PASSWORD_LENGTH; i++) {
		password[i] = _getch();
		if (password[i] == '\r' || password[i] == '\n' || password[i] == ' ') {
			password[i] = '\0';
			putchar('\n');
			break;
		}
		if (password[i] == 8) {
			if (i > 0) {
				i--;
			}
			_getch();
		}
		
	}
#endif
	userRole = authentication(login, password);
    
	for (i = 0; i < ROLES_NUMBER; ++i) {
		if (userRole == roles[i]) {
			return rolesFunctions[i];
		}
	}

	return *error;
}

int showBankInfo() {
	printf("LLC \"Bank T\"\n");
	printf("There will be some information about bank.\n");
	return 0;
}

int registerNewUser() {
	char login[MAX_LOGIN_LENGTH];
	char password[MAX_PASSWORD_LENGTH];
	char getUserSelect[255];
	char* errorMessage = 0;

	printf("Enter login: ");
	gets(login);
	printf("Enter password: ");
	gets(password);
	sprintf(getUserSelect, "INSERT INTO registration (login, password) VALUES ('%s', '%s');", login, password);
	if (sqlite3_exec(dataBase, getUserSelect, 0, 0, &errorMessage) != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errorMessage);
		sqlite3_free(errorMessage);
	}
	return 0;
}

int logIn() {
	void(*userFunction) (void);
	userFunction = (void(*)()) authorization();
	userFunction();
	return 0;
}


void menu(int itemNum, char* menuItem[], void*(*menuFunctions[])()) {
	int command = -1;
	char buffer[100];
	void* (*function) (void);
	int(*userFunction) (void);
	int i;

	while (command) {
		printf("Input command:\n");
		printf("  0. Exit\n");
		for (i = 0; i < itemNum; ++i) {
			printf("  %d. %s\n", i + 1, menuItem[i]);
		}
		scanf("%d", &command);
		gets(buffer);
		if (command > 0 && command <= itemNum) {
			function = menuFunctions[command - 1];
			function();
		} else if (command != 0) {
			printf("Invalid command.\n");
		}
	}
}



