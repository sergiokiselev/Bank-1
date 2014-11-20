#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <iostream>
#include "sqlite3.h"
#include <cstdio>
#include <malloc.h>
#include <cmath>
#include <cstdlib>
#include <string.h>

char* resultPassword;
char* resultRole;
char* dbName = "db";
int resultId;

static int watchClientCallback(void *NotUsed, int argc, char **argv, char **azColName) {
	char* p1,* p2,* p3, *p4;
	p1 = (char*)malloc(sizeof(char)*100);
	p2 = (char*)malloc(sizeof(char)*100);
	p3 = (char*)malloc(sizeof(char)*100);
	p4 = (char*)malloc(sizeof(char)*100);
	for(int i = 0; i < argc; i+=4) {
		sprintf(p1, "%s", argv[i] ? argv[i] : "NULL");
		sprintf(p2, "%s", argv[i+1] ? argv[i+1] : "NULL");
		sprintf(p3, "%s", argv[i+2] ? argv[i+2] : "NULL");
		sprintf(p4, "%s", argv[i+3] ? argv[i+3] : "NULL");
		printf("Account Id: %s\nbalance: %s account type: %s overdraft start: %s\n", p1, p2, p3, p4);
	}
	return 0;
}

static int watchBalanceCallback(void *NotUsed, int argc, char **argv, char **azColName) {
	char* p1 = (char*)malloc(sizeof(char)*100);
	sprintf(p1, "%s", argv[0] ? argv[0] : "NULL");
	printf("balance is %s\n", p1);
	return 0;
}

void watchClientAccounts(int clientId, sqlite3* db) {
	char* command = (char*)malloc(sizeof(char)*100);
	char *zErrMsg = 0;
	int rc;
	sprintf(command, "select accountid, balance, accountType, oberdraft_start from bank_accounts where clientid = '%d'", clientId);
	rc = sqlite3_exec(db, command, watchClientCallback, 0, &zErrMsg);
}

void watchAccountBalance(int accountId, sqlite3* db) {
	char* command = (char*)malloc(sizeof(char)*100);
	char *zErrMsg = 0;
	int rc;
	sprintf(command, "select balance from bank_accounts where accountid = '%d'", accountId);
	rc = sqlite3_exec(db, command, watchBalanceCallback, 0, &zErrMsg);
}

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
//			addNewClient(login,password,3);
			break;
		case 2:
	//		deleteClient(2);
			break;
		case 3:
		//	addAccountToClient(login,password);
			break;
		case 4:
			//deleteAccountToClient(6);
			break;
		default:
			//printf("cannot find such operation");
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
	//	operationistPutMoneyToAccount(accountId, moneyValue);
		break;
	case 2:
		//operationTakeMoneyFromAccount(accountId, moneyValue);
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
		char* command = (char*)malloc(sizeof(char)*20);
		if (strcmp(resultRole, "1") == 0) {
			adminOperation();
		} else if (strcmp(resultRole, "2") == 0) {
			operationistOperation();
		} else if (strcmp(resultRole, "3") == 0) {
			while(1) {
				printf("Choose operation:\n1 - Exit\n2 - Watch client accounts\n3 - Watch account balance\n");
				scanf("%s", command);
				if (!strcmp(command, "1")) {
					break;
				} else if (!strcmp(command, "2")) {
					watchClientAccounts(resultId, db);
				} else if (!strcmp(command, "3")) {
					char* accountIdStr = (char*)malloc(sizeof(char)*20);
					int accountId;
					printf("Enter account id:\n");
					scanf("%s", accountIdStr);
					accountId = atoi(accountIdStr);
					watchAccountBalance(accountId ,db);	
				}
			}
		}
		
	}
	else {
		printf("Incorrect login and/or password.\n");
	}
	sqlite3_close(db);
	system("pause");
	return 0;
}