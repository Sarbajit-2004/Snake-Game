
Overview

The Snake game is implemented in C and follows the classic gameplay:

Gameplay: The snake moves continuously on the screen, growing longer each time it eats food. The player uses arrow keys to change its direction. The game ends if the snake collides with its own body or the game borders.

Score & Lives: The score increases as the snake eats food, and the player starts with a fixed number of lives. Losing a life occurs when the snake hits a boundary or itself, and a record is kept for high scores.

Common Game Logic
Both versions (Windows and Ubuntu) share core gameplay mechanics:

Data Structures:

struct coordinate:
The game uses a structure to store the coordinates and current movement direction of each part of the snake.


![image](https://github.com/user-attachments/assets/c31fb3de-0ea5-408a-8f98-dd0bec94ef40)

typedef struct coordinate coordinate;
Global Variables:
Variables like head, bend[], body[], length, and life manage the snake’s current state, its turning points, and overall game parameters (score, lives, etc.).

Movement & Bends:

Movement Logic:
The snake is moved by updating its head position based on the current direction. The rest of the body is shifted accordingly.

Bend Points:
The code keeps track of turning points (bends) in the snake’s path. Each time the snake changes direction, a new bend is recorded, and the body segments follow these recorded bends to simulate realistic movement.

Input Handling:

The game responds to arrow key inputs to change the snake’s direction.

A check prevents the snake from reversing instantly (e.g., moving right then immediately left).

Food & Growth:

When the snake’s head reaches the food’s coordinates, the snake grows in length, and new food is generated randomly within defined screen boundaries.

The food is represented by a character (like “F” or “*”) drawn on the screen.

Collision & Lives:

Collision detection is implemented to check if the snake has hit the game boundaries or its own body.

When a collision occurs, the player loses a life. If all lives are lost, the game terminates, and a record is saved.

Display and Delay:

The game refreshes the display continuously, redrawing the snake, food, and borders.

A delay function controls the game speed.

Record Keeping:

The game saves the player’s name, score, and the date/time of play into a file (e.g., record.txt) for later viewing.

Windows Version
Key Features and Libraries
Headers:
The Windows version uses headers such as <conio.h>, <windows.h>, and <process.h> for console manipulation, non-blocking input, and cursor positioning.

Console Control:

gotoxy() and GotoXY():
These functions use Windows API calls (specifically SetConsoleCursorPosition()) to move the cursor to specific coordinates on the screen.

Clearing the Screen:
system("cls") is used to clear the console.

Input and Delay:

Non-Blocking Input:
Functions like kbhit() from <conio.h> allow the game to check if a key has been pressed without waiting.

Delay:
A busy-wait loop (or sometimes sleep functions) is used to control the game speed.

Game Logic Specifics:
The Windows version includes functions such as Move(), Food(), Bend(), Boarder(), Up(), Down(), Left(), and Right() that manage the snake’s movement and interaction with the game environment.

Bend Logic:
The game tracks bends in an array (bend[]) so that when the snake turns, the body segments follow the path of these bends.

How It Runs on Windows
Initialization:

The game starts with a welcome screen and instructions.

The console is prepared using Windows-specific calls.

Game Loop:

The snake continuously moves on the screen.

User inputs (arrow keys) are processed via getch().

The snake’s head and body positions are updated, and the console is re-drawn accordingly.

Collision & Record:

When a collision is detected, the game either resets the snake (if lives remain) or ends the game and writes the record.

Ubuntu Version (Using ncurses)
Key Changes for Portability
Libraries:
The Ubuntu version replaces Windows-specific headers with <ncurses.h>, which provides functions for controlling the terminal in a portable way.

Terminal Initialization:

ncurses Setup:
The game starts by calling initscr(), cbreak(), and noecho(), which configure the terminal for non-buffered, non-echo input.

Input Configuration:
keypad(stdscr, TRUE) enables the arrow keys, and nodelay(stdscr, TRUE) makes getch() non-blocking.

Cursor Positioning:

The functions gotoxy() and GotoXY() now use the ncurses move(y, x) function to set the cursor position.


![image](https://github.com/user-attachments/assets/32066c34-0a5e-43c8-95e4-edd69b23c8ce)


Screen Clearing and Refreshing:

Instead of system("cls"), the Ubuntu version uses clear() to refresh the terminal screen.

The screen is updated with refresh() after each drawing operation.

Delay Function:

The busy-wait loop is replaced by napms(100), which pauses the game for a given number of milliseconds in a more efficient and portable way.

Non-Blocking Input Replacement:

A custom kbhit() function is implemented using ncurses’ getch() and ungetch() to check for pending input without blocking the game loop.

How It Runs on Ubuntu
Initialization:

The ncurses library is initialized, setting up the terminal for dynamic updating.

The welcome screen and instructions are printed using printw().

Game Loop:

The game loop uses ncurses functions to read user inputs and update the game state.

Movement functions (like Up(), Down(), Left(), Right()) update the snake’s coordinates on the terminal.

The border and food are drawn using ncurses drawing functions.

Collision and Record:

Collision detection works in the same logical way as in the Windows version.

When the game ends, the high-score record is written to a file and optionally displayed using ncurses functions.

Comparative Summary
Platform-Specific Code:

Windows Version:
Uses <conio.h>, <windows.h>, and system calls like system("cls") to control the console. Input is managed by getch(), and cursor positioning is achieved with Windows API functions.

Ubuntu Version:
Uses the ncurses library to handle all terminal operations (drawing, input, screen clearing, and delays) in a cross-platform manner. This approach is more portable and idiomatic for Unix-like systems.

Game Logic Consistency:
Despite the differences in how the terminal is controlled, both versions share the same core game logic:

The snake is represented as an array of coordinate structures.

Movement is handled by updating the head and shifting the body.

Bends (turns) are recorded so that the snake's body follows correctly.

Food generation, collision detection, score updates, and life management remain essentially the same.

Porting Challenges:

Input Handling:
Transitioning from Windows’ blocking/non-blocking input with <conio.h> to ncurses’ getch() and a custom kbhit() function.

Screen Control:
Replacing Windows-specific console functions with ncurses functions like move(), refresh(), and clear().

Delay Mechanism:
Instead of using a busy-wait loop or Windows sleep functions, the Ubuntu version uses napms() for a smoother delay.

Conclusion
Your Snake game is a well-structured project that demonstrates classic game development concepts such as:

Managing a continuously updating game state,

Handling real-time user input,

Performing collision detection and game state updates,

And using platform-specific libraries to control the console.

The Windows version leverages native API functions for console control, while the Ubuntu version uses the ncurses library to achieve similar functionality in a portable way. Both versions adhere to the same core logic, ensuring that the gameplay experience remains consistent across platforms.
