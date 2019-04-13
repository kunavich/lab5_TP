#define _CRT_SECURE_NO_WARNINGS
#include "SQL_functions.h"
#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include < string.h >


sqlite3 *db = NULL;
char *err = NULL;
char* retStr;
char* buf;

void itoa(int number, char* str) {
	sprintf(str, "%d", number);
}

char verifyLogin(char* login, char* password) {
	char* sql = (char*)malloc(64);
	sqlite3_stmt *res;
	strcpy(sql, "select * from BANK_USERS where Login=? AND Password=?");
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	free(sql);
	if (rc == SQLITE_OK)
	{
		sqlite3_bind_text(res, 1, login, -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(res, 2, password, -1, SQLITE_TRANSIENT);
	}
	else {
		fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
		return ERR;
	}

	if (sqlite3_step(res) == SQLITE_ROW) {
		int columnCnt = sqlite3_column_count(res);
		int i;
		for (i = 0; i < columnCnt; ++i)
			if (!strcmp(sqlite3_column_name(res, i), "Role"))
			{
				int k = sqlite3_column_int(res, i);
				sqlite3_finalize(res);
				return k;
			}
	}
	return ERR;
}

void freeAllSQL() {
	if (retStr != NULL)
		free(retStr);
	if (err != NULL)
		free(err);
	free(buf);
}
void createNewAccount(char* login, char* passwd, char* FirstName, char* LastName, char* Passport, char* Address, char* DateOfBirth, char* PhoneNumber) {
	char* sql = (char*)malloc(4098);
	strcpy(sql, "INSERT INTO BANK_CLIENTS (Login, FirstName, LastName, Passport, Address, DateOfBirth, PhoneNumber) VALUES ('");
	strcat(sql, login);
	strcat(sql, "', '");
	strcat(sql, FirstName);
	strcat(sql, "', '");
	strcat(sql, LastName);
	strcat(sql, "', '");
	strcat(sql, Passport);
	strcat(sql, "', '");
	strcat(sql, Address);
	strcat(sql, "', '");
	strcat(sql, DateOfBirth);
	strcat(sql, "', '");
	strcat(sql, PhoneNumber);
	strcat(sql, "');");
	sqlite3_exec(db, sql, 0, 0, &err);
	strcpy(sql, "INSERT INTO BANK_USERS (Login, Password, Role) VALUES ('");
	strcat(sql, login);
	strcat(sql, "', '");
	strcat(sql, passwd);
	strcat(sql, "', 3);");
	sqlite3_exec(db, sql, 0, 0, &err);
	free(sql);
}
char tryCreateAccount(char* login) {
	char* sql = (char*)malloc(64);
	sqlite3_stmt *res;
	strcpy(sql, "select * from BANK_USERS where Login=?");
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
	free(sql);
	if (rc == SQLITE_OK)
		sqlite3_bind_text(res, 1, login, -1, SQLITE_TRANSIENT);
	else {
		fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
		return ERR;
	}
	if (sqlite3_step(res) == SQLITE_ROW) {
		return ERR;
	}
	return 1;
}
char withdrawCash(char* login, char* passwd, char* withdrawMethood, int bankAccount, int summ) {
	int b = summ;
	if (!strcmp(withdrawMethood, "cash")) {
		char* sql = (char*)malloc(200);
		strcpy(sql, "update BANK_ACCOUNTS set Balance=Balance-");
		itoa(summ, sql + strlen(sql));
		strcat(sql, " where ID=");
		itoa(bankAccount, sql + strlen(sql));
		strcat(sql, " AND Login='");
		strcat(sql, login);
		strcat(sql, "';");
		sqlite3_exec(db, sql, 0, 0, &err);
		return sqlite3_changes(db);
	}
	return 0;
}
char addCash(char* login, char* passwd, char* addMethood, int bankAccount, int summ) {
	int b = summ;
	if (!strcmp(addMethood, "cash")) {
		char* sql = (char*)malloc(200);
		strcpy(sql, "update BANK_ACCOUNTS set Balance=Balance+");
		itoa(summ, sql + strlen(sql));
		strcat(sql, " where ID=");
		itoa(bankAccount, sql + strlen(sql));
		strcat(sql, " AND Login='");
		strcat(sql, login);
		strcat(sql, "';");
		sqlite3_exec(db, sql, 0, 0, &err);
		return sqlite3_changes(db);
	}
	return 0;
}


int printActualBalanceCB(void *arg, int argc, char **argv, char **azColName) {
	int i;
	int ID = -1;
	int Login = -1;
	for (i = 0; i < argc; i++) {
		if (!strcmp(azColName[i], "ID")) {
			ID = i;
		}
		if (!strcmp(azColName[i], "Login"))
			Login = i;
		if (Login != -1 && ID != -1)
			break;
	}
	strcat(retStr, "\n\n___Accound ID: ");
	strcat(retStr, argv[ID]);
	strcat(retStr, "__\n");
	for (i = 0; i < argc; i++)
		if (i != ID && i != Login) {
			sprintf(buf, "%-20s:%40s\n", azColName[i], argv[i]);
			strcat(retStr, buf);
		}
	return 0;
}
char* getActualBlance(char* login) {
	retStr = (char*)malloc(5000);
	retStr[0] = 0;
	char* sql = (char*)malloc(100);
	strcpy(sql, "select * from BANK_ACCOUNTS where Login='");
	strcat(sql, login);
	strcat(sql, "';");
	if (sqlite3_exec(db, sql, printActualBalanceCB, 0, &err))
	{
		sprintf(retStr, "Error SQL: %s", err);
		sqlite3_free(err);
	}
	return retStr;
}
char payToTarget(char* login, char* passwd, char* bankAccount, char* target, char* summ) { return 1; }

void initAllSQL() {
	sqlite3_open("Bank.db", &db);
	buf = (char*)malloc(200);
};


int printClientsUserCB(void *arg, int argc, char **argv, char **azColName) {
	int i;
	int Login = -1;
	for (i = 0; i < argc; i++)
		if (!strcmp(azColName[i], "Login")) {
			sprintf(buf, "\n\n______________Account of %-20s______________\n", argv[i]);
			strcat(retStr, buf);
			Login = i;
			break;
		}
	for (i = 0; i < argc; i++)
		if (i != Login) {
			sprintf(buf, "%-20s:%40s\n", azColName[i], argv[i]);
			strcat(retStr, buf);
		}
	return 0;
}
char* getAllClients() {
	retStr = (char*)malloc(5000);
	retStr[0] = 0;
	if (sqlite3_exec(db, "select * from BANK_CLIENTS", printClientsUserCB, 0, &err))
	{
		sprintf(retStr, "Error SQL: %s", err);
		sqlite3_free(err);
	}
	return retStr;
};
int printAccountsUserCB(void *arg, int argc, char **argv, char **azColName) {
	int i;
	int ID = -1;
	int Login = -1;
	char buf[200];
	for (i = 0; i < argc; i++) {
		if (!strcmp(azColName[i], "Login")) {
			sprintf(buf, "\n\n___________________Account of %-10s___________________\n", argv[i]);
			strcat(retStr, buf);
			Login = i;
		}
		if (!strcmp(azColName[i], "ID"))
			ID = i;
		if (ID != -1 && Login != -1)
			break;
	}
	for (i = 0; i < argc; i++)
		if (i != ID && i != Login) {
			char buf[200];
			sprintf(buf, "%-15s:%40s\n", azColName[i], argv[i]);
			strcat(retStr, buf);
		}
	return 0;
}
char* getAllAccounts() {
	retStr = (char*)malloc(5000);
	retStr[0] = 0;
	if (sqlite3_exec(db, "select * from BANK_ACCOUNTS", printClientsUserCB, 0, &err))
	{
		sprintf(retStr, "Error SQL: %s", err);
		sqlite3_free(err);
	}
	return retStr;
};


char* getAllClientsAdmin() {
	return getAllAccounts();
};
char* getAllAccountsAdmin() {
	return getAllClients();
};
char deleteClientAdmin(char* login, char* mylogin, char* mypassword) {
	if (verifyLogin(mylogin, mypassword) == ADMIN)
		return 0;
	char* sqlP = (char*)malloc(100);
	strcpy(sqlP, "where login = '");
	strcpy(sqlP, login);
	strcpy(sqlP, "';");

	char* sql = (char*)malloc(100);
	strcpy(sql, "delete from BANK_CLIENTS ");
	strcat(sql, sqlP);
	sqlite3_exec(db, sql, 0, 0, &err);
	strcpy(sql, "delete from BANK_USERS ");
	strcat(sql, sqlP);
	sqlite3_exec(db, sql, 0, 0, &err);
	strcpy(sql, "delete from BANK_ACCOUNTS ");
	strcat(sql, sqlP);
	sqlite3_exec(db, sql, 0, 0, &err);
	free(sql);
	free(sqlP);
	return 1;

};

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
	int PerTransactionFee) {
	char* numbuf = (char*)malloc(20);
	char* sql = (char*)malloc(4098);
	strcpy(sql, "INSERT INTO BANK_ACCOUNTS (Login, Balance, isSaving, isOverdraftEnable, DateOfCreate, TotalTransactions) VALUES ('");
	strcat(sql, login);
	strcat(sql, "', ");
	itoa(balance, numbuf);
	strcat(sql, numbuf);

	if (isSaving == '0')
		strcat(sql, ", 'YES', ");
	else
		strcat(sql, ", 'NO', ");

	if (isOverdraftEnable == '0')
		strcat(sql, "'YES', '");
	else
		strcat(sql, "'NO', '");

	strcat(sql, DateOfCreate);
	strcat(sql, "', 0);");

	sqlite3_exec(db, sql, 0, 0, &err);

	strcpy(sql, "INSERT INTO BANK_CONFIG (InterestRate, MaxNumberOfTransactions, MaxOverdraftSumm, MaxOverdraftPeriod, FinePerDay, PerTransactionFee) VALUES ( ");
	itoa(InterestRate, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ", ");
	itoa(TotalTransaction, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ", ");
	itoa(MaxOverdraftSumm, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ", ");
	itoa(MaxOverdraftPeriod, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ", ");
	itoa(FinePerDay, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ", ");
	itoa(PerTransactionFee, numbuf);
	strcat(sql, numbuf);
	strcat(sql, ");");
	sqlite3_exec(db, sql, 0, 0, &err);
	free(sql);
}
