#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <deque>
#include <utility>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
#endif

using namespace std;

#ifdef _WIN32
bool keyPressed(char& c) {
    if (_kbhit()) {
        c = static_cast<char>(_getch());
        return true;
    }
    return false;
}

struct RawMode {};
#else
struct RawMode {
    termios old{};
    RawMode() {
        tcgetattr(STDIN_FILENO, &old);
        termios raw = old;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }
    ~RawMode() { tcsetattr(STDIN_FILENO, TCSANOW, &old); }
};

bool keyPressed(char& c) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    timeval tv{0, 0};
    if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0)
        return read(STDIN_FILENO, &c, 1) == 1;
    return false;
}
#endif

vector<vector<int>> buildGridGame() {
    return vector<vector<int>>(10, vector<int>(10, 0));
}

void printMatrix(const vector<vector<int>>& matrix, int score) {
    cout << "\033[H\033[J";
    cout << "Score : " << score << " | Z/Q/S/D : bouger | X : quitter\n\n";

    for (const auto& row : matrix) {
        for (int cell : row) {
            if (cell == 1) {
                cout << "1 ";
            } else if (cell == 8) {
                cout << "8 ";
            } else {
                cout << ". ";
            }
        }
        cout << '\n';
    }
    cout << flush;
}

void spawnSnake(vector<vector<int>>& grid, deque<pair<int, int>>& snake) {
    static mt19937 gen{random_device{}()};
    uniform_int_distribution<int> distX(0, static_cast<int>(grid.size()) - 1);
    uniform_int_distribution<int> distY(0, static_cast<int>(grid[0].size()) - 1);

    int x = distX(gen);
    int y = distY(gen);
    snake.clear();
    snake.emplace_front(x, y);
    grid[x][y] = 1;
}

void spawnFood(vector<vector<int>>& grid, int& x, int& y) {
    static mt19937 gen{random_device{}()};
    uniform_int_distribution<int> distX(0, static_cast<int>(grid.size()) - 1);
    uniform_int_distribution<int> distY(0, static_cast<int>(grid[0].size()) - 1);

    do {
        x = distX(gen);
        y = distY(gen);
    } while (grid[x][y] != 0);

    grid[x][y] = 8;
}

void eat(vector<vector<int>>& grid, deque<pair<int, int>>& snake, int nextX, int nextY, int& foodX, int& foodY, int& score) {
    snake.emplace_front(nextX, nextY);
    grid[nextX][nextY] = 1;
    score++;
    spawnFood(grid, foodX, foodY);
}

bool isOpposite(char currentDir, char nextDir) {
    return (currentDir == 'z' && nextDir == 's') ||
           (currentDir == 's' && nextDir == 'z') ||
           (currentDir == 'q' && nextDir == 'd') ||
           (currentDir == 'd' && nextDir == 'q');
}

bool move(vector<vector<int>>& grid, deque<pair<int, int>>& snake, char direction, int& foodX, int& foodY, int& score) {
    int nextX = snake.front().first;
    int nextY = snake.front().second;

    switch (direction) {
        case 'z': nextX--; break;
        case 's': nextX++; break;
        case 'q': nextY--; break;
        case 'd': nextY++; break;
        default: break;
    }

    if (nextX < 0 || nextX >= static_cast<int>(grid.size()) ||
        nextY < 0 || nextY >= static_cast<int>(grid[0].size())) {
        return false;
    }

    if (grid[nextX][nextY] == 1) {
        return false;
    }

    if (grid[nextX][nextY] == 8) {
        eat(grid, snake, nextX, nextY, foodX, foodY, score);
        return true;
    }

    snake.emplace_front(nextX, nextY);
    grid[nextX][nextY] = 1;

    pair<int, int> tail = snake.back();
    snake.pop_back();
    grid[tail.first][tail.second] = 0;

    return true;
}

int main() {
    RawMode raw;
    auto grid = buildGridGame();
    deque<pair<int, int>> snake;
    int foodX = 0, foodY = 0;
    int score = 0;

    spawnSnake(grid, snake);
    spawnFood(grid, foodX, foodY);

    char dir = 'd';
    bool running = true;

    printMatrix(grid, score);

    while (running) {
        char c = '\0';
        while (keyPressed(c)) {
            if (c == 'z' || c == 'q' || c == 's' || c == 'd') {
                if (!isOpposite(dir, c)) {
                    dir = c;
                }
            }
            if (c == 'x') {
                running = false;
            }
        }

        if (!running) break;

        if (!move(grid, snake, dir, foodX, foodY, score)) {
            cout << "\nPerdu ! Score final : " << score << endl;
            break;
        }

        printMatrix(grid, score);
        this_thread::sleep_for(chrono::milliseconds(250));
    }

    return 0;
}