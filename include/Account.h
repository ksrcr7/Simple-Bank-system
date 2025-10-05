#ifndef BANK_ACCOUNT_ACCOUNT_H
#define BANK_ACCOUNT_ACCOUNT_H

#include "Person.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Account{
public:
    Account()=default;
    enum class TransactionTypes{Deposit,Withdraw,TransferIn,TransferOut,Open,Close};
    enum class AccountType{CheckingAccount,SavingAccount,FixedDepositAccount};

    struct Date{
        string day,month,year;
    };
    struct Transaction{
        Date trans;
        long double amount{};
        string source,destination;
        TransactionTypes type;
        long double balance_after{};

    };


    static std::string random_digits(std::size_t len=10);
    [[nodiscard]] string GetAccountNumber()const;
    [[nodiscard]] long double GetBalance()const;
    [[nodiscard]] AccountType GetAccountType()const;
    [[nodiscard]] static const char* AccountTypeToString(AccountType);
    [[nodiscard]] const Date& GetOpeningsDate()const;
    void DisplayAccountInfo()const;

    bool SetOwner(Person,std::string *err= nullptr);
    bool SetInitialBalance(long double,string *err = nullptr);
    bool SetAccountNumber(string* err = nullptr);
    bool SetOpeningsDate(const string&,const string&,const string&,string* err = nullptr);
    bool Deposit(long double,string* err = nullptr);
    bool Withdraw(long double,string* err = nullptr);
    bool SetAccountType(AccountType,string* err = nullptr);
    bool Transfer(Account&,long double,string* err = nullptr);
    bool CloseAccount(string* err = nullptr);
    bool AppendTransaction(TransactionTypes,long double,const string&,const string&,const Date&,string* err = nullptr);





private:
    string AccountNumber;
    Person person;
    long double Balance{};
    AccountType AccType{AccountType::CheckingAccount};
    Date OpeningsDate;
    bool is_closed = {false};
    vector<Transaction> AccountTransactions;
    bool TransactionValidation(const Transaction&,string* err = nullptr);

};






















#endif //BANK_ACCOUNT_ACCOUNT_H
