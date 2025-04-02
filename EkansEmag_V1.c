
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>   // Windows: For _kbhit() and _getch()
#include <windows.h> // Windows: For Sleep()
#else
#include <unistd.h>  // Linux: For usleep()
#include <termios.h> // Linux: For keyboard input
#endif

#define WIDTH 30
#define HEIGHT 20
#define MAX_LENGTH 100

int gameOver, score;
int x, y, foodX, foodY, direction;
int tailX[MAX_LENGTH], tailY[MAX_LENGTH]; // Snake body
int length; // Snake length

// Function to get non-blocking keyboard input (Linux version)
#ifndef _WIN32
int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Function to set up initial game state
void setup() {
    gameOver = 0;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
    score = 0;
    length = 0;
    direction = 0; // No movement initially
}

// Function to draw the game screen
void draw() {
    system("cls"); // Windows
    #ifndef _WIN32
    system("clear"); // Linux/macOS
    #endif

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#");

            if (i == y && j == x)
                printf("O"); // Snake head
            else if (i == foodY && j == foodX)
                printf("*"); // Food
            else {
                int printTail = 0;
                for (int k = 0; k < length; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printf("o");
                        printTail = 1;
                    }
                }
                if (!printTail) printf(" ");
            }

            if (j == WIDTH - 1) printf("#");
        }
        printf("\n");
    }

    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
    printf("Score: %d\n", score);
}

// Function to take user input for movement
void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if (direction != 2) direction = 1; break; // Left
            case 'd': if (direction != 1) direction = 2; break; // Right
            case 'w': if (direction != 4) direction = 3; break; // Up
            case 's': if (direction != 3) direction = 4; break; // Down
            case 'x': gameOver = 1; break; // Exit
        }
    }
}

// Function to update game logic
void logic() {
    int prevX = tailX[0], prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < length; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the snake
    if (direction == 1) x--; // Left
    if (direction == 2) x++; // Right
    if (direction == 3) y--; // Up
    if (direction == 4) y++; // Down

    // Check for wall collision
    if (x >= WIDTH || x < 0 || y >= HEIGHT || y < 0)
        gameOver = 1;

    // Check for self-collision
    for (int i = 0; i < length; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // Check if food is eaten
    if (x == foodX && y == foodY) {
        score += 10;
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
        length++;
    }
}

// Main function
int main() {
    srand(time(0));
    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        #ifdef _WIN32
        Sleep(100); // Windows: 100 ms delay
        #else
        usleep(100000); // Linux: 100,000 microseconds (100ms)
        #endif
    }

    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
