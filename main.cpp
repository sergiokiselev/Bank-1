#include <stdio.h>
#include <iostream>
#include "sqlite3.h"

char* resultPassword;
char* resultRole;
char* dbName = "d://TP/db";

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

void operationistPutMoneyToAccount(int accountId, double moneyValue){

	sqlite3 *db;
	sqlite3_stmt *statement;
	char * zErrMsg = 0;
	char* selectCommand = (char*)malloc(sizeof(char)*100);
	double balance = -1;
	int rc = 0;

	if (sqlite3_open(dbName, &db) == SQLITE_OK) {
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
		   rc = sqlite3_exec(db, selectCommand, callback, 0, &zErrMsg);

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
	char* selectCommand = (char*)malloc(sizeof(char)*100);
	double balance = -1;
	int rc = 0;
	if (sqlite3_open(dbName, &db) == SQLITE_OK) {
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
		   rc = sqlite3_exec(db, selectCommand, callback, 0, &zErrMsg);

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
int main(int argc, char **argv){
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
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

	//operationistPutMoneyToAccount(1, 350);
	operationTakeMoneyFromAccount(1, 250);
	/*printf("Enter login and password\n");
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
	}*/
	sqlite3_close(db);
	system("pause");
	return 0;
}