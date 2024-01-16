#include <curses.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>

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


struct Coordinate {
    int x, y;
};


void setup();
void draw();
void input();
void logic();
void generateFood();
void generateSpecialFood();
void gameover();
void record();
void showTopRecords();


WINDOW *win;
std::vector<Coordinate> snake;
Coordinate food, specialFood;
int snakeDir;
int score, specialFoodTimer;

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

    win = newwin(20, 40, 0, 0);
    snakeDir = RIGHT_KEY;       
    score = 0;
    specialFoodTimer = 0;

 
    Coordinate head = {5, 5};
    for (int i = 0; i < 5; i++) {
        snake.push_back({head.x - i, head.y});
    }

    generateFood();
    generateSpecialFood();

   
    std::cout << "Game setup complete!\n";
}

void draw() {
    clear();

    
    box(win, 0, 0);
    wrefresh(win);

    // Draw food
    mvprintw(food.y, food.x, "F");

    
    if (specialFoodTimer > 0) {
        mvprintw(specialFood.y, specialFood.x, "M");
        specialFoodTimer--;
    } else {
        generateSpecialFood();
    }

    
    for (const auto &part : snake) {
        mvprintw(part.y, part.x, "O");
    }

   
    mvprintw(0, 0, "Score: %d", score);

    refresh(); 
}

void input() {
    int key = getch();
    switch (key) {
        case UP_KEY:
        case W_KEY:
            if (snakeDir != DOWN_KEY) snakeDir = UP_KEY;
            break;
        case DOWN_KEY:
        case S_KEY:
            if (snakeDir != UP_KEY) snakeDir = DOWN_KEY;
            break;
        case RIGHT_KEY:
        case D_KEY:
            if (snakeDir != LEFT_KEY) snakeDir = RIGHT_KEY;
            break;
        case LEFT_KEY:
        case A_KEY:
            if (snakeDir != RIGHT_KEY) snakeDir = LEFT_KEY;
            break;
        case ESC_KEY:
            endwin();
            record();
            showTopRecords();
            std::exit(0);
            break;
        default:
            break;
    }
}

void logic() {
    
    Coordinate newHead = snake[0];
    switch (snakeDir) {
        case UP_KEY:
            newHead.y--;
            break;
        case DOWN_KEY:
            newHead.y++;
            break;
        case RIGHT_KEY:
            newHead.x++;
            break;
        case LEFT_KEY:
            newHead.x--;
            break;
        default:
            break;
    }

    
    if (newHead.x == food.x && newHead.y == food.y) {
        score++;
        generateFood();
    } else {
        snake.pop_back(); 
    }

    
    if (newHead.x == specialFood.x && newHead.y == specialFood.y && specialFoodTimer > 0) {
        score += 5; 
        specialFoodTimer = 0; 
    }

    
    if (newHead.x <= 1 || newHead.x >= 38 || newHead.y <= 1 || newHead.y >= 18) {
        gameover();
    }
    for (const auto &part : snake) {
        if (newHead.x == part.x && newHead.y == part.y) {
            gameover();
        }
    }

    snake.insert(snake.begin(), newHead); 
}

void generateFood() {
    srand(time(nullptr));
    food.x = rand() % 38 + 2; 
    food.y = rand() % 18 + 2; 
}

void generateSpecialFood() {
    srand(time(nullptr));
    specialFood.x = rand() % 38 + 2; 
    specialFood.y = rand() % 18 + 2; 
    specialFoodTimer = 100; 
}

void gameover() {
    clear();
    mvprintw(10, 15, "Game Over - Your Score: %d", score);
    refresh();
    record();
    showTopRecords();
    getch(); 
    endwin();
    std::exit(0);
}

void record() {
    std::string playerName;
    std::ofstream info("records.txt", std::ios::app);
    if (!info.is_open()) {
        std::cerr << "Error opening records file.\n";
        return;
    }

    std::cout << "Enter your name: ";
    std::cin >> playerName;

    std::transform(playerName.begin(), playerName.end(), playerName.begin(), ::toupper);

    info << "Player Name: " << playerName << '\n';
    time_t mytime;
    mytime = time(nullptr);
    info << "Played Date: " << ctime(&mytime);
    info << "Score: " << score << '\n';
    info << '\n';
    info.close();
}

void showTopRecords() {
    struct Record {
        std::string name;
        int score;
    };

    std::ifstream info("records.txt");
    if (!info.is_open()) {
        std::cerr << "Error opening records file.\n";
        return;
    }

    std::vector<Record> records;
    Record temp;
    while (info >> temp.name >> temp.name >> temp.score) {
        records.push_back(temp);
    }

    info.close();

   
    std::sort(records.begin(), records.end(), [](const Record &a, const Record &b) {
        return a.score > b.score;
    });

    std::cout << "\nTop Player Records:\n";
    std::cout << "Rank\tPlayer Name\tScore\n";

    for (size_t i = 0; i < std::min(records.size(), static_cast<size_t>(5)); i++) {
        std::cout << i + 1 << '\t' << records[i].name << '\t' << records[i].score << '\n';
    }
}

