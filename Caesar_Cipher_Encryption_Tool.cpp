// ============================================
//  Caesar Cipher Encryption Tool
//  Author  : You
//  Compile : g++ -o caesar caesar_cipher.cpp
//  Run     : ./caesar
//  Demonstrates: loops, character manipulation
// ============================================

#include <iostream>
#include <string>
#include <limits>
using namespace std;

// -- Core cipher -----------------------------

string caesarEncrypt(const string& text, int shift) {
    shift = ((shift % 26) + 26) % 26;   // normalise (handles negatives too)
    string result = "";

    for (char ch : text) {
        if (isupper(ch)) {
            result += char((ch - 'A' + shift) % 26 + 'A');
        } else if (islower(ch)) {
            result += char((ch - 'a' + shift) % 26 + 'a');
        } else {
            result += ch;               // spaces, digits, punctuation unchanged
        }
    }
    return result;
}

string caesarDecrypt(const string& text, int shift) {
    return caesarEncrypt(text, 26 - shift);   // reverse the shift
}

// -- Helpers ---------------------------------

void printDivider(char c = '-', int n = 44) {
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void showAlphabetMap(int shift) {
    shift = ((shift % 26) + 26) % 26;
    cout << "\n  Alphabet mapping (shift = " << shift << "):\n";
    cout << "  Original : ";
    for (char c = 'A'; c <= 'Z'; c++) cout << c;
    cout << "\n  Shifted  : ";
    for (int i = 0; i < 26; i++) cout << char((i + shift) % 26 + 'A');
    cout << "\n";
}

// -- Main ------------------------------------

int main() {
    printDivider('=');
    cout << "       CAESAR CIPHER ENCRYPTION TOOL\n";
    printDivider('=');

    string message;
    int    shift, choice;

    // Get message
    cout << "\n Enter message : ";
    getline(cin, message);

    // Get shift key
    while (true) {
        cout << " Shift key (1-25) : ";
        if (cin >> shift && shift >= 1 && shift <= 25) break;
        cout << " [!] Invalid input. Enter a number between 1 and 25.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore();   // flush newline

    // Show alphabet map
    showAlphabetMap(shift);

    // Choose mode
    cout << "\n Mode:\n";
    cout << "  1) Encrypt\n";
    cout << "  2) Decrypt\n";
    while (true) {
        cout << " Choice (1 or 2) : ";
        if (cin >> choice && (choice == 1 || choice == 2)) break;
        cout << " [!] Please enter 1 or 2.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Run cipher
    string output = (choice == 1)
        ? caesarEncrypt(message, shift)
        : caesarDecrypt(message, shift);

    string modeLabel = (choice == 1) ? "ENCRYPTED" : "DECRYPTED";

    // Print results
    cout << "\n";
    printDivider();
    cout << " BEFORE (" << message.size() << " chars)\n";
    printDivider();
    cout << " " << message << "\n\n";
    printDivider();
    cout << " AFTER - " << modeLabel << " (key = " << shift << ")\n";
    printDivider();
    cout << " " << output << "\n";
    printDivider('=');

    return 0;
}