#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// Function declarations
void displayMenu();
double add(double a, double b);
double subtract(double a, double b);
double multiply(double a, double b);
double divide(double a, double b);
double power(double base, double exp);
double squareRoot(double num);
void getInput(double &a, double &b);
void getSingleInput(double &a);

int main() {
    int choice;
    double num1, num2, result;
    char continueCalc;
    
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "     MENU-DRIVEN CALCULATOR\n";
    cout << "========================================\n";
    
    do {
        displayMenu();
        cout << "\nEnter your choice (1-8): ";
        cin >> choice;
        
        // Input validation
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1:
                getInput(num1, num2);
                result = add(num1, num2);
                cout << "\nResult: " << num1 << " + " << num2 << " = " << result << endl;
                break;
                
            case 2:
                getInput(num1, num2);
                result = subtract(num1, num2);
                cout << "\nResult: " << num1 << " - " << num2 << " = " << result << endl;
                break;
                
            case 3:
                getInput(num1, num2);
                result = multiply(num1, num2);
                cout << "\nResult: " << num1 << " × " << num2 << " = " << result << endl;
                break;
                
            case 4:
                getInput(num1, num2);
                if (num2 == 0) {
                    cout << "\nError: Division by zero is undefined!\n";
                } else {
                    result = divide(num1, num2);
                    cout << "\nResult: " << num1 << " ÷ " << num2 << " = " << result << endl;
                }
                break;
                
            case 5:
                getInput(num1, num2);
                result = power(num1, num2);
                cout << "\nResult: " << num1 << "^" << num2 << " = " << result << endl;
                break;
                
            case 6:
                getSingleInput(num1);
                if (num1 < 0) {
                    cout << "\nError: Cannot calculate square root of negative number!\n";
                } else {
                    result = squareRoot(num1);
                    cout << "\nResult: √" << num1 << " = " << result << endl;
                }
                break;
                
            case 7:
                cout << "\n--- Calculator History Cleared ---\n";
                break;
                
            case 8:
                cout << "\nThank you for using the calculator!\n";
                cout << "========================================\n";
                break;
                
            default:
                cout << "\nInvalid choice! Please select 1-8.\n";
        }
        
        if (choice >= 1 && choice <= 7) {
            cout << "\nDo you want to perform another calculation? (y/n): ";
            cin >> continueCalc;
            continueCalc = tolower(continueCalc);
            
            if (continueCalc != 'y') {
                choice = 8;
                cout << "\nThank you for using the calculator!\n";
                cout << "========================================\n";
            }
        }
        
    } while (choice != 8);
    
    return 0;
}

// Function to display menu
void displayMenu() {
    cout << "\n========================================\n";
    cout << "              OPERATIONS\n";
    cout << "========================================\n";
    cout << "1. Addition (+)\n";
    cout << "2. Subtraction (-)\n";
    cout << "3. Multiplication (x)\n";
    cout << "4. Division (/)\n";
    cout << "5. Power (x^y)\n";
    cout << "6. Square Root (sq)\n";
    cout << "7. Clear\n";
    cout << "8. Exit\n";
    cout << "========================================\n";
}

// Function to get two numbers from user
void getInput(double &a, double &b) {
    cout << "Enter first number: ";
    cin >> a;
    cout << "Enter second number: ";
    cin >> b;
}

// Function to get single number from user
void getSingleInput(double &a) {
    cout << "Enter number: ";
    cin >> a;
}

// Addition function
double add(double a, double b) {
    return a + b;
}

// Subtraction function
double subtract(double a, double b) {
    return a - b;
}

// Multiplication function
double multiply(double a, double b) {
    return a * b;
}

// Division function
double divide(double a, double b) {
    return a / b;
}

// Power function
double power(double base, double exp) {
    return pow(base, exp);
}

// Square root function
double squareRoot(double num) {
    return sqrt(num);
}