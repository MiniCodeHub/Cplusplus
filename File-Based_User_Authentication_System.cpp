#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Simple hash function (NOT cryptographically secure - for demonstration only)
// In production, use bcrypt, argon2, or similar libraries
unsigned long simpleHash(const string& str) {
    unsigned long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

class AuthSystem {
private:
    const string filename = "users.txt";
    
    // Check if username already exists
    bool userExists(const string& username) {
        ifstream file(filename);
        string line, storedUser;
        
        while (getline(file, line)) {
            istringstream iss(line);
            if (iss >> storedUser) {
                if (storedUser == username) {
                    file.close();
                    return true;
                }
            }
        }
        file.close();
        return false;
    }
    
public:
    // Register a new user
    bool registerUser(const string& username, const string& password) {
        // Validate input
        if (username.empty() || password.empty()) {
            cout << "Username and password cannot be empty!\n";
            return false;
        }
        
        if (username.find(' ') != string::npos) {
            cout << "Username cannot contain spaces!\n";
            return false;
        }
        
        if (password.length() < 4) {
            cout << "Password must be at least 4 characters!\n";
            return false;
        }
        
        // Check if user already exists
        if (userExists(username)) {
            cout << "Username already exists! Please choose another.\n";
            return false;
        }
        
        // Hash the password (simple hash for demonstration)
        unsigned long hashedPassword = simpleHash(password);
        
        // Save to file
        ofstream file(filename, ios::app);
        if (!file) {
            cout << "Error: Could not open file for writing!\n";
            return false;
        }
        
        file << username << " " << hashedPassword << "\n";
        file.close();
        
        cout << "Registration successful! Welcome, " << username << "!\n";
        return true;
    }
    
    // Login verification
    bool loginUser(const string& username, const string& password) {
        if (username.empty() || password.empty()) {
            cout << "Please enter both username and password!\n";
            return false;
        }
        
        // Hash the entered password
        unsigned long hashedPassword = simpleHash(password);
        
        // Read from file and verify
        ifstream file(filename);
        if (!file) {
            cout << "No registered users found!\n";
            return false;
        }
        
        string storedUser;
        unsigned long storedHash;
        bool found = false;
        
        while (file >> storedUser >> storedHash) {
            if (storedUser == username) {
                found = true;
                if (storedHash == hashedPassword) {
                    file.close();
                    cout << "Login successful! Welcome back, " << username << "!\n";
                    return true;
                } else {
                    file.close();
                    cout << "Incorrect password!\n";
                    return false;
                }
            }
        }
        
        file.close();
        if (!found) {
            cout << "Username not found!\n";
        }
        return false;
    }
    
    // Display all registered users (for demonstration)
    void listUsers() {
        ifstream file(filename);
        if (!file) {
            cout << "No users registered yet.\n";
            return;
        }
        
        cout << "\nRegistered Users:\n";
        cout << "-------------------\n";
        
        string username;
        unsigned long hash;
        int count = 0;
        
        while (file >> username >> hash) {
            count++;
            cout << count << ". " << username << "\n";
        }
        
        if (count == 0) {
            cout << "No users found.\n";
        }
        
        file.close();
    }
};

void displayMenu() {
    cout << "   Authentication System    \n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. List Users\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

int main() {
    AuthSystem auth;
    int choice;
    string username, password;
    
    cout << "Welcome to the User Authentication System!\n";
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear the newline character
        
        switch (choice) {
            case 1: {
                cout << "\n=== REGISTRATION ===\n";
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                auth.registerUser(username, password);
                break;
            }
            
            case 2: {
                cout << "\n=== LOGIN ===\n";
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                auth.loginUser(username, password);
                break;
            }
            
            case 3: {
                auth.listUsers();
                break;
            }
            
            case 4: {
                cout << "\nThank you for using the Authentication System!\n";
                cout << "Goodbye!\n";
                return 0;
            }
            
            default: {
                cout << "Invalid option! Please choose 1-4.\n";
                break;
            }
        }
    }
    
    return 0;
}