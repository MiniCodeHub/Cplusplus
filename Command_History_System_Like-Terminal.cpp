#include <iostream>
#include <vector>
#include <string>
#include <limits>

class CommandHistory {
private:
    std::vector<std::string> history;
    int currentIndex;
    std::string tempCommand;
    static const size_t MAX_HISTORY = 100;

public:
    CommandHistory() : currentIndex(-1) {}

    // Add a command to history
    void addCommand(const std::string& cmd) {
        if (cmd.empty()) return;
        
        // Avoid duplicate consecutive commands
        if (!history.empty() && history.back() == cmd) {
            resetIndex();
            return;
        }
        
        history.push_back(cmd);
        
        // Maintain maximum history size
        if (history.size() > MAX_HISTORY) {
            history.erase(history.begin());
        }
        
        resetIndex();
    }

    // Navigate to previous command (up arrow)
    std::string getPrevious(const std::string& current) {
        if (history.empty()) return current;
        
        // First time pressing up - save current input
        if (currentIndex == -1) {
            tempCommand = current;
            currentIndex = history.size() - 1;
        } else if (currentIndex > 0) {
            currentIndex--;
        }
        
        return history[currentIndex];
    }

    // Navigate to next command (down arrow)
    std::string getNext() {
        if (currentIndex == -1) return tempCommand;
        
        currentIndex++;
        
        // Past the end - return to current input
        if (currentIndex >= static_cast<int>(history.size())) {
            resetIndex();
            return tempCommand;
        }
        
        return history[currentIndex];
    }

    // Reset navigation index
    void resetIndex() {
        currentIndex = -1;
        tempCommand.clear();
    }

    // Display all history
    void displayHistory() const {
        if (history.empty()) {
            std::cout << "No command history.\n";
            return;
        }
        
        std::cout << "\n=== Command History ===\n";
        for (size_t i = 0; i < history.size(); ++i) {
            std::cout << "  " << (i + 1) << ": " << history[i] << "\n";
        }
        std::cout << "=======================\n\n";
    }

    // Search history for commands containing a substring
    void searchHistory(const std::string& query) const {
        if (history.empty()) {
            std::cout << "No command history.\n";
            return;
        }
        
        std::cout << "\n=== Search Results for '" << query << "' ===\n";
        bool found = false;
        
        for (size_t i = 0; i < history.size(); ++i) {
            if (history[i].find(query) != std::string::npos) {
                std::cout << "  " << (i + 1) << ": " << history[i] << "\n";
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "  No matches found.\n";
        }
        std::cout << "=================================\n\n";
    }

    // Clear all history
    void clearHistory() {
        history.clear();
        resetIndex();
        std::cout << "History cleared.\n";
    }

    // Get history size
    size_t size() const {
        return history.size();
    }

    // Execute command from history by number
    std::string getByNumber(int num) const {
        if (num < 1 || num > static_cast<int>(history.size())) {
            return "";
        }
        return history[num - 1];
    }
};

class Terminal {
private:
    CommandHistory cmdHistory;
    bool running;

    void executeCommand(const std::string& cmd) {
        if (cmd.empty()) return;

        // Handle special commands
        if (cmd == "exit" || cmd == "quit") {
            running = false;
            std::cout << "Goodbye!\n";
            return;
        }
        
        if (cmd == "history") {
            cmdHistory.displayHistory();
            return;
        }
        
        if (cmd == "clear") {
            cmdHistory.clearHistory();
            return;
        }
        
        if (cmd.substr(0, 7) == "search ") {
            std::string query = cmd.substr(7);
            cmdHistory.searchHistory(query);
            return;
        }
        
        if (cmd[0] == '!' && cmd.length() > 1) {
            try {
                int num = std::stoi(cmd.substr(1));
                std::string histCmd = cmdHistory.getByNumber(num);
                if (!histCmd.empty()) {
                    std::cout << "Executing: " << histCmd << "\n";
                    executeCommand(histCmd);
                    cmdHistory.addCommand(histCmd);
                } else {
                    std::cout << "Invalid history number.\n";
                }
            } catch (...) {
                std::cout << "Invalid history command.\n";
            }
            return;
        }
        
        // Simulate command execution
        std::cout << "Executing: " << cmd << "\n";
        std::cout << "Command completed successfully.\n";
        
        cmdHistory.addCommand(cmd);
    }

    void showHelp() {
        std::cout << "\n=== Terminal Commands ===\n";
        std::cout << "  history          - Show all commands\n";
        std::cout << "  search <term>    - Search command history\n";
        std::cout << "  !<num>           - Execute command number from history\n";
        std::cout << "  clear            - Clear history\n";
        std::cout << "  help             - Show this help\n";
        std::cout << "  exit/quit        - Exit terminal\n";
        std::cout << "\nSimulated Navigation:\n";
        std::cout << "  up               - Previous command\n";
        std::cout << "  down             - Next command\n";
        std::cout << "=========================\n\n";
    }

public:
    Terminal() : running(true) {}

    void run() {
        std::cout << "=== Command History Terminal ===\n";
        std::cout << "Type 'help' for available commands\n\n";

        std::string currentInput;

        while (running) {
            std::cout << "$ ";
            std::string input;
            std::getline(std::cin, input);

            if (input == "up") {
                currentInput = cmdHistory.getPrevious(currentInput);
                std::cout << "[UP] " << currentInput << "\n";
                continue;
            }
            
            if (input == "down") {
                currentInput = cmdHistory.getNext();
                std::cout << "[DOWN] " << currentInput << "\n";
                continue;
            }

            if (input == "help") {
                showHelp();
                continue;
            }

            currentInput = input;
            cmdHistory.resetIndex();
            executeCommand(input);
        }
    }
};

int main() {
    Terminal terminal;
    terminal.run();
    return 0;
}