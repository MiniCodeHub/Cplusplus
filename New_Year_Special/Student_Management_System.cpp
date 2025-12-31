#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

class Student {
private:
    int id;
    string name;
    int age;
    string course;
    float gpa;

public:
    Student() : id(0), name(""), age(0), course(""), gpa(0.0) {}
    
    Student(int i, string n, int a, string c, float g) 
        : id(i), name(n), age(a), course(c), gpa(g) {}

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    int getAge() const { return age; }
    string getCourse() const { return course; }
    float getGpa() const { return gpa; }

    // Setters
    void setName(string n) { name = n; }
    void setAge(int a) { age = a; }
    void setCourse(string c) { course = c; }
    void setGpa(float g) { gpa = g; }

    void display() const {
        cout << left << setw(8) << id 
             << setw(20) << name 
             << setw(6) << age 
             << setw(20) << course 
             << fixed << setprecision(2) << gpa << endl;
    }

    // File operations
    void saveToFile(ofstream &out) const {
        out << id << endl << name << endl << age << endl 
            << course << endl << gpa << endl;
    }

    void loadFromFile(ifstream &in) {
        in >> id;
        in.ignore();
        getline(in, name);
        in >> age;
        in.ignore();
        getline(in, course);
        in >> gpa;
        in.ignore();
    }
};

class StudentManagementSystem {
private:
    vector<Student> students;
    int nextId;
    const string filename = "students.txt";

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

public:
    StudentManagementSystem() : nextId(1) {
        loadFromFile();
    }

    ~StudentManagementSystem() {
        saveToFile();
    }

    void addStudent() {
        string name, course;
        int age;
        float gpa;

        cout << "\n=== Add New Student ===\n";
        cout << "Enter Name: ";
        clearInput();
        getline(cin, name);

        cout << "Enter Age: ";
        while (!(cin >> age) || age < 1 || age > 150) {
            cout << "Invalid age. Enter again: ";
            clearInput();
        }

        cout << "Enter Course: ";
        clearInput();
        getline(cin, course);

        cout << "Enter GPA (0.0-4.0): ";
        while (!(cin >> gpa) || gpa < 0.0 || gpa > 4.0) {
            cout << "Invalid GPA. Enter again: ";
            clearInput();
        }

        students.push_back(Student(nextId++, name, age, course, gpa));
        cout << "\n Student added successfully!\n";
    }

    void displayAllStudents() {
        if (students.empty()) {
            cout << "\nNo students in the system.\n";
            return;
        }

        cout << "\n=== All Students ===\n";
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(6) << "Age" 
             << setw(20) << "Course" 
             << "GPA" << endl;
        cout << string(60, '-') << endl;

        for (const auto &student : students) {
            student.display();
        }
        cout << "\nTotal Students: " << students.size() << endl;
    }

    void searchStudent() {
        if (students.empty()) {
            cout << "\nNo students to search.\n";
            return;
        }

        int id;
        cout << "\nEnter Student ID to search: ";
        cin >> id;

        for (const auto &student : students) {
            if (student.getId() == id) {
                cout << "\n=== Student Found ===\n";
                cout << left << setw(8) << "ID" 
                     << setw(20) << "Name" 
                     << setw(6) << "Age" 
                     << setw(20) << "Course" 
                     << "GPA" << endl;
                cout << string(60, '-') << endl;
                student.display();
                return;
            }
        }
        cout << "\n Student with ID " << id << " not found.\n";
    }

    void updateStudent() {
        if (students.empty()) {
            cout << "\nNo students to update.\n";
            return;
        }

        int id;
        cout << "\nEnter Student ID to update: ";
        cin >> id;

        for (auto &student : students) {
            if (student.getId() == id) {
                cout << "\n=== Update Student ===\n";
                cout << "Current details:\n";
                student.display();

                string name, course;
                int age;
                float gpa;

                cout << "\nEnter new Name (current: " << student.getName() << "): ";
                clearInput();
                getline(cin, name);
                student.setName(name);

                cout << "Enter new Age (current: " << student.getAge() << "): ";
                while (!(cin >> age) || age < 1 || age > 150) {
                    cout << "Invalid age. Enter again: ";
                    clearInput();
                }
                student.setAge(age);

                cout << "Enter new Course (current: " << student.getCourse() << "): ";
                clearInput();
                getline(cin, course);
                student.setCourse(course);

                cout << "Enter new GPA (current: " << student.getGpa() << "): ";
                while (!(cin >> gpa) || gpa < 0.0 || gpa > 4.0) {
                    cout << "Invalid GPA. Enter again: ";
                    clearInput();
                }
                student.setGpa(gpa);

                cout << "\n Student updated successfully!\n";
                return;
            }
        }
        cout << "\n Student with ID " << id << " not found.\n";
    }

    void deleteStudent() {
        if (students.empty()) {
            cout << "\nNo students to delete.\n";
            return;
        }

        int id;
        cout << "\nEnter Student ID to delete: ";
        cin >> id;

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getId() == id) {
                cout << "\nDeleting student: " << it->getName() << endl;
                students.erase(it);
                cout << " Student deleted successfully!\n";
                return;
            }
        }
        cout << "\n Student with ID " << id << " not found.\n";
    }

    void saveToFile() {
        ofstream out(filename);
        if (!out) {
            cout << "Error saving to file.\n";
            return;
        }

        out << nextId << endl;
        out << students.size() << endl;
        for (const auto &student : students) {
            student.saveToFile(out);
        }
        out.close();
    }

    void loadFromFile() {
        ifstream in(filename);
        if (!in) return;

        in >> nextId;
        int count;
        in >> count;
        in.ignore();

        for (int i = 0; i < count; i++) {
            Student s;
            s.loadFromFile(in);
            students.push_back(s);
        }
        in.close();
    }
};

void displayMenu() {
    cout << "1. Add Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student\n";
    cout << "5. Delete Student\n";
    cout << "6. Exit\n";
    cout << "\nEnter your choice: ";
}

int main() {
    StudentManagementSystem sms;
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                sms.addStudent();
                break;
            case 2:
                sms.displayAllStudents();
                break;
            case 3:
                sms.searchStudent();
                break;
            case 4:
                sms.updateStudent();
                break;
            case 5:
                sms.deleteStudent();
                break;
            case 6:
                cout << "\nThank you for using Student Management System!\n";
                return 0;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}