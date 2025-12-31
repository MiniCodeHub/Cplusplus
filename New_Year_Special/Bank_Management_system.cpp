/*
============================================================================
    BANK MANAGEMENT SYSTEM - COMPLETE TUTORIAL CODE
    Perfect for 45-70 Minute Video Tutorial
    Build progressively from basic to advanced features
============================================================================

TUTORIAL PROGRESSION:
    PHASE 1 (10 mins): Basic Account Class
    PHASE 2 (10 mins): Deposit & Withdrawal
    PHASE 3 (10 mins): Money Transfer
    PHASE 4 (15 mins): Transaction History & Authentication
    PHASE 5 (10 mins): Multiple Accounts & Bank System
    PHASE 6 (10 mins): File Persistence
    PHASE 7 (5 mins): Polish & Advanced Features
*/

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <limits>
using namespace std;

//============================================================================
// PHASE 4: Transaction Structure (for history tracking)
//============================================================================
struct Transaction {
    string type;           // DEPOSIT, WITHDRAWAL, TRANSFER_IN, TRANSFER_OUT
    double amount;
    string date;
    double balanceAfter;
    string details;        // Additional info (like transfer recipient)
    
    Transaction(string t, double amt, string d, double bal, string det = "")
        : type(t), amount(amt), date(d), balanceAfter(bal), details(det) {}
};

//============================================================================
// PHASE 1-7: Complete Account Class
//============================================================================
class Account {
private:
    int accountNumber;
    string accountHolder;
    string password;
    double balance;
    string accountType;       // SAVINGS, CURRENT, BUSINESS
    vector<Transaction> transactionHistory;
    string creationDate;
    bool isActive;
    
    // Helper function to get current date
    string getCurrentDate() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        sprintf(buffer, "%02d/%02d/%04d", 
                ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }
    
    // Helper function to get current time
    string getCurrentTime() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        sprintf(buffer, "%02d:%02d:%02d", 
                ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
        return string(buffer);
    }

public:
    //========================================================================
    // PHASE 1: Constructor & Basic Setup
    //========================================================================
    Account(int accNum, string holder, string pass, double initialBalance, string type = "SAVINGS")
        : accountNumber(accNum), accountHolder(holder), password(pass), 
          balance(initialBalance), accountType(type), isActive(true) {
        
        creationDate = getCurrentDate();
        
        // Record initial deposit as first transaction
        transactionHistory.push_back(
            Transaction("OPENING", initialBalance, creationDate, balance, "Account opened")
        );
        
        cout << "\n============================================================\n";
        cout << "         ACCOUNT CREATED SUCCESSFULLY!                      \n";
        cout << "============================================================\n";
        cout << "Account Number : " << accountNumber << endl;
        cout << "Account Holder : " << accountHolder << endl;
        cout << "Account Type   : " << accountType << endl;
        cout << "Initial Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Created On     : " << creationDate << endl;
    }
    
    //========================================================================
    // PHASE 1: Getters (Essential for data access)
    //========================================================================
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }
    string getAccountType() const { return accountType; }
    bool getActiveStatus() const { return isActive; }
    
    //========================================================================
    // PHASE 4: Authentication (Security Layer)
    //========================================================================
    bool verifyPassword(string pass) const {
        return password == pass;
    }
    
    void changePassword(string oldPass, string newPass) {
        if (!verifyPassword(oldPass)) {
            cout << "\nIncorrect current password!\n";
            return;
        }
        
        if (newPass.length() < 4) {
            cout << "\nNew password must be at least 4 characters!\n";
            return;
        }
        
        password = newPass;
        cout << "\nPassword changed successfully!\n";
    }
    
    //========================================================================
    // PHASE 2: Deposit Function (First Banking Operation)
    //========================================================================
    void deposit(double amount) {
        // Validation 1: Positive amount
        if (amount <= 0) {
            cout << "\nInvalid deposit amount. Must be positive!\n";
            return;
        }
        
        // Validation 2: Reasonable limits (prevent money laundering simulation)
        if (amount > 100000) {
            cout << "\nSingle deposit limit is $100,000. Please contact branch for larger deposits.\n";
            return;
        }
        
        // Perform deposit
        balance += amount;
        
        // Record transaction
        transactionHistory.push_back(
            Transaction("DEPOSIT", amount, getCurrentDate(), balance)
        );
        
        // Success message
        cout << "\n============================================================\n";
        cout << "           DEPOSIT SUCCESSFUL!                              \n";
        cout << "============================================================\n";
        cout << "Deposited Amount  : $" << fixed << setprecision(2) << amount << endl;
        cout << "Previous Balance  : $" << (balance - amount) << endl;
        cout << "Current Balance   : $" << balance << endl;
        cout << "Transaction Time  : " << getCurrentTime() << endl;
    }
    
    //========================================================================
    // PHASE 2: Withdrawal Function (With Safety Checks)
    //========================================================================
    void withdraw(double amount) {
        // Validation 1: Positive amount
        if (amount <= 0) {
            cout << "\nInvalid withdrawal amount. Must be positive!\n";
            return;
        }
        
        // Validation 2: Sufficient balance
        if (amount > balance) {
            cout << "\n============================================================\n";
            cout << "         INSUFFICIENT FUNDS!                                \n";
            cout << "============================================================\n";
            cout << "Requested Amount    : $" << fixed << setprecision(2) << amount << endl;
            cout << "Available Balance   : $" << balance << endl;
            cout << "Shortfall           : $" << (amount - balance) << endl;
            return;
        }
        
        // Validation 3: Minimum balance (for savings accounts)
        if (accountType == "SAVINGS" && (balance - amount) < 100) {
            cout << "\nSavings account must maintain minimum balance of $100!\n";
            cout << "Available for withdrawal: $" << fixed << setprecision(2) << (balance - 100) << endl;
            return;
        }
        
        // Perform withdrawal
        balance -= amount;
        
        // Record transaction
        transactionHistory.push_back(
            Transaction("WITHDRAWAL", amount, getCurrentDate(), balance)
        );
        
        // Success message
        cout << "\n============================================================\n";
        cout << "         WITHDRAWAL SUCCESSFUL!                             \n";
        cout << "============================================================\n";
        cout << "Withdrawn Amount  : $" << fixed << setprecision(2) << amount << endl;
        cout << "Previous Balance  : $" << (balance + amount) << endl;
        cout << "Current Balance   : $" << balance << endl;
        cout << "Transaction Time  : " << getCurrentTime() << endl;
    }
    
    //========================================================================
    // PHASE 3: Money Transfer (Account-to-Account)
    //========================================================================
    bool transfer(Account& recipient, double amount) {
        // Validation 1: Positive amount
        if (amount <= 0) {
            cout << "\nInvalid transfer amount!\n";
            return false;
        }
        
        // Validation 2: Not transferring to self
        if (recipient.accountNumber == this->accountNumber) {
            cout << "\nCannot transfer to the same account!\n";
            return false;
        }
        
        // Validation 3: Sufficient balance
        if (amount > balance) {
            cout << "\nInsufficient funds for transfer!\n";
            cout << "Available: $" << fixed << setprecision(2) << balance << endl;
            return false;
        }
        
        // Validation 4: Recipient account is active
        if (!recipient.isActive) {
            cout << "\nRecipient account is not active!\n";
            return false;
        }
        
        // Perform transfer
        balance -= amount;
        recipient.balance += amount;
        
        // Record transaction for sender
        string senderDetails = "Transfer to " + recipient.accountHolder + " (Acc: " + to_string(recipient.accountNumber) + ")";
        transactionHistory.push_back(
            Transaction("TRANSFER_OUT", amount, getCurrentDate(), balance, senderDetails)
        );
        
        // Record transaction for recipient
        string recipientDetails = "Transfer from " + accountHolder + " (Acc: " + to_string(accountNumber) + ")";
        recipient.transactionHistory.push_back(
            Transaction("TRANSFER_IN", amount, getCurrentDate(), recipient.balance, recipientDetails)
        );
        
        // Success message
        cout << "\n============================================================\n";
        cout << "         TRANSFER SUCCESSFUL!                               \n";
        cout << "============================================================\n";
        cout << "From              : " << accountHolder << " (Acc: " << accountNumber << ")" << endl;
        cout << "To                : " << recipient.accountHolder << " (Acc: " << recipient.accountNumber << ")" << endl;
        cout << "Amount Transferred: $" << fixed << setprecision(2) << amount << endl;
        cout << "Your New Balance  : $" << balance << endl;
        cout << "Transaction Time  : " << getCurrentTime() << endl;
        
        return true;
    }
    
    //========================================================================
    // PHASE 1: Display Account Information
    //========================================================================
    void displayAccountInfo() const {
        cout << "\n============================================================\n";
        cout << "           ACCOUNT INFORMATION                              \n";
        cout << "============================================================\n";
        cout << "Account Number    : " << accountNumber << endl;
        cout << "Account Holder    : " << accountHolder << endl;
        cout << "Account Type      : " << accountType << endl;
        cout << "Current Balance   : $" << fixed << setprecision(2) << balance << endl;
        cout << "Account Status    : " << (isActive ? "ACTIVE" : "INACTIVE") << endl;
        cout << "Created On        : " << creationDate << endl;
        cout << "Total Transactions: " << transactionHistory.size() << endl;
        cout << "============================================================\n";
    }
    
    //========================================================================
    // PHASE 4: Transaction History Display
    //========================================================================
    void displayTransactionHistory() const {
        if (transactionHistory.empty()) {
            cout << "\nNo transactions recorded yet.\n";
            return;
        }
        
        cout << "\n=============================================================================\n";
        cout << "                 TRANSACTION HISTORY                                      \n";
        cout << "=============================================================================\n";
        cout << left << setw(12) << "Date" 
             << setw(18) << "Type" 
             << right << setw(12) << "Amount" 
             << setw(15) << "Balance After"
             << "  Details" << endl;
        cout << string(75, '-') << endl;
        
        for (const auto& txn : transactionHistory) {
            cout << left << setw(12) << txn.date
                 << setw(18) << txn.type
                 << right << setw(12) << fixed << setprecision(2) << txn.amount
                 << setw(15) << txn.balanceAfter;
            
            if (!txn.details.empty()) {
                cout << "  " << txn.details;
            }
            cout << endl;
        }
        cout << "=============================================================================\n";
        cout << "Total Transactions: " << transactionHistory.size() << endl;
    }
    
    //========================================================================
    // PHASE 7: Advanced Features - Apply Interest
    //========================================================================
    void applyInterest(double annualRate) {
        if (accountType != "SAVINGS") {
            cout << "\nInterest is only applicable to SAVINGS accounts!\n";
            return;
        }
        
        double interest = balance * (annualRate / 100.0);
        balance += interest;
        
        transactionHistory.push_back(
            Transaction("INTEREST", interest, getCurrentDate(), balance, 
                       "Annual interest @ " + to_string(annualRate) + "%")
        );
        
        cout << "\n============================================================\n";
        cout << "         INTEREST CREDITED!                                 \n";
        cout << "============================================================\n";
        cout << "Interest Rate     : " << fixed << setprecision(2) << annualRate << "%" << endl;
        cout << "Interest Amount   : $" << interest << endl;
        cout << "New Balance       : $" << balance << endl;
    }
    
    //========================================================================
    // PHASE 7: Advanced Features - Account Statement
    //========================================================================
    void generateStatement(int lastNTransactions = 10) const {
        cout << "\n============================================================\n";
        cout << "           ACCOUNT STATEMENT                                \n";
        cout << "============================================================\n";
        cout << "Account Holder : " << accountHolder << endl;
        cout << "Account Number : " << accountNumber << endl;
        cout << "Account Type   : " << accountType << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << balance << endl;
        cout << "Statement Date : " << getCurrentDate() << endl;
        cout << "============================================================\n";
        
        int count = min(lastNTransactions, (int)transactionHistory.size());
        cout << "\nLast " << count << " Transactions:\n";
        cout << string(60, '-') << endl;
        
        for (int i = transactionHistory.size() - count; i < transactionHistory.size(); i++) {
            const auto& txn = transactionHistory[i];
            cout << txn.date << " | " << left << setw(15) << txn.type 
                 << " | $" << right << setw(10) << fixed << setprecision(2) << txn.amount << endl;
        }
        cout << "============================================================\n";
    }
    
    //========================================================================
    // PHASE 6: File Operations - Save Account Data
    //========================================================================
    void saveToFile(ofstream& out) const {
        out << accountNumber << endl
            << accountHolder << endl
            << password << endl
            << balance << endl
            << accountType << endl
            << creationDate << endl
            << isActive << endl
            << transactionHistory.size() << endl;
        
        for (const auto& txn : transactionHistory) {
            out << txn.type << endl
                << txn.amount << endl
                << txn.date << endl
                << txn.balanceAfter << endl
                << txn.details << endl;
        }
    }
    
    //========================================================================
    // PHASE 6: File Operations - Load Account Data
    //========================================================================
    void loadFromFile(ifstream& in) {
        in >> accountNumber;
        in.ignore();
        getline(in, accountHolder);
        getline(in, password);
        in >> balance;
        in.ignore();
        getline(in, accountType);
        getline(in, creationDate);
        in >> isActive;
        
        int txnCount;
        in >> txnCount;
        in.ignore();
        
        transactionHistory.clear();
        for (int i = 0; i < txnCount; i++) {
            string type, date, details;
            double amount, balanceAfter;
            
            getline(in, type);
            in >> amount;
            in.ignore();
            getline(in, date);
            in >> balanceAfter;
            in.ignore();
            getline(in, details);
            
            transactionHistory.push_back(Transaction(type, amount, date, balanceAfter, details));
        }
    }
    
    //========================================================================
    // PHASE 7: Advanced Features - Close Account
    //========================================================================
    void closeAccount(string pass) {
        if (!verifyPassword(pass)) {
            cout << "\nIncorrect password! Cannot close account.\n";
            return;
        }
        
        if (balance > 0) {
            cout << "\nAccount has remaining balance of $" << fixed << setprecision(2) << balance << endl;
            cout << "Please withdraw all funds before closing.\n";
            return;
        }
        
        isActive = false;
        cout << "\nAccount closed successfully.\n";
    }
};

//============================================================================
// PHASE 5: Bank Management System Class
//============================================================================
class BankManagementSystem {
private:
    vector<Account> accounts;
    int nextAccountNumber;
    const string filename = "bank_data.txt";
    
    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

public:
    BankManagementSystem() : nextAccountNumber(10001) {
        loadAllAccounts();
    }
    
    ~BankManagementSystem() {
        saveAllAccounts();
    }
    
    //========================================================================
    // PHASE 5: Create New Account
    //========================================================================
    void createAccount() {
        string name, password, accountType;
        double initialDeposit;
        
        cout << "\n============================================================\n";
        cout << "         CREATE NEW ACCOUNT                                 \n";
        cout << "============================================================\n";
        
        cout << "\nEnter Full Name: ";
        clearInput();
        getline(cin, name);
        
        if (name.empty()) {
            cout << "\nName cannot be empty!\n";
            return;
        }
        
        cout << "Set Account Password (min 4 characters): ";
        getline(cin, password);
        
        if (password.length() < 4) {
            cout << "\nPassword must be at least 4 characters!\n";
            return;
        }
        
        cout << "\nSelect Account Type:\n";
        cout << "1. SAVINGS (Minimum balance: $100, Interest: 4% annually)\n";
        cout << "2. CURRENT (No minimum balance, No interest)\n";
        cout << "3. BUSINESS (No minimum balance, Premium features)\n";
        cout << "Enter choice (1-3): ";
        
        int typeChoice;
        cin >> typeChoice;
        
        switch (typeChoice) {
            case 1: accountType = "SAVINGS"; break;
            case 2: accountType = "CURRENT"; break;
            case 3: accountType = "BUSINESS"; break;
            default:
                cout << "\nInvalid choice! Defaulting to SAVINGS.\n";
                accountType = "SAVINGS";
        }
        
        cout << "\nEnter Initial Deposit Amount: $";
        while (!(cin >> initialDeposit) || initialDeposit < 0) {
            cout << "Invalid amount. Please enter a positive number: $";
            clearInput();
        }
        
        // Minimum deposit validation
        double minDeposit = (accountType == "SAVINGS") ? 100.0 : 50.0;
        if (initialDeposit < minDeposit) {
            cout << "\nMinimum initial deposit for " << accountType << " account is $" << minDeposit << "!\n";
            return;
        }
        
        // Create the account
        accounts.push_back(Account(nextAccountNumber++, name, password, initialDeposit, accountType));
        
        cout << "\nYour account has been created successfully!\n";
        cout << "Please note down your Account Number: " << (nextAccountNumber - 1) << endl;
        
        pause();
    }
    
    //========================================================================
    // PHASE 5: Find Account by Number
    //========================================================================
    Account* findAccount(int accNum) {
        for (auto& acc : accounts) {
            if (acc.getAccountNumber() == accNum) {
                return &acc;
            }
        }
        return nullptr;
    }
    
    //========================================================================
    // PHASE 5: Authenticate and Access Account
    //========================================================================
    void accessAccount() {
        int accNum;
        string password;
        
        cout << "\n============================================================\n";
        cout << "           ACCOUNT LOGIN                                    \n";
        cout << "============================================================\n";
        
        cout << "\nEnter Account Number: ";
        cin >> accNum;
        
        Account* account = findAccount(accNum);
        
        if (account == nullptr) {
            cout << "\nAccount not found! Please check the account number.\n";
            pause();
            return;
        }
        
        cout << "Enter Password: ";
        clearInput();
        getline(cin, password);
        
        if (!account->verifyPassword(password)) {
            cout << "\nIncorrect password! Access denied.\n";
            pause();
            return;
        }
        
        cout << "\nLogin successful! Welcome, " << account->getAccountHolder() << "!\n";
        pause();
        
        // Account Operations Menu
        accountMenu(account);
    }
    
    //========================================================================
    // PHASE 5: Account Operations Menu
    //========================================================================
    void accountMenu(Account* account) {
        int choice;
        
        while (true) {
            cout << "\n============================================================\n";
            cout << "        ACCOUNT OPERATIONS MENU                             \n";
            cout << "============================================================\n";
            cout << "Account: " << account->getAccountHolder() 
                 << " | Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
            cout << "============================================================\n";
            cout << "1.  Check Balance\n";
            cout << "2.  Deposit Money\n";
            cout << "3.  Withdraw Money\n";
            cout << "4.  Transfer Money\n";
            cout << "5.  View Transaction History\n";
            cout << "6.  Generate Account Statement\n";
            cout << "7.  Change Password\n";
            cout << "8.  Apply Interest (Savings only)\n";
            cout << "9.  Close Account\n";
            cout << "10. Logout\n";
            cout << "============================================================\n";
            cout << "Enter choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: // Check Balance
                    account->displayAccountInfo();
                    pause();
                    break;
                    
                case 2: { // Deposit
                    double amount;
                    cout << "\nEnter amount to deposit: $";
                    cin >> amount;
                    account->deposit(amount);
                    pause();
                    break;
                }
                
                case 3: { // Withdraw
                    double amount;
                    cout << "\nEnter amount to withdraw: $";
                    cin >> amount;
                    account->withdraw(amount);
                    pause();
                    break;
                }
                
                case 4: { // Transfer
                    int recipientAccNum;
                    double amount;
                    
                    cout << "\nEnter recipient account number: ";
                    cin >> recipientAccNum;
                    
                    Account* recipient = findAccount(recipientAccNum);
                    
                    if (recipient == nullptr) {
                        cout << "\nRecipient account not found!\n";
                    } else {
                        cout << "Recipient: " << recipient->getAccountHolder() << endl;
                        cout << "Enter amount to transfer: $";
                        cin >> amount;
                        account->transfer(*recipient, amount);
                    }
                    pause();
                    break;
                }
                
                case 5: // Transaction History
                    account->displayTransactionHistory();
                    pause();
                    break;
                    
                case 6: { // Generate Statement
                    int numTransactions;
                    cout << "\nHow many recent transactions to show? (max 50): ";
                    cin >> numTransactions;
                    account->generateStatement(numTransactions);
                    pause();
                    break;
                }
                
                case 7: { // Change Password
                    string oldPass, newPass;
                    cout << "\nEnter current password: ";
                    clearInput();
                    getline(cin, oldPass);
                    cout << "Enter new password: ";
                    getline(cin, newPass);
                    account->changePassword(oldPass, newPass);
                    pause();
                    break;
                }
                
                case 8: { // Apply Interest
                    double rate = 4.0; // Default savings rate
                    account->applyInterest(rate);
                    pause();
                    break;
                }
                
                case 9: { // Close Account
                    string password;
                    cout << "\nEnter password to confirm account closure: ";
                    clearInput();
                    getline(cin, password);
                    account->closeAccount(password);
                    pause();
                    return; // Exit to main menu
                }
                
                case 10: // Logout
                    cout << "\nLogged out successfully!\n";
                    pause();
                    return;
                    
                default:
                    cout << "\nInvalid choice! Please try again.\n";
                    pause();
            }
        }
    }
    
    //========================================================================
    // PHASE 5: List All Accounts (Admin View)
    //========================================================================
    void listAllAccounts() {
        if (accounts.empty()) {
            cout << "\nNo accounts in the system.\n";
            pause();
            return;
        }
        
        cout << "\n=============================================================================\n";
        cout << "                   ALL ACCOUNTS                                           \n";
        cout << "=============================================================================\n";
        cout << left << setw(10) << "Acc No" 
             << setw(25) << "Holder Name" 
             << setw(12) << "Type"
             << setw(10) << "Status"
             << right << setw(15) << "Balance" << endl;
        cout << string(72, '-') << endl;
        
        double totalBalance = 0;
        int activeCount = 0;
        
        for (const auto& acc : accounts) {
            cout << left << setw(10) << acc.getAccountNumber()
                 << setw(25) << acc.getAccountHolder()
                 << setw(12) << acc.getAccountType()
                 << setw(10) << (acc.getActiveStatus() ? "ACTIVE" : "CLOSED")
                 << right << setw(15) << fixed << setprecision(2) << acc.getBalance() << endl;
            
            totalBalance += acc.getBalance();
            if (acc.getActiveStatus()) activeCount++;
        }
        
        cout << "=============================================================================\n";
        cout << "Total Accounts: " << accounts.size() 
             << " | Active: " << activeCount 
             << " | Total Balance: $" << totalBalance << endl;
        
        pause();
    }
    
    //========================================================================
    // PHASE 6: Save All Accounts to File
    //========================================================================
    void saveAllAccounts() {
        ofstream out(filename);
        if (!out) {
            cout << "Error: Could not save data to file!\n";
            return;
        }
        
        out << nextAccountNumber << endl;
        out << accounts.size() << endl;
        
        for (const auto& acc : accounts) {
            acc.saveToFile(out);
        }
        
        out.close();
    }
    
    //========================================================================
    // PHASE 6: Load All Accounts from File
    //========================================================================
    void loadAllAccounts() {
        ifstream in(filename);
        if (!in) {
            cout << "No previous data found. Starting fresh.\n";
            return;
        }
        
        in >> nextAccountNumber;
        int count;
        in >> count;
        in.ignore();
        
        for (int i = 0; i < count; i++) {
            Account acc(0, "", "", 0); // Temporary account
            acc.loadFromFile(in);
            accounts.push_back(acc);
        }
        
        in.close();
        cout << "Loaded " << count << " accounts from previous session.\n";
    }
};

//============================================================================
// MAIN FUNCTION - Entry Point
//============================================================================
int main() {
    // Display welcome banner
    cout << "\n";
    cout << "=============================================================================\n";
    cout << "                                                                             \n";
    cout << "                BANK MANAGEMENT SYSTEM                                       \n";
    cout << "                                                                             \n";
    cout << "           Secure - Reliable - Feature-Rich                                 \n";
    cout << "                                                                             \n";
    cout << "=============================================================================\n";
    cout << endl;
    
    // Create bank system instance
    BankManagementSystem bank;
    
    int choice;
    
    // Main program loop
    while (true) {
        cout << "\n============================================================\n";
        cout << "              MAIN MENU                                     \n";
        cout << "============================================================\n";
        cout << "1. Create New Account\n";
        cout << "2. Access Existing Account\n";
        cout << "3. View All Accounts (Admin)\n";
        cout << "4. About System\n";
        cout << "5. Exit\n";
        cout << "============================================================\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                bank.createAccount();
                break;
                
            case 2:
                bank.accessAccount();
                break;
                
            case 3:
                bank.listAllAccounts();
                break;
                
            case 4:
                cout << "\n============================================================\n";
                cout << "           ABOUT THIS SYSTEM                                \n";
                cout << "============================================================\n";
                cout << "\nFeatures:\n";
                cout << "Multiple account types (Savings, Current, Business)\n";
                cout << "Secure password-protected accounts\n";
                cout << "Deposit and withdrawal operations\n";
                cout << "Inter-account money transfers\n";
                cout << "Complete transaction history\n";
                cout << "Interest calculation for savings accounts\n";
                cout << "Account statements generation\n";
                cout << "Data persistence across sessions\n";
                cout << "\nVersion: 1.0\n";
                cout << "Developed as educational project\n";
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                break;
                
            case 5:
                cout << "\n============================================================\n";
                cout << "     Thank you for using Bank Management System!           \n";
                cout << "         Your data has been saved securely                 \n";
                cout << "============================================================\n";
                cout << "\nGoodbye! Have a great day!\n\n";
                return 0;
                
            default:
                cout << "\nInvalid choice! Please enter a number between 1-5.\n";
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
        }
    }
    
    return 0;
}
