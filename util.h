#include "sqlite3.h"
#include "adminFunctions.h"
#include "operationistFunctions.h"
#include "clientFunctions.h"

extern char* dataBaseName;
int resultId;

int openDataBase();
int closeDataBase();

void adminOperation();
void clientOperation();
void operationistOperation();
