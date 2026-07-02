#include <iostream>
#include <vector>

using namespace std;

// Book Structure
struct Book {
    int id;
    string title;
    string author;
    bool issued;
};

// Global Book Collection
vector<Book> library;

// Display Main Menu
void showMenu() {
    cout << "\n==============================\n";
    cout << " LIBRARY MANAGEMENT SYSTEM\n";
    cout << "==============================\n";
    cout << "1. Add Book\n";
    cout << "2. Display Books\n";
    cout << "3. Search Book\n";
    cout << "4. Delete Book\n";
    cout << "5. Issue Book\n";
    cout << "6. Return Book\n";
    cout << "7. Save Data\n";
    cout << "8. Load Data\n";
    cout << "9. Exit\n";
    cout << "------------------------------\n";
    cout << "Enter Choice: ";
}

int main() {

    int choice;

    do {

        showMenu();

        cin >> choice;

        switch(choice) {

            case 1:
                cout << "\n[Add Book Module Coming in Episode 2]\n";
                break;

            case 2:
                cout << "\n[Display Books Module Coming in Episode 3]\n";
                break;

            case 3:
                cout << "\n[Search Book Module Coming in Episode 4]\n";
                break;

            case 4:
                cout << "\n[Delete Book Module Coming in Episode 5]\n";
                break;

            case 5:
                cout << "\n[Issue Book Module Coming in Episode 6]\n";
                break;

            case 6:
                cout << "\n[Return Book Module Coming in Episode 7]\n";
                break;

            case 7:
                cout << "\n[Save Data Module Coming in Episode 8]\n";
                break;

            case 8:
                cout << "\n[Load Data Module Coming in Episode 9]\n";
                break;

            case 9:
                cout << "\nThank you for using the Library Management System!\n";
                break;

            default:
                cout << "\nInvalid Choice! Please try again.\n";
        }

    } while(choice != 9);

    return 0;
}