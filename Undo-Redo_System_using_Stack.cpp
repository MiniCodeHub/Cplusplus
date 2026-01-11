#include <iostream>
#include <stack>
#include <string>
#include <memory>
#include <vector>

// Base class for all actions (Command Pattern)
class Action {
public:
    virtual ~Action() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string description() const = 0;
};

// Text Editor class to maintain state
class TextEditor {
private:
    std::string content;
    
public:
    void insert(const std::string& text, size_t pos) {
        if (pos <= content.length()) {
            content.insert(pos, text);
        }
    }
    
    void remove(size_t pos, size_t length) {
        if (pos < content.length()) {
            content.erase(pos, length);
        }
    }
    
    std::string getContent() const { return content; }
    void setContent(const std::string& newContent) { content = newContent; }
};

// Insert Text Action
class InsertAction : public Action {
private:
    TextEditor& editor;
    std::string text;
    size_t position;
    
public:
    InsertAction(TextEditor& ed, const std::string& txt, size_t pos)
        : editor(ed), text(txt), position(pos) {}
    
    void execute() override {
        editor.insert(text, position);
    }
    
    void undo() override {
        editor.remove(position, text.length());
    }
    
    std::string description() const override {
        return "Insert '" + text + "' at position " + std::to_string(position);
    }
};

// Delete Text Action
class DeleteAction : public Action {
private:
    TextEditor& editor;
    std::string deletedText;
    size_t position;
    size_t length;
    
public:
    DeleteAction(TextEditor& ed, size_t pos, size_t len)
        : editor(ed), position(pos), length(len) {
        // Store the text being deleted for undo
        std::string content = editor.getContent();
        if (pos < content.length()) {
            deletedText = content.substr(pos, len);
        }
    }
    
    void execute() override {
        editor.remove(position, length);
    }
    
    void undo() override {
        editor.insert(deletedText, position);
    }
    
    std::string description() const override {
        return "Delete " + std::to_string(length) + " chars at position " + std::to_string(position);
    }
};

// Task Manager class
class TaskManager {
private:
    std::vector<std::string> tasks;
    
public:
    void addTask(const std::string& task) {
        tasks.push_back(task);
    }
    
    void removeTask(size_t index) {
        if (index < tasks.size()) {
            tasks.erase(tasks.begin() + index);
        }
    }
    
    std::vector<std::string> getTasks() const { return tasks; }
    
    void displayTasks() const {
        if (tasks.empty()) {
            std::cout << "  (No tasks)\n";
            return;
        }
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << tasks[i] << "\n";
        }
    }
};

// Add Task Action
class AddTaskAction : public Action {
private:
    TaskManager& manager;
    std::string task;
    
public:
    AddTaskAction(TaskManager& mgr, const std::string& t)
        : manager(mgr), task(t) {}
    
    void execute() override {
        manager.addTask(task);
    }
    
    void undo() override {
        auto tasks = manager.getTasks();
        if (!tasks.empty()) {
            manager.removeTask(tasks.size() - 1);
        }
    }
    
    std::string description() const override {
        return "Add task: '" + task + "'";
    }
};

// Remove Task Action
class RemoveTaskAction : public Action {
private:
    TaskManager& manager;
    std::string removedTask;
    size_t index;
    
public:
    RemoveTaskAction(TaskManager& mgr, size_t idx)
        : manager(mgr), index(idx) {
        auto tasks = manager.getTasks();
        if (idx < tasks.size()) {
            removedTask = tasks[idx];
        }
    }
    
    void execute() override {
        manager.removeTask(index);
    }
    
    void undo() override {
        auto tasks = manager.getTasks();
        // Re-insert at the same position
        if (index <= tasks.size()) {
            tasks.insert(tasks.begin() + index, removedTask);
            // Rebuild task list
            TaskManager temp;
            for (const auto& t : tasks) {
                temp.addTask(t);
            }
            manager = temp;
        }
    }
    
    std::string description() const override {
        return "Remove task at position " + std::to_string(index + 1);
    }
};

// Undo/Redo Manager
class UndoRedoManager {
private:
    std::stack<std::unique_ptr<Action>> undoStack;
    std::stack<std::unique_ptr<Action>> redoStack;
    
public:
    void executeAction(std::unique_ptr<Action> action) {
        action->execute();
        std::cout << "[EXECUTED] " << action->description() << "\n";
        
        undoStack.push(std::move(action));
        
        // Clear redo stack when new action is executed
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
    
    void undo() {
        if (undoStack.empty()) {
            std::cout << "Nothing to undo!\n";
            return;
        }
        
        auto action = std::move(undoStack.top());
        undoStack.pop();
        
        action->undo();
        std::cout << "[UNDO] " << action->description() << "\n";
        
        redoStack.push(std::move(action));
    }
    
    void redo() {
        if (redoStack.empty()) {
            std::cout << "Nothing to redo!\n";
            return;
        }
        
        auto action = std::move(redoStack.top());
        redoStack.pop();
        
        action->execute();
        std::cout << "[REDO] " << action->description() << "\n";
        
        undoStack.push(std::move(action));
    }
    
    void showHistory() const {
        std::cout << "\nHistory:\n";
        std::cout << "Undo stack size: " << undoStack.size() << "\n";
        std::cout << "Redo stack size: " << redoStack.size() << "\n";
    }
    
    bool canUndo() const { return !undoStack.empty(); }
    bool canRedo() const { return !redoStack.empty(); }
};

// Demo function
void runDemo() {
    std::cout << "=== UNDO/REDO SYSTEM DEMO ===\n\n";
    
    // Text Editor Demo
    std::cout << "--- TEXT EDITOR DEMO ---\n";
    TextEditor editor;
    UndoRedoManager textManager;
    
    // Perform actions
    textManager.executeAction(std::make_unique<InsertAction>(editor, "Hello", 0));
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    textManager.executeAction(std::make_unique<InsertAction>(editor, " World", 5));
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    textManager.executeAction(std::make_unique<InsertAction>(editor, "!", 11));
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    // Undo
    textManager.undo();
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    textManager.undo();
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    // Redo
    textManager.redo();
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    // New action (clears redo stack)
    textManager.executeAction(std::make_unique<DeleteAction>(editor, 5, 6));
    std::cout << "Content: \"" << editor.getContent() << "\"\n\n";
    
    textManager.redo(); // Should fail
    std::cout << "\n";
    
    // Task Manager Demo
    std::cout << "--- TASK MANAGER DEMO ---\n";
    TaskManager taskMgr;
    UndoRedoManager taskUndoMgr;
    
    taskUndoMgr.executeAction(std::make_unique<AddTaskAction>(taskMgr, "Buy groceries"));
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.executeAction(std::make_unique<AddTaskAction>(taskMgr, "Write code"));
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.executeAction(std::make_unique<AddTaskAction>(taskMgr, "Study C++"));
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.undo();
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.undo();
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.redo();
    taskMgr.displayTasks();
    std::cout << "\n";
    
    taskUndoMgr.showHistory();
}

int main() {
    runDemo();
    
    std::cout << "\n=== INTERACTIVE MODE ===\n";
    std::cout << "Commands: insert <pos> <text>, delete <pos> <len>, undo, redo, show, quit\n\n";
    
    TextEditor editor;
    UndoRedoManager manager;
    std::string command;
    
    while (true) {
        std::cout << "> ";
        std::cin >> command;
        
        if (command == "quit") break;
        
        if (command == "insert") {
            size_t pos;
            std::string text;
            std::cin >> pos;
            std::cin.ignore();
            std::getline(std::cin, text);
            manager.executeAction(std::make_unique<InsertAction>(editor, text, pos));
            std::cout << "Content: \"" << editor.getContent() << "\"\n";
        }
        else if (command == "delete") {
            size_t pos, len;
            std::cin >> pos >> len;
            manager.executeAction(std::make_unique<DeleteAction>(editor, pos, len));
            std::cout << "Content: \"" << editor.getContent() << "\"\n";
        }
        else if (command == "undo") {
            manager.undo();
            std::cout << "Content: \"" << editor.getContent() << "\"\n";
        }
        else if (command == "redo") {
            manager.redo();
            std::cout << "Content: \"" << editor.getContent() << "\"\n";
        }
        else if (command == "show") {
            std::cout << "Content: \"" << editor.getContent() << "\"\n";
            manager.showHistory();
        }
        else {
            std::cout << "Unknown command\n";
        }
    }
    
    return 0;
}