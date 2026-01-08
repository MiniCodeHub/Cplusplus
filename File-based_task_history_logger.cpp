#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

struct LogEntry {
    string timestamp;
    string action;
    string taskName;
    
    string toString() const {
        return "[" + timestamp + "] " + action + ": " + taskName;
    }
};

class TaskLogger {
private:
    const string LOG_FILE = "task_history.log";
    vector<string> tasks;
    
    string getCurrentTimestamp() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        
        stringstream ss;
        ss << setfill('0') 
           << setw(4) << 1900 + ltm->tm_year << "-"
           << setw(2) << 1 + ltm->tm_mon << "-"
           << setw(2) << ltm->tm_mday << " "
           << setw(2) << ltm->tm_hour << ":"
           << setw(2) << ltm->tm_min << ":"
           << setw(2) << ltm->tm_sec;
        
        return ss.str();
    }
    
    void writeLog(const string& action, const string& taskName) {
        ofstream logFile(LOG_FILE, ios::app);
        if (logFile.is_open()) {
            logFile << getCurrentTimestamp() << "|" << action << "|" << taskName << endl;
            logFile.close();
        } else {
            cerr << "Error: Could not open log file for writing." << endl;
        }
    }
    
    vector<LogEntry> readLogs() {
        vector<LogEntry> entries;
        ifstream logFile(LOG_FILE);
        
        if (logFile.is_open()) {
            string line;
            while (getline(logFile, line)) {
                LogEntry entry;
                size_t pos1 = line.find('|');
                size_t pos2 = line.find('|', pos1 + 1);
                
                if (pos1 != string::npos && pos2 != string::npos) {
                    entry.timestamp = line.substr(0, pos1);
                    entry.action = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    entry.taskName = line.substr(pos2 + 1);
                    entries.push_back(entry);
                }
            }
            logFile.close();
        }
        
        return entries;
    }
    
public:
    void addTask(const string& taskName) {
        tasks.push_back(taskName);
        writeLog("ADD", taskName);
        cout << "Task added: " << taskName << endl;
    }
    
    void deleteTask(const string& taskName) {
        auto it = find(tasks.begin(), tasks.end(), taskName);
        if (it != tasks.end()) {
            tasks.erase(it);
            writeLog("DELETE", taskName);
            cout << "Task deleted: " << taskName << endl;
        } else {
            cout << "Task not found: " << taskName << endl;
        }
    }
    
    void displayCurrentTasks() {
        cout << "\nCurrent Tasks:" << endl;
        if (tasks.empty()) {
            cout << "No tasks available." << endl;
        } else {
            for (size_t i = 0; i < tasks.size(); i++) {
                cout << (i + 1) << ". " << tasks[i] << endl;
            }
        }
    }
    
    void displayHistory() {
        vector<LogEntry> entries = readLogs();
        
        cout << "\nTask History (Latest First):" << endl;
        if (entries.empty()) {
            cout << "No history available." << endl;
            return;
        }
        
        // Display in reverse order (latest first)
        for (auto it = entries.rbegin(); it != entries.rend(); ++it) {
            cout << it->toString() << endl;
        }
        cout << "\nTotal entries: " << entries.size() << endl;
    }
    
    void clearHistory() {
        ofstream logFile(LOG_FILE, ios::trunc);
        logFile.close();
        cout << "History cleared." << endl;
    }
};

void displayMenu() {
    cout << "\n----------------------------------------" << endl;
    cout << "   Task History Logger              " << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Add Task" << endl;
    cout << "2. Delete Task" << endl;
    cout << "3. View Current Tasks" << endl;
    cout << "4. View History (Latest First)" << endl;
    cout << "5. Clear History" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option: ";
}

int main() {
    TaskLogger logger;
    int choice;
    string taskName;
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer
        
        switch (choice) {
            case 1:
                cout << "Enter task name: ";
                getline(cin, taskName);
                logger.addTask(taskName);
                break;
                
            case 2:
                cout << "Enter task name to delete: ";
                getline(cin, taskName);
                logger.deleteTask(taskName);
                break;
                
            case 3:
                logger.displayCurrentTasks();
                break;
                
            case 4:
                logger.displayHistory();
                break;
                
            case 5:
                logger.clearHistory();
                break;
                
            case 6:
                cout << "Goodbye!" << endl;
                return 0;
                
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    }
    
    return 0;
}