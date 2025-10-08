#include "Account.h"
#include "Utils.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>





static std::mt19937& rng(){
    static thread_local std::mt19937 gen ([]{
        std::random_device rd;
        std::seed_seq seed{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
        return std::mt19937 (seed);
    }());
    return gen;
}

std::string Account::random_digits(std::size_t len) {
    std::uniform_int_distribution<int> dist(0,9);
    std::string out;
    out.reserve(len);
    for(std::size_t i=0; i<len; ++i){
        out.push_back(static_cast<char>('0' + dist(rng())));
    }
    return out;

}



string Account::GetAccountNumber() const {
    return this->AccountNumber;
}



bool Account::SetOwner(Person p, std::string *err) {
    if(p.GetIdCode().empty()){
        if(err) *err = "Error! Id-Code is empty.";
        return false;
    }
    person = std::move(p);
    if(err) err->clear();
    return true;

}

bool Account::SetInitialBalance(long double amount, string *err ) {
    if(!is_finite_ld(amount)){
        if(err) *err = "Error! please enter finite amount.";
        return false;
    }
    if(amount < 0.0L){
        if(err) *err = "Error! amount must be positive.";
        return false;
    }

    this->Balance = amount;
    if(err) err->clear();
    return true;
}

bool Account::Deposit(long double amount,const Date& date, string *err) {
    if(Account_is_closed){
        if(err) *err = "Error! Account is already closed.";
        return false;
    }
    if(!is_finite_ld(amount) || amount < 0.0L){
        if(err) *err = "Error! deposit must be finite/positive number.";
        return false;
    }
    long double next = this->Balance + amount;
    if(!is_finite_ld(next)){
        if(err) *err = "Error! balance overflow!";
        return false;
    }

    this->Balance = next;

    if(!AppendTransaction(TransactionTypes::Deposit,amount,"Cash",
                          this->AccountNumber,date,err)){
        this->Balance -=amount;
        return false;
    }

    if(err) err->clear();
    return true;
}

bool Account::Withdraw(long double wd,const Date& date,string *err) {
    if(Account_is_closed){
        if(err) *err = "Error! Account is already closed.";
        return false;
    }

    if(!is_finite_ld(wd) || wd <= 0.0L){
        if(err) *err = "Error! withdraw should finite/positive number";
        return false;
    }

    constexpr long double EPS = 1e-12L;
    if(this->Balance + EPS < wd){
        if (err) *err = "Error! insufficient funds.";
        return false;
    }


    long double next = this->Balance - wd;
    if(!is_finite_ld(next)){
        if(err) *err = "Error! balance underflow!";
        return false;
    }

    this->Balance = next;

    if(!AppendTransaction(TransactionTypes::Withdraw,wd,
                          this->AccountNumber,"Cash",date,err)){
        this->Balance += wd;
        return false;
    }

    if(err) err->clear();
    return true;

}

long double Account::GetBalance() const {
    return this->Balance;
}

bool Account::SetAccountNumber(string *err) {
    this->AccountNumber = random_digits(10);
    if(err) err->clear();
    return true;
}

bool Account::SetAccountType(Account::AccountType t, string *err) {
    AccType = t;
    if(err) err->clear();
    return true;

}

Account::AccountType Account::GetAccountType() const {
    return AccType;
}

 const char *Account::AccountTypeToString(Account::AccountType t) {
    switch (t) {
        case AccountType::CheckingAccount: return "Checking";
        case AccountType::FixedDepositAccount: return "FixedDeposit";
        case AccountType::SavingAccount: return "Saving";

    }
    return "Unknown";
}



bool Account::SetOpeningsDate(const string &DAY, const string &MONTH, const string &YEAR, string *err) {
    auto fail = [&](const char* msg){
        if(err) *err = msg;
        return false;
    };

    string D = trim_copy(DAY);
    string M = trim_copy(MONTH);
    string Y = trim_copy(YEAR);

    if(!is_Digit(D))return fail("Error! day must be digits only.");
    if(!is_Digit(M))return fail("Error! month must be digits only.");
    if(!is_Digit(Y))return fail("Error! year must be digits only.");

    if(Y.size() > 4)return fail("Error! year must be 4 digits(e.g. 2025)");
    if(M.size() > 2)return fail("Error! month must be at most 2 digits.");
    if(D.size() > 2)return fail("Error! day must be at most 2 digits.");

    int yi = stoi(Y);
    int mi = stoi((M));
    int di = stoi(D);

    if(yi < 2025 )return fail("Error! year must be at least 2025.");
    if(mi < 1 || mi > 12)return fail("Error! month must be in range [1-12].");

    int dim = days_in_month(mi,yi);
    if(di < 1 || di > dim)return fail("Error! day out of range for given year/month");

    OpeningsDate.day = pad2(di);
    OpeningsDate.month = pad2(mi);
    OpeningsDate.year = Y;

    if(err) err->clear();
    return true;



}

const Account::Date &Account::GetOpeningsDate() const {
    return OpeningsDate;
}

void Account::DisplayAccountInfo() const {
   cout<<left<<setw(15)<<"Name:"<<person.GetName()<<endl;
   cout<<left<<setw(15)<<"FamilyName:"<<person.GetFamilyName()<<endl;
   cout<<left<<setw(15)<<"Id-Code:"<<person.GetIdCode()<<endl;
   cout<<left<<setw(15)<<"Account Number:"<<this->AccountNumber<<endl;
   cout<<left<<setw(15)<<"Balance:"<<this->Balance<<"$"<<endl;
   cout<<left<<setw(15)<<"Account Type:"<<Account::AccountTypeToString(AccType)<<endl;
   cout << left << setw(15) << "Openings Date:" << OpeningsDate.day << "/" << OpeningsDate.month << "/" << OpeningsDate.year << endl;
   if(this->Account_is_closed)
       cout<<left<<setw(15)<<"Account status:"<<"Closed"<<endl;
   else
       cout<<left<<setw(15)<<"Account status:"<<"Open"<<endl;

}

bool Account::Transfer(Account &Destination,long double amount,const Date& date, string *err) {
    auto fail = [&](const char* msg){
        if(err) *err = msg;
        return false;
    };

    if(this->Account_is_closed){
        return fail("Error! Account is already closed");
    }

    if(Destination.Account_is_closed){
        return fail("Error! Account of Destination is closed.");
    }

    if(this->AccountNumber == Destination.AccountNumber){
        return fail("Error! AccountNumber is same as Destination.");
    }

    if(!is_finite_ld(amount) || amount<=0.0L )return fail("Error! the amount must be finite positive number.");
    constexpr long double EPS = 1e-12L;
    if(this->Balance + EPS < amount)return fail("Error! insufficient funds.");

    if(amount > this->Balance)return fail("Error! Balance is not enough for Transfer.");
    if(!is_finite_ld(Destination.Balance + amount))return fail("Error! Destination is overflow!");


    this->Balance = this->Balance - amount;
    Destination.Balance = Destination.Balance + amount;

    size_t src_size_before = this->AccountTransactions.size();

    if(!this->AppendTransaction(TransactionTypes::TransferOut,amount,
                          this->AccountNumber,Destination.AccountNumber,date,err)){
        this->Balance += amount;
        Destination.Balance -= amount;
        return false;
    }
    size_t dst_size_before = Destination.AccountTransactions.size();

    if(!Destination.AppendTransaction(TransactionTypes::TransferIn,amount,
                                      this->AccountNumber,Destination.AccountNumber,date,err)){
        this->Balance += amount;
        Destination.Balance -= amount;
        this->AccountTransactions.resize(src_size_before);
        return false;
    }

    if(err) err->clear();
    return true;


}

bool Account::CloseAccount(string *err) {
    if(Account_is_closed){
        if(err) *err = "Error! Account is already closed.";
        return false;
    }

    if(this->Balance != 0.0L){
        if(err) *err = "Error! Balance must be zero.";
        return false;
    }

    Account_is_closed = true;
    if(err) err->clear();
    return true;



}

bool Account::TransactionValidation(const Account::Transaction &t, string *err) {
    if(!is_finite_ld(t.amount)){
        if(err) *err = "Error! the amount must be finite number.";
        return false;
    }
    if(t.amount <= 0){
        if(err) *err = "Error! amount must be positive number.";
        return false;
    }
    if(t.source.empty() || t.destination.empty()){
        if(err) *err = "Error! AccountNumber of source or destination is empty.";
        return false;
    }
    if(!is_finite_ld(t.balance_after)){
        if(err) *err = "Error! balance_after must be finite number.";
        return false;
    }

    if(t.balance_after < 0){
        if(err) *err = "Error! balance_after can not be negative.";
        return false;
    }

    if(err) err->clear();
    return true;





}

bool Account::AppendTransaction(Account::TransactionTypes type, long double amount, const string &source,
                                const string &destination,const Account::Date &date, string *err) {

      if(this->Account_is_closed){
          if(err) *err = "Error! Account is already closed.";
          return false;
      }
      Transaction t;
      t.amount = amount;
      t.source = source;
      t.destination = destination;
      t.type = type;
      t.trans = date;
      t.balance_after = this->Balance;

      if(TransactionValidation(t,err)){
          AccountTransactions.emplace_back(t);
          if(err) err->clear();
          return true;
      }
      return false;



}

const vector<Account::Transaction> &Account::GetTransactions() const {
    return this->AccountTransactions;
}

void Account::DisplayTransactions() const {
    for(const auto& it : this->AccountTransactions){
        cout<<left<<setw(15)<<"Date:"<<it.trans.day<<"/"<<it.trans.month<<"/"<<it.trans.year<<endl;
        cout<<left<<setw(15)<<"Amount:"<<it.amount<<endl;
        cout<<left<<setw(15)<<"Source:"<<it.source<<endl;
        cout<<left<<setw(15)<<"Destination:"<<it.destination<<endl;
        cout<<left<<setw(15)<<"TransactionType:"<<Account::TransactionTypeToString(it.type)<<endl;
        cout<<left<<setw(15)<<"Updated Balance:"<<it.balance_after<<endl;
        cout<<"--------------------------------------"<<endl;
    }


}

const char *Account::TransactionTypeToString(Account::TransactionTypes t) {
    switch (t) {
        case TransactionTypes::Deposit:return "Deposit";
        case TransactionTypes::Withdraw:return "Withdraw";
        case TransactionTypes::TransferIn:return "TransferIn";
        case TransactionTypes::TransferOut:return "TransferOut";
        case TransactionTypes::Open:return "Open";
        case TransactionTypes::Close:return "Close";

    }
    return "Unknown";

}

void Account::SaveToFile(ostream &os) const {
    os<<"Account Number:"<<this->AccountNumber<<endl;
    os<<"Balance:"<<this->Balance<<endl;
    os<<"AccountType:"<<Account::AccountTypeToString(AccType)<<endl;
    os<<"Status:"<<(Account::Account_is_closed ? "Closed":"Open")<<endl;
    os<<"OpeningsDate:"<<this->OpeningsDate.day<<"/"<<this->OpeningsDate.month<<"/"<<this->OpeningsDate.year<<endl;

    os<<"Number of Transactions:"<<this->AccountTransactions.size()<<endl;
    for(const auto& t: this->AccountTransactions){
        os<<"Date:"<<t.trans.day<<"/"<<t.trans.month<<"/"<<t.trans.year<<endl;
        os<<"Amount:"<<t.amount<<endl;
        os<<"Source:"<<t.source<<endl;
        os<<"Destination:"<<t.destination<<endl;
        os<<"TransactionType:"<<Account::TransactionTypeToString(t.type)<<endl;
        os<<"Updated Balance:"<<t.balance_after<<endl;
        os<<"---"<<endl;
    }
}























