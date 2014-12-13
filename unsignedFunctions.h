
int openDataBase();
int closeDataBase();

void adminOperation();
void operationistOperation();
void clientOperation();
int unauthorizedRole();

char authentication(char* login, char* password);

#define DATABASE_ERROR 1
#define ADMIN_ROLE '1'
#define OPERATOR_ROLE '2'
#define CLIENT_ROLE '3'
#define AUTHENTICATION_ERROR 'e'
#define NO_SUCH_USER 'u'
#define ROLES_NUMBER 3

#define MAX_LOGIN_LINGTH 32
#define MAX_PASSWORD_LINGTH 32


void menu(int itemNum, char* menuItem[], void*(*menuFunctions[])());


