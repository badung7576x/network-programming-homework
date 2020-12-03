#define MAXCHAR 100
#define ACTIVE_CODE "LTM121216"
#define ACTIVE 1
#define BLOCK 0
#define IDLE 2
#define LIMIT_INCORRECT 3
#define ONLINE 1
#define OFFLINE 0

typedef struct Acc {
    char username[MAXCHAR];
    char password[MAXCHAR];
    int status;
    int passIncorrect;
    int activeIncorrect;
    int isAlive;
    char homepage[MAXCHAR];
    char homepage_c[MAXCHAR];
    struct Acc *next;
} Account;


int getInput(char inputStr[MAXCHAR], char *result);
Account *createAccount(char username[MAXCHAR], char password[MAXCHAR], int status, char homepage[MAXCHAR]);
void addAccount(Account **head, Account *node);
void checkAccountStatus(Account *acc);
int hasAccount(char un[MAXCHAR]);
int checkPass(Account *account, char password[MAXCHAR]);
Account *getAccount(char username[MAXCHAR]);
void registerAccount();
void activeAccount();
void signIn();
void search();
void changePassword();
void logOut();
void showDomain();
void showIp();