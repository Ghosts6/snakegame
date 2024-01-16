#🐍snakegame:

![Screenshot 2024-01-16 16:39:53](https://github.com/Ghosts6/snakegame_withc/assets/95994481/dece8de4-ea23-404f-afe4-a5ba0b4d0883)

This source code of snake game that i write with language like c,c++ and python:inside game we define def for generate food and special food
and def for draw snake and border also logic for game roll and etc...,this game also record your score and show top score at end of game 
in terminal and file we create for this work

[Screencast 2024-01-16 16:41:23.webm](https://github.com/Ghosts6/snakegame_withc/assets/95994481/b6a8679c-fe14-4e3c-99bf-a6e791a1ded9)


#snakegame.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <ctype.h>


// Define key codes
#define UP_KEY 65
#define DOWN_KEY 66
#define RIGHT_KEY 67
#define LEFT_KEY 68
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100
#define ESC_KEY 27

// Struct for direction
struct coordinate {
    int x, y;
};

typedef struct coordinate coordinate;

coordinate head, food[3], specialFood[2], body[100];
int length, life, score, specialFoodTimer, topScore;
int directionX, directionY;

// Function prototypes
void setup();
void draw();
void input();
void logic();
void generateFood();
void generateSpecialFood();
void gameover();
void record();
void showTopRecords();
void updateTopScore();

int main() {
    setup();

    while (1) {
        draw();
        input();
        logic();
        usleep(100000); 
    }

    endwin(); 
    return 0;
}

void setup() {
    initscr();            
    keypad(stdscr, TRUE); 
    nodelay(stdscr, TRUE); 
    noecho();             
    curs_set(0);          

    length = 15; 
    head.x = 10;
    head.y = 10;
    body[0].x = head.x - length + 1;
    body[0].y = head.y;

    generateFood();
    generateSpecialFood();
    specialFoodTimer = 0;

    life = 3;
    score = 0;
    topScore = 0;

    directionX = 1; 
    directionY = 0;


    printf("Game setup complete!\n");
}

void draw() {
    clear();


    for (int i = 0; i < COLS; i++) {
        mvprintw(0, i, "-");
        mvprintw(LINES - 1, i, "-");
    }
    for (int i = 1; i < LINES - 1; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, COLS - 1, "|");
    }


    for (int i = 0; i < 3; i++) {
        mvprintw(food[i].y, food[i].x, "F");
    }


    if (specialFoodTimer > 0) {
        for (int i = 0; i < 2; i++) {
            mvprintw(specialFood[i].y, specialFood[i].x, "M");
        }
        specialFoodTimer--;
    } else {
        generateSpecialFood();
    }

    for (int i = 0; i < length; i++) {
        mvprintw(body[i].y, body[i].x, "-");
    }
    mvprintw(head.y, head.x, "O");


    mvprintw(0, COLS / 2 - 5, "Score: %d", score);
    mvprintw(0, COLS / 2 + 5, "Life: %d", life);

    mvprintw(LINES - 1, COLS / 2 - 5, "Top Score: %d", topScore);

    refresh();
}

void input() {
    int key = getch();
    switch (key) {
        case UP_KEY:
        case W_KEY:
            if (directionY == 0) {
                directionX = 0;
                directionY = -1;
            }
            break;
        case DOWN_KEY:
        case S_KEY:
            if (directionY == 0) {
                directionX = 0;
                directionY = 1;
            }
            break;
        case RIGHT_KEY:
        case D_KEY:
            if (directionX == 0) { 
                directionX = 1;
                directionY = 0;
            }
            break;
        case LEFT_KEY:
        case A_KEY:
            if (directionX == 0) {
                directionX = -1;
                directionY = 0;
            }
            break;
        case ESC_KEY:
            endwin();
            exit(0);
            break;
        default:
            break;
    }
}

void logic() {

    for (int i = 0; i < 3; i++) {
        if (head.x == food[i].x && head.y == food[i].y) {
            score++;


            for (int j = 0; j < 2; j++) {
                if (length < 1000) {
                    length+=5;
                }
            }

            generateFood();
        }
    }


    for (int i = 0; i < 2; i++) {
        if (head.x == specialFood[i].x && head.y == specialFood[i].y) {
            score += 15; 
            specialFoodTimer = 0; 
        }
    }

 
    head.x += directionX;
    head.y += directionY;


    if (head.x <= 0 || head.x >= COLS - 1 || head.y <= 0 || head.y >= LINES - 1) {
        gameover();
    }
    for (int i = 1; i < length; i++) {
        if (head.x == body[i].x && head.y == body[i].y) {
            gameover();
        }
    }

 
    for (int i = length - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0] = head;

 
    updateTopScore();
}

void generateFood() {
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        food[i].x = rand() % (COLS - 2) + 1; 
        food[i].y = rand() % (LINES - 3) + 1; 
    }
}

void generateSpecialFood() {
    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        specialFood[i].x = rand() % (COLS - 2) + 1;
        specialFood[i].y = rand() % (LINES - 3) + 1;
    }
    specialFoodTimer = 50; 
}

void gameover() {

    updateTopScore();
    record();
    showTopRecords();
    endwin(); 
    printf("Game Over! Your score: %d\n", score);
    exit(0);
}

void record() {
    char playerName[20], capitalizedName[20];

    FILE *info = fopen("records.txt", "a+");
    if (info == NULL) {
        printf("Error opening records file.\n");
        return;
    }

    int j;  

    printf("Enter your name: ");
    scanf("%19s", playerName);

    for (j = 0; playerName[j] != '\0'; j++) {
        capitalizedName[0] = toupper(playerName[0]);
        if (playerName[j - 1] == ' ') {
            capitalizedName[j] = toupper(playerName[j]);
            capitalizedName[j - 1] = playerName[j - 1];
        } else {
            capitalizedName[j] = playerName[j];
        }
    }
    capitalizedName[j] = '\0';

    fprintf(info, "Player Name: %s\n", capitalizedName);
    time_t mytime;
    mytime = time(NULL);
    fprintf(info, "Played Date: %s", ctime(&mytime));
    fprintf(info, "Score: %d\n", score);
    fprintf(info, "\n");
    fclose(info);
}

void showTopRecords() {
    struct Record {
        char name[20];
        int score;
    };

    int count;
    FILE *info = fopen("records.txt", "r");
    if (info == NULL) {
        printf("Error opening records file.\n");
        return;
    }

    fseek(info, 0, SEEK_END);
    count = ftell(info) / sizeof(struct Record);
    rewind(info);

    struct Record *records = malloc(count * sizeof(struct Record));
    if (records == NULL) {
        printf("Memory allocation error.\n");
        fclose(info);
        return;
    }

    for (int i = 0; i < count; i++) {
        fscanf(info, "Player Name: %s", records[i].name);
        fscanf(info, "Played Date: %*[^\n]\n");
        fscanf(info, "Score: %d", &records[i].score);
        fscanf(info, "\n");
    }

    fclose(info);

  
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].score < records[j + 1].score) {
                struct Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    printf("\nTop Player Records:\n");
    printf("Rank\tPlayer Name\tScore\n");

    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t\t%d\n", i + 1, records[i].name, records[i].score);
    }

    free(records);
}

void updateTopScore() {
    if (score > topScore) {
        topScore = score;
    }
}


```

#snakegame.cpp

```cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>

// Define key codes
#define UP_KEY 65
#define DOWN_KEY 66
#define RIGHT_KEY 67
#define LEFT_KEY 68
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100
#define ESC_KEY 27

// Struct for direction
struct Coordinate {
    int x, y;
};

typedef struct Coordinate Coordinate;

Coordinate head, food[3], specialFood[2], body[100];
int length, life, score, specialFoodTimer, topScore;
int directionX, directionY;

// Function prototypes
void setup();
void draw();
void input();
void logic();
void generateFood();
void generateSpecialFood();
void gameover();
void record();
void showTopRecords();
void updateTopScore();

int main() {
    setup();

    while (1) {
        draw();
        input();
        logic();
        usleep(100000); 
    }

    endwin(); 
    return 0;
}

void setup() {
    initscr();            
    keypad(stdscr, TRUE); 
    nodelay(stdscr, TRUE); 
    noecho();             
    curs_set(0);          

    length = 15; 
    head.x = 10;
    head.y = 10;
    body[0].x = head.x - length + 1;
    body[0].y = head.y;

    generateFood();
    generateSpecialFood();
    specialFoodTimer = 0;

    life = 3;
    score = 0;
    topScore = 0;

    directionX = 1; 
    directionY = 0;

    std::cout << "Game setup complete!" << std::endl;
}

void draw() {
    clear();

    for (int i = 0; i < COLS; i++) {
        mvprintw(0, i, "-");
        mvprintw(LINES - 1, i, "-");
    }
    for (int i = 1; i < LINES - 1; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, COLS - 1, "|");
    }

    for (int i = 0; i < 3; i++) {
        mvprintw(food[i].y, food[i].x, "F");
    }

    if (specialFoodTimer > 0) {
        for (int i = 0; i < 2; i++) {
            mvprintw(specialFood[i].y, specialFood[i].x, "M");
        }
        specialFoodTimer--;
    } else {
        generateSpecialFood();
    }

    for (int i = 0; i < length; i++) {
        mvprintw(body[i].y, body[i].x, "-");
    }
    mvprintw(head.y, head.x, "O");

    mvprintw(0, COLS / 2 - 5, "Score: %d", score);
    mvprintw(0, COLS / 2 + 5, "Life: %d", life);

    mvprintw(LINES - 1, COLS / 2 - 5, "Top Score: %d", topScore);

    refresh();
}

void input() {
    int key = getch();
    switch (key) {
        case UP_KEY:
        case W_KEY:
            if (directionY == 0) {
                directionX = 0;
                directionY = -1;
            }
            break;
        case DOWN_KEY:
        case S_KEY:
            if (directionY == 0) {
                directionX = 0;
                directionY = 1;
            }
            break;
        case RIGHT_KEY:
        case D_KEY:
            if (directionX == 0) { 
                directionX = 1;
                directionY = 0;
            }
            break;
        case LEFT_KEY:
        case A_KEY:
            if (directionX == 0) {
                directionX = -1;
                directionY = 0;
            }
            break;
        case ESC_KEY:
            endwin();
            exit(0);
            break;
        default:
            break;
    }
}

void logic() {
    for (int i = 0; i < 3; i++) {
        if (head.x == food[i].x && head.y == food[i].y) {
            score++;

            for (int j = 0; j < 2; j++) {
                if (length < 1000) {
                    length+=5;
                }
            }

            generateFood();
        }
    }

    for (int i = 0; i < 2; i++) {
        if (head.x == specialFood[i].x && head.y == specialFood[i].y) {
            score += 15; 
            specialFoodTimer = 0; 
        }
    }

    head.x += directionX;
    head.y += directionY;

    if (head.x <= 0 || head.x >= COLS - 1 || head.y <= 0 || head.y >= LINES - 1) {
        gameover();
    }
    for (int i = 1; i < length; i++) {
        if (head.x == body[i].x && head.y == body[i].y) {
            gameover();
        }
    }

    for (int i = length - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    body[0] = head;

    updateTopScore();
}

void generateFood() {
    srand(time(NULL));
    for (int i = 0; i < 3; i++) {
        food[i].x = rand() % (COLS - 2) + 1; 
        food[i].y = rand() % (LINES - 3) + 1; 
    }
}

void generateSpecialFood() {
    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        specialFood[i].x = rand() % (COLS - 2) + 1;
        specialFood[i].y = rand() % (LINES - 3) + 1;
    }
    specialFoodTimer = 50; 
}

void gameover() {
    updateTopScore();
    record();
    showTopRecords();
    endwin(); 
    std::cout << "Game Over! Your score: " << score << std::endl;
    exit(0);
}

void record() {
    char playerName[20], capitalizedName[20];

    FILE *info = fopen("records.txt", "a+");
    if (info == NULL) {
        std::cout << "Error opening records file." << std::endl;
        return;
    }

    int j;  

    std::cout << "Enter your name: ";
    scanf("%19s", playerName);

    for (j = 0; playerName[j] != '\0'; j++) {
        capitalizedName[0] = toupper(playerName[0]);
        if (playerName[j - 1] == ' ') {
            capitalizedName[j] = toupper(playerName[j]);
            capitalizedName[j - 1] = playerName[j - 1];
        } else {
            capitalizedName[j] = playerName[j];
        }
    }
    capitalizedName[j] = '\0';

    fprintf(info, "Player Name: %s\n", capitalizedName);
    time_t mytime;
    mytime = time(NULL);
    fprintf(info, "Played Date: %s", ctime(&mytime));
    fprintf(info, "Score: %d\n", score);
    fprintf(info, "\n");
    fclose(info);
}

void showTopRecords() {
    struct Record {
        char name[20];
        int score;
    };

    int count;
    FILE *info = fopen("records.txt", "r");
    if (info == NULL) {
        std::cout << "Error opening records file." << std::endl;
        return;
    }

    fseek(info, 0, SEEK_END);
    count = ftell(info) / sizeof(struct Record);
    rewind(info);

    struct Record *records = (struct Record*)malloc(count * sizeof(struct Record));
    if (records == NULL) {
        std::cout << "Memory allocation error." << std::endl;
        fclose(info);
        return;
    }

    for (int i = 0; i < count; i++) {
        fscanf(info, "Player Name: %s", records[i].name);
        fscanf(info, "Played Date: %*[^\n]\n");
        fscanf(info, "Score: %d", &records[i].score);
        fscanf(info, "\n");
    }

    fclose(info);

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].score < records[j + 1].score) {
                struct Record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    std::cout << "\nTop Player Records:\n";
    std::cout << "Rank\tPlayer Name\tScore\n";

    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << "\t" << records[i].name << "\t\t" << records[i].score << std::endl;
    }

    free(records);
}

void updateTopScore() {
    if (score > topScore) {
        topScore = score;
    }
}
```

#snakegame.py
```python
import curses
import random
import time


W_KEY = 119
A_KEY = 97
S_KEY = 115
D_KEY = 100
ESC_KEY = 27


class Coordinate:
    def __init__(self, x, y):
        self.x = x
        self.y = y

head = Coordinate(10, 10)
body = [Coordinate(head.x - i, head.y) for i in range(15)]
food = [Coordinate(random.randint(1, 38), random.randint(1, 18)) for _ in range(3)]
special_food = [Coordinate(random.randint(1, 38), random.randint(1, 18)) for _ in range(2)]
length = 15
life = 3
score = 0
special_food_timer = 0
top_score = 0
direction_x = 1
direction_y = 0

def setup():
    curses.initscr()
    curses.curs_set(0) 
    curses.noecho()
    curses.cbreak()
    curses.start_color()
    win = curses.newwin(curses.LINES, curses.COLS)
    win.keypad(True)
    win.timeout(100)  
    return win

def draw(win):
    global special_food_timer
    win.clear()

    for i in range(curses.COLS):  
        win.addch(0, i, '-')
        win.addch(curses.LINES-2, i, '-')  

    for i in range(1, curses.LINES-2): 
        win.addch(i, 0, '|')
        win.addch(i, curses.COLS-1, '|')

    for f in food:
        win.addch(f.y, f.x, 'F')

    if special_food_timer > 0:
        for sf in special_food:
            win.addch(sf.y, sf.x, 'M')
        special_food_timer -= 1
    else:
        generate_special_food()

    for b in body:
        try:
            win.addch(b.y, b.x, '-')
        except curses.error:
            pass

    try:
        win.addch(head.y, head.x, 'O')
    except curses.error:
        pass

    win.addstr(curses.LINES-2, curses.COLS//2 - 5, f"Score: {score}") 
    win.addstr(curses.LINES-2, curses.COLS//2 + 5, f"Life: {life}")  
    win.addstr(curses.LINES-1, curses.COLS//2 - 5, f"Top Score: {top_score}")

    win.refresh()

def input_key(win):
    global direction_x, direction_y

    key = win.getch()  

    if key == ESC_KEY:
        curses.endwin()
        exit()
    elif key in [W_KEY] and direction_y == 0:
        direction_x, direction_y = 0, -1
    elif key in [S_KEY] and direction_y == 0:
        direction_x, direction_y = 0, 1
    elif key in [D_KEY] and direction_x == 0:
        direction_x, direction_y = 1, 0
    elif key in [A_KEY] and direction_x == 0:
        direction_x, direction_y = -1, 0

def logic():
    global head, body, length, score, special_food_timer, life, top_score

    for f in food:
        if head.x == f.x and head.y == f.y:
            score += 1

            for _ in range(2):
                if length < 1000:
                    length += 5

            generate_food()

    for sf in special_food:
        if head.x == sf.x and head.y == sf.y:
            score += 15
            special_food_timer = 0

    head = Coordinate(head.x + direction_x, head.y + direction_y)

    if (
        head.x <= 0
        or head.x >= curses.COLS-1
        or head.y <= 0
        or head.y >= curses.LINES-2
        or any(head.x == b.x and head.y == b.y for b in body[1:])
    ):
        game_over()

    body.insert(0, Coordinate(head.x, head.y))

    if len(body) > length:
        body.pop()

    update_top_score()

def generate_food():
    global food
    food = [Coordinate(random.randint(1, curses.COLS-2), random.randint(1, curses.LINES-3)) for _ in range(3)]

def generate_special_food():
    global special_food, special_food_timer
    special_food = [Coordinate(random.randint(1, curses.COLS-2), random.randint(1, curses.LINES-3)) for _ in range(2)]
    special_food_timer = 50

def game_over():
    update_top_score()
    record()
    show_top_records()
    curses.endwin()
    print(f"Game Over! Your score: {score}")
    exit()

def record():
    player_name = input("Enter your name: ")
    capitalized_name = player_name.title()

    with open("records.txt", "a+") as info:
        info.write(f"Player Name: {capitalized_name}\n")
        info.write(f"Played Date: {time.ctime()}\n")
        info.write(f"Score: {score}\n\n")

def show_top_records():
    records = []

    with open("records.txt", "r") as info:
        lines = info.readlines()

    for i in range(0, len(lines), 3):
        player_name = lines[i].split(":")[1].strip()
        score = int(lines[i + 2].split(":")[1].strip())
        records.append({"Player Name": player_name, "Score": score})

    records.sort(key=lambda x: x["Score"], reverse=True)

    print("\nTop Player Records:")
    print("Rank\tPlayer Name\tScore")
    for i, record in enumerate(records, start=1):
        print(f"{i}\t{record['Player Name']}\t\t{record['Score']}")

def update_top_score():
    global top_score
    if score > top_score:
        top_score = score

def main(win):
    win = setup()

    while True:
        draw(win)
        input_key(win)
        logic()

curses.wrapper(main)


```
