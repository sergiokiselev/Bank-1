#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "sqlite3.h"
#include <iostream>


char* resultPassword;
char* resultRole;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	resultPassword = (char*)malloc(sizeof(char)*100);
	resultRole = (char*)malloc(sizeof(char)*100);
	sprintf(resultPassword, "%s", argv[0] ? argv[0] : "NULL");
	sprintf(resultRole, "%s", argv[1] ? argv[1] : "NULL");
	return 0;
}

void adminOperation() {
	printf("Admin operation\n");
}

void operationistOperation() {
	printf("Operationist operation\n");
}

int main(int argc, char **argv){
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char* dbName = "db";
	char* login, *password;
	char* selectCommand = (char*)malloc(sizeof(char)*100);
	login = (char*)malloc(sizeof(char)*100);
	password = (char*)malloc(sizeof(char)*100);

	rc = sqlite3_open(dbName, &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		system("pause");
		return(1);
	}
	printf("Enter login and password\n");
	scanf("%s %s",login, password);
	sprintf(selectCommand, "select password, role from admin where login = '%s'", login);
	rc = sqlite3_exec(db, selectCommand, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

	}
	if (strcmp(resultPassword, password) == 0) {
		printf("Choose operation:\n 1 - exit\n");
		if (strcmp(resultRole, "1") == 0) {

			adminOperation();
		} else if (strcmp(resultRole, "2") == 0) {
			operationistOperation();
		}
	}
	sqlite3_close(db);
	system("pause");
	return 0;
}