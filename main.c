#include "sqlite3.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "adminFunctions.h"
#include "operationistFunctions.h"
#include "clientFunctions.h"
//#include "unsignedFunctions.h"

char* resultPassword;
char resultRole;
char* dbName = "db";
int resultId;



static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	resultPassword = (char*)malloc(sizeof(char)* 100);
	resultRole = (char*)malloc(sizeof(char)* 100);
	sprintf(resultPassword, "%s", argv[0] ? argv[0] : "NULL");
	sprintf(resultRole, "%s", argv[1] ? argv[1] : "NULL");
	return 0;
}

void adminOperation() {
	int menuItem;
	char* login, *password;
	printf("Admin operation: 1 - add client, 2 - delete client, 3 - add account, 4 - delete account. \n");
	scanf("%d",&menuItem);
	printf("Enter login and password of client:");
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
			addAccountToClient(login,password,1111,2,55);
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

extern sqlite3 *dataBase;
sqlite3 *db;


void clientOperation() {
	int command = -1;

    printf("Choose operation:\n1 - Exit\n2 - Watch client accounts\n3 - Watch account balance\n");
    scanf("%s", command);
    if (!strcmp(command, "1")) {
		return;
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

int main() {
	if (sqlite3_open(dbName, &dataBase)) {
		fprintf(stderr, "Can't open database.\n Error: %s\n", sqlite3_errmsg(dataBase));
		sqlite3_close(dataBase);
		return 1;
	}
	unauthorizedRole();
	return 0;
}
