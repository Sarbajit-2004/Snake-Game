#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

int height = 15, width = 15, x, y, fruitX, fruitY, flag, gameEnd, score;
int tailX[100], tailY[100];
int body = 0;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = y;
    coord.Y = x;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void makeLogic() {
    int prevX, prevY, prev2X, prev2Y;
    prevX = tailX[0];
    prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;
    
    for (int i = 1; i <= body; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    switch (flag) {
        case 1: x--; break; // Up
        case 2: x++; break; // Down
        case 3: y--; break; // Left
        case 4: y++; break; // Right
    }
    
    // Wrap around the screen instead of dying at the boundary
    if (x == 0) x = height - 1;
    else if (x == height) x = 1;
    if (y == 0) y = width - 1;
    else if (y == width) y = 1;
    
    // Check if the snake eats itself
    for (int i = 0; i < body; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameEnd = 1;
        }
    }
    
    if (x == fruitX && y == fruitY) {
        do {
            fruitX = rand() % (height - 1) + 1;
        } while (fruitX == 0);
        
        do {
            fruitY = rand() % (width - 1) + 1;
        } while (fruitY == 0);
        
        score += 10;
        body++;
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'w': flag = 1; break;
            case 's': flag = 2; break;
            case 'a': flag = 3; break;
            case 'd': flag = 4; break;
        }
    }
}

void setup() {
    x = height / 2;
    y = width / 2;
    
    do {
        fruitX = rand() % (height - 1) + 1;
    } while (fruitX == 0);
    
    do {
        fruitY = rand() % (width - 1) + 1;
    } while (fruitY == 0);
    
    gameEnd = 0;
    score = 0;
    body = 0;
    hideCursor();
}

void draw() {
    gotoxy(0, 0);
    
    for (int i = 0; i <= height; i++) {
        for (int j = 0; j <= width; j++) {
            if (i == 0 || i == height || j == 0 || j == width) {
                setColor(7); // White color for the border
                printf("*");
            } else if (i == x && j == y) {
                setColor(10); // Green for the snake head
                printf("0");
            } else if (i == fruitX && j == fruitY) {
                setColor(12); // Red for the fruit
                printf("F");
            } else {
                int ch = 0;
                for (int k = 0; k < body; k++) {
                    if (i == tailX[k] && j == tailY[k]) {
                        setColor(14); // Yellow for the snake body
                        printf("o");
                        ch = 1;
                        break;
                    }
                }
                if (!ch) {
                    setColor(0);
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    setColor(7); // Reset to default color
    printf("Your score: %d\n", score);
}

int main() {
    setup();
    while (!gameEnd) {
        input();
        draw();
        makeLogic();
        Sleep(100);
    }
    setColor(12);
    printf("Game Over! Your final score is %d\n", score);
    setColor(7);
    return 0;
}
