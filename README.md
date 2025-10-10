# Simple-Bank-system

# 🏦 Bank Account Management System (C++ Project)

A complete **bank account management** system written in modern **C++17**, designed to simulate the core logic of a real banking backend.  
It provides full functionality for **creating, managing, and tracking bank accounts and transactions**, including validation, ownership control, and persistence-ready structure.

---

## 🚀 Features

### 🧍 Person Class
Manages personal data and validation:
- Fields: name, family name, nationality, gender, ID code, birthdate.
- Validations:
  - Names only contain letters and spaces.
  - ID code numeric only, up to 5 digits.
  - Birthdate fully validated (year, month, leap years, etc.).
- Provides full getters/setters and formatted info output.

---

### 💳 Account Class
Represents a single bank account with full transaction tracking.
- Stores:
  - Account number (unique 10-digit random)
  - Balance
  - Account type (Checking / Savings / Business)
  - Owner (`Person`)
  - Status (Open / Closed)
  - Opening date
  - Transaction history
- Supports:
  - Deposit / Withdraw / Transfer
  - Account opening & closing
  - Transaction logging with details (amount, source, destination, date, updated balance)
- Fully validated operations:
  - Prevent overdrafts, invalid amounts, and closed-account actions.

---

### 🏛 Bank Management Class
The main controller for all users and accounts.
- Uses efficient data structures:
  - `unordered_map<string, Account>` → all accounts
  - `unordered_map<string, vector<string>>` → account list by owner
  - `unordered_map<string, Person>` → registered members
- Core operations:
  - AddPerson → register new customer  
  - OpenAccount → create a new account  
  - DepositToAccount / WithdrawFromAccount / TransferBetweenAccounts  
  - CloseAccount → safely close active account  
  - Keep all relations consistent between members and accounts

---

## 📂 Project Structure

Bank_account/
│
├── include/
│ ├── Person.h
│ ├── Account.h
│ └── Management.h
│
├── src/
│ ├── Person.cpp
│ ├── Account.cpp
│ └── Management.cpp
│
├── test/
│ └── main.cpp
│
├── CMakeLists.txt
└── README.md

---

## ⚙️ Technologies
- **Language:** C++17  
- **Build:** CMake  
- **IDE:** JetBrains CLion  
- **Libraries:** `<regex>`, `<unordered_map>`, `<vector>`, `<iomanip>`, `<algorithm>`, `<cmath>`, `<fstream>`

---

## 🧪 Example Usage
```cpp
int main() {
    Management bank;
    Person p;
    p.SetName("John");
    p.SetFamilyName("Doe");
    p.SetNationality("German");
    p.SetIdCode("12345");

    Account::Date date{"01", "10", "2025"};
    std::string err;

    bank.OpenAccount(p, 1000.0, Account::AccountType::Checking, date, &err);
    std::cout << err << std::endl;

    bank.DepositToAccount("1234567890", 200.0, date, &err);
    bank.WithdrawFromAccount("1234567890", 50.0, date, &err);
    bank.TransferBetweenAccounts("1234567890", "9876543210", 100.0, date, &err);
}

Author

[Kasra Bakhshaei]
Dentistry Student & Software Developer
Based in Germany 🇩🇪
Passionate about system design, automation, and performance.
