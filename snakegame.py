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

