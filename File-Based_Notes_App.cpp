#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <ctime>
#include <algorithm>

namespace fs = std::filesystem;

class NotesApp {
private:
    std::string notesDirectory;

    // Create notes directory if it doesn't exist
    void ensureDirectoryExists() {
        if (!fs::exists(notesDirectory)) {
            fs::create_directories(notesDirectory);
        }
    }

    // Get current timestamp for filename
    std::string getCurrentTimestamp() {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        return oss.str();
    }

    // Format timestamp for display
    std::string formatTimestamp(const std::string& filename) {
        if (filename.length() < 15) return filename;
        
        std::string timestamp = filename.substr(0, 15);
        std::string year = timestamp.substr(0, 4);
        std::string month = timestamp.substr(4, 2);
        std::string day = timestamp.substr(6, 2);
        std::string hour = timestamp.substr(9, 2);
        std::string minute = timestamp.substr(11, 2);
        std::string second = timestamp.substr(13, 2);
        
        return year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
    }

public:
    NotesApp(const std::string& directory = "notes") : notesDirectory(directory) {
        ensureDirectoryExists();
    }

    // Save a new note
    void saveNote() {
        std::cout << "\n=== Create New Note ===\n";
        std::cout << "Enter note title: ";
        std::string title;
        std::cin.ignore();
        std::getline(std::cin, title);

        std::cout << "Enter note content (type 'END' on a new line to finish):\n";
        std::string content, line;
        
        while (std::getline(std::cin, line)) {
            if (line == "END") break;
            content += line + "\n";
        }

        // Create filename with timestamp
        std::string filename = getCurrentTimestamp() + "_" + title + ".txt";
        // Remove invalid characters from filename
        for (char& c : filename) {
            if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || 
                c == '"' || c == '<' || c == '>' || c == '|') {
                c = '_';
            }
        }

        std::string filepath = notesDirectory + "/" + filename;
        std::ofstream file(filepath);
        
        if (file.is_open()) {
            file << "Title: " << title << "\n";
            file << "Date: " << getCurrentTimestamp() << "\n";
            file << "---\n";
            file << content;
            file.close();
            std::cout << "\nNote saved successfully as: " << filename << "\n";
        } else {
            std::cerr << "\nError: Could not save note!\n";
        }
    }

    // List all notes
    void listNotes() {
        std::cout << "\n=== All Notes ===\n";
        std::vector<fs::directory_entry> notes;
        
        for (const auto& entry : fs::directory_iterator(notesDirectory)) {
            if (entry.path().extension() == ".txt") {
                notes.push_back(entry);
            }
        }

        if (notes.empty()) {
            std::cout << "No notes found.\n";
            return;
        }

        // Sort by filename (which includes timestamp)
        std::sort(notes.begin(), notes.end(), 
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                return a.path().filename() > b.path().filename(); // Newest first
            });

        int index = 1;
        for (const auto& entry : notes) {
            std::string filename = entry.path().filename().string();
            std::string timestamp = formatTimestamp(filename);
            
            // Extract title (remove timestamp and extension)
            std::string displayName = filename;
            if (filename.length() > 16) {
                displayName = filename.substr(16); // Skip timestamp and underscore
            }
            if (displayName.length() > 4 && displayName.substr(displayName.length() - 4) == ".txt") {
                displayName = displayName.substr(0, displayName.length() - 4);
            }
            
            std::cout << index++ << ". [" << timestamp << "] " << displayName << "\n";
        }
    }

    // View a specific note
    void viewNote() {
        std::vector<fs::directory_entry> notes;
        
        for (const auto& entry : fs::directory_iterator(notesDirectory)) {
            if (entry.path().extension() == ".txt") {
                notes.push_back(entry);
            }
        }

        if (notes.empty()) {
            std::cout << "\nNo notes available to view.\n";
            return;
        }

        // Sort by filename
        std::sort(notes.begin(), notes.end(), 
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                return a.path().filename() > b.path().filename();
            });

        listNotes();
        
        std::cout << "\nEnter note number to view: ";
        int choice;
        std::cin >> choice;

        if (choice < 1 || choice > static_cast<int>(notes.size())) {
            std::cout << "Invalid selection!\n";
            return;
        }

        std::string filepath = notes[choice - 1].path().string();
        std::ifstream file(filepath);
        
        if (file.is_open()) {
            std::cout << "\n=== Note Content ===\n";
            std::string line;
            while (std::getline(file, line)) {
                std::cout << line << "\n";
            }
            file.close();
            std::cout << "===================\n";
        } else {
            std::cerr << "Error reading note!\n";
        }
    }

    // Delete a note
    void deleteNote() {
        std::vector<fs::directory_entry> notes;
        
        for (const auto& entry : fs::directory_iterator(notesDirectory)) {
            if (entry.path().extension() == ".txt") {
                notes.push_back(entry);
            }
        }

        if (notes.empty()) {
            std::cout << "\nNo notes available to delete.\n";
            return;
        }

        std::sort(notes.begin(), notes.end(), 
            [](const fs::directory_entry& a, const fs::directory_entry& b) {
                return a.path().filename() > b.path().filename();
            });

        listNotes();
        
        std::cout << "\nEnter note number to delete (0 to cancel): ";
        int choice;
        std::cin >> choice;

        if (choice == 0) return;

        if (choice < 1 || choice > static_cast<int>(notes.size())) {
            std::cout << "Invalid selection!\n";
            return;
        }

        std::string filepath = notes[choice - 1].path().string();
        
        std::cout << "Are you sure you want to delete this note? (y/n): ";
        char confirm;
        std::cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            if (fs::remove(filepath)) {
                std::cout << "\nNote deleted successfully!\n";
            } else {
                std::cerr << "\nError deleting note!\n";
            }
        } else {
            std::cout << "\nDeletion cancelled.\n";
        }
    }

    // Display menu and run the app
    void run() {
        int choice;
        
        do {
            std::cout << "\n=============================\n";
            std::cout << "   FILE-BASED NOTES APP\n";
            std::cout << "=============================\n";
            std::cout << "1. Create New Note\n";
            std::cout << "2. View Note\n";
            std::cout << "3. List All Notes\n";
            std::cout << "4. Delete Note\n";
            std::cout << "5. Exit\n";
            std::cout << "-----------------------------\n";
            std::cout << "Enter choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    saveNote();
                    break;
                case 2:
                    viewNote();
                    break;
                case 3:
                    listNotes();
                    break;
                case 4:
                    deleteNote();
                    break;
                case 5:
                    std::cout << "\nGoodbye! Your notes are saved in the '" 
                              << notesDirectory << "' directory.\n";
                    break;
                default:
                    std::cout << "\nInvalid choice! Please try again.\n";
            }
        } while (choice != 5);
    }
};

int main() {
    NotesApp app;
    app.run();
    return 0;
}