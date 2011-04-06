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
#include <unistd.h>
#include <ctype.h>
#include <video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>


/* GUI includes */
#include <curses.h>

/* local includes */
#include <misc.h>
#include <map.h>
#include <pacman.h>

/*
 * Ask the user a yes/no question.
 * In curses, there are no buttons or alike. 
 */
int askUser (char *msg) 
{
    char ch;
    showString(msg);
    Refresh();
    ch = getChar();
    continueGame();
    clearScreen();
    return ( ch =='Y' || ch == 'y' ) ? 0 : 1;
}

/*
 * Just notify the user about an event.
 * GUIs shall have an "ok"-button, which just let the
 * thing return;
 */
void notifyUser(char *msg) {
    showString(msg);
    Refresh();
    getChar();
    continueGame();
    clearScreen();
}


/*
 * Exit on various signals, we should e.g. register
 * SIGINT to use this function. Currently not used.
 */
void PacExitSig(int signum) 
{
    PacExit("EXITREQUEST");
}

/*
 * This is the wrapper-function we should call when we
 * want to exit, or if the user has to decide if he
 * wants to. The keywords (char *error) shall stay upper-
 * case for consistency reasons.
 */
int PacExit(char *error) 
{
    if(error == NULL) { /* no reason given. This leads to a normal exit */
        EndWin();
        exit(0);
    }
    if(strcmp(error,"EXITREQUEST") == 0) 
        return askUser("Do you really want to quit? [y]es/[n]o");
    else if(strcmp(error,"ONCE AGAIN?") == 0)
        return askUser("Do you want to play again? [y]es/[n]o");
    else {
        EndWin();
        fprintf(stderr,"pacman: internal error in PacExit() where error == %s\n",error);
        exit(-1);
    }
}

/*
 * This function let our hero eat pills and powerpills (or not, if there
 * is nothing), and relies on two functions from map.c (is{Power,}Pill())
 * TODO: Does this belong here, or into pacdude.c? Or into map.c?
 */
void eatPill(Pacman *p, Map *m) 
{
    if(isPill(m,p->xpos,p->ypos)) {
        p->score++;
        m->mapstore[p->ypos][p->xpos] = SPACE;
        return;
    } else if(isPowerPill(m,p->xpos,p->ypos)) {
	    /* MEGHA-TODO: 
		20:27:06 < Jason-Bourne> na man he needs to move a tile faster
		20:27:11 < Jason-Bourne> like he's on steroids
		20:27:28 < Jason-Bourne> so he can pump dem ghosts full of lead
	    */
        return;
    } else 
        return;
}

/*
 * Print a usage overview to stderr if the application was called
 * without arguments. Not very cool for GUIs, tho, think about that.
 */
void printUsage(void) 
{
    fprintf(stderr, "Pacman - Yet another clone, but this time the code looks schweet\n");
    fprintf(stderr, "Usage: pacman <mapfile> [<opt1> <optN>]\n");
    fprintf(stderr, "Options\n-.-.-.-.-.-.-.-.-.-\n");
    fprintf(stderr, "-s <scorelimit>\t\t-- how much pills do you have to eat?\n");
    /* ... */ 
}

/*
 * parse the cmd-line arguments.
 * POSIX: getopt(3)
 */
void parseCMDLineArgs(int argc, char **argv)
{
    int opt;
    strncpy(metadata.mapfile,argv[1],STD_FIELDSIZE);
    while((opt = getopt(argc, argv, "s:")) != EOF) {
        switch(opt) {
            case 's':
                scorelimit = atoi(optarg);
                if(scorelimit == 0) {
                    scorelimit = 80;
                    fprintf(stderr, "WARNING: scorelimit reset due to atoi() == 0\n");
                }
                break;
            default:
                printUsage();
                exit(1);
        }
    }
    m = readMap(metadata.mapfile);
}
