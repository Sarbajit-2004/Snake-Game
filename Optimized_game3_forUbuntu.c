#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

// Define arrow key constants based on ncurses
#define UP      KEY_UP
#define DOWN    KEY_DOWN
#define LEFT    KEY_LEFT
#define RIGHT   KEY_RIGHT

int length;
int bend_no;
int len;
int life;
int key_input;

void record();
void load();
void Delay(long double);
void Move();
void Food();
int Score();
void Print();
void gotoxy(int x, int y);
void GotoXY(int x, int y);
void Bend();
void Boarder();
void Down();
void Left();
void Up();
void Right();
void ExitGame();
int Scoreonly();
int kbhit(); // a helper to mimic conio.h's kbhit

// Structure for coordinates and current direction.
struct coordinate {
    int x;
    int y;
    int direction;
};

typedef struct coordinate coordinate;

coordinate head, bend[500], food, body[30];

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    
    Print();
    clear();
    load();
    
    length = 5;
    head.x = 25;
    head.y = 20;
    head.direction = RIGHT;
    
    Boarder();
    Food(); // generate initial food coordinates
    life = 3;
    bend_no = 0;
    bend[0] = head;
    Move();   // start the game loop

    endwin();
    return 0;
}

// kbhit() for ncurses: returns nonzero if a key is waiting.
int kbhit() {
    int ch = getch();
    if(ch != ERR) {
        ungetch(ch);
        return 1;
    }
    return 0;
}

void Move() {
    int a, i;
    do {
        Food();
        len = 0;
        for(i = 0; i < 30; i++) {
            body[i].x = 0;
            body[i].y = 0;
            if(i == length)
                break;
        }
        Delay(length);
        Boarder();
        if(head.direction == RIGHT)
            Right();
        else if(head.direction == LEFT)
            Left();
        else if(head.direction == DOWN)
            Down();
        else if(head.direction == UP)
            Up();
        ExitGame();
    } while(!kbhit());
    
    a = getch();
    if(a == 27) { // ESC to exit
        clear();
        exit(0);
    }
    key_input = getch();
    if((key_input == RIGHT && head.direction != LEFT && head.direction != RIGHT) ||
       (key_input == LEFT && head.direction != RIGHT && head.direction != LEFT) ||
       (key_input == UP && head.direction != DOWN && head.direction != UP) ||
       (key_input == DOWN && head.direction != UP && head.direction != DOWN)) {
           bend_no++;
           bend[bend_no] = head;
           head.direction = key_input;
           if(key_input == UP)
               head.y--;
           if(key_input == DOWN)
               head.y++;
           if(key_input == RIGHT)
               head.x++;
           if(key_input == LEFT)
               head.x--;
           Move();
    } else if(key_input == 27) {
        clear();
        exit(0);
    } else {
        printw("\a"); // beep on invalid key
        Move();
    }
}

void gotoxy(int x, int y) {
    move(y, x);
    refresh();
}

void GotoXY(int x, int y) {
    move(y, x);
    refresh();
}

void load() {
    int r, q;
    gotoxy(36, 14);
    printw("loading...");
    gotoxy(30, 15);
    for(r = 1; r <= 20; r++) {
        for(q = 0; q <= 10000000; q++); // delay loop
        printw("%c", 177);
        refresh();
    }
    getch();
}

void Delay(long double k) {
    Score();
    napms(100); // fixed delay; adjust as needed
}

void ExitGame() {
    int i, check = 0;
    for(i = 4; i < length; i++) {  // check collision with self (skip head few segments)
        if(body[0].x == body[i].x && body[0].y == body[i].y) {
            check++;
        }
        if(i == length || check != 0)
            break;
    }
    if(head.x <= 10 || head.x >= 70 || head.y <= 10 || head.y >= 30 || check != 0) {
        life--;
        if(life >= 0) {
            head.x = 25;
            head.y = 20;
            bend_no = 0;
            head.direction = RIGHT;
            Move();
        } else {
            clear();
            printw("All lives completed\nBetter Luck Next Time!!!\nPress any key to quit the game\n");
            record();
            exit(0);
        }
    }
}

void Food() {
    if(head.x == food.x && head.y == food.y) {
        length++;
        time_t a = time(0);
        srand(a);
        food.x = rand() % 70;
        if(food.x <= 10)
            food.x += 11;
        food.y = rand() % 30;
        if(food.y <= 10)
            food.y += 11;
    } else if(food.x == 0) { // initialize food if not already set
        food.x = rand() % 70;
        if(food.x <= 10)
            food.x += 11;
        food.y = rand() % 30;
        if(food.y <= 10)
            food.y += 11;
    }
}

void Left() {
    int i;
    for(i = 0; i <= (bend[bend_no].x - head.x) && len < length; i++) {
        GotoXY(head.x + i, head.y);
        if(len == 0)
            printw("<");
        else
            printw("*");
        body[len].x = head.x + i;
        body[len].y = head.y;
        len++;
    }
    Bend();
    if(!kbhit())
        head.x--;
}

void Right() {
    int i;
    for(i = 0; i <= (head.x - bend[bend_no].x) && len < length; i++) {
        body[len].x = head.x - i;
        body[len].y = head.y;
        GotoXY(body[len].x, body[len].y);
        if(len == 0)
            printw(">");
        else
            printw("*");
        len++;
    }
    Bend();
    if(!kbhit())
        head.x++;
}

void Down() {
    int i;
    for(i = 0; i <= (head.y - bend[bend_no].y) && len < length; i++) {
        GotoXY(head.x, head.y - i);
        if(len == 0)
            printw("v");
        else
            printw("*");
        body[len].x = head.x;
        body[len].y = head.y - i;
        len++;
    }
    Bend();
    if(!kbhit())
        head.y++;
}

void Up() {
    int i;
    for(i = 0; i <= (bend[bend_no].y - head.y) && len < length; i++) {
        GotoXY(head.x, head.y + i);
        if(len == 0)
            printw("^");
        else
            printw("*");
        body[len].x = head.x;
        body[len].y = head.y + i;
        len++;
    }
    Bend();
    if(!kbhit())
        head.y--;
}

void Bend() {
    int i, j, diff;
    for(i = bend_no; i >= 0 && len < length; i--) {
        if(bend[i].x == bend[i-1].x) {
            diff = bend[i].y - bend[i-1].y;
            if(diff < 0)
                for(j = 1; j <= (-diff); j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y + j;
                    GotoXY(body[len].x, body[len].y);
                    printw("*");
                    len++;
                    if(len == length)
                        break;
                }
            else if(diff > 0)
                for(j = 1; j <= diff; j++) {
                    body[len].x = bend[i].x;
                    body[len].y = bend[i].y - j;
                    GotoXY(body[len].x, body[len].y);
                    printw("*");
                    len++;
                    if(len == length)
                        break;
                }
        } else if(bend[i].y == bend[i-1].y) {
            diff = bend[i].x - bend[i-1].x;
            if(diff < 0)
                for(j = 1; j <= (-diff) && len < length; j++) {
                    body[len].x = bend[i].x + j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printw("*");
                    len++;
                    if(len == length)
                        break;
                }
            else if(diff > 0)
                for(j = 1; j <= diff && len < length; j++) {
                    body[len].x = bend[i].x - j;
                    body[len].y = bend[i].y;
                    GotoXY(body[len].x, body[len].y);
                    printw("*");
                    len++;
                    if(len == length)
                        break;
                }
        }
    }
}

void Boarder() {
    clear();
    GotoXY(food.x, food.y);   // display food
    printw("F");
    int i;
    for(i = 10; i < 71; i++) {
        GotoXY(i, 10);
        printw("!");
        GotoXY(i, 30);
        printw("!");
    }
    for(i = 10; i < 31; i++) {
        GotoXY(10, i);
        printw("!");
        GotoXY(70, i);
        printw("!");
    }
}

void Print() {
    printw("\tWelcome to the mini Snake game. (Press any key to continue)\n");
    getch();
    clear();
    printw("\tGame instructions:\n");
    printw("\n-> Use arrow keys to move the snake.\n\n-> You will be provided with food at various coordinates on the screen which you must eat. Every time you eat food the snake grows and your score increases.\n\n-> You have three lives. You lose a life when you hit the wall or the snake's body.\n\n-> You can pause the game by pressing any key. To resume, press any key again.\n\n-> To exit, press ESC.\n");
    printw("\n\nPress any key to play game...");
    if(getch() == 27)
        exit(0);
}

void record() {
    char plname[20], nplname[20];
    int i, j, px;
    int cha;
    FILE *info;
    info = fopen("record.txt", "a+");
    getch();
    clear();
    printw("Enter your name\n");
    echo();
    getnstr(plname, 19);
    noecho();
    for(j = 0; plname[j] != '\0'; j++) {
        if(j == 0)
            nplname[j] = toupper(plname[j]);
        else if(plname[j-1] == ' ')
            nplname[j] = toupper(plname[j]);
        else
            nplname[j] = plname[j];
    }
    nplname[j] = '\0';
    fprintf(info, "Player Name :%s\n", nplname);
    time_t mytime = time(NULL);
    fprintf(info, "Played Date:%s", ctime(&mytime));
    fprintf(info, "Score:%d\n", px = Scoreonly());
    for(i = 0; i <= 50; i++)
        fprintf(info, "%c", '_');
    fprintf(info, "\n");
    fclose(info);
    printw("Wanna see past records? Press 'y'\n");
    cha = getch();
    clear();
    if(cha == 'y') {
        info = fopen("record.txt", "r");
        int c;
        while((c = fgetc(info)) != EOF) {
            printw("%c", c);
        }
        fclose(info);
    }
}

int Score() {
    int score;
    GotoXY(20, 8);
    score = length - 5;
    printw("SCORE : %d", (length - 5));
    GotoXY(50, 8);
    printw("Life : %d", life);
    return score;
}

int Scoreonly() {
    int score = Score();
    clear();
    return score;
}
