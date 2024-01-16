import curses
from random import randint
from time import sleep

def main(stdscr):
    curses.curs_set(0)
    sh, sw = stdscr.getmaxyx()
    if sh < 5 or sw < 20:
        stdscr.addstr(0, 0, "Terminal size too small. Please resize and try again.")
        stdscr.refresh()
        stdscr.getch()
        return

    w = curses.newwin(sh, sw, 0, 0)
    w.keypad(1)
    w.timeout(100)

    try:
        for i in range(sh):
            w.addch(i, 0, '|')
            w.addch(i, sw-1, '|')
        for i in range(sw):
            w.addch(0, i, '-')
            w.addch(sh-1, i, '-')

        snake = [[sh//2, sw//2-1], [sh//2, sw//2], [sh//2, sw//2+1], [sh//2, sw//2+2], [sh//2, sw//2+3]]
        snake_dir = 0


        food = [sh // 2, sw // 2]
        w.addch(food[0], food[1], curses.ACS_PI)

        special_food = [sh // 2, sw // 2 + 5]
        w.addch(special_food[0], special_food[1], ord('M'))


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
                        randint(1, sh - 2),
                        randint(1, sw - 2)
                    ]
                    food = nf if nf not in snake else None
                w.addch(food[0], food[1], curses.ACS_PI)


            if snake[0] == special_food:
                score += 5
                special_food = None
                while special_food is None:
                    sf = [
                        randint(1, sh - 2),
                        randint(1, sw - 2)
                    ]
                    special_food = sf if sf not in snake and sf not in [food] else None
                w.addch(special_food[0], special_food[1], ord('M'))


            for segment in snake:
                if 0 < segment[0] < sh and 0 < segment[1] < sw:
                    w.addch(segment[0], segment[1], ord('O'))

            for i in range(sh):
                if 0 < i < sh and 0 < sw-1 < sw:
                    w.addch(i, 0, '|')
                    w.addch(i, sw-1, '|')
            for i in range(sw):
                if 0 < 0 < sh-1 and 0 < i < sw-1:
                    w.addch(0, i, '-')
                    w.addch(sh-1, i, '-')


            w.addstr(0, sw // 2 - 5, f'Score: {score}')

            w.refresh()

            sleep(0.1)
    except curses.error as e:
        stdscr.addstr(sh // 2, sw // 2, f"Error: {e}", curses.A_BOLD)
        stdscr.refresh()
        stdscr.getch()  
if __name__ == "__main__":
    curses.wrapper(main)

