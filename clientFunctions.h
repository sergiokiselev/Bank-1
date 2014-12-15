//#include "sqlite3.h"

int watchClientAccounts(int clientId);

int watchAccountBalance(int accountId, int clientId);

int watchAccountCards(int accountId, int clientId);

int sendMoneyToAnotherAccount(int clientId, int fromAccountId, int toAccountId, int sum);
