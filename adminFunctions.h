
void addAccountToClient(char* login, char* password, int pin, int accountType, int overdraft);

void deleteAccountToClient(int id);

void addNewClient(char* login, char* password, int role);

void deleteClient(int id);

void addNewCard(char* login, char* password, int accountid, int pin);

void deleteCard(int id);

void showTransactions(int count);