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
