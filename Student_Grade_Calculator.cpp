#include <iostream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

// Function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to calculate grade based on marks
char calculateGrade(double marks) {
    if (marks >= 90) {
        return 'A';
    } else if (marks >= 80) {
        return 'B';
    } else if (marks >= 70) {
        return 'C';
    } else if (marks >= 60) {
        return 'D';
    } else {
        return 'F';
    }
}

// Function to get grade description
string getGradeDescription(char grade) {
    switch(grade) {
        case 'A': return "Excellent";
        case 'B': return "Good";
        case 'C': return "Average";
        case 'D': return "Below Average";
        case 'F': return "Fail";
        default: return "Invalid";
    }
}

// Function to display header
void displayHeader() {
    cout << "\n========================================\n";
    cout << "   STUDENT GRADE CALCULATOR\n";
    cout << "========================================\n\n";
}

// Function to display grade with formatting
void displayResult(string studentName, double marks, char grade) {
    cout << "\n" << string(45, '=') << "\n";
    cout << "  RESULT CARD\n";
    cout << string(45, '=') << "\n";
    cout << left << setw(20) << "  Student Name:" << studentName << "\n";
    cout << left << setw(20) << "  Marks Obtained:" << fixed << setprecision(2) << marks << " / 100\n";
    cout << left << setw(20) << "  Grade:" << grade << "\n";
    cout << left << setw(20) << "  Performance:" << getGradeDescription(grade) << "\n";
    cout << string(45, '=') << "\n";
}

int main() {
    string studentName;
    double marks;
    char choice;

    do {
        displayHeader();

        // Get student name
        cout << "Enter student name: ";
        getline(cin, studentName);

        // Get marks with validation
        while (true) {
            cout << "Enter marks (0-100): ";
            
            if (cin >> marks) {
                if (marks >= 0 && marks <= 100) {
                    break;
                } else {
                    cout << "Invalid! Marks must be between 0 and 100.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                clearInputBuffer();
            }
        }

        // Calculate grade
        char grade = calculateGrade(marks);

        // Display result
        displayResult(studentName, marks, grade);

        // Grade breakdown display
        cout << "\nGrading Scale:\n";
        cout << "   A: 90-100 (Excellent)\n";
        cout << "   B: 80-89  (Good)\n";
        cout << "   C: 70-79  (Average)\n";
        cout << "   D: 60-69  (Below Average)\n";
        cout << "   F: 0-59   (Fail)\n";

        // Ask if user wants to continue
        cout << "\nCalculate another grade? (y/n): ";
        clearInputBuffer();
        cin.get(choice);
        clearInputBuffer();

    } while (choice == 'y' || choice == 'Y');

    cout << "\nThank you for using Grade Calculator!\n\n";

    return 0;
}