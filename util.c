#include "sqlite3.h"
#include "adminFunctions.h"
#include "operationistFunctions.h"
#include "clientFunctions.h"
#include <stdio.h>

#ifndef STOP
#define STOP
char* dataBaseName = "db";
sqlite3* dataBase;


int resultId;


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


void adminOperation() {
	char* login, *password;
    int role, id ,pin, accountType, overdraft,countTransactions;
    char* command = (char*)malloc(sizeof(char)* 10);
    while(1) {
        printf("\n\nAdmin operations:\n  0 - exit.\n  1 - add client,\n  2 - delete client,\n  3 - add account,\n  4 - delete account,\n  5 - add new card,\n  6 - delete card,\n  7 - show transaction,\n  8 - show users\n");
        scanf("%s", command);
        if (!strcmp(command, "0")) {
            return;
        }
        else if (!strcmp(command, "1")) {
            printf("Enter login and password of client:");
            login = (char*)malloc(sizeof(char)* 100);
            password = (char*)malloc(sizeof(char)* 100);
            scanf("%s %s", login, password);
            printf("Input role for client.\n");
            scanf("%d",&role);
            addNewClient(login,password,role);
        }
        else if (!strcmp(command, "2")) {
            printf("Input client id.\n");
            scanf("%d", &id);
            deleteClient(id);
        }
        else if (!strcmp(command, "3")) {
            printf("Enter login and password of client:");
            login = (char*)malloc(sizeof(char)* 100);
            password = (char*)malloc(sizeof(char)* 100);
            scanf("%s %s", login, password);
            printf("Input pin\n");
            scanf("%d", &pin);
            printf("Input account type\n");
            scanf("%d", &accountType);
            printf("Input overdraft\n");
            scanf("%d", &overdraft);
            addAccountToClient(login,password, pin, accountType, overdraft);
        }
        else if (!strcmp(command, "4")) {
            printf("Enter login and password of client:");
            login = (char*)malloc(sizeof(char)* 100);
            password = (char*)malloc(sizeof(char)* 100);
            scanf("%s %s", login, password);
            printf("Input account id.\n");
            scanf("%d", &id);
            deleteAccountToClient(id);
        }
        else if (!strcmp(command, "5")) {
            printf("Enter login and password of client:");
            login = (char*)malloc(sizeof(char)* 100);
            password = (char*)malloc(sizeof(char)* 100);
            scanf("%s %s", login, password);
            printf("Input pin\n");
            scanf("%d", &pin);
            printf("Input account id\n");
            scanf("%d", &id);
            addNewCard(login,password,id,pin);
        }
        else if (!strcmp(command, "6")) {
            printf("Enter login and password of client:");
            login = (char*)malloc(sizeof(char)* 100);
            password = (char*)malloc(sizeof(char)* 100);
            scanf("%s %s", login, password);
            printf("Input card id\n");
            scanf("%d", &id);
            deleteCard(id);
        }
        else if (!strcmp(command, "7")) {
            printf("Input count of transactions to show\n");
            scanf("%d", &countTransactions);
            showTransactions(countTransactions);
        } 
        else if (!strcmp(command, "8")) {
            showUsers();
        }
    }
}

void operationistOperation() {
	int menuItem, accountId, moneyValue;
	char* command = (char*)malloc(sizeof(char)* 10);
	while(1) {
		printf("\nChoose operation:\n1. Put money to account.\n2. Take money from account.\n3. Exit.\n");
		scanf("%s", command);
		if (!strcmp(command, "3")) {
			return;
		}
		else if (!strcmp(command, "1")) {
			printf("Input accountId to what put money.\n");
			scanf("%d", &accountId);
			printf("Input money value which put to account.\n");
			scanf("%d", &moneyValue);
			operationistPutMoneyToAccount(accountId, moneyValue);
		}
		else if (!strcmp(command, "2")) {
			printf("Input accountId to what put money.\n");
			scanf("%d", &accountId);
			printf("Input money value which take from account.\n");
			scanf("%d", &moneyValue);
			operationTakeMoneyFromAccount(accountId, moneyValue);
		}
	}
}


void clientOperation() {
	char* command = (char*)malloc(sizeof(char)* 10);
	char* accountIdStr = (char*)malloc(sizeof(char)* 20);
	int accountId;
	int sum;
	int toAccountId;

	while(1) {
		printf("\nChoose operation:\n  0. Exit\n  1. Watch client accounts\n  2. Watch account balance\n  3. Watch acount cards\n  4. Send money to account\n");
		scanf("%s", command);
		if (!strcmp(command, "0")) {
			return;
		} else if (!strcmp(command, "1")) {
			watchClientAccounts(resultId);
		} else if (!strcmp(command, "2")) {
			printf("Enter account id:\n");
			scanf("%s", accountIdStr);
			accountId = atoi(accountIdStr);
			watchAccountBalance(accountId, resultId);	
		} else if (!strcmp(command, "3")) {
			printf("Enter account id:\n");
			scanf("%s", accountIdStr);
			accountId = atoi(accountIdStr);
			watchAccountCards(accountId, resultId);
		} else if (!strcmp(command, "4")) {
			printf("Enter from account id:\n");
			scanf("%s", accountIdStr);
			accountId = atoi(accountIdStr);
			printf("Enter to account id:\n");
			scanf("%s", accountIdStr);
			toAccountId = atoi(accountIdStr);
			printf("Enter to sum:\n");
			scanf("%s", accountIdStr);
			sum = atoi(accountIdStr);
			sendMoneyToAnotherAccount(resultId, accountId, toAccountId, sum);
		}
	}
}

#endif
