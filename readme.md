#🐍snakegame:

This source code of snake game that i write with language like c,c++ and python:

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

// Define constants
#define SNAKE_SIZE 5 
#define FOOD_SIZE 3
#define SPECIAL_FOOD_SIZE 4

// Struct for direction
struct coordinate {
    int x, y;
};

typedef struct coordinate coordinate;

coordinate head, food, specialFood, body[SNAKE_SIZE * 10];
int length, life, score, specialFoodTimer;
int directionX, directionY;


void setup();
void draw();
void input();
void logic();
void generateFood();
void generateSpecialFood();
void gameover();
void record();
void showTopRecords();

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

    length = SNAKE_SIZE;
    head.x = 10;
    head.y = 10;
    for (int i = 0; i < SNAKE_SIZE; i++) {
        body[i].x = head.x - i;
        body[i].y = head.y;
    }

    generateFood();
    generateSpecialFood();
    specialFoodTimer = 0;

    life = 3;
    score = 0;

    directionX = 1; // 
    directionY = 0;


    printf("Game setup complete!\n");
}

void draw() {
    clear();


    for (int i = 0; i < COLS; i++) {
        for (int j = 0; j < LINES; j++) {
            if (i == 0 || i == COLS - 1 || j == 0 || j == LINES - 1) {
                mvprintw(j, i, "-");
            }
        }
    }

 
    for (int i = 0; i < FOOD_SIZE; i++) {
        mvprintw(food.y, food.x + i, "F");
    }


    if (specialFoodTimer > 0) {
        for (int i = 0; i < SPECIAL_FOOD_SIZE; i++) {
            mvprintw(specialFood.y, specialFood.x + i, "M");
        }
        specialFoodTimer--;
    } else {
        generateSpecialFood();
    }


    for (int i = 0; i < length; i++) {
        for (int j = 0; j < SNAKE_SIZE; j++) {
            if (i == 0) {
                mvprintw(body[i].y, body[i].x + j, "O");
            } else {
                mvprintw(body[i].y, body[i].x + j, "-");
            }
        }
    }

 
    mvprintw(0, COLS / 2 - 5, "Score: %d", score);
    mvprintw(0, COLS / 2 + 5, "Life: %d", life);

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
    
    if (head.x == food.x && head.y == food.y) {
        score++;
        generateFood();
        length += SNAKE_SIZE;
    }

    
    if (head.x == specialFood.x && head.y == specialFood.y) {
        score += 5; 
        specialFoodTimer = 0; 
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
    for (int i = 0; i < SNAKE_SIZE; i++) {
        body[0].x = head.x + i;
        body[0].y = head.y;
    }
}

void generateFood() {
    srand(time(NULL));
    food.x = rand() % (COLS - FOOD_SIZE - 2) + 1; 
    food.y = rand() % (LINES - 3) + 1; 
}

void generateSpecialFood() {
    srand(time(NULL));
    specialFood.x = rand() % (COLS - SPECIAL_FOOD_SIZE - 2) + 1; 
    specialFood.y = rand() % (LINES - 3) + 1;
    specialFoodTimer = 100; 
}

void gameover() {
    clear();
    mvprintw(LINES / 2, COLS / 2 - 5, "Game Over!");
    refresh();
    record();
    showTopRecords();
    endwin();
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

```

#snakegame.cpp




#snakegame.py
```python
import curses
from random import randint
from time import sleep


stdscr = curses.initscr()
curses.curs_set(0)
sh, sw = stdscr.getmaxyx()
w = curses.newwin(sh, sw, 0, 0)
w.keypad(1)
w.timeout(100)


snake = [[4, 4], [4, 3], [4, 2]]
snake_dir = 0  

#  food
food = [sh // 2, sw // 2]
w.addch(food[0], food[1], curses.ACS_PI)

# Game logic
score = 0
while True:
   
    key = w.getch()

    
    if key in [curses.KEY_RIGHT, ord('d')] and snake_dir != 1:
        snake_dir = 0
    elif key in [curses.KEY_LEFT, ord('a')] and snake_dir != 0:
        snake_dir = 1
    elif key in [curses.KEY_UP, ord('w')] and snake_dir != 3:
        snake_dir = 2
    elif key in [curses.KEY_DOWN, ord('s')] and snake_dir != 2:
        snake_dir = 3

    # Move the snake
    new_head = [snake[0][0], snake[0][1]]
    if snake_dir == 0:
        new_head[1] += 1
    elif snake_dir == 1:
        new_head[1] -= 1
    elif snake_dir == 2:
        new_head[0] -= 1
    elif snake_dir == 3:
        new_head[0] += 1

    snake.insert(0, new_head)

    
    if snake[0] == food:
        score += 1
        food = None
        while food is None:
            nf = [
                randint(1, sh - 1),
                randint(1, sw - 1)
            ]
            food = nf if nf not in snake else None
        w.addch(food[0], food[1], curses.ACS_PI)
    else:
        
        tail = snake.pop()
        w.addch(tail[0], tail[1], ' ')

    
    if (
        snake[0][0] in [0, sh] or
        snake[0][1] in [0, sw] or
        snake[0] in snake[1:]
    ):
        break

   
    w.addch(snake[0][0], snake[0][1], '*')


w.addstr(sh // 2, sw // 2, f'Game Over - Your Score: {score}', curses.A_BOLD)
w.refresh()
sleep(2)


curses.endwin()
```
