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

