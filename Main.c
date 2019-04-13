#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "SQL_functions.h"

#define cls system("cls")

#define maxEnterPath 80

#define REGISTER 4

char* headerINA = "  *********** Welcome to Bank System! ***********\n\n";
char* login;
char* passwd;


void stdclear() {
	while (getchar() != '\n');
}

void getstring(char* buf) {
	char c;
	int i = 0;
	scanf("%c", &c);
	while (c != '\n') {
		buf[i++] = c;
		scanf("%c", &c);
	}
	buf[i] = 0;
}

void getPassword(char* buf) {
	int i = 0;
	char symb;
	for (i = 0; (symb = _getch()) != 13; ++i)
		buf[i] = symb;
	buf[i] = 0;
}

char printStartMenu() {
	cls;
	printf("%s   Press 1 to login\n   Press 2 to create account\n   Press 0 to close program\n", headerINA);
	char command;
	command = getchar();
	stdclear();
	while (command < '0' || command > '2') {
		printf("No such command, please enter 1 or 2\n");
		command = getchar();
		stdclear();
	}
	return command - 48;
}

void printAdminMenu() {
	cls;
	printf("  1 - Show all clients\n   2 - Show all accounts\n   3 - delete user\n   0 - logout\n");
	char command;
	char *bankAccount = (char*)malloc(80);
	int summ;
	while ((command = getchar()) != '0') {
		stdclear();
		char* str = "";
		switch (command - 48)
		{
		case 1: printf("   All clients: \n%s\n\n", str = getAllClientsAdmin()); break;
		case 2: printf("   All accounts: \n%s\n\n", str = getAllAccountsAdmin()); break;
		case 3: {
			printf("\nEnter login of client: ");
			char* lg = (char*)malloc(40);
			if (!deleteClientAdmin(lg, login, passwd))
				printf("No such client with login %s", lg);
			else
				printf("Delete successfully done!\n\n");
			free(lg);
		}break;
		default:
			printf("No such command, enter 1, 2, 3 or 0 to logout\n");
		}
		free(str);
	}
	stdclear();
}

void printUserMenu() {
	cls;
	printf("  1 - Show all clients\n   2 - Show all accounts\n   3 - Create client account\n   0 - logout\n\n");
	char command;
	char *bankAccount = (char*)malloc(80);
	int summ;
	while ((command = getchar()) != '0') {
		stdclear();
		char* str;
		switch (command - 48) {
		case 1: printf("   All clients: \n%s\n\n", str = getAllClients()); free(str); break;
		case 2: printf("   All accounts: \n%s\n\n", str = getAllAccounts()); free(str); break;
		case 3: {
			printf("   Creating new account:\n\n");
			char* login_ = (char*)malloc(40);
			int  balance;
			char isSaving;
			char isOverdraftEnable;
			char* DateOfCreate = (char*)malloc(40);
			int TotalTransaction;
			int InterestRate;
			int MaxOverdraftSumm;
			int MaxOverdraftPeriod;
			int FinePerDay;
			int PerTransactionFee;

			printf("\n   Enter login: ");
			getstring(login_);
			while (tryCreateAccount(login_) == 1) {
				printf("No such client. Enter correct login: ");
				getstring(login_);
			}
			printf("   Enter start balance: ");
			scanf("%d", &balance);
			stdclear();
			printf("   Enter 0 to enable saving: ");
			scanf("%c", &isSaving);
			stdclear();
			printf("   Enter 0 to enable Overdraft: ");
			scanf("%c", &isOverdraftEnable);
			stdclear();
			printf("   Enter date: ");
			getstring(DateOfCreate);
			printf("   Enter max transaction count: ");
			scanf("%d", &TotalTransaction);
			stdclear();
			printf("   Enter interest rate: ");
			scanf("%d", &InterestRate);
			stdclear();
			printf("   Enter max overdraft summ: ");
			scanf("%d", &MaxOverdraftSumm);
			stdclear();
			printf("   Enter max overdraft period: ");
			scanf("%d", &MaxOverdraftPeriod);
			stdclear();
			printf("   Enter fine per day: ");
			scanf("%d", &FinePerDay);
			stdclear();
			printf("   Enter per transaction fee: ");
			scanf("%d", &PerTransactionFee);
			stdclear();

			createAccount(login_, balance, isSaving, isOverdraftEnable, DateOfCreate, TotalTransaction, InterestRate, MaxOverdraftSumm, MaxOverdraftPeriod, FinePerDay, PerTransactionFee);
			free(login_);
			free(DateOfCreate);


		}break;
		default:
			printf("No such command, enter 1, 2, 3 or 0 to logout");
		}

	}
}

void printClientMenu() {
	cls;
	printf("   1 - Payment\n   2 - Withdraw cash\n   3 - Add cash\n   4 - Balance\n   0 - logout");
	char command;
	int bankAccount;
	int summ;
	while ((command = getchar()) != '0') {
		stdclear();

		switch (command - 48)
		{
		case 1: {
			char* target = (char*)malloc(80);
			printf("\nEnter target account: ");
			getstring(target);
			printf("Enter your bank account: ");
			scanf("%d", &bankAccount);
			stdclear();
			scanf("Enter summ: %d", &summ);
			stdclear();
			if (!payToTarget(login, passwd, bankAccount, target, summ))
				printf("Transaction error. Wrong data.");
			else
				printf("\nSucces!");
			free(target);
		}break;
		case 2: {
			char* withdrawMethood = (char*)malloc(80);
			printf("Enter withdraw cash methood: ");
			getstring(withdrawMethood);
			printf("Enter your bank account: ");
			scanf("%d", &bankAccount);
			stdclear();
			printf("Enter summ: ");
			scanf("%d", &summ);
			stdclear();
			if (!withdrawCash(login, passwd, withdrawMethood, bankAccount, summ))
				printf("Transaction error. Wrong data.");
			else
				printf("\nSucces!");
			free(withdrawCash);
		}break;
		case 3: {
			char* addMethood = (char*)malloc(80);
			int summ;
			printf("Enter add cash methood: ");
			getstring(addMethood);
			printf("Enter your bank account: ");
			scanf("%d", &bankAccount);
			stdclear();
			printf("Enter summ: ");
			scanf("%d", &summ);
			stdclear();
			if (!addCash(login, passwd, addMethood, bankAccount, summ))
				printf("Transaction error. Wrong data.");
			else
				printf("\nSucces!");
			free(addMethood);
		}break;
		case 4: {
			char* balance = getActualBlance(login);
			printf("Your balance: \n%s", balance);
			free(balance);
		}break;
		}
	}
}

void printRegisterMenu() {
	cls;
	printf("%s   Creating new account\n\n", headerINA);
	while (1) {
		printf("   Enter login: ");
		getstring(login);
		if (!tryCreateAccount(login))
			printf("This login and password already exists.\nTry a different login.\n\n");
		else
			break;
	}
	printf("   Enter password: ");
	getPassword(passwd);
	char* FirstName = (char*)malloc(40);
	char* LastName = (char*)malloc(40);
	char* Passport = (char*)malloc(40);
	char* Address = (char*)malloc(100);
	char* DateOfBirth = (char*)malloc(40);
	char* PhoneNumber = (char*)malloc(40);
	printf("\n   Enter your Name: ");
	getstring(FirstName);
	printf("   Enter your Surname: ");
	getstring(LastName);
	printf("   Enter your passport number: ");
	getstring(Passport);
	printf("   Enter your address: ");
	getstring(Address);
	printf("   Enter your date of birth: ");
	getstring(DateOfBirth);
	printf("   Enter your phone number: ");
	getstring(PhoneNumber);
	createNewAccount(login, passwd, FirstName, LastName, Passport, Address, DateOfBirth, PhoneNumber);
	free(FirstName);
	free(LastName);
	free(Passport);
	free(Address);
	free(DateOfBirth);
	free(PhoneNumber);
}

char printLoginMenu() {
	cls;
	printf(headerINA);
	while (1) {
		printf("   Enter login: ");
		getstring(login);
		printf("   Enter password: ");
		getPassword(passwd);
		char result = verifyLogin(login, passwd);
		if (result != ERR)
			return result;
		printf("No such user!\nPlease enter correct login and password!\nPress Enter to try again or 0 to register:");
		char command = getchar();
		if (command == '0') {
			stdclear();
			return REGISTER;
		}
	}
}

void freeAllUses() {
	free(login);
	free(passwd);
	freeAllSQL();
}

void initAllUses() {
	login = (char*)malloc(maxEnterPath);
	passwd = (char*)malloc(maxEnterPath);
	initAllSQL();
}

int main() {
	initAllUses();
	char lb;
	while (lb = printStartMenu()) {
		if (lb)
			switch (printLoginMenu()) {
			case ADMIN: printAdminMenu(); break;
			case USER: printUserMenu(); break;
			case CLIENT: printClientMenu(); break;
			case REGISTER: printRegisterMenu(); break;
			}
		else
			printRegisterMenu();
	}
	return 0;
}
