#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <ctime>

// User class handling authentication
class User {
protected:
    std::string username;
    std::string hashedPassword;
public:
    User(std::string uname, std::string password);
    virtual bool authenticate(std::string passwordAttempt) const;
    virtual ~User() {}
};

// Placeholder function definitions
User::User(std::string uname, std::string password) : username(uname), hashedPassword(password) {}
bool User::authenticate(std::string passwordAttempt) const {
    // Placeholder for authentication logic
    return passwordAttempt == hashedPassword;
}




class Account {
protected:
    double balance;
    std::string accountNumber;
public:
    Account(std::string accNum, double initialBalance);
    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;
    virtual void displayBalance() const = 0;
    virtual ~Account() {}

    Account& operator+=(double amount);
    Account& operator-=(double amount);
};

class SavingsAccount : public Account {
private:
    double interestRate;
public:
    SavingsAccount(std::string accNum, double initialBalance, double rate);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void displayBalance() const override;
    void applyInterest();
};

class CheckingAccount : public Account {
private:
    double overdraftLimit;
public:
    CheckingAccount(std::string accNum, double initialBalance, double overdraft);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void displayBalance() const override;
};

class BusinessAccount : public Account {
private:
    double transactionLimit;
    double feePerTransaction;
public:
    BusinessAccount(std::string accNum, double initialBalance, double limit, double fee);
    void deposit(double amount) override;
    bool withdraw(double amount) override;
    void displayBalance() const override;
};


// Placeholder function definitions
Account::Account(double initialBalance) : balance(initialBalance) {}
void Account::deposit(double amount) {
    // Placeholder for deposit logic
    balance += amount;
}
bool Account::withdraw(double amount) {
    // Placeholder for withdrawal logic
    if (amount > balance) return false;
    balance -= amount;
    return true;
}
void Account::displayBalance() const {
    std::cout << "Balance: " << balance << std::endl;
}


// Transaction class
class Transaction {
private:
    std::string transactionType;
    double amount;
    std::time_t timestamp;
public:
    Transaction(std::string type, double amt);
    virtual void logTransaction() const;
    friend std::ostream& operator<<(std::ostream& os, const Transaction& t);
};

// Placeholder function definitions
Transaction::Transaction(std::string type, double amt) : transactionType(type), amount(amt) {
    timestamp = std::time(nullptr);
}
void Transaction::logTransaction() const {
    // Placeholder for transaction logging logic
    std::cout << "Transaction: " << transactionType << " Amount: " << amount << std::endl;
}
std::ostream& operator<<(std::ostream& os, const Transaction& t) {
    os << "Transaction: " << t.transactionType << " Amount: " << t.amount;
    return os;
}


// Bank class managing accounts and transactions
class Bank {
private:
    std::map<std::string, std::shared_ptr<Account>> accounts;
public:
    void addAccount(const std::string& username, std::shared_ptr<Account> account);
    bool processTransaction(const std::string& username, double amount, char type);
};

// Placeholder function definitions
void Bank::addAccount(const std::string& username, std::shared_ptr<Account> account) {
    // Placeholder for adding an account
    accounts[username] = account;
}
bool Bank::processTransaction(const std::string& username, double amount, char type) {
    // Placeholder for processing transactions
    if (accounts.find(username) == accounts.end()) return false;
    if (type == 'D') {
        *accounts[username] += amount;
    }
    else if (type == 'W') {
        *accounts[username] -= amount;
    }
    return true;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
