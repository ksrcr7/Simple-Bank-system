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
