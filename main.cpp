#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include "sqlite3.h"
#include "adminFunctions.c"
#include "operationistFunctions.c"

char* resultPassword;
char* resultRole;
char* dbName = "db";

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	resultPassword = (char*)malloc(sizeof(char)* 100);
	resultRole = (char*)malloc(sizeof(char)* 100);
	sprintf(resultPassword, "%s", argv[0] ? argv[0] : "NULL");
	sprintf(resultRole, "%s", argv[1] ? argv[1] : "NULL");
	return 0;
}



void adminOperation() {
	int menuItem;
	printf("Admin operation: 1 - add client, 2 - delete client, 3 - add account, 4 - delete account. \n");
	scanf("%d",&menuItem);
	printf("Enter login and password of client:");
	char* login, *password;
	login = (char*)malloc(sizeof(char)* 100);
	password = (char*)malloc(sizeof(char)* 100);
	scanf("%s %s", login, password);
	switch (menuItem) {
		case 1:
			addNewClient(login,password,3);
			break;
		case 2:
			deleteClient(2);
			break;
		case 3:
			addAccountToClient(login,password);
			break;
		case 4:
			deleteAccountToClient(6);
			break;
		default:
			printf("cannot find such operation");
			break;
	}
}

void operationistOperation() {
	int menuItem, accountId, moneyValue;

	printf("Choose operation:\n1. Put money to account.\n2. Take money from account.\n");
	scanf("%d", &menuItem);
	printf("Input accountId to what put money.\n");
	scanf("%d", &accountId);
	printf("Input money value which put to account.\n");
	scanf("%d", &moneyValue);

	switch (menuItem)
	{
	case 1:
		operationistPutMoneyToAccount(accountId, moneyValue);
		break;
	case 2:
		operationTakeMoneyFromAccount(accountId, moneyValue);
		break;
	default:
		printf("Incorrect menu item.");
		break;
	}
}

int main(int argc, char **argv){
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char* login, *password;
	char* selectCommand = (char*)malloc(sizeof(char)* 100);
	login = (char*)malloc(sizeof(char)* 100);
	password = (char*)malloc(sizeof(char)* 100);

	rc = sqlite3_open(dbName, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		system("pause");
		return(1);
	}

	printf("Enter login and password\n");
	scanf("%s %s", login, password);
	sprintf(selectCommand, "select password, role from user where login = '%s'", login);
	rc = sqlite3_exec(db, selectCommand, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	if (strcmp(resultPassword, password) == 0) {
		if (strcmp(resultRole, "1") == 0) {
			adminOperation();
		}
		else if (strcmp(resultRole, "2") == 0) {
			operationistOperation();
		}
		
	}
	else {
		printf("Incorrect login and/or password.\n");
	}
	sqlite3_close(db);
	system("pause");
	return 0;
}