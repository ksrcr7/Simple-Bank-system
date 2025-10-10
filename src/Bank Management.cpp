#include "Bank Management.h"
#include "Utils.h"
#include <algorithm>


bool Management::OpenAccount(const Person &person, long double initial_balance, Account::AccountType type,
                             const Date &date, string *err) {

    if(err) err->clear();

    if(person.GetIdCode().empty()){
        if(err) *err = "Error! IdCode is empty.";
        return false;
    }


    if(!AddPerson(person,err)){
        return false;
    }

    if(!is_finite_ld(initial_balance) || initial_balance <= 0){
        if(err) *err = "Error! initial balance must be finite positive.";
        return false;
    }

    Account NewAccount;

    do{
        if(!NewAccount.SetAccountNumber(err)){
            return false;
        }
    } while (KeepAccounts.find(NewAccount.GetAccountNumber()) != KeepAccounts.end());


    if(!NewAccount.SetOwner(person,err))return false;
    if(!NewAccount.SetInitialBalance(initial_balance,err))return false;
    if(!NewAccount.SetAccountType(type,err))return false;
    if(!NewAccount.SetOpeningsDate(date.day,date.month,date.year,err))return false;

    if(!NewAccount.AppendTransaction(Account::TransactionTypes::Open,initial_balance,"Cash",NewAccount.GetAccountNumber(),
                                     date,err))return false;

    const string AccNum = NewAccount.GetAccountNumber();
    KeepAccounts.emplace(AccNum,NewAccount);
    AccountsByOwner[person.GetIdCode()].push_back(AccNum);

    return true;

}

bool Management::AddPerson(const Person &p,string* err) {
    if(p.GetIdCode().empty()){
        if(err) *err = "Error! IdCode is empty.";
        return false;
    }

    if(MembersById.find(p.GetIdCode()) != MembersById.end()){
        if(err) *err = "Error! this IdCode is already exists.";
        return false;
    }

    MembersById[p.GetIdCode()] = p;
    if(err) err->clear();
    return true;



}

bool Management::CloseAccount(const string &owner_id, const string &account_number, const Date &date, string *err) {
    if(owner_id.empty()){
        if(err) *err = "Error! Id is empty.";
        return false;
    }

    if(account_number.empty()){
        if(err) *err = "Error! AccountNumber is empty.";
        return false;
    }

    auto itlist = AccountsByOwner.find(owner_id);
    if(itlist == AccountsByOwner.end()){
        if(err) *err = "Error! owner has no accounts.";
        return false;
    }

    auto itowner = MembersById.find(account_number);
    if(itowner == MembersById.end()){
        if(err) *err = "Error! owner not found.";
        return false;
    }

    const auto& accounts = itlist->second;
    if(find(accounts.begin(),accounts.end(),account_number) == accounts.end()){
        if (err) *err = "Error! account does not belong to this owner.";
        return false;
    }

    auto itAcc = KeepAccounts.find(account_number);
    if(itAcc == KeepAccounts.end()){
        if (err) *err = "Error! account not found.";
        return false;
    }

    Account& acc = itAcc->second;
    if(!acc.is_closed()){
        if (err) *err = "Error! account is already closed.";
        return false;
    }

    constexpr long double EPS = 1e-12L;
    if(fabsl(acc.GetBalance()) > EPS){
        if (err) *err = "Error! balance must be zero before closing.";
        return false;
    }

    if(!acc.AppendTransaction(Account::TransactionTypes::Close,0.0L,acc.GetAccountNumber(),"Closed",date,err)){
        return false;
    }

    acc.SetClosed(true);
    if(err) err->clear();
    return true;

}

bool Management::DepositAccount(const string &account_number, long double amount, const Date &date, string *err) {
    if(account_number.empty()){
        if(err) *err = "Error! AccountNumber is empty.";
        return false;
    }

    auto it = KeepAccounts.find(account_number);
    if(it == KeepAccounts.end()){
        if(err) *err = "Error! Account is not found.";
        return false;
    }

    Account& acc = it->second;

    if (!acc.Deposit(amount, date, err)) {
        return false;
    }

    if(err) err->clear();
    return true;




}

bool Management::WithdrawFromAccount(const string &account_number, long double amount, const Date &date, string *err) {

    if(account_number.empty()){
        if(err) *err = "Error! AccountNumber is empty.";
        return false;
    }

    auto it = KeepAccounts.find(account_number);
    if(it == KeepAccounts.end()){
        if(err) *err = "Error! Account is not found.";
        return false;
    }

    Account& acc = it->second;

    if(!acc.Withdraw(amount,date,err))return false;

    if(err) err->clear();
    return true;

}

bool Management::TransferBetweenAccounts(const string &SourceAccNum, const string &DestinationAccNum,
                                         long double amount, const Date &date, string *err) {

    if(SourceAccNum.empty()){
        if(err) *err = "Error! Source Account Number is empty.";
        return false;
    }

    if(DestinationAccNum.empty()){
        if(err) *err = "Error! Destination Account Number is empty.";
        return false;
    }

    auto ItSource = KeepAccounts.find(SourceAccNum);
    auto ItDestination = KeepAccounts.find(DestinationAccNum);

    if(ItSource == KeepAccounts.end()){
        if(err) *err = "Error! Source Account is not found.";
        return false;
    }

    if(ItDestination == KeepAccounts.end()){
        if(err) *err = "Error! Destination Account is not found.";
        return false;
    }

    Account& acc1 = ItSource->second;
    Account& acc2 = ItDestination->second;

    if(!acc1.Transfer(acc2,amount,date,err))return false;

    if(err) err->clear();
    return true;



}



