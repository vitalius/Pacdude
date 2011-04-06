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

#include <pacman.h>
#include <stdio.h>


/*
 * Look ahead in the direction of the pacman
 */
int getnextx(Pacman *p) 
{
    if(PLEFT ==  p->dir)
        return p->xpos - 1;
    else if(PRIGHT == p->dir)
        return p->xpos + 1;
    else
        return p->xpos;
}


int getnexty(Pacman *p)
{
    if(PUP == p->dir)
        return p->ypos - 1;
    else if(PDOWN == p->dir)
        return p->ypos + 1;
    else
        return p->ypos;
}


void stopPac(Pacman *p)
{
    p->dir = PSTOP;
    p->dx = 0;
    p->dy = 0;
    return;
}


void movePac(Pacman *p)
{
    p->xpos += p->dx;
    p->ypos += p->dy;
}


#ifdef DEBUG
void printPacdude ( Pacman *p )
{
    printf("DIR:%i X:%i Y:%i Super:%i\n", p->dir, p->xpos, p->ypos, p->super);
    return;
}

int main(void)
{
    Pacman p = { PSTOP, 10, 10, 0 };
    printPacdude(&p);
    
    return 0;
}

#endif
