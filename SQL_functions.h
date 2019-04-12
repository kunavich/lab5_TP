#define _CRT_SECURE_NO_WARNINGS
#define ADMIN 1
#define USER 2
#define CLIENT 3
#define ERR 0
char verifyLogin(char* login, char* password);
char tryCreateAccount(char* login);
void createNewAccount(char* login, char* passwd, char* FirstName, char* LastName, char* Passport, char* Address, char* DateOfBirth, char* PhoneNumber);

char payToTarget(char* login, char* passwd, char* bankAccount, char* target, char* summ);
char withdrawCash(char* login, char* passwd, char* withdrawMethood, int bankAccount, int summ);
char addCash(char* login, char* passwd, char* addMethood, int bankAccount, int summ);
char* getActualBlance(char* login);

void createAccount(char* login,
	int  balance,
	char isSaving,
	char isOverdraftEnable,
	char* DateOfCreate,
	int TotalTransaction,
	int InterestRate,
	int MaxOverdraftSumm,
	int MaxOverdraftPeriod,
	int FinePerDay,
	int PerTransactionFee);

char* getAllClients();
char* getAllClientsAdmin();
char* getAllAccountsAdmin();
char deleteClientAdmin(char* login, char* mylogin, char* mypassword);
void freeAllSQL();
void initAllSQL();
