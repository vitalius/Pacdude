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
#include <stdlib.h>
#include <ghosts.h>


int getx(Ghost *g, int dir)
{
    if (GLEFT == dir)
        return g->xpos - 1;
    else if (GRIGHT == dir)
        return g->xpos + 1;
    else 
        return g->xpos;
}


int gety(Ghost *g, int dir) 
{
    if (GUP == dir) 
        return g->ypos - 1;    
    else if (GDOWN == dir) 
        return g->ypos + 1;	         
    else
        return g->ypos;
}


int reverseDir(Ghost *g) 
{
    if (g->dir == GUP) return GDOWN;
    if (g->dir == GDOWN) return GUP;
    if (g->dir == GLEFT) return GRIGHT;
    if (g->dir == GRIGHT) return GLEFT;
    return g->dir;
}


int chooseDir(Ghost *g, int dirs[]) 
{
    int rnd, i, total=0;

    for(i=0;i<NUMDIRS;i++)
        total += dirs[i];
    
    rnd = 1+(int) ( ((float)total) *rand()/(RAND_MAX+1.0) );
    /* rnd = 1 + (rand() % (total - 1)); */ /* a Very-Bad-Way, causes SIGFPE */
    i = 0;
    do { 
        rnd -= dirs[i++]; 
    } while ( rnd>0 );
    
    return i-1;
}


#ifdef DEBUG

void printGhost(Ghost *g) 
{
    printf("Direction:%i X:%i Y:%i Name:%c\n",
	   g->dir, g->xpos, g->ypos, g->name);
}


/*
 * Choice of direction is random. However
 * the probability of going forward is greater
 * than turning (1000 vs 10)
 */
void getDirWeights(Ghost *g, int dirs[])
{
    int i;
    for(i=0; i<NUMDIRS; i++)
	dirs[i] = 10;
    
    dirs[g->dir] = 100;
    return;
}


int main(void) 
{
    Ghost b = { 1, 10,10, 'B' };
    int i, dirs[NUMDIRS];
    
    for(i=0; i<=100; i++) {
        printGhost(&b);
        getDirWeights(&b, dirs);
        b.dir = chooseDir(&b, dirs);
    }
    
    return 0;
}
#endif
