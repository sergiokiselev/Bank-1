#include "stdio.h"
#include "sqlite3.h"
#include <malloc.h>
#include <string.h>
#include "unsignedFunctions.h"


char* dataBaseName = "db";
sqlite3* dataBase;

//sqlite3* openDataBase(char* dataBaseName, sqlite3* dataBase) {
int openDataBase() {
	if (sqlite3_open(dataBaseName, &dataBase)) {
		fprintf(stderr, "Can't open database.\n Error: %s\n", sqlite3_errmsg(dataBase));
		sqlite3_close(dataBase);
		return 1;
	}
	return 0;
};
int closeDataBase() {
	if (dataBase == NULL) {
		return 0;
	}
	if (sqlite3_close(dataBase)) {
		fprintf(stderr, "Can't close database.\n Error: %s\n", sqlite3_errmsg(dataBase));
		return 1;
	}
	return 0;
};

char roles[ROLES_NUMBER] = { ADMIN_ROLE, OPERATOR_ROLE, CLIENT_ROLE };
void* rolesFunctions[ROLES_NUMBER] = { *adminOperation, *operationistOperation, *clientOperation };

char login[32];
char password[32];
char userRole;

char* resultPassword;
char resultRole;

char authentication(char* login, char* password);
void* authorization();
int logIn();
int registerNewUser();
int showBankInfo();

static int fillResult(void *NotUsed, int argc, char **argv, char **azColName);

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
	resultPassword = (char*)malloc(sizeof(char)* 32);

	if (argv[0]) {
		sprintf(resultPassword, "%s", argv[0]);
	}
	else {
		resultPassword = NULL;
	}

	if (argv[1]) {
		resultRole = argv[1][0];
	}
	else {
		resultPassword = NULL;
	}
	return 0;
}

char authentication(char* login, char* password) {
	char getUserSelect[255];
	char* errorMessage = 0;
	if (strlen(login) > MAX_LOGIN_LINGTH && strlen(password) > MAX_PASSWORD_LINGTH) {
		return AUTHENTICATION_ERROR;
	}
	sprintf(getUserSelect, "SELECT password, role FROM user WHERE login = '%s'", login);
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
	return NO_SUCH_USER;
}

void* authorization() {
	int i;
	printf("Enter login: ");
	gets(login);
	printf("Enter password: ");
	gets(password);
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



