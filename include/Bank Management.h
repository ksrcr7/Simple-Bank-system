

#ifndef BANK_ACCOUNT_BANK_MANAGEMENT_H
#define BANK_ACCOUNT_BANK_MANAGEMENT_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "Person.h"
#include "Account.h"

using Date = Account::Date;

class Management{
private:
    unordered_map<string,Account> KeepAccounts;
    unordered_map<string,vector<string>> AccountsByOwner;
    unordered_map<string,Person> MembersById;


public:
    bool OpenAccount(const Person&,long double,Account::AccountType,const Date&,string* err = nullptr);
    bool CloseAccount(const string&,const string&,const Date&,string* err = nullptr);
    bool DepositAccount(const string&,long double,const Date&,string* err = nullptr);
    bool WithdrawFromAccount(const string&,long double,const Date&,string* err = nullptr);
    bool TransferBetweenAccounts(const string&,const string&,long double,const Date&,string* err = nullptr);
    bool AddPerson(const Person&,string* err = nullptr);




};


























#endif //BANK_ACCOUNT_BANK_MANAGEMENT_H
