#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <sstream>
using namespace std;

class ExpressionEvaluator {
private:
    // Check if character is an operator
    bool isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }
    
    // Get precedence of operators
    int precedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }
    
    // Check if operator is right associative
    bool isRightAssociative(char op) {
        return op == '^';
    }
    
    // Perform arithmetic operation
    double applyOperator(double a, double b, char op) {
        switch(op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) {
                    throw runtime_error("Division by zero");
                }
                return a / b;
            case '^': return pow(a, b);
            default: return 0;
        }
    }

public:
    // Convert infix to postfix
    string infixToPostfix(const string& infix) {
        stack<char> operators;
        string postfix = "";
        
        for (size_t i = 0; i < infix.length(); i++) {
            char c = infix[i];
            
            // Skip whitespace
            if (isspace(c)) continue;
            
            // If operand (digit or decimal point), add to output
            if (isdigit(c) || c == '.') {
                while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.')) {
                    postfix += infix[i++];
                }
                postfix += ' '; // Space separator
                i--; // Adjust index
            }
            // If opening parenthesis, push to stack
            else if (c == '(') {
                operators.push(c);
            }
            // If closing parenthesis, pop until opening parenthesis
            else if (c == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    postfix += operators.top();
                    postfix += ' ';
                    operators.pop();
                }
                if (!operators.empty()) operators.pop(); // Remove '('
            }
            // If operator
            else if (isOperator(c)) {
                while (!operators.empty() && operators.top() != '(' &&
                       (precedence(operators.top()) > precedence(c) ||
                        (precedence(operators.top()) == precedence(c) && !isRightAssociative(c)))) {
                    postfix += operators.top();
                    postfix += ' ';
                    operators.pop();
                }
                operators.push(c);
            }
        }
        
        // Pop remaining operators
        while (!operators.empty()) {
            if (operators.top() != '(') {
                postfix += operators.top();
                postfix += ' ';
            }
            operators.pop();
        }
        
        return postfix;
    }
    
    // Evaluate postfix expression
    double evaluatePostfix(const string& postfix) {
        stack<double> values;
        istringstream iss(postfix);
        string token;
        
        while (iss >> token) {
            // If token is a number
            if (isdigit(token[0]) || (token.length() > 1 && token[0] == '-')) {
                values.push(stod(token));
            }
            // If token is an operator
            else if (token.length() == 1 && isOperator(token[0])) {
                if (values.size() < 2) {
                    throw runtime_error("Invalid expression");
                }
                
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                double result = applyOperator(a, b, token[0]);
                values.push(result);
            }
        }
        
        if (values.size() != 1) {
            throw runtime_error("Invalid expression");
        }
        
        return values.top();
    }
    
    // Evaluate infix expression directly
    double evaluateInfix(const string& infix) {
        string postfix = infixToPostfix(infix);
        return evaluatePostfix(postfix);
    }
    
    // Display step-by-step conversion
    void demonstrateConversion(const string& infix) {
        cout << "\n=== Infix to Postfix Conversion ===" << endl;
        cout << "Infix:    " << infix << endl;
        
        string postfix = infixToPostfix(infix);
        cout << "Postfix:  " << postfix << endl;
        
        try {
            double result = evaluatePostfix(postfix);
            cout << "Result:   " << result << endl;
        } catch (const exception& e) {
            cout << "Error:    " << e.what() << endl;
        }
    }
};

int main() {
    ExpressionEvaluator evaluator;
    
    cout << "Expression Evaluator (Infix to Postfix)" << endl;
    cout << "========================================" << endl;
    
    // Test cases
    string expressions[] = {
        "3 + 5 * 2",
        "(3 + 5) * 2",
        "10 + 2 * 6",
        "100 * 2 + 12",
        "100 * ( 2 + 12 )",
        "2 ^ 3 ^ 2",
        "(2 ^ 3) ^ 2",
        "3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3"
    };
    
    for (const string& expr : expressions) {
        evaluator.demonstrateConversion(expr);
    }
    
    // Interactive mode
    cout << "\n=== Interactive Mode ===" << endl;
    cout << "Enter infix expressions (or 'quit' to exit)" << endl;
    cout << "Supported operators: + - * / ^ ( )" << endl;
    
    string input;
    while (true) {
        cout << "\nEnter expression: ";
        getline(cin, input);
        
        if (input == "quit" || input == "exit") break;
        if (input.empty()) continue;
        
        try {
            evaluator.demonstrateConversion(input);
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return 0;
}