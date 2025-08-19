#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip> 

using namespace std;

// Abstract base class for Interest Calculation
class InterestCalculator 
{
public:
    virtual double calculateInterest(double balance) const = 0;
    virtual string getInterestType() const = 0; // To get the interest type
};

// Derived class for Fixed Interest
class FixedInterestCalculator : public InterestCalculator 
{
public:
    double calculateInterest(double balance) const override 
    {
        return balance * 0.05; // 5% fixed interest
    }

    string getInterestType() const override 
    {
        return "Fixed (5%)";
    }
};

// Derived class for Variable Interest
class VariableInterestCalculator : public InterestCalculator 
{
public:
    double calculateInterest(double balance) const override 
    {
        return balance * 0.03; 
    }

    string getInterestType() const override 
    {
        return "Variable (3%)";
    }
};

// Class to represent a Bank Account
class BankAccount 
{
private:
    int accountNumber;
    string accountHolderName;
    double balance;
    InterestCalculator* interestCalculator;
    vector<string> transactionHistory;

public:
    BankAccount(int accNum, const string& name, double bal, InterestCalculator* calc)
        : accountNumber(accNum), accountHolderName(name), balance(bal), interestCalculator(calc) 
    {
        transactionHistory.push_back("Account created with balance: Rs." + to_string(balance));
    }

    void deposit(double amount) 
    {
        balance += amount;
        transactionHistory.push_back("Deposited: Rs." + to_string(amount));
    }

    void withdraw(double amount) 
    {
        if (amount > balance) 
        {
            throw runtime_error("Insufficient funds");
        }
        balance -= amount;
        transactionHistory.push_back("Withdrew: Rs." + to_string(amount));
    }

    void display() const 
    {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: Rs." << fixed << setprecision(2) << balance << endl;
        cout << "Interest Type: " << interestCalculator->getInterestType() << endl; 
    }

    void displayTransactionHistory() const 
    {
        cout << "Transaction History for Account " << accountNumber << ":" << endl;
        for (const string& entry : transactionHistory) 
        {
            cout << entry << endl;
        }
    }

    void applyInterest() 
    {
        double interest = interestCalculator->calculateInterest(balance);
        balance += interest;
        transactionHistory.push_back("Interest applied: Rs." + to_string(interest));
        cout << "Interest applied: Rs." << fixed << setprecision(2) << interest << endl; 
        cout << "New Balance: Rs." << fixed << setprecision(2) << balance << endl; 
    }

    int getAccountNumber() const 
    {
        return accountNumber;
    }

    double getBalance() const
    {
        return balance;
    }

    void changeInterestType(InterestCalculator* newCalculator) 
    {
        interestCalculator = newCalculator;
        transactionHistory.push_back("Interest type changed.");
    }

    void saveToFile(ofstream& file) const 
    {
        file << accountNumber << endl;
        file << accountHolderName << endl;
        file << balance << endl;
    }

    void loadFromFile(ifstream& file) 
    {
        file >> accountNumber;
        file.ignore();
        getline(file, accountHolderName);
        file >> balance;
    }
};

// Function to create a new account
void createAccount(BankAccount**& accounts, int& accountCount) 
{
    int accNum;
    string name;
    double bal;
    int interestType;
    
    cout << "Enter Account Number: ";
    cin >> accNum;
    cin.ignore();
    cout << "Enter Account Holder Name: ";
    getline(cin, name);
    cout << "Enter Initial Balance: ";
    cin >> bal;
    cout << "Choose Interest Type (1 for Fixed, 2 for Variable): ";
    cin >> interestType;

    InterestCalculator* calc = nullptr;
    if (interestType == 1) 
    {
        calc = new FixedInterestCalculator();
    } 
    else if (interestType == 2) 
    {
        calc = new VariableInterestCalculator();
    } 
    else 
    {
        cout << "Invalid choice. Using Fixed Interest by default." << endl;
        calc = new FixedInterestCalculator();
    }

    // Create a new account and store it in the dynamic array
    BankAccount* newAccount = new BankAccount(accNum, name, bal, calc);
    BankAccount** newAccounts = new BankAccount*[accountCount + 1];
    
    for (int i = 0; i < accountCount; i++) 
    {
        newAccounts[i] = accounts[i];
    }
    newAccounts[accountCount] = newAccount;

    delete[] accounts; 
    accounts = newAccounts;
    accountCount++;
}

// Function to view an account
void viewAccount(BankAccount** accounts, int accountCount) 
{
    int accNum;
    cout << "Enter Account Number: ";
    cin >> accNum;

    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            accounts[i]->display();
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to check balance of an account
void checkBalance(BankAccount** accounts, int accountCount) 
{
    int accNum;
    cout << "Enter Account Number: ";
    cin >> accNum;

    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            cout << "Current Balance: Rs." << fixed << setprecision(2) << accounts[i]->getBalance() << endl;
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to deposit money into an account
void depositMoney(BankAccount** accounts, int accountCount) 
{
    int accNum;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accNum;

    // Find the account
    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            cout << "Enter amount to deposit: ";
            cin >> amount;
            accounts[i]->deposit(amount); // Call deposit method on the account
            cout << "Deposited Rs." << amount << " successfully." << endl;
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to withdraw money from an account
void withdrawMoney(BankAccount** accounts, int accountCount) 
{
    int accNum;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accNum;

    // Find the account
    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            try 
            {
                accounts[i]->withdraw(amount); // Call withdraw method on the account
                cout << "Withdrew Rs." << amount << " successfully." << endl;
            } 
            catch (const runtime_error& e) 
            {
                cout << "Error: " << e.what() << endl; // Handle insufficient funds
            }
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to delete an account
void deleteAccount(BankAccount**& accounts, int& accountCount) 
{
    int accNum;
    cout << "Enter Account Number to delete: ";
    cin >> accNum;

    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            delete accounts[i];

            for (int j = i; j < accountCount - 1; j++) 
            {
                accounts[j] = accounts[j + 1];
            }

            accountCount--;
            cout << "Account deleted successfully." << endl;
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to change the interest type of an account
void changeInterestType(BankAccount** accounts, int accountCount) 
{
    int accNum, newInterestType;
    cout << "Enter Account Number: ";
    cin >> accNum;

    for (int i = 0; i < accountCount; i++) 
    {
        if (accounts[i]->getAccountNumber() == accNum) 
        {
            cout << "Choose new Interest Type (1 for Fixed, 2 for Variable): ";
            cin >> newInterestType;

            InterestCalculator* newCalc = nullptr;
            if (newInterestType == 1) 
            {
                newCalc = new FixedInterestCalculator();
            } 
            else if (newInterestType == 2) 
            {
                newCalc = new VariableInterestCalculator();
            } 
            else 
            {
                cout << "Invalid choice. Keeping current interest type." << endl;
                return;
            }

            accounts[i]->changeInterestType(newCalc);
            delete newCalc; // Avoid memory leak
            cout << "Interest type changed successfully." << endl;
            return;
        }
    }

    cout << "Account not found." << endl;
}

// Function to save accounts to a file
void saveAccounts(BankAccount** accounts, int accountCount) 
{
    ofstream file("accounts.txt");
    if (!file) 
    {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < accountCount; i++) 
    {
        accounts[i]->saveToFile(file);
    }

    file.close();
    cout << "Accounts saved to file successfully." << endl;
}

// Function to load accounts from a file
void loadAccounts(BankAccount**& accounts, int& accountCount) 
{
    ifstream file("accounts.txt");
    if (!file) 
    {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    accountCount = 0;
    while (file.peek() != EOF) 
    {
        accountCount++;
        int accNum;
        string name;
        double bal;

        file >> accNum;
        file.ignore();
        getline(file, name);
        file >> bal;

        // Assuming all accounts use Fixed Interest for simplicity
        InterestCalculator* calc = new FixedInterestCalculator();
        BankAccount* newAccount = new BankAccount(accNum, name, bal, calc);

        BankAccount** newAccounts = new BankAccount*[accountCount];
        for (int i = 0; i < accountCount - 1; i++) 
        {
            newAccounts[i] = accounts[i];
        }
        newAccounts[accountCount - 1] = newAccount;

        delete[] accounts; 
        accounts = newAccounts;
    }

    file.close();
    cout << "Accounts loaded from file successfully." << endl;
}

// Function to display menu
void displayMenu() 
{
    cout << "\nBank Management System\n";
    cout << "1. Create Account\n";
    cout << "2. View Account\n";
    cout << "3. Deposit Money\n";
    cout << "4. Withdraw Money\n";
    cout << "5. Check Balance\n";
    cout << "6. Delete Account\n";
    cout << "7. Change Interest Type\n";
    cout << "8. Save Accounts\n";
    cout << "9. Load Accounts\n";
    cout << "10. Exit\n";
}


int main() 
{
    BankAccount** accounts = nullptr;
    int accountCount = 0;
    int choice;

    do 
    {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                createAccount(accounts, accountCount);
                break;
            case 2:
                viewAccount(accounts, accountCount);
                break;
            case 3:
                depositMoney(accounts, accountCount);
                break;
            case 4:
                withdrawMoney(accounts, accountCount);
                break;
            case 5:
                checkBalance(accounts, accountCount);
                break;
            case 6:
                deleteAccount(accounts, accountCount);
                break;
            case 7:
                changeInterestType(accounts, accountCount);
                break;
            case 8:
                saveAccounts(accounts, accountCount);
                break;
            case 9:
                loadAccounts(accounts, accountCount);
                break;
            case 10:
                cout << "Exiting the program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 10);

    // Clean up memory
    for (int i = 0; i < accountCount; i++) 
    {
        delete accounts[i];
    }
    delete[] accounts;

    return 0;
}
