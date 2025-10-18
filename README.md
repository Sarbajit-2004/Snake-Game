# 🐍 Cross-Platform Snake Game in C (Windows + Ubuntu)

> A feature-rich, terminal-based Snake game implemented in **C**, supporting both **Windows Console** and **Linux (Ubuntu)** using **ncurses**.  
> This project demonstrates real-time input handling, collision detection, and platform-specific console management while maintaining a shared core logic.

---

## 🎮 Overview

The Snake game recreates the classic arcade experience in a lightweight console environment.

### 🕹️ Gameplay
- The snake moves continuously across the screen, growing longer each time it eats food.
- The player controls the snake’s direction using the **arrow keys**.
- The game ends if the snake collides with its body or the game borders.

### 📈 Scoring & Lives
- Each piece of food increases your **score** and **snake length**.
- The player begins with a fixed number of **lives**.
- Colliding with a wall or yourself reduces one life; when all lives are lost, the game ends.
- A **record file** stores high scores with player name and date.

---

## 🧠 Common Game Logic

Both the Windows and Ubuntu versions share identical gameplay mechanics — only differing in how they interact with the terminal.

### 🧩 Data Structures

#### `struct coordinate`
The core structure stores the coordinates and movement direction of each part of the snake.

```c
typedef struct coordinate {
    int x;
    int y;
    int direction;
} coordinate;
```

Each body segment and the snake head use this structure to update position and handle turns.

![Snake Coordinates](https://github.com/user-attachments/assets/c31fb3de-0ea5-408a-8f98-dd0bec94ef40)

### 🧱 Global Variables
- `head` – pointer to the snake’s head  
- `bend[]` – array storing the turning points  
- `body[]` – array of snake body coordinates  
- `length` – current snake length  
- `life` – remaining lives  

These maintain the overall state of the game.

---

## 🐍 Movement & Bends

### ▶️ Movement Logic
The snake’s head position is updated according to its current direction.  
Each body segment follows the previous one, giving smooth motion.

### ↩️ Bend Points
Whenever the player changes direction, a **new bend** is recorded.  
The segments follow these bends in sequence, simulating natural snake motion.

---

## ⌨️ Input Handling
- Arrow keys control movement.  
- Reversal prevention ensures you can’t instantly move in the opposite direction (e.g., right → left).  
- Inputs are captured non-blocking, allowing continuous motion.

---

## 🍎 Food & Growth
- Food is randomly placed within the playable area.  
- When the snake’s head overlaps with the food position:
  - The snake grows by one segment.
  - A new food position is generated.
- Food is displayed using symbols like `"F"` or `"*"`.

---

## 💥 Collision & Lives
- The snake loses a life upon:
  - Hitting a wall  
  - Hitting its own body  
- If all lives are lost:
  - The game ends  
  - High scores are written to `record.txt`

---

## 🖥️ Display & Speed
- The screen is continuously refreshed with updated snake and food positions.
- A **delay function** regulates speed.
- Each frame redraws borders, snake, and food.

---

## 🧾 Record Keeping
At the end of each session:
- The player’s name, score, and date/time are stored in `record.txt`.
- The record file preserves top scores across sessions.

---

## 🪟 Windows Version

### ⚙️ Key Libraries
```c
#include <conio.h>
#include <windows.h>
#include <process.h>
```

These headers allow console manipulation, input handling, and delays.

### 🖲️ Console Control
- `gotoxy()` and `GotoXY()` use `SetConsoleCursorPosition()` from the Windows API to move the cursor.
- The screen is cleared with:
  ```c
  system("cls");
  ```

### ⌨️ Input & Delay
- **Non-blocking input:** via `kbhit()` and `getch()`  
- **Delay:** implemented using `Sleep()` or a simple loop

### 🧩 Game Logic Functions
Key functions include:
- `Move()` – controls continuous movement  
- `Food()` – handles spawning  
- `Bend()` – manages turning logic  
- `Up()`, `Down()`, `Left()`, `Right()` – control direction  
- `Boarder()` – draws game boundary  

### 🧭 Execution Flow
1. Welcome screen & instructions  
2. Console setup (cursor position, color)  
3. Game loop → movement → collision detection → scoring  
4. Records saved on exit

---

## 🐧 Ubuntu (ncurses) Version

### ⚙️ Key Libraries
```c
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
```

Replaces all Windows-specific APIs with **ncurses**, making it fully portable on Linux.

### 🧾 Terminal Initialization
```c
initscr();
cbreak();
noecho();
keypad(stdscr, TRUE);
nodelay(stdscr, TRUE);
```

These configure **non-buffered, non-echoed, and non-blocking input**.

### 🧭 Cursor Control
- Instead of `gotoxy()`, ncurses uses:
  ```c
  move(y, x);
  ```
- The screen is cleared with `clear()` and updated with `refresh()`.

### 💤 Delay Handling
- Uses:
  ```c
  napms(100);
  ```
  to pause for 100ms between frames (smoother & CPU-friendly).

### 🔁 Non-Blocking Input
Implements a custom `kbhit()` function using `getch()` and `ungetch()`.

### 🧩 ncurses Rendering
- Borders, snake, and food are drawn with `printw()` or `mvprintw()`.  
- Refreshing occurs in every loop iteration.

![ncurses Snake Screenshot](https://github.com/user-attachments/assets/32066c34-0a5e-43c8-95e4-edd69b23c8ce)

---

## ⚖️ Comparative Summary

| Feature | Windows Version | Ubuntu (ncurses) Version |
|----------|----------------|--------------------------|
| **Libraries** | `<conio.h>`, `<windows.h>` | `<ncurses.h>` |
| **Screen Control** | `system("cls")`, cursor via Windows API | `clear()`, `move()`, `refresh()` |
| **Input Handling** | `getch()` / `kbhit()` | `getch()` + `nodelay()` |
| **Delay Mechanism** | `Sleep()` / busy-wait loop | `napms()` |
| **Cursor Control** | `SetConsoleCursorPosition()` | `move(y, x)` |
| **Portability** | Windows only | Cross-platform |
| **Performance** | High, but non-portable | Efficient & portable |
| **Look & Feel** | Classic DOS look | Terminal-friendly ncurses UI |

---

## ⚔️ Porting Challenges

- Replacing Windows API with portable ncurses calls  
- Rewriting screen refresh and cursor management  
- Adapting non-blocking input logic  
- Maintaining consistent timing and game speed  

---

## 🧩 Folder Structure

```
snake-game/
├── src/
│   ├── snake_windows.c
│   ├── snake_ubuntu.c
│   └── utils.h
├── assets/
│   └── record.txt
├── README.md
└── Makefile
```

---

## 🏁 How to Run

### 🪟 On Windows
```bash
gcc snake_windows.c -o snake.exe
snake.exe
```

### 🐧 On Ubuntu / Linux
```bash
sudo apt install libncurses5-dev libncursesw5-dev
gcc snake_ubuntu.c -lncurses -o snake
./snake
```

---

## 🧪 Example Output

```
Score: 45   Lives: 2
##############################
#                            #
#             F              #
#       oooooooo             #
#                            #
##############################
```

---

## 🧠 Learning Highlights

This project demonstrates:
- Real-time input handling in C  
- Struct-based state management  
- Collision detection and game logic separation  
- Cross-platform portability using platform-specific libraries  
- Record management using file I/O  

---

## 👨‍💻 Author

**Sarbajit Kumar De**  
🎓 Final Year | CSE  
📧 `sarbajit.dev@outlook.com`  
🌐 [github.com/SarbajitDe](https://github.com/SarbajitDe)

---
---

> _"Classic gameplay meets modern C design — one codebase, two platforms."_ 🐍✨
