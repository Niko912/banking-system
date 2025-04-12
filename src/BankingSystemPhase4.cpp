#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>

// Template to allow generic type operations for deposits and withdrawals
template<typename T>
T addAmount(T balance, T amount) {
    return balance + amount;
}

template<typename T>
T subtractAmount(T balance, T amount) {
    if (amount > balance) {
        throw std::runtime_error("Insufficient funds for withdrawal.");
    }
    return balance - amount;
}

// Simple structure to store each transaction
struct TransactionNode {
    std::string type;     // Deposit or Withdraw
    double amount;
    std::time_t time;
    TransactionNode* next;

    TransactionNode(std::string t, double a) : type(t), amount(a), next(nullptr) {
        time = std::time(nullptr);
    }
};

// Class to represent a single customer
class Customer {
public:
    std::string name;
    std::string accountNumber;
    double balance;
    TransactionNode* transactionHead;

    Customer(std::string n, std::string accNum, double initialBalance) {
        name = n;
        accountNumber = accNum;
        balance = initialBalance;
        transactionHead = nullptr;
    }

    void deposit(double amount) {
        balance = addAmount(balance, amount);
        addTransaction("Deposit", amount);
        std::cout << "Deposited: " << amount << "\n";
    }

    bool withdraw(double amount) {
        try {
            balance = subtractAmount(balance, amount);
            addTransaction("Withdraw", amount);
            std::cout << "Withdrawn: " << amount << "\n";
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }

    void addTransaction(std::string type, double amount) {
        TransactionNode* newTransaction = new TransactionNode(type, amount);
        newTransaction->next = transactionHead;
        transactionHead = newTransaction;
    }

    void showTransactions() {
        std::cout << "Transaction History for " << name << ":\n";
        TransactionNode* current = transactionHead;
        while (current != nullptr) {
            std::cout << current->type << " $" << current->amount
                << " at " << std::ctime(&current->time);
            current = current->next;
        }
    }

    void display() {
        std::cout << "Customer: " << name << " | Account: " << accountNumber
            << " | Balance: $" << balance << "\n";
    }
};

// Node for customer linked list
struct CustomerNode {
    Customer* customer;
    CustomerNode* next;

    CustomerNode(Customer* c) : customer(c), next(nullptr) {}
};

// Class to manage multiple customers using a linked list
class Bank {
private:
    CustomerNode* head;

public:
    Bank() {
        head = nullptr;
    }

    void addCustomer(std::string name, std::string accNum, double balance) {
        Customer* newCustomer = new Customer(name, accNum, balance);
        CustomerNode* newNode = new CustomerNode(newCustomer);
        newNode->next = head;
        head = newNode;
        std::cout << "Added customer: " << name << "\n";
    }

    Customer* findCustomer(std::string accNum) {
        CustomerNode* current = head;
        while (current != nullptr) {
            if (current->customer->accountNumber == accNum) {
                return current->customer;
            }
            current = current->next;
        }
        return nullptr;
    }

    void deleteCustomer(std::string accNum) {
        CustomerNode* current = head;
        CustomerNode* prev = nullptr;

        while (current != nullptr) {
            if (current->customer->accountNumber == accNum) {
                if (prev == nullptr) {
                    head = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current->customer;
                delete current;
                std::cout << "Customer deleted.\n";
                return;
            }
            prev = current;
            current = current->next;
        }
        std::cout << "Customer not found.\n";
    }

    void updateCustomerName(std::string accNum, std::string newName) {
        Customer* customer = findCustomer(accNum);
        if (customer != nullptr) {
            customer->name = newName;
            std::cout << "Customer name updated.\n";
        }
        else {
            std::cout << "Customer not found.\n";
        }
    }

    void listCustomers() {
        CustomerNode* current = head;
        while (current != nullptr) {
            current->customer->display();
            current = current->next;
        }
    }
};

int main() {
    Bank bank;

    // Sample data
    bank.addCustomer("Alice", "A001", 500.0);
    bank.addCustomer("Bob", "B001", 1000.0);

    Customer* customer = bank.findCustomer("A001");
    if (customer != nullptr) {
        customer->deposit(200.0);
        customer->withdraw(800.0); // Should throw exception
        customer->withdraw(100.0);
        customer->showTransactions();
    }

    bank.updateCustomerName("B001", "Robert");
    bank.listCustomers();

    bank.deleteCustomer("A001");
    bank.listCustomers();

    return 0;
}
