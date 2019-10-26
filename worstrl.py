#!/usr/bin/python3
# extremely minimal "roguelike" walking simulator.
# just Python 3 and curses in 80x24 terminal.
# hjkl or arrows to move, q or ESC quits, w for cheat mode.

import curses
from curses import wrapper
import random

XMIN = 1
XMAX = 80
YMIN = 1
YMAX = 24

def main(stdscr):
    curses.curs_set(0)
    stdscr.clear()

    win = curses.newwin(YMAX, XMAX, 0, 0)

    ypos = random.randint(YMIN+1,YMAX-1)
    xpos = random.randint(XMIN+1,XMAX-1)
    
    ystair = random.randint(YMIN+1,YMAX-1)
    xstair = random.randint(XMIN+1,XMAX-1)
    
    while True:
        win.clear()
        win.border("|","|","-","-","+","+","+","+")
        win.addch(ypos-1,xpos-1,"@")
        win.addch(ystair-1,xstair-1,">")
        stdscr.refresh()
        win.refresh()

        c = stdscr.getch()
        #c = win.getkey()
        curses.flushinp()

        if c == curses.KEY_LEFT  or c == ord("h"):
            xpos -= 1
        if c == curses.KEY_DOWN  or c == ord("j"):
            ypos += 1
        if c == curses.KEY_UP    or c == ord("k"):
            ypos -= 1
        if c == curses.KEY_RIGHT or c == ord("l"):
            xpos += 1
        
        if (xpos < XMIN+1):
            xpos = XMIN+1
        if (xpos > XMAX-1):
            xpos = XMAX-1
        if (ypos < YMIN+1):
            ypos = YMIN+1
        if (ypos > YMAX-1):
            ypos = YMAX-1

        if c == ord("w"): ## warp around
            ypos = random.randint(YMIN+1,YMAX-1)
            xpos = random.randint(XMIN+1,XMAX-1)
            
        if (xpos == xstair and ypos == ystair):
            ypos = random.randint(YMIN+1,YMAX-1)
            xpos = random.randint(XMIN+1,XMAX-1)
            ystair = random.randint(YMIN+1,YMAX-1)
            xstair = random.randint(XMIN+1,XMAX-1)

        if c == ord("q") or c == 27:  ## ESC == 27
            break

if __name__ == "__main__":
    os.putenv('ESCDELAY', '1')
    wrapper(main)
