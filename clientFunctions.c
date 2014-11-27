#include "clientFunctions.h"

char* dbName4 = "db";
int tmpClientId;
int resultId;

static int watchClientCallback(void *NotUsed, int argc, char **argv, char **azColName) {
	char* p1,* p2,* p3, *p4;
	int i;
	p1 = (char*)malloc(sizeof(char)*100);
	p2 = (char*)malloc(sizeof(char)*100);
	p3 = (char*)malloc(sizeof(char)*100);
	p4 = (char*)malloc(sizeof(char)*100);
	for(i = 0; i < argc; i+=4) {
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

static int watchCardsCallback(void *NotUsed, int argc, char **argv, char **azColName) {
	char* p1 = (char*)malloc(sizeof(char)*100);
	int i;
	printf("Account cards ids:\n");
	for (i = 0; i < argc; i++) {
		sprintf(p1, "%s", argv[0] ? argv[0] : "NULL");
		printf("card ¹ %s\n", p1);
	}
	return 0;
}

static int getClientByAcountCallback(void *NotUsed, int argc, char **argv, char **azColName) {
	char* p1 = (char*)malloc(sizeof(char)*100);
	sprintf(p1, "%s", argv[0] ? argv[0] : "NULL");
	tmpClientId = atoi(p1);
	return 0;
}

int watchClientAccounts(int clientId) {
	sqlite3 *database;
	char *zErrMsg = 0;
	char* command = (char*)malloc(sizeof(char)*100);
	int rc;
	if (sqlite3_open(dbName4, &database) == SQLITE_OK) {
		sprintf(command, "select accountid, balance, accountType, oberdraft_start from bank_accounts where clientid = '%d'", clientId);
		rc = sqlite3_exec(database, command, watchClientCallback, 0, &zErrMsg);
	}
	return 0;
}

int watchAccountBalance(int accountId, int clientId) {
	sqlite3 *database;
	char* command = (char*)malloc(sizeof(char)*100);
	char *zErrMsg = 0;
	int rc;
	if (sqlite3_open(dbName4, &database) == SQLITE_OK) {
		sprintf(command, "select balance from bank_accounts where accountid = '%d' and clientid = '%d'", accountId, clientId);
		rc = sqlite3_exec(database, command, watchBalanceCallback, 0, &zErrMsg);
	}
	return 0;
}

int watchAccountCards(int accountId, int clientId) {
	sqlite3 *database;
	char* command = (char*)malloc(sizeof(char)*100);
	char *zErrMsg = 0;
	int rc;
	if (sqlite3_open(dbName4, &database) == SQLITE_OK) {
		sprintf(command, "select clientid from bank_accounts where accountid = '%d'", accountId);
		rc = sqlite3_exec(database, command, getClientByAcountCallback, 0, &zErrMsg);
		if (tmpClientId == clientId) {
			sprintf(command, "select card_id from card where account_id = '%d'", accountId);
			rc = sqlite3_exec(database, command, watchCardsCallback, 0, &zErrMsg);
		}
	}
	return 0;
}