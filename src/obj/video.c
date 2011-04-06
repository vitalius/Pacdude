/*-
 * Copyright (c) 2005
 *      Rupert-Giles <neil@neilmathers.co.uk>. All rights reserved.
 *      lantis <sretzki@gmx.de>. All rights reserved.
 *      vitalius <citruz@gmail.com>.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY  
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL  
 * ANY OF THE AUTHORS OR THE VOICES IN THEIR HEADS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES  
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <map.h>
#include <video.h>
#include <signal.h>
#include <string.h>


char **videoBuffer;

void funCount (void) {
    static int count;
    char str[255];    
    count++;
    sprintf(str,"%i", count);
    mvaddstr(LINES-1, COLS-5, str);
    return;
}


/*
 *  Initialize the screen
 */
void InitWin(void) 
{
    int i;
    initscr();
    start_color(); /* enable color */
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    /* Create color pairs - args: COLOR_NUMBER; TEXT COLOR; BACKGROUND; */
    init_pair(1, COLOR_BLUE, COLOR_BLACK);   /* Maze */
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); /* Pills and PacDude */

    init_pair(5, COLOR_YELLOW, COLOR_BLUE);  /* Msg Window */
    init_pair(6, COLOR_WHITE, COLOR_BLUE);
    
    init_pair(8, COLOR_RED, COLOR_BLACK);    /* Blinky */
    init_pair(9, COLOR_MAGENTA, COLOR_BLACK);/* Pinky */
    init_pair(10, COLOR_CYAN, COLOR_BLACK);  /* Inky */
    init_pair(11, COLOR_GREEN, COLOR_BLACK); /* Clyde */
    
    
    if((videoBuffer = malloc(sizeof(char*)*LINES)) == NULL)
        exit(3);
    
    for(i=0;i<LINES;i++)
        if((videoBuffer[i] = malloc(sizeof(char)*COLS)) == NULL)
            exit(3);
        else {
            int j;
            for(j=0;j<COLS;j++)
                videoBuffer[i][j] = 'X';
	}
    
    clearScreen();
    
    return;
}



inline void putChar(int x, int y, char ch) 
{
    if(videoBuffer[x][y] != ch) {
        videoBuffer[x][y] = ch; 
        mvaddch(x, y, ch);
    }
    return;
}



int getChar(void) 
{  
    return getch();
}



void EndWin(void) 
{
    endwin();
    return;
}


inline void Refresh(void) 
{
    mvaddch(LINES-1, COLS-1, ' ');
    /* flushinp(); */
    refresh();
    return;
}



/*
 *  Display a string.
 *    Make it bold and centered and put it in a box.
 */
void showString(char *str) 
{	
    /* Calculate box size */
    int x1 = (COLS-strlen(str))/2 - 1;
    int x2 = x1 + strlen(str) + 2;
    int y1 = LINES/2-2;
    int y2 = y1+3;
    WINDOW *msgwindow = newwin(y2-y1, x2-x1, y1,x1);

    flushinp();
    wattron(msgwindow, A_BOLD);

    /* Border */
    wattron(msgwindow, COLOR_PAIR(5));
    wborder(msgwindow, 0, 0, 0, 0, 0, 0, 0, 0);
    wattroff(msgwindow, COLOR_PAIR(5));

    /* Msg string */
    wattron(msgwindow, COLOR_PAIR(6));
    mvwaddstr(msgwindow, 1,1, str);
    wattron(msgwindow, COLOR_PAIR(6));

    wattroff(msgwindow, A_BOLD);
    
    wmove(msgwindow, y2+1, x2+1);
    wrefresh(msgwindow);
    delwin(msgwindow);

    return;
}



void clearScreen(void) 
{
    int i,j;
    clear();
    refresh();
    for(i=0;i<LINES;i++)
    	for(j=0;j<COLS;j++)
            videoBuffer[i][j] = ' ';
    
    return;
}


/*
 *  Plot the maze on the screen
 */
void drawMaze ( Map *m ) 
{
    int i, j;
    
    for(i=0; i<m->map_y; i++)
        for(j=0; j<m->map_x-1; j++)/* -1 is a tmp hack, Rupert fix j00r code */
            if(isWall(m, j,i)) {
                attron(COLOR_PAIR(1));
                putChar(i,j,'@');
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(4));
                attron(A_BOLD);
                
                if(isPill(m, j,i))
                    putChar(i,j,'.');
                else if(isPowerPill(m, j,i))
                    putChar(i,j,'*');
                else if(isGhostGate(m, j,i))
                    putChar(i,j,'^');
                else if(isLeftTransport(m, j,i))
                    putChar(i,j,'<');
                else if(isRightTransport(m, j,i))
                    putChar(i,j,'>');
                else 
                    putChar(i,j,' ');
                
                attroff(COLOR_PAIR(4));
                attroff(A_BOLD);
            }
    
    return;
}



/*
 *  Draw ghost, every 4 sequntial calls draws 4 diff ghosts
 */
void drawGhostAt(int y, int x) 
{
    static int i = 0;
    char sprites[] = { 'B', 'P', 'I', 'C' };
    
    /* Traverse through sprites[] with static array reference 'i' */
    if( i > 3 )
        i = 0;  
    
    attron(COLOR_PAIR(8+i));  
    attron(A_BOLD);
    putChar(y, x, sprites[i]);
    attroff(COLOR_PAIR(8+i));
    attroff(A_BOLD);
    
    i++;
    
    return;
}



/*
 *  Draw Pacman on the screen
 */
void drawPacmanAt(int x, int y) 
{
    attron(COLOR_PAIR(4));	
    attron(A_BOLD);
    putChar(y, x, 'O');
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    return;
}


/*
 *  Dispaly scoring statistics
 */
void drawScore(int score, int lives, int level) 
{
    char str[BUFSIZ];
    int i;
    
    sprintf(str, "LIVES ");
    mvaddstr(2, 40, str);
    for(i=0;i<lives;i++)
        drawPacmanAt(40+strlen(str)+i, 2);
    
    sprintf(str, "SCORE %i", score);
    mvaddstr(3, 40, str);
    sprintf(str, "LEVEL %i", level);
    mvaddstr(5, 40, str);
    return;
}


#ifdef DEBUG
int main(void) 
{
    int i,j;
    InitWin();

    for(i=0;i<LINES;i++)
    	for(j=0;j<COLS;j++)
            putChar(i,j, 'A');	    
    
    Refresh();
    getch();
    EndWin();
    return 0;
}
#endif
