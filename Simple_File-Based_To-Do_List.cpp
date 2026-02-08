#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

class TodoList {
private:
    vector<string> tasks;
    string filename;

public:
    TodoList(string file) : filename(file) {
        loadTasks();
    }

    void displayMenu() {
        cout << "\n========== TO-DO LIST ==========\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Delete Task\n";
        cout << "4. Clear All Tasks\n";
        cout << "5. Exit\n";
        cout << "================================\n";
        cout << "Enter your choice: ";
    }

    void addTask() {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nEnter task description: ";
        string task;
        getline(cin, task);
        
        if (!task.empty()) {
            tasks.push_back(task);
            saveTasks();
            cout << "Task added successfully!\n";
        } else {
            cout << "Task cannot be empty!\n";
        }
    }

    void viewTasks() {
        cout << "\n========== YOUR TASKS ==========\n";
        if (tasks.empty()) {
            cout << "No tasks yet. Add some tasks!\n";
        } else {
            for (size_t i = 0; i < tasks.size(); i++) {
                cout << i + 1 << ". " << tasks[i] << "\n";
            }
        }
        cout << "================================\n";
    }

    void deleteTask() {
        if (tasks.empty()) {
            cout << "\nNo tasks to delete!\n";
            return;
        }

        viewTasks();
        cout << "\nEnter task number to delete: ";
        int taskNum;
        cin >> taskNum;

        if (taskNum > 0 && taskNum <= static_cast<int>(tasks.size())) {
            cout << "Deleting: " << tasks[taskNum - 1] << "\n";
            tasks.erase(tasks.begin() + taskNum - 1);
            saveTasks();
            cout << "Task deleted successfully!\n";
        } else {
            cout << "Invalid task number!\n";
        }
    }

    void clearAllTasks() {
        if (tasks.empty()) {
            cout << "\nNo tasks to clear!\n";
            return;
        }

        cout << "\nAre you sure you want to delete all tasks? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') {
            tasks.clear();
            saveTasks();
            cout << "All tasks cleared!\n";
        } else {
            cout << "Operation cancelled.\n";
        }
    }

    void saveTasks() {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const string& task : tasks) {
                outFile << task << "\n";
            }
            outFile.close();
        } else {
            cerr << "Error: Unable to save tasks to file!\n";
        }
    }

    void loadTasks() {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string task;
            while (getline(inFile, task)) {
                if (!task.empty()) {
                    tasks.push_back(task);
                }
            }
            inFile.close();
        }
        // If file doesn't exist, it will be created on first save
    }

    void run() {
        int choice;
        
        cout << "Welcome to Your To-Do List Manager!\n";
        
        while (true) {
            displayMenu();
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input! Please enter a number.\n";
                continue;
            }

            switch (choice) {
                case 1:
                    addTask();
                    break;
                case 2:
                    viewTasks();
                    break;
                case 3:
                    deleteTask();
                    break;
                case 4:
                    clearAllTasks();
                    break;
                case 5:
                    cout << "\nSaving and exiting... Goodbye!\n";
                    return;
                default:
                    cout << "\nInvalid choice! Please select 1-5.\n";
            }
        }
    }
};

int main() {
    TodoList myList("tasks.txt");
    myList.run();
    return 0;
}