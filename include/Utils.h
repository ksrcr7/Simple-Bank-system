

#ifndef BANK_ACCOUNT_UTILS_H
#define BANK_ACCOUNT_UTILS_H

#include "Account.h"
#include <algorithm>
#include <cmath>

static inline bool is_finite_ld(long double x){
    return std::isfinite(x);
}

static inline string trim_copy(const string& id){
    auto start = find_if(id.begin(),id.end(),[]
            (unsigned char ch){return !isspace(ch);});
    auto end = find_if(id.rbegin(),id.rend(),[]
            (unsigned char ch){return !isspace(ch);}).base();

    if(start >= end)return {};
    return string(start,end);
}

static inline bool is_Digit(const string& id){
    return std::all_of(id.begin(),id.end(),
                       [](unsigned char ch){return isdigit(ch);});

}

static inline bool is_leap(int y){
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

static inline int days_in_month(int m, int y){
    switch (m) {
        case 1:case 3:case 5:case 7:case 8:case 10:case 12:return 31;
        case 4:case 6:case 9:case 11:return 30;
        case 2:return is_leap(y)? 29:28;
        default:return 0;

    }

}



static inline Account::AccountType parse_account_type_str(const std::string& s){
    if (s == "Checking")     return Account::AccountType::CheckingAccount;
    if (s == "Savings")      return Account::AccountType::SavingAccount;
    if (s == "FixedDeposit") return Account::AccountType::FixedDepositAccount;
    throw std::runtime_error("Unknown AccountType: " + s);
}

static inline Account::TransactionTypes parse_tx_type_str(const std::string& s){
    if (s == "Deposit")     return Account::TransactionTypes::Deposit;
    if (s == "Withdraw")    return Account::TransactionTypes::Withdraw;
    if (s == "TransferIn")  return Account::TransactionTypes::TransferIn;
    if (s == "TransferOut") return Account::TransactionTypes::TransferOut;
    throw std::runtime_error("Unknown TransactionType: " + s);
}





static inline string pad2(int v){
    char buf[3];
    std::snprintf(buf, sizeof(buf),"%02d",v);
    return string(buf);
}

















#endif //BANK_ACCOUNT_UTILS_H
