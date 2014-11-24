#include "clientFunctions.h"

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