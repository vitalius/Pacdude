#include <stdio.h>
#include <curses.h>


int main(int, char **);
int submenue(void);


/* This functions sh0wred us two important things:
	- curses' retarded primitiveness must be hidden
	  by us in an extra abstraction layer, so we
	  end up having drawing primitives like svgalibs
	  and so on provide, and
	
	- How complicated one can do a "box" in an API.
*/ 
int submenue(void) {
	int i,j;
	addch(ACS_ULCORNER);
	for(i=0;i<20;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	addch('\n');
	for(i=0;i < 10; i++)
		for(j=0; j <= 21; j++)
			if(j == 0)
				addch(ACS_VLINE);
			else if(j == 21) {
				addch(ACS_VLINE);
				addch('\n');
			}
			else
				addch(' ');
			addch(ACS_LLCORNER);
			for(i=0;i < 20; i++)
				addch(ACS_HLINE);
			addch(ACS_LRCORNER);
			addch('\n');
			mvprintw(5,7, "My phat box");
			getch();
			clear();
			return 0;	
}
int main(int argc, char **argv) {
	WINDOW *mywin; // a handler for our window, a matrix. Maybe we don't even need that.
	int input /* for user input */, y_axis = 10 /* for some curses magic demonstration */;

	initscr(); // init the screen. a must for curses.


	/* change terminal so that characters entered returned 
		are imidietly passed to us, and to not display
		writtin characters */
	cbreak();
	noecho();
	keypad(stdscr,TRUE); // to getch() F1 or similar
	printw("Enter something.. press \"s\" to get a cool slideshow, \"q\" to quit\n");
	while((input = getch()) != 'q') {
		switch(input) {
			case KEY_DOWN:
				mvprintw(y_axis,30,"KEY_DOWN: %d\n",input);
				y_axis++;
				break;
			case 's':
				clear();
				(int *)submenue();
				break;
			default:
				printw("unknown key: %d\n",input);
		}
	}
	
	
	endwin(); // must be called at exit. could be atexit()ed, and MyQuit() should call it.
	return 0;
}
