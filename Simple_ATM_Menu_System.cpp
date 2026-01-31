#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

class ATM {
private:
    double balance;
    string accountHolder;
    int pin;
    bool isAuthenticated;

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    ATM(string name, int userPin, double initialBalance = 0.0) {
        accountHolder = name;
        pin = userPin;
        balance = initialBalance;
        isAuthenticated = false;
    }

    bool authenticate() {
        int enteredPin;
        int attempts = 0;
        const int MAX_ATTEMPTS = 3;

        cout << "\n=================================\n";
        cout << "       ATM AUTHENTICATION\n";
        cout << "=================================\n";

        while (attempts < MAX_ATTEMPTS) {
            cout << "Enter PIN: ";
            cin >> enteredPin;

            if (cin.fail()) {
                clearInput();
                cout << "Invalid input. Please enter numbers only.\n";
                attempts++;
                continue;
            }

            if (enteredPin == pin) {
                isAuthenticated = true;
                cout << "\nAuthentication successful!\n";
                cout << "Welcome, " << accountHolder << "!\n";
                return true;
            } else {
                attempts++;
                if (attempts < MAX_ATTEMPTS) {
                    cout << "Incorrect PIN. " << (MAX_ATTEMPTS - attempts) 
                         << " attempts remaining.\n";
                }
            }
        }

        cout << "\nToo many incorrect attempts. Access denied.\n";
        return false;
    }

    void checkBalance() {
        if (!isAuthenticated) return;
        
        cout << "\n=================================\n";
        cout << "       BALANCE INQUIRY\n";
        cout << "=================================\n";
        cout << "Account Holder: " << accountHolder << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "=================================\n";
    }

    void deposit() {
        if (!isAuthenticated) return;

        double amount;
        cout << "\n=================================\n";
        cout << "          DEPOSIT\n";
        cout << "=================================\n";
        cout << "Enter amount to deposit: $";
        cin >> amount;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Transaction cancelled.\n";
            return;
        }

        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value.\n";
            return;
        }

        balance += amount;
        cout << "\nDeposit successful!\n";
        cout << "Deposited: $" << fixed << setprecision(2) << amount << endl;
        cout << "New Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "=================================\n";
    }

    void withdraw() {
        if (!isAuthenticated) return;

        double amount;
        cout << "\n=================================\n";
        cout << "         WITHDRAWAL\n";
        cout << "=================================\n";
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Enter amount to withdraw: $";
        cin >> amount;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Transaction cancelled.\n";
            return;
        }

        if (amount <= 0) {
            cout << "Invalid amount. Please enter a positive value.\n";
            return;
        }

        if (amount > balance) {
            cout << "\nInsufficient funds!\n";
            cout << "Available Balance: $" << fixed << setprecision(2) << balance << endl;
            return;
        }

        balance -= amount;
        cout << "\nWithdrawal successful!\n";
        cout << "Withdrawn: $" << fixed << setprecision(2) << amount << endl;
        cout << "Remaining Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "=================================\n";
    }

    void displayMenu() {
        cout << "\n=================================\n";
        cout << "          ATM MENU\n";
        cout << "=================================\n";
        cout << "1. Check Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Exit\n";
        cout << "=================================\n";
        cout << "Select an option (1-4): ";
    }

    void run() {
        if (!authenticate()) {
            return;
        }

        int choice;
        bool running = true;

        while (running) {
            displayMenu();
            cin >> choice;

            if (cin.fail()) {
                clearInput();
                cout << "\nInvalid input. Please enter a number between 1 and 4.\n";
                continue;
            }

            switch (choice) {
                case 1:
                    checkBalance();
                    break;
                case 2:
                    deposit();
                    break;
                case 3:
                    withdraw();
                    break;
                case 4:
                    cout << "\n=================================\n";
                    cout << "Thank you for using our ATM!\n";
                    cout << "Goodbye, " << accountHolder << "!\n";
                    cout << "=================================\n";
                    running = false;
                    break;
                default:
                    cout << "\nInvalid option. Please select 1-4.\n";
            }
        }
    }
};

int main() {
    cout << "=================================\n";
    cout << "    WELCOME TO XYZ BANK ATM\n";
    cout << "=================================\n";

    // Create an ATM instance with account holder "John Doe", PIN: 1234, initial balance: $1000
    ATM myATM("John Doe", 1234, 1000.00);
    
    myATM.run();

    return 0;
}