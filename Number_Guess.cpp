#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    char playAgain;

    do {
        srand(time(0));
        int secretNumber = rand() % 100 + 1; // 1 to 100
        int guess;
        int attempts = 0;

        cout << "Number Guessing Game\n";
        cout << "Guess a number between 1 and 100\n\n";

        do {
            cout << "Enter your guess: ";
            cin >> guess;
            attempts++;

            if (guess > secretNumber) {
                cout << "Too High!\n";
            } else if (guess < secretNumber) {
                cout << "Too Low!\n";
            } else {
                cout << "Correct! You guessed it in "
                     << attempts << " attempts.\n";
            }
        } while (guess != secretNumber);

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;
        cout << "\n";

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "Thanks for playing!\n";
    return 0;
}
