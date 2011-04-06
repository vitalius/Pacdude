/*
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <sys/time.h>

/* local includes */
#include <map.h>
#include <ghosts.h>
#include <video.h>
#include <pacman.h>
#include <misc.h>


#define TIMERDELAY 200

/*
 * When does Pacdewd win? This value should be changeable throught
 * getopt() (which I should implement sooner or later, too ^^), and
 * the map (or rather, write a function that counts PILLs in the map,
 * substract 20 per cent, then merge it with scorelimit or something..
 * I need help over there ;-)
 */
int scorelimit = 80;

Map *m;
Pacman p = { PSTOP, 14,18, 0,0, 0, 3, 0 };  /* Our hero... the Super Beaver! */
Ghost g[4] = { {2, 12,12},                  /* Blinky */
               {2, 12,12},                  /* Pinky */
               {2, 12,12},                  /* Inky */
               {2, 12,12} };                /* Clyde */

void drawScene(int signum);

/*
 * Alarm ticker
 * This is identical to alarm(); routine, except this alows us to set 
 * miliseconds rather than just second. And therefor gives more flexebility
 * in the frames drawn on screen.
 */
int setTicker(int n_msecs) 
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
    
    n_sec = n_msecs / 1000;
    n_usecs = ( n_msecs % 1000 ) * 1000L;
    
    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;
    
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}



/*
 * Wrapper for ignoring SIGALRM or resetting it.
 */
void Pause(void) 
{
    signal(SIGALRM, SIG_IGN);
    return;
}


void continueGame(void) 
{
    drawScene(1);
    return;
}



/*
 * Assign direction weights, calculate the probability
 * and move the ghost to a new spot;
 */
void moveGhost(Ghost *g, Map *m) 
{
    int dirs[NUMDIRS], i;

    /* Assign direction weights */
    for (i=0; i<NUMDIRS; i++)
        if ( !isWall(m, gety(g, i), getx(g, i)) && 
             !isLeftTransport(m, gety(g, i), getx(g, i)) &&
             !isRightTransport(m, gety(g, i), getx(g, i)) )
            dirs[i] = 100;
        else
            dirs[i] = 0;
    
    /* We want to reverse very rarely */  
    if ( dirs[reverseDir(g)] != 0 )
        dirs[reverseDir(g)] = 3;
    
    /* Go straigh more often than turn */
    if ( dirs[g->dir] != 0 )
        dirs[g->dir] = 180;
    
    /* Move ghost in a new direction */
    g->dir = chooseDir(g, dirs);
    g->xpos = getx(g, g->dir);
    g->ypos = gety(g, g->dir);    
    
    return;
}



/*
 * Move pacman on the map, check all boundaries, etc.
 */
void movePacman(Pacman *p, Map *m) 
{
    if(isWall(m, getnextx(p), getnexty(p)))
        stopPac(p);
    else {
        switch(p->dir) {
        case PLEFT:
            p->dx = -1; 
            p->dy = 0;
            break;
        case PRIGHT:
            p->dx = 1;
            p->dy = 0;
            break;
        case PUP:
            p->dx = 0;
            p->dy = -1;
            break;
        case PDOWN:
            p->dx = 0;
            p->dy = 1;
            break;
        default:
            break;
        }
    }
    
    movePac(p);
    return;
}


/*
 * Attempt to change direction of the pacman
 */ 
void tryDir(Pacman *p, int dir, Map *m) 
{
    Pacman tmp;
    tmp.dir = dir;
    tmp.xpos = p->xpos;
    tmp.ypos = p->ypos;
    tmp.dx = p->dx;
    tmp.dy = p->dy;
    tmp.super = 0;
    
    if(isWall(m, getnextx(&tmp), getnexty(&tmp)))
        return;
    else
        p->dir = dir;
    
    return;
}



/*
 *  Reset pacdude and ghosts locations
 */
void resetObjects(Pacman *p, Ghost *g)
{
    int i;
    
    p->dir = PSTOP;
    p->xpos = 14;
    p->ypos = 18;
    p->dx = 0;
    p->dy = 0;
    p->super = 0;
  
    for(i=0;i<4;i++) {
        g[i].dir = 2;
        g[i].xpos = 12;
        g[i].ypos = 12;
    }
    return;
}



/*
 *  Check collision status with ghosts 
 */
inline void checkCollision(Pacman *p, Ghost *ghosts) 
{
    int i;
    for(i=0;i<4;i++) 
        if((p->ypos == ghosts[i].xpos) && (p->xpos == ghosts[i].ypos)) {
            p->lives--;            
            notifyUser("Congradulations, You've been caught!");
    	    if(p->lives <= 0) {
                Pause();
                if(PacExit("ONCE AGAIN?") == 1)
                    PacExit(NULL);
                else {
                    p->lives = 3;
                    p->score = 0;
                    m = readMap(metadata.mapfile);
                    drawMaze(m);
                    resetObjects(p,g);
                    clearScreen();
                }
            }
            resetObjects(p,g);
            clearScreen();
        }

    return;
}



/*
 * Draw our stuff
 */
void drawScene(int signum)
{
    int i;
    signal(SIGALRM, SIG_IGN);

    drawMaze(m);

    for(i=0;i<4;i++)
        moveGhost(&g[i], m);
    checkCollision(&p, g);
    
    movePacman(&p, m);
    drawPacmanAt(p.xpos, p.ypos);
    for(i=0;i<4;i++)
        drawGhostAt(g[i].xpos, g[i].ypos);
    checkCollision(&p, g);
    eatPill(&p,m); 
    drawScore(p.score,p.lives,1);
    
    Refresh();
    if(p.score == scorelimit) {
        Pause();
        notifyUser("Congratulations, you have won!");
        if(PacExit("ONCE AGAIN?") == 1)
            PacExit(NULL);
        else {
            p.lives = 3;
            p.score = 0;
            /* reread map and redraw it, as we manipulated the original
            copy in our stack... */
	        m = readMap(metadata.mapfile);
            drawMaze(m);
            resetObjects(&p,g);
            clearScreen();
            continueGame();
        }
    }

    /* Reset the alarm signal handler to 'self'
       which makes this function Signal-Recursive(tm)(R)(lol) */
    signal(SIGALRM, drawScene);
    setTicker(TIMERDELAY);
}


/*
 * Play a game. Or two, depending on the actions of the user.
 * can also exit the game. 
 */
void play(void) 
{
    int input;
    
 PLAY_AGAIN:
    drawScene(1); /* Init a recursive signal function to draw scene */
    
    while((input = getChar()) != 'q') {
        switch (input) {
        case KEY_LEFT:
            tryDir(&p,PLEFT,m);
            break;
        case KEY_RIGHT:
            tryDir(&p,PRIGHT,m);
            break;
        case KEY_UP:
            tryDir(&p,PUP,m);
            break;
        case KEY_DOWN:
            tryDir(&p,PDOWN,m);
            break;
        default:
            break;
        }
    }
    Pause();
    if(PacExit("EXITREQUEST") == 0) {
        EndWin();
        exit(0);
    } else 
        goto PLAY_AGAIN;
    /* NOTREACHED */
}


void showIntro(void) {
    notifyUser("Beavers use complex routing algorithms to transport w00t efficiently");
    return;
}


int main(int argc , char **argv) 
{
    if(argc < 2) {
        printUsage();
        return 1;
    } else
        parseCMDLineArgs(argc,argv);
    /* signal(SIGINT,PacExitSig); */
    InitWin();
    showIntro();
    play(); 
    /* NOTREACHED */
    
    return 0; /* makes compiler happy */
}
