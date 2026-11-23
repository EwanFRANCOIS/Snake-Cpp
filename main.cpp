#include <iostream>
#include <vector>
#include <bits/this_thread_sleep.h>
#include <thread>
#include <chrono>
#include <random>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <sys/select.h>
#endif

// Looking if the key is pressed depending on the os of the user (windows or Linux)
#ifdef _WIN32
bool keyPressed(char& c) {
    if (_kbhit()) {
        c = _getch();
        return true;
    }
    return false;
}

struct RawMode{}; // Nothing to do since we are on windows

#else
// This struct remove the "line enter" to the terminal and restore it at its detruction
struct RawMode {
    termios old;
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
    timeval tv{0, 0};  // timeout nul = don't block
    if (select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv) > 0)
        return read(STDIN_FILENO, &c, 1) == 1;
    return false;
}
#endif

using namespace std;

// build grid game with 10x10 matrix
vector<vector<int>> buildGridGame() {
    vector<vector<int>> gridGame(10, vector<int>(10, 0));
    return gridGame;
}

void printMatrix(const vector<vector<int>>& matrix) {
    cout << "\033[H\033[J"; // Clear the screen
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// spawn the snake which is 1x1 at the starting position which is random (the random below is the new way for
// C++, i don't really know how it works but it kinda works the same as regular random)
void spawnSnake(vector<vector<int>>& grid, int& x, int& y) {
    static mt19937 gen{random_device{}()};
    uniform_int_distribution<int> distX(0, grid.size() - 1);
    uniform_int_distribution<int> distY(0, grid[0].size() - 1);
    x = distX(gen);
    y = distY(gen);
    grid[x][y] = 1;
}

// move the snake when the user presses [z, s, q, d] keys AND return false if the snake leave the grid
bool move(vector<vector<int>>& grid, char direction, int& x, int& y) {
    int nextX = x;   // We start for the current position of the snake
    int nextY = y;

    switch (direction) {
        case 'z': nextX--; break;
        case 's': nextX++; break;
        case 'q': nextY--; break;
        case 'd': nextY++; break;
    }

    if (nextX < 0 || nextX >= (int)grid.size() ||
        nextY < 0 || nextY >= (int)grid[0].size()) {
        return false;
        }
    grid[x][y] = 0;
    grid[nextX][nextY] = 1;
    x = nextX;
    y = nextY;
    return true;
}

int main() {
    RawMode raw;  // Do nothing in windows but set up the terminal in linux
    auto grid = buildGridGame();
    int x, y;
    spawnSnake(grid, x, y);

    char dir = 'd';
    bool running = true;

    while (running) {
        char c;
        while (keyPressed(c)) {
            if (c == 'z' || c == 'q' || c == 's' || c == 'd') dir = c;
            if (c == 'x') running = false;
        }

        if (!move(grid, dir, x, y)) {
            cout << "Perdu !" << endl;
            break;
        }

        printMatrix(grid);
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    return 0;
}

