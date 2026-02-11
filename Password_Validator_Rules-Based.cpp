#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class PasswordValidator {
private:
    string password;
    
    // Individual validation methods
    bool hasMinimumLength(int minLength = 8) {
        return password.length() >= minLength;
    }
    
    bool hasUppercase() {
        for (char c : password) {
            if (isupper(c)) return true;
        }
        return false;
    }
    
    bool hasLowercase() {
        for (char c : password) {
            if (islower(c)) return true;
        }
        return false;
    }
    
    bool hasDigit() {
        for (char c : password) {
            if (isdigit(c)) return true;
        }
        return false;
    }
    
    bool hasSpecialChar() {
        string specialChars = "!@#$%^&*()_+-=[]{}|;:',.<>?/~`";
        for (char c : password) {
            if (specialChars.find(c) != string::npos) {
                return true;
            }
        }
        return false;
    }
    
    bool hasNoSpaces() {
        return password.find(' ') == string::npos;
    }
    
    bool isNotCommon() {
        // List of common passwords to avoid
        string commonPasswords[] = {
            "password", "123456", "12345678", "qwerty", 
            "abc123", "password123", "admin", "letmein"
        };
        
        for (const string& common : commonPasswords) {
            if (password == common) return false;
        }
        return true;
    }

public:
    PasswordValidator(const string& pwd) : password(pwd) {}
    
    // Validate with detailed feedback
    void validateDetailed() {
        cout << "\n========== PASSWORD VALIDATION ==========\n";
        cout << "Password: " << string(password.length(), '*') << "\n\n";
        
        bool allPassed = true;
        
        cout << "[" << (hasMinimumLength() ? "PASS" : "FAIL") << "] Minimum 8 characters\n";
        if (!hasMinimumLength()) allPassed = false;
        
        cout << "[" << (hasUppercase() ? "PASS" : "FAIL") << "] Contains uppercase letter\n";
        if (!hasUppercase()) allPassed = false;
        
        cout << "[" << (hasLowercase() ? "PASS" : "FAIL") << "] Contains lowercase letter\n";
        if (!hasLowercase()) allPassed = false;
        
        cout << "[" << (hasDigit() ? "PASS" : "FAIL") << "] Contains digit\n";
        if (!hasDigit()) allPassed = false;
        
        cout << "[" << (hasSpecialChar() ? "PASS" : "FAIL") << "] Contains special character\n";
        if (!hasSpecialChar()) allPassed = false;
        
        cout << "[" << (hasNoSpaces() ? "PASS" : "FAIL") << "] No spaces\n";
        if (!hasNoSpaces()) allPassed = false;
        
        cout << "[" << (isNotCommon() ? "PASS" : "FAIL") << "] Not a common password\n";
        if (!isNotCommon()) allPassed = false;
        
        cout << "\n========================================\n";
        if (allPassed) {
            cout << "PASSWORD IS VALID! Strong password.\n";
        } else {
            cout << "PASSWORD IS INVALID. Please fix the issues above.\n";
        }
        cout << "========================================\n";
    }
    
    // Simple boolean validation
    bool isValid() {
        return hasMinimumLength() && hasUppercase() && hasLowercase() 
               && hasDigit() && hasSpecialChar() && hasNoSpaces() && isNotCommon();
    }
    
    // Get password strength
    string getStrength() {
        int score = 0;
        if (hasMinimumLength()) score++;
        if (hasUppercase()) score++;
        if (hasLowercase()) score++;
        if (hasDigit()) score++;
        if (hasSpecialChar()) score++;
        if (hasNoSpaces()) score++;
        if (isNotCommon()) score++;
        if (password.length() >= 12) score++; // Bonus for longer passwords
        
        if (score <= 3) return "Weak";
        else if (score <= 5) return "Medium";
        else if (score <= 7) return "Strong";
        else return "Very Strong";
    }
};

int main() {
    cout << "=== PASSWORD VALIDATOR ===\n";
    cout << "Test your password strength!\n\n";
    
    // Test examples
    string testPasswords[] = {
        "password",           // Common, weak
        "Pass123",           // Too short, no special char
        "Password123",       // No special char
        "Pass@123",          // Good!
        "MyS3cur3P@ssw0rd!", // Very strong
        "abc 123",           // Has spaces
        "ALLUPPERCASE123!",  // No lowercase
        "alllowercase123!"   // No uppercase
    };
    
    for (const string& pwd : testPasswords) {
        PasswordValidator validator(pwd);
        validator.validateDetailed();
        cout << "Strength: " << validator.getStrength() << "\n\n";
    }
    
    // Interactive mode
    cout << "\n\n=== TRY YOUR OWN PASSWORD ===\n";
    cout << "Enter a password to validate: ";
    string userPassword;
    getline(cin, userPassword);
    
    PasswordValidator userValidator(userPassword);
    userValidator.validateDetailed();
    cout << "Strength: " << userValidator.getStrength() << "\n";
    
    return 0;
}