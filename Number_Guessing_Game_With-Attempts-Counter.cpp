#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    // Seed random number generator
    srand(time(0));
    
    // Generate random number between 1 and 100
    int secretNumber = rand() % 100 + 1;
    int guess;
    int attempts = 0;
    
    cout << "================================" << endl;
    cout << "  Welcome to Number Guessing!  " << endl;
    cout << "================================" << endl;
    cout << "I'm thinking of a number between 1 and 100." << endl;
    cout << "Can you guess it?" << endl << endl;
    
    // Main game loop
    do {
        cout << "Enter your guess: ";
        cin >> guess;
        attempts++;
        
        // Check for invalid input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input! Please enter a number." << endl << endl;
            attempts--;
            continue;
        }
        
        // Check if guess is out of range
        if (guess < 1 || guess > 100) {
            cout << "Please guess a number between 1 and 100!" << endl << endl;
            attempts--;
            continue;
        }
        
        // Provide hints
        if (guess < secretNumber) {
            cout << "Too low! Try again." << endl << endl;
        } else if (guess > secretNumber) {
            cout << "Too high! Try again." << endl << endl;
        } else {
            cout << "\nCongratulations! You guessed it!" << endl;
            cout << "The number was: " << secretNumber << endl;
            cout << "It took you " << attempts << " attempt(s)." << endl;
            
            // Performance rating
            if (attempts <= 5) {
                cout << "Amazing! You're a guessing master!" << endl;
            } else if (attempts <= 10) {
                cout << "Great job! Well done!" << endl;
            } else {
                cout << "Good effort! Better luck next time!" << endl;
            }
        }
        
    } while (guess != secretNumber);
    
    cout << "\nThanks for playing!" << endl;
    
    return 0;
}