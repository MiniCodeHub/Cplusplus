#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Contact {
    string name;
    string phone;
    string email;
};

void addContact() {
    ofstream file("contacts.txt", ios::app);
    Contact c;

    cout << "\nEnter name: ";
    getline(cin, c.name);

    cout << "Enter phone: ";
    getline(cin, c.phone);

    cout << "Enter email: ";
    getline(cin, c.email);

    file << c.name << "|" << c.phone << "|" << c.email << endl;
    file.close();

    cout << "Contact added successfully.\n";
}

void viewContacts() {
    ifstream file("contacts.txt");
    string line;

    cout << "\nContact List:\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

void searchContact() {
    ifstream file("contacts.txt");
    string keyword, line;
    bool found = false;

    cout << "\nEnter name to search: ";
    getline(cin, keyword);

    while (getline(file, line)) {
        if (line.find(keyword) != string::npos) {
            cout << "Found: " << line << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "Contact not found.\n";
    }

    file.close();
}

void deleteContact() {
    ifstream file("contacts.txt");
    ofstream temp("temp.txt");
    string name, line;
    bool deleted = false;

    cout << "\nEnter name to delete: ";
    getline(cin, name);

    while (getline(file, line)) {
        if (line.find(name) == string::npos) {
            temp << line << endl;
        } else {
            deleted = true;
        }
    }

    file.close();
    temp.close();

    remove("contacts.txt");
    rename("temp.txt", "contacts.txt");

    if (deleted) {
        cout << "Contact deleted successfully.\n";
    } else {
        cout << "Contact not found.\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\nAddress Book Menu\n";
        cout << "1. Add Contact\n";
        cout << "2. View Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Delete Contact\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                addContact();
                break;
            case 2:
                viewContacts();
                break;
            case 3:
                searchContact();
                break;
            case 4:
                deleteContact();
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
