#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

enum class Priority {
    HIGH = 3,
    MEDIUM = 2,
    LOW = 1
};

class Task {
private:
    static int nextId;
    int id;
    string description;
    Priority priority;
    bool completed;
    
public:
    Task(const string& desc, Priority prio) 
        : id(nextId++), description(desc), priority(prio), completed(false) {}
    
    int getId() const { return id; }
    string getDescription() const { return description; }
    Priority getPriority() const { return priority; }
    bool isCompleted() const { return completed; }
    
    void markCompleted() { completed = true; }
    
    string getPriorityString() const {
        switch(priority) {
            case Priority::HIGH: return "HIGH";
            case Priority::MEDIUM: return "MEDIUM";
            case Priority::LOW: return "LOW";
            default: return "UNKNOWN";
        }
    }
    
    // Comparison operator for sorting (higher priority first)
    bool operator<(const Task& other) const {
        if (completed != other.completed) return completed < other.completed;
        return static_cast<int>(priority) > static_cast<int>(other.priority);
    }
    
    void display() const {
        cout << "[" << (completed ? "X" : " ") << "] "
             << "ID:" << setw(3) << id << " | "
             << setw(6) << left << getPriorityString() << " | "
             << description << endl;
    }
};

int Task::nextId = 1;

class TaskScheduler {
private:
    vector<Task> tasks;
    
    void sortTasks() {
        sort(tasks.begin(), tasks.end());
    }
    
public:
    void addTask(const string& desc, Priority prio) {
        tasks.emplace_back(desc, prio);
        sortTasks();
        cout << "\nTask added successfully! (Priority: " 
             << tasks.back().getPriorityString() << ")\n";
    }
    
    void displayAllTasks() const {
        if (tasks.empty()) {
            cout << "\nNo tasks in scheduler.\n";
            return;
        }
        
        cout << "\n" << string(60, '=') << endl;
        cout << "          TASK SCHEDULER - ALL TASKS" << endl;
        cout << string(60, '=') << endl;
        
        for (const auto& task : tasks) {
            task.display();
        }
        cout << string(60, '=') << endl;
    }
    
    void showNextTask() const {
        for (const auto& task : tasks) {
            if (!task.isCompleted()) {
                cout << "\nNEXT TASK TO EXECUTE:\n";
                cout << string(40, '-') << endl;
                task.display();
                cout << string(40, '-') << endl;
                return;
            }
        }
        cout << "\nAll tasks completed!\n";
    }
    
    bool executeNextTask() {
        for (auto& task : tasks) {
            if (!task.isCompleted()) {
                cout << "\nExecuting task: " << task.getDescription() << endl;
                task.markCompleted();
                cout << "Task completed!\n";
                sortTasks();
                return true;
            }
        }
        cout << "\nNo pending tasks to execute.\n";
        return false;
    }
    
    bool executeTaskById(int id) {
        for (auto& task : tasks) {
            if (task.getId() == id && !task.isCompleted()) {
                cout << "\nExecuting task: " << task.getDescription() << endl;
                task.markCompleted();
                cout << "Task completed!\n";
                sortTasks();
                return true;
            }
        }
        cout << "\nTask not found or already completed.\n";
        return false;
    }
    
    void displayStats() const {
        int total = tasks.size();
        int completed = 0, high = 0, medium = 0, low = 0;
        
        for (const auto& task : tasks) {
            if (task.isCompleted()) completed++;
            switch(task.getPriority()) {
                case Priority::HIGH: high++; break;
                case Priority::MEDIUM: medium++; break;
                case Priority::LOW: low++; break;
            }
        }
        
        cout << "\nSCHEDULER STATISTICS:\n";
        cout << "  Total Tasks: " << total << endl;
        cout << "  Completed: " << completed << " | Pending: " << (total - completed) << endl;
        cout << "  Priority Breakdown: HIGH(" << high << ") MEDIUM(" << medium 
             << ") LOW(" << low << ")\n";
    }
};

void displayMenu() {
    cout << "\n+------------------------------------+\n";
    cout << "|   PRIORITY TASK SCHEDULER MENU    |\n";
    cout << "+------------------------------------+\n";
    cout << "1. Add Task (High Priority)\n";
    cout << "2. Add Task (Medium Priority)\n";
    cout << "3. Add Task (Low Priority)\n";
    cout << "4. Display All Tasks\n";
    cout << "5. Show Next Task\n";
    cout << "6. Execute Next Task\n";
    cout << "7. Execute Task by ID\n";
    cout << "8. Show Statistics\n";
    cout << "0. Exit\n";
    cout << "\nChoice: ";
}

int main() {
    TaskScheduler scheduler;
    int choice;
    string description;
    int taskId;
    
    cout << "WELCOME TO PRIORITY TASK SCHEDULER\n";
    
    // Sample tasks for demonstration
    scheduler.addTask("Critical bug fix in production", Priority::HIGH);
    scheduler.addTask("Code review for feature branch", Priority::MEDIUM);
    scheduler.addTask("Update documentation", Priority::LOW);
    scheduler.addTask("Security patch deployment", Priority::HIGH);
    
    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                cout << "Enter task description: ";
                getline(cin, description);
                scheduler.addTask(description, Priority::HIGH);
                break;
                
            case 2:
                cout << "Enter task description: ";
                getline(cin, description);
                scheduler.addTask(description, Priority::MEDIUM);
                break;
                
            case 3:
                cout << "Enter task description: ";
                getline(cin, description);
                scheduler.addTask(description, Priority::LOW);
                break;
                
            case 4:
                scheduler.displayAllTasks();
                break;
                
            case 5:
                scheduler.showNextTask();
                break;
                
            case 6:
                scheduler.executeNextTask();
                break;
                
            case 7:
                cout << "Enter task ID: ";
                cin >> taskId;
                scheduler.executeTaskById(taskId);
                break;
                
            case 8:
                scheduler.displayStats();
                break;
                
            case 0:
                cout << "\nThank you for using Task Scheduler!\n";
                return 0;
                
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    }
    
    return 0;
}