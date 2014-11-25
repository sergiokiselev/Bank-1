#include "stdio.h"
#include "sqlite3.h"
#include "operationistFunctions.h"
//#include "util.h"

extern sqlite3 *dataBase;
extern char* dataBaseName;


void operationistPutMoneyToAccount(int accountId, double moneyValue){

	sqlite3 *db;
	sqlite3_stmt *statement;
	char * zErrMsg = 0;
	char* selectCommand = (char*)malloc(sizeof(char)* 100);
	double balance = -1;
	int rc = 0;

	if (sqlite3_open(dataBaseName, &db) == SQLITE_OK) {
		sprintf(selectCommand, "select balance from bank_accounts where accountid = %d", accountId);
		if (sqlite3_prepare_v2(db, selectCommand, -1, &statement, NULL) == SQLITE_OK) {
			while (sqlite3_step(statement) == SQLITE_ROW) {
				balance = sqlite3_column_double(statement, 0);
			}

			if (balance == -1) {
				printf("User with accountId = %d doesn't exist.\n", accountId);
				sqlite3_close(db);
				return;
			}

			balance += moneyValue;
			sprintf(selectCommand, "update bank_accounts set balance = %f where accountid = %d", balance, accountId);
			rc = sqlite3_exec(db, selectCommand, 0, 0, &zErrMsg);

			if (rc != SQLITE_OK) {
				printf("SQL update error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				sqlite3_close(db);
				return;
			}
			else {
				printf("Money were put to accountid = %d\n", accountId);
				sqlite3_close(db);
			}
		}
	}
}
void operationTakeMoneyFromAccount(int accountId, double moneyValue){

	sqlite3 *db;
	sqlite3_stmt *statement;
	char * zErrMsg = 0;
	char* selectCommand = (char*)malloc(sizeof(char)* 100);
	double balance = -1;
	int rc = 0;
	if (sqlite3_open(dataBaseName, &db) == SQLITE_OK) {
		sprintf(selectCommand, "select balance from bank_accounts where accountid = %d", accountId);
		if (sqlite3_prepare_v2(db, selectCommand, -1, &statement, NULL) == SQLITE_OK) {
			while (sqlite3_step(statement) == SQLITE_ROW) {
				balance = sqlite3_column_double(statement, 0);
			}

			if (balance == -1) {
				printf("User with accountId = %d doesn't exist.\n", accountId);
				sqlite3_close(db);
				return;
			}

			if (balance < moneyValue) { balance = 0; }
			else { balance -= moneyValue; }

			sprintf(selectCommand, "update bank_accounts set balance = %f where accountid = %d", balance, accountId);
			rc = sqlite3_exec(db, selectCommand, 0, 0, &zErrMsg);

			if (rc != SQLITE_OK) {
				printf("SQL update error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				sqlite3_close(db);
				return;
			}
			else {
				printf("Money were taken from accountid = %d\n", accountId);
				sqlite3_close(db);
			}
		}
	}
}