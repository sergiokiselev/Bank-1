#include "sqlite3.h"

#ifndef _UTIL_
#define _UTIL_
char* dataBaseName = "db";
sqlite3* dataBase;

int openDataBase();
int closeDataBase();

#endif _UTIL_