#define MAXCHAR 100
#define ACTIVE 1
#define BLOCK 0
#define IDLE 2
#define LIMIT_INCORRECT 3
#define ONLINE 1
#define OFFLINE 0
#define FALSE 0
#define TRUE 1
#define LOGIN_BLOCK 100
#define LOGIN_IDLE 200
#define LOGIN_SUCCESS 300
#define LOGIN_FAILURE 400

typedef struct Acc {
    int userno;
    char username[MAXCHAR];
    char password[MAXCHAR];
    int status;
    int passIncorrect;
    int activeIncorrect;
    int isAlive;
    struct Acc *next;
} Account;



int getInput(char buffer[MAXCHAR], char *result);
int checkString(char *str);
Account *createAccount(char username[MAXCHAR], char password[MAXCHAR], int status);
void addAccount(Account **head, Account *node);
int hasAccount(char un[MAXCHAR]);
int checkPass(Account *account, char password[MAXCHAR]);
Account *getAccount(char username[MAXCHAR]);
void registerAccount();
void activeAccount();
int signIn(char username[MAXCHAR], char password[MAXCHAR]);
int changePassword(char username[MAXCHAR], char newpass[MAXCHAR]);
void encodePass(char *str, char *result);
void logout(char username[MAXCHAR]);
