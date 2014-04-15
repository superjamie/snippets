#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <signal.h>

/* curses with window resizing
 *
 * this handles dynamic resizing of graphical terminals when ncurses is
 * drawing separate WINDOW*s instead of just using stdscr.
 * documentation on actually implementing this properly is patchy. i
 * don't know if this is "right" but hey it works!
 */

// move all this to a header file

int termx, termy, mapwinx, mapwiny, sidebarx, sidebary;

WINDOW *mapwin, *sidebar;

void terminal_start();
void terminal_stop();
void get_window_dimensions();
void draw_map_window();
void draw_sidebar_window();
void resizehandler(int);

// start c file here

void terminal_start() {

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();
}

void terminal_stop() {

	endwin();
}

void get_window_dimensions() {

	getmaxyx(stdscr, termy, termx);

	mapwinx = termx - 25;
	mapwiny = sidebary = termy;

	mapwin = newwin(mapwiny-1, mapwinx-1, 0, 0);
	sidebar = newwin(sidebary-1, sidebarx-1, 0, mapwinx-1);
}

/* i should probably slim these two down to a single draw_window()
 * function which takes args WINDOW x y char
 */

void draw_map_window() {

	int x, y;
	for (y = 0; y < mapwiny-1; y++) {
	    for (x = 0; x < mapwinx-1; x++) {
		mvwaddch(mapwin, y, x, 'm');
	    }
	}

	wborder(mapwin, 0, 0, 0, 0, 0, 0, 0, 0);

	wrefresh(mapwin);
}

void draw_sidebar_window() {

	int x, y;
	for (y = 0; y < sidebary-1; y++) {
	    for (x = 0; x < sidebarx-1; x++) {
		mvwaddch(sidebar, y, x, 's');
	    }
	}

	wborder(sidebar, 0, 0, 0, 0, 0, 0, 0, 0);

	wrefresh(sidebar);
}

// i stole this idea of a resize handler which just calls start and stop from stone soup - http://crawl.develz.org/

void resizehandler(int sig) {

	terminal_stop();
	terminal_start();

	get_window_dimensions();

	draw_map_window();
	draw_sidebar_window();
}

int main(int argc, char *argv[]) {

	terminal_start();

	termx = 0; termy = 0; mapwinx = 0; mapwiny = 0; sidebarx = 25; sidebary = 0;

	signal(SIGWINCH, resizehandler);

	int c;
	get_window_dimensions();

	// game loop here
	while (true) {

		draw_map_window();
		draw_sidebar_window();

		// handle input
		c = 0;
		c = getch();

		if (c == 'q' || c == 27 /*ESC*/)
		    break;
	}

	terminal_stop();

	return 0;
}
