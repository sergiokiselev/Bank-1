#include "sqlite3.h"
#include "adminFunctions.h"
#include "operationistFunctions.h"
#include "clientFunctions.h"


char* dataBaseName = "db";
sqlite3* dataBase;

int openDataBase();
int closeDataBase();

void adminOperation();
void clientOperation();
void operationistOperation();
