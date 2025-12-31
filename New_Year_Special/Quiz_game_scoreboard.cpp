#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <random>
using namespace std;

struct Question {
    string question;
    vector<string> options;
    int correctAnswer;
    string category;
    int difficulty; // 1=Easy, 2=Medium, 3=Hard

    Question(string q, vector<string> opts, int correct, string cat, int diff)
        : question(q), options(opts), correctAnswer(correct), category(cat), difficulty(diff) {}
};

struct Player {
    string name;
    int score;
    string date;

    Player(string n, int s, string d) : name(n), score(s), date(d) {}

    bool operator>(const Player &other) const {
        return score > other.score;
    }
};

class QuizGame {
private:
    vector<Question> questionBank;
    vector<Player> leaderboard;
    string playerName;
    int currentScore;
    int correctAnswers;
    int totalQuestions;
    const string leaderboardFile = "leaderboard.txt";

    void initializeQuestions() {
        // General Knowledge - Easy
        questionBank.push_back(Question(
            "What is the capital of France?",
            {"London", "Berlin", "Paris", "Madrid"},
            3, "General Knowledge", 1
        ));

        questionBank.push_back(Question(
            "How many continents are there?",
            {"5", "6", "7", "8"},
            3, "General Knowledge", 1
        ));

        // Science - Easy
        questionBank.push_back(Question(
            "What planet is known as the Red Planet?",
            {"Venus", "Mars", "Jupiter", "Saturn"},
            2, "Science", 1
        ));

        questionBank.push_back(Question(
            "What is the chemical symbol for water?",
            {"H2O", "CO2", "O2", "N2"},
            1, "Science", 1
        ));

        // Programming - Medium
        questionBank.push_back(Question(
            "Which data structure uses LIFO (Last In First Out)?",
            {"Queue", "Stack", "Array", "Tree"},
            2, "Programming", 2
        ));

        questionBank.push_back(Question(
            "What does 'OOP' stand for in programming?",
            {"Object Oriented Programming", "Open Office Protocol", "Online Operation Process", "Optimized Output Program"},
            1, "Programming", 2
        ));

        // Mathematics - Medium
        questionBank.push_back(Question(
            "What is 15% of 200?",
            {"25", "30", "35", "40"},
            2, "Mathematics", 2
        ));

        questionBank.push_back(Question(
            "What is the value of Pi to 2 decimal places?",
            {"3.12", "3.14", "3.16", "3.18"},
            2, "Mathematics", 2
        ));

        // History - Hard
        questionBank.push_back(Question(
            "In which year did World War II end?",
            {"1943", "1944", "1945", "1946"},
            3, "History", 3
        ));

        questionBank.push_back(Question(
            "Who was the first person to walk on the moon?",
            {"Buzz Aldrin", "Neil Armstrong", "Yuri Gagarin", "John Glenn"},
            2, "History", 3
        ));

        // Science - Hard
        questionBank.push_back(Question(
            "What is the speed of light in vacuum (approx)?",
            {"300,000 km/s", "150,000 km/s", "500,000 km/s", "200,000 km/s"},
            1, "Science", 3
        ));

        questionBank.push_back(Question(
            "What is the powerhouse of the cell?",
            {"Nucleus", "Ribosome", "Mitochondria", "Endoplasmic Reticulum"},
            3, "Science", 3
        ));
    }

    string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        sprintf(buffer, "%02d/%02d/%04d", 
                ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void displayQuestion(const Question &q, int qNum) {
        cout << "\n========================================================\n";
        cout << "  Question " << qNum << " of " << totalQuestions;
        cout << " | Category: " << q.category;
        cout << " | Difficulty: ";
        if (q.difficulty == 1) cout << "Easy";
        else if (q.difficulty == 2) cout << "Medium";
        else cout << "Hard";
        cout << "\n========================================================\n";
        
        cout << "\n" << q.question << "\n\n";
        
        for (size_t i = 0; i < q.options.size(); i++) {
            cout << i + 1 << ". " << q.options[i] << endl;
        }
        
        cout << "\nYour answer (1-" << q.options.size() << "): ";
    }

    int getPoints(int difficulty) {
        if (difficulty == 1) return 10;
        if (difficulty == 2) return 20;
        return 30;
    }

public:
    QuizGame() : currentScore(0), correctAnswers(0), totalQuestions(0) {
        initializeQuestions();
        loadLeaderboard();
    }

    void displayWelcome() {
        cout << "\n";
        cout << "===========================================================\n";
        cout << "                                                           \n";
        cout << "          ULTIMATE QUIZ GAME                               \n";
        cout << "                                                           \n";
        cout << "  Test Your Knowledge Across Multiple Categories!         \n";
        cout << "                                                           \n";
        cout << "===========================================================\n";
        cout << "\nEnter your name: ";
        clearInput();
        getline(cin, playerName);
        cout << "\nWelcome, " << playerName <<"! Let's begin!\n";
    }

    void playQuiz() {
        cout << "\n========================================================\n";
        cout << "      SELECT QUIZ MODE                                  \n";
        cout << "========================================================\n";
        cout << "1. Quick Quiz (5 questions)\n";
        cout << "2. Standard Quiz (10 questions)\n";
        cout << "3. Full Quiz (All questions)\n";
        cout << "\nEnter choice: ";
        
        int choice;
        cin >> choice;

        vector<Question> selectedQuestions;
        
        if (choice == 1) {
            totalQuestions = min(5, (int)questionBank.size());
        } else if (choice == 2) {
            totalQuestions = min(10, (int)questionBank.size());
        } else {
            totalQuestions = questionBank.size();
        }

        // Select random questions
        vector<int> indices;
        for (int i = 0; i < (int)questionBank.size(); i++) {
            indices.push_back(i);
        }
        
        // Use modern random shuffle
        random_device rd;
        mt19937 g(rd());
        shuffle(indices.begin(), indices.end(), g);

        for (int i = 0; i < totalQuestions; i++) {
            selectedQuestions.push_back(questionBank[indices[i]]);
        }

        // Play the quiz
        currentScore = 0;
        correctAnswers = 0;

        for (int i = 0; i < totalQuestions; i++) {
            displayQuestion(selectedQuestions[i], i + 1);
            
            int answer;
            cin >> answer;

            if (answer == selectedQuestions[i].correctAnswer) {
                int points = getPoints(selectedQuestions[i].difficulty);
                currentScore += points;
                correctAnswers++;
                cout << "\nCorrect! +" << points << " points\n";
            } else {
                cout << "\nWrong! The correct answer was: " 
                     << selectedQuestions[i].options[selectedQuestions[i].correctAnswer - 1] << endl;
            }

            cout << "Current Score: " << currentScore << " | Correct: " 
                 << correctAnswers << "/" << (i + 1) << endl;
            
            if (i < totalQuestions - 1) {
                cout << "\nPress Enter for next question...";
                cin.ignore();
                cin.get();
            }
        }

        displayResults();
        saveScore();
    }

    void displayResults() {
        cout << "\n\n";
        cout << "===========================================================\n";
        cout << "                 QUIZ COMPLETED!                           \n";
        cout << "===========================================================\n";
        cout << "\nPlayer: " << playerName << endl;
        cout << "Total Questions: " << totalQuestions << endl;
        cout << "Correct Answers: " << correctAnswers << endl;
        cout << "Wrong Answers: " << (totalQuestions - correctAnswers) << endl;
        cout << "Accuracy: " << fixed << setprecision(1) 
             << (correctAnswers * 100.0 / totalQuestions) << "%\n";
        cout << "\n===========================================================\n";
        cout << "  FINAL SCORE: " << setw(10) << currentScore << " points\n";
        cout << "===========================================================\n";

        // Performance message
        double percentage = (correctAnswers * 100.0 / totalQuestions);
        if (percentage >= 90) {
            cout << "\nOutstanding! You're a quiz master!\n";
        } else if (percentage >= 70) {
            cout << "\nGreat job! Very impressive!\n";
        } else if (percentage >= 50) {
            cout << "\nGood effort! Keep learning!\n";
        } else {
            cout << "\nKeep practicing! You'll improve!\n";
        }
    }

    void displayLeaderboard() {
        if (leaderboard.empty()) {
            cout << "\nNo scores yet. Be the first to play!\n";
            return;
        }

        sort(leaderboard.begin(), leaderboard.end(), greater<Player>());

        cout << "\n===========================================================\n";
        cout << "                  LEADERBOARD                              \n";
        cout << "===========================================================\n";
        cout << left << setw(6) << "Rank" 
             << setw(20) << "Player" 
             << setw(10) << "Score"
             << setw(15) << "Date" << endl;
        cout << string(51, '-') << endl;

        int displayCount = min(10, (int)leaderboard.size());
        for (int i = 0; i < displayCount; i++) {
            string medal = "";
            if (i == 0) medal = "#1 ";
            else if (i == 1) medal = "#2 ";
            else if (i == 2) medal = "#3 ";
            
            cout << medal << left << setw(4) << (i + 1)
                 << setw(20) << leaderboard[i].name
                 << setw(10) << leaderboard[i].score
                 << setw(15) << leaderboard[i].date << endl;
        }
        
        if (leaderboard.size() > 10) {
            cout << "\n... and " << (leaderboard.size() - 10) << " more players\n";
        }
    }

    void saveScore() {
        leaderboard.push_back(Player(playerName, currentScore, getCurrentDate()));
        
        ofstream out(leaderboardFile, ios::app);
        if (out) {
            out << playerName << endl
                << currentScore << endl
                << getCurrentDate() << endl;
            out.close();
        }
    }

    void loadLeaderboard() {
        ifstream in(leaderboardFile);
        if (!in) return;

        string name, date;
        int score;
        
        while (getline(in, name)) {
            in >> score;
            in.ignore();
            getline(in, date);
            leaderboard.push_back(Player(name, score, date));
        }
        in.close();
    }

    void displayRules() {
        cout << "\n===========================================================\n";
        cout << "                   GAME RULES                              \n";
        cout << "===========================================================\n";
        cout << "\nHow to Play:\n";
        cout << "   * Answer multiple-choice questions from various categories\n";
        cout << "   * Each question has 4 options, select the correct one\n";
        cout << "\nScoring System:\n";
        cout << "   * Easy Questions   : 10 points\n";
        cout << "   * Medium Questions : 20 points\n";
        cout << "   * Hard Questions   : 30 points\n";
        cout << "\nCategories:\n";
        cout << "   * General Knowledge\n";
        cout << "   * Science\n";
        cout << "   * Programming\n";
        cout << "   * Mathematics\n";
        cout << "   * History\n";
        cout << "\nChallenge yourself and climb the leaderboard!\n";
    }
};

void displayMainMenu() {
    cout << "\n========================================================\n";
    cout << "         MAIN MENU                                      \n";
    cout << "========================================================\n";
    cout << "1. Start New Quiz\n";
    cout << "2. View Leaderboard\n";
    cout << "3. Game Rules\n";
    cout << "4. Exit\n";
    cout << "\nEnter your choice: ";
}

int main() {
    QuizGame game;
    game.displayWelcome();

    int choice;
    bool firstTime = true;

    while (true) {
        if (firstTime) {
            game.displayRules();
            firstTime = false;
        }

        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                game.playQuiz();
                break;
            case 2:
                game.displayLeaderboard();
                break;
            case 3:
                game.displayRules();
                break;
            case 4:
                cout << "\n===========================================================\n";
                cout << "   Thanks for playing! Come back soon!                    \n";
                cout << "===========================================================\n";
                return 0;
            default:
                cout << "\n✗ Invalid choice. Please try again.\n";
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}