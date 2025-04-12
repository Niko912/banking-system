#include <iostream>
#include <string>
#include <ctime>
#include <memory>


// Linked List Structures
/**
 * Node representing a transaction record in a linked list.
 */
struct TransactionNode {
    std::string type;
    double amount;
    std::time_t timestamp;
    TransactionNode* next;

    TransactionNode(std::string t, double amt) : type(t), amount(amt), timestamp(std::time(nullptr)), next(nullptr) {}
};

/**
 * Node representing a customer record in a linked list.
 */
struct CustomerNode {
    std::string username;
    std::string accountNumber;
    std::string password;
    double balance;
    TransactionNode* transactions;
    CustomerNode* next;

    CustomerNode(std::string uname, std::string accNum, std::string pass, double bal)
        : username(uname), accountNumber(accNum), password(pass), balance(bal), transactions(nullptr), next(nullptr) {
    }
};


// Special Functionality Classes


/**
 * Calculates interest for savings accounts.
 */
class InterestCalculator {
public:
    static double calculate(double balance, double rate) {
        return balance * rate;
    }
};

/**
 * Handles overdraft protection for checking accounts.
 */
class OverdraftProtection {
public:
    static bool allowWithdrawal(double balance, double amount, double limit) {
        return (balance + limit >= amount);
    }
};


// Bank System


/**
 * Main class handling customer and transaction management.
 */
class BankSystem {
private:
    CustomerNode* head;

public:
    BankSystem() : head(nullptr) {}
    ~BankSystem();

    void addCustomer(std::string username, std::string accNum, std::string pass, double balance);
    bool deleteCustomer(const std::string& accNum);
    CustomerNode* findCustomer(const std::string& accNum);
    bool updateBalance(const std::string& accNum, double newBalance);
    void addTransaction(const std::string& accNum, const std::string& type, double amount);
    void displayCustomerInfo(const std::string& accNum);
    void displayAllCustomers();
};


// Destructor
BankSystem::~BankSystem() {
    while (head) {
        TransactionNode* tx = head->transactions;
        while (tx) {
            TransactionNode* tmpTx = tx;
            tx = tx->next;
            delete tmpTx;
        }
        CustomerNode* temp = head;
        head = head->next;
        delete temp;
    }
}


// Customer Functions
void BankSystem::addCustomer(std::string username, std::string accNum, std::string pass, double balance) {
    CustomerNode* newCustomer = new CustomerNode(username, accNum, pass, balance);
    newCustomer->next = head;
    head = newCustomer;
}

bool BankSystem::deleteCustomer(const std::string& accNum) {
    CustomerNode* curr = head;
    CustomerNode* prev = nullptr;

    while (curr) {
        if (curr->accountNumber == accNum) {
            if (prev) prev->next = curr->next;
            else head = curr->next;

            TransactionNode* tx = curr->transactions;
            while (tx) {
                TransactionNode* tmp = tx;
                tx = tx->next;
                delete tmp;
            }
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

CustomerNode* BankSystem::findCustomer(const std::string& accNum) {
    CustomerNode* curr = head;
    while (curr) {
        if (curr->accountNumber == accNum) return curr;
        curr = curr->next;
    }
    return nullptr;
}

bool BankSystem::updateBalance(const std::string& accNum, double newBalance) {
    CustomerNode* customer = findCustomer(accNum);
    if (customer) {
        customer->balance = newBalance;
        return true;
    }
    return false;
}


// Transaction Functions
void BankSystem::addTransaction(const std::string& accNum, const std::string& type, double amount) {
    CustomerNode* customer = findCustomer(accNum);
    if (!customer) return;

    TransactionNode* newTx = new TransactionNode(type, amount);
    newTx->next = customer->transactions;
    customer->transactions = newTx;

    if (type == "deposit") {
        customer->balance += amount;
    }
    else if (type == "withdraw") {
        customer->balance -= amount;
    }
}


// Display Functions
void BankSystem::displayCustomerInfo(const std::string& accNum) {
    CustomerNode* customer = findCustomer(accNum);
    if (!customer) {
        std::cout << "Customer not found.\n";
        return;
    }

    std::cout << "Username: " << customer->username << "\n";
    std::cout << "Account #: " << customer->accountNumber << "\n";
    std::cout << "Balance: $" << customer->balance << "\n";

    TransactionNode* tx = customer->transactions;
    while (tx) {
        std::cout << "  [" << std::ctime(&(tx->timestamp)) << "] "
            << tx->type << " - $" << tx->amount << "\n";
        tx = tx->next;
    }
}

void BankSystem::displayAllCustomers() {
    CustomerNode* curr = head;
    while (curr) {
        displayCustomerInfo(curr->accountNumber);
        std::cout << "----------------------\n";
        curr = curr->next;
    }
}


// Main (Example Usage)
int main() {
    BankSystem bank;

    bank.addCustomer("Alice", "A123", "pass123", 1000);
    bank.addTransaction("A123", "deposit", 200);
    bank.addTransaction("A123", "withdraw", 100);

    bank.addCustomer("Bob", "B456", "secure456", 500);
    bank.addTransaction("B456", "deposit", 300);

    bank.displayAllCustomers();

    // Apply interest to Alice
    CustomerNode* alice = bank.findCustomer("A123");
    if (alice) {
        double interest = InterestCalculator::calculate(alice->balance, 0.05);
        bank.addTransaction("A123", "interest", interest);
    }

    bank.displayCustomerInfo("A123");

    return 0;
}
