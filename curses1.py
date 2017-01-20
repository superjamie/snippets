#!/usr/bin/python

"""
A simple example of python curses:
- drawing in the main stdscr
- getting input
- getting the size of the stdscr
- making a new window
- moving and resizing the window
- printing in both windows
- overwriting the stdscr with the new window
- borders
- (sortof) safe ways to catch exceptions so you have less of a chance
  of messing up your terminal if a curses method fails

https://docs.python.org/2/library/curses.html
"""

import curses

def reset(stdscr):
    curses.nocbreak()
    stdscr.keypad(0)
    curses.echo()
    curses.curs_set(1)
    curses.endwin()

def main(stdscr):
    okwin = curses.newwin(1, 1, 1, 1)
    b = " "

    while True:
        stdscr.border(0)
        stdscr.refresh()

        c = stdscr.getch()
        if c == ord('q'):
            break  # Exit the while()
        else:
            height, width = stdscr.getmaxyx()
            for y in range(1, height-1):
                for x in range (1, width-1):
                    stdscr.addch(y, x, c)
            oh = height / 2
            ow = width / 2
            oy = height / 4
            ox = width / 4
            okwin.resize(oh, ow)
            okwin.mvwin(oy, ox)
            for y in range(1, oh-1):
                for x in range (1, ow-1):
                    okwin.addch(y, x, b)
            okwin.border(0)
            okwin.overwrite(stdscr)
            b = c

    reset(stdscr)
    exit()

try:
    stdscr = curses.initscr()

    # turn off echo to screen
    curses.noecho()

    # don't require enter to be pressed for input
    curses.cbreak()

    # enable keypad mode to get things like arrow keys
    stdscr.keypad(1)

    # make cursor invisible
    curses.curs_set(0)

    main(stdscr)

except curses.error:
    # end
    reset(stdscr)
    exit()
