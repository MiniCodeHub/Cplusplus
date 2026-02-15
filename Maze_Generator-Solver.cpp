#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

// Cell structure for the maze
struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // top, right, bottom, left
};

// Direction enum
enum Direction { TOP = 0, RIGHT = 1, BOTTOM = 2, LEFT = 3 };

// Point structure for pathfinding
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Maze {
private:
    int width, height;
    vector<vector<Cell>> grid;
    random_device rd;
    mt19937 gen;
    Point start, end;
    vector<Point> solution;

    // Direction vectors
    const int dx[4] = {0, 1, 0, -1};  // right, down, left, up
    const int dy[4] = {-1, 0, 1, 0};

public:
    Maze(int w, int h) : width(w), height(h), gen(rd()) {
        grid.resize(height, vector<Cell>(width));
        start = Point(0, 0);
        end = Point(width - 1, height - 1);
    }

    // Generate maze using recursive backtracking
    void generateRecursiveBacktracking() {
        stack<Point> stack;
        Point current(0, 0);
        grid[0][0].visited = true;
        stack.push(current);

        while (!stack.empty()) {
            current = stack.top();
            vector<Direction> neighbors = getUnvisitedNeighbors(current);

            if (!neighbors.empty()) {
                // Choose random unvisited neighbor
                uniform_int_distribution<> dis(0, neighbors.size() - 1);
                Direction dir = neighbors[dis(gen)];
                
                Point next = getNeighbor(current, dir);
                
                // Remove walls between current and next
                removeWall(current, next, dir);
                
                grid[next.y][next.x].visited = true;
                stack.push(next);
            } else {
                stack.pop();
            }
        }
    }

    // Get unvisited neighbors
    vector<Direction> getUnvisitedNeighbors(Point p) {
        vector<Direction> neighbors;
        
        if (p.y > 0 && !grid[p.y - 1][p.x].visited) neighbors.push_back(TOP);
        if (p.x < width - 1 && !grid[p.y][p.x + 1].visited) neighbors.push_back(RIGHT);
        if (p.y < height - 1 && !grid[p.y + 1][p.x].visited) neighbors.push_back(BOTTOM);
        if (p.x > 0 && !grid[p.y][p.x - 1].visited) neighbors.push_back(LEFT);
        
        return neighbors;
    }

    // Get neighbor in direction
    Point getNeighbor(Point p, Direction dir) {
        switch(dir) {
            case TOP: return Point(p.x, p.y - 1);
            case RIGHT: return Point(p.x + 1, p.y);
            case BOTTOM: return Point(p.x, p.y + 1);
            case LEFT: return Point(p.x - 1, p.y);
        }
        return p;
    }

    // Remove wall between two cells
    void removeWall(Point current, Point next, Direction dir) {
        grid[current.y][current.x].walls[dir] = false;
        
        // Remove opposite wall
        Direction opposite = (Direction)((dir + 2) % 4);
        grid[next.y][next.x].walls[opposite] = false;
    }

    // Solve maze using BFS
    bool solveBFS() {
        queue<Point> q;
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        vector<vector<Point>> parent(height, vector<Point>(width, Point(-1, -1)));
        
        q.push(start);
        visited[start.y][start.x] = true;
        
        while (!q.empty()) {
            Point current = q.front();
            q.pop();
            
            if (current == end) {
                // Reconstruct path
                reconstructPath(parent);
                return true;
            }
            
            // Check all 4 directions
            for (int i = 0; i < 4; i++) {
                if (!grid[current.y][current.x].walls[i]) {
                    Point next = getNeighbor(current, (Direction)i);
                    
                    if (next.x >= 0 && next.x < width && next.y >= 0 && next.y < height &&
                        !visited[next.y][next.x]) {
                        visited[next.y][next.x] = true;
                        parent[next.y][next.x] = current;
                        q.push(next);
                    }
                }
            }
        }
        
        return false;
    }

    // Solve maze using DFS
    bool solveDFS() {
        vector<vector<bool>> visited(height, vector<bool>(width, false));
        vector<Point> path;
        
        if (dfsHelper(start, visited, path)) {
            solution = path;
            return true;
        }
        
        return false;
    }

    // DFS helper
    bool dfsHelper(Point current, vector<vector<bool>>& visited, vector<Point>& path) {
        visited[current.y][current.x] = true;
        path.push_back(current);
        
        if (current == end) {
            return true;
        }
        
        // Try all 4 directions
        for (int i = 0; i < 4; i++) {
            if (!grid[current.y][current.x].walls[i]) {
                Point next = getNeighbor(current, (Direction)i);
                
                if (next.x >= 0 && next.x < width && next.y >= 0 && next.y < height &&
                    !visited[next.y][next.x]) {
                    if (dfsHelper(next, visited, path)) {
                        return true;
                    }
                }
            }
        }
        
        path.pop_back();
        return false;
    }

    // Reconstruct path from parent array
    void reconstructPath(vector<vector<Point>>& parent) {
        solution.clear();
        Point current = end;
        
        while (!(current.x == -1 && current.y == -1)) {
            solution.push_back(current);
            current = parent[current.y][current.x];
        }
        
        reverse(solution.begin(), solution.end());
    }

    // Display maze in ASCII
    void display(bool showSolution = false) {
        // Create display grid
        int displayHeight = height * 2 + 1;
        int displayWidth = width * 2 + 1;
        vector<vector<char>> display(displayHeight, vector<char>(displayWidth, ' '));
        
        // Draw walls
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int dy = y * 2 + 1;
                int dx = x * 2 + 1;
                
                // Draw cell
                display[dy][dx] = ' ';
                
                // Draw walls using ASCII characters
                if (grid[y][x].walls[TOP]) {
                    display[dy - 1][dx] = '-';
                }
                if (grid[y][x].walls[RIGHT]) {
                    display[dy][dx + 1] = '|';
                }
                if (grid[y][x].walls[BOTTOM]) {
                    display[dy + 1][dx] = '-';
                }
                if (grid[y][x].walls[LEFT]) {
                    display[dy][dx - 1] = '|';
                }
                
                // Draw corners
                display[dy - 1][dx - 1] = '+';
                display[dy - 1][dx + 1] = '+';
                display[dy + 1][dx - 1] = '+';
                display[dy + 1][dx + 1] = '+';
            }
        }
        
        // Mark start and end
        display[start.y * 2 + 1][start.x * 2 + 1] = 'S';
        display[end.y * 2 + 1][end.x * 2 + 1] = 'E';
        
        // Show solution if requested
        if (showSolution && !solution.empty()) {
            for (const Point& p : solution) {
                if (!(p == start) && !(p == end)) {
                    display[p.y * 2 + 1][p.x * 2 + 1] = '*';
                }
            }
        }
        
        // Print display
        clearScreen();
        for (const auto& row : display) {
            for (char c : row) {
                cout << c;
            }
            cout << endl;
        }
    }

    // Animate solution step by step
    void animateSolution(int delayMs = 200) {
        for (size_t i = 0; i <= solution.size(); i++) {
            vector<Point> partialSolution(solution.begin(), solution.begin() + i);
            vector<Point> temp = solution;
            solution = partialSolution;
            display(true);
            solution = temp;
            this_thread::sleep_for(chrono::milliseconds(delayMs));
        }
    }

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};

int main() {
    cout << "========================================\n";
    cout << "   MAZE GENERATOR & SOLVER\n";
    cout << "========================================\n\n";

    int width, height;
    cout << "Enter maze width (5-30): ";
    cin >> width;
    cout << "Enter maze height (5-30): ";
    cin >> height;

    width = max(5, min(30, width));
    height = max(5, min(30, height));

    Maze maze(width, height);

    cout << "\nGenerating maze using Recursive Backtracking...\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    maze.generateRecursiveBacktracking();

    cout << "\nGenerated Maze:\n\n";
    maze.display(false);

    cout << "\n\nPress Enter to solve with BFS...";
    cin.ignore();
    cin.get();

    cout << "\nSolving maze using BFS (Breadth-First Search)...\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    
    if (maze.solveBFS()) {
        cout << "Solution found!\n\n";
        cout << "Press Enter to see animated solution...";
        cin.get();
        maze.animateSolution(150);
    } else {
        cout << "No solution found!\n";
    }

    cout << "\n\nPress Enter to try DFS...";
    cin.get();

    cout << "\nSolving maze using DFS (Depth-First Search)...\n";
    this_thread::sleep_for(chrono::milliseconds(500));
    
    if (maze.solveDFS()) {
        cout << "Solution found!\n\n";
        maze.display(true);
    } else {
        cout << "No solution found!\n";
    }

    cout << "\n\nThanks for using Maze Generator & Solver!\n";
    cout << "\nLegend:\n";
    cout << "  S = Start\n";
    cout << "  E = End\n";
    cout << "  * = Solution Path\n";

    return 0;
}