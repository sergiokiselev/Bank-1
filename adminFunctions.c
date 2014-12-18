#include "stdio.h"
#include "sqlite3.h"
#include "adminFunctions.h"
#include "util.h"

extern sqlite3 *dataBase;
extern char* dataBaseName;

void addAccountToClient(char* login, char* password, int pin, int accountType, int overdraft) {
	int primaryKey = -1; 
	int client_id = -1;
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	sqlite3_stmt *statement;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
       const char *sql = "SELECT MAX(accountid) FROM bank_accounts";
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               client_id = sqlite3_column_int(statement, 0);
           }
	   }
    }
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		char *sql = (char*)malloc(sizeof(char)*200);
		sprintf(sql, "SELECT user_id FROM user where login = '%s' AND password = '%s'", login, password);
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               primaryKey = sqlite3_column_int(statement, 0);
           }
	   }
    }
	if (primaryKey == -1) {
		printf("Sqlite error!");
		return;
	}
	sprintf(insertCommand, "insert into bank_accounts values(%d, 0, %d, %d, %d, %d);",++client_id , pin, primaryKey, accountType, overdraft);   
	rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
    printf("added account with id:%d for client with login:%d, pin:%d, account type:%d, overdraft:%d", client_id, login, pin, accountType, overdraft);
}


void deleteAccountToClient(int id) {
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		sprintf(insertCommand, "delete from BANK_ACCOUNTS where accountid = %d",id); 
		rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
		if (rc1 != SQLITE_OK) {
			printf("Sqlite error!");
		}
	}
    printf("deleted account by id:%d", id);
}


void addNewClient(char* login, char* password, int role) {
	int primaryKey; 
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
       const char *sql = "SELECT MAX(user_id) FROM user";
       sqlite3_stmt *statement;
       if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
           while (sqlite3_step(statement) == SQLITE_ROW) {
               primaryKey = sqlite3_column_int(statement, 0);
           }
	   }
    }
	sprintf(insertCommand, "insert into user values(%d, '%s', '%s', %d);",++primaryKey , login, password, role);   
	rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
    printf("added new client with id:%d, login:%d, password:%d, role:%d");
}

void deleteClient(int id) {
	int rc1;
	sqlite3 *database;
	char* insertCommand = (char*)malloc(sizeof(char)*200);
	char *zErrMsg = 0;
	if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
		sprintf(insertCommand, "delete from user where user_id = %d",id); 
		rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
		if (rc1 != SQLITE_OK) {
			printf("Sqlite error!");
		}
	}
    printf("deleted client by id:%d", id);

}


void addNewCard(char* login, char* password, int accountid, int pin) {
    int primaryKey;
    int rc1;
    sqlite3 *database;
    char* insertCommand = (char*)malloc(sizeof(char)*200);
    char *zErrMsg = 0;
    if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
        const char *sql = "SELECT MAX(card_id) FROM card";
        sqlite3_stmt *statement;
        if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                primaryKey = sqlite3_column_int(statement, 0);
            }
        }
    }
    sprintf(insertCommand, "insert into card values(%d, '%s', '%s', %d);",++primaryKey , accountid, pin);
    rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
    printf("added new card for client with id = %d, login:%d, account id = %d , pin = %d", primaryKey, login, accountid, pin);
}

void deleteCard(int id) {
    int rc1;
    sqlite3 *database;
    char* insertCommand = (char*)malloc(sizeof(char)*200);
    char *zErrMsg = 0;
    if (sqlite3_open(dataBaseName, &database) == SQLITE_OK) {
        sprintf(insertCommand, "delete from card where card_id = %d",id);
        rc1 = sqlite3_exec(database, insertCommand, 0, 0, &zErrMsg);
        if (rc1 != SQLITE_OK) {
            printf("Sqlite error!");
        }
    }
    printf("deleted card by id:%d", id);
}


void showTransactions(int count) {
	char getTransactionsSelect[255];
	char* errorMessage = 0;
	char* result = (char*)malloc(sizeof(char)* 255);
	sqlite3_stmt *statement;

	sprintf(getTransactionsSelect, "SELECT * FROM transactions ORDER BY id ASC limit %d;", count);
	if (sqlite3_prepare_v2(dataBase, getTransactionsSelect, -1, &statement, NULL) == SQLITE_OK) {
		printf("%s\t%s\t%s\t%s\n", sqlite3_column_name(statement, 0), sqlite3_column_name(statement, 1), sqlite3_column_name(statement, 2), sqlite3_column_name(statement, 3));
		while (sqlite3_step(statement) == SQLITE_ROW) {
			printf("%s\t%s\t\t%s\t%s\n", sqlite3_column_text(statement, 0), sqlite3_column_text(statement, 1), sqlite3_column_text(statement, 2), sqlite3_column_text(statement, 3));
		}
	}

}