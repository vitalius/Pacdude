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


#ifndef _MAP_H
#define _MAP_H

#define MAX_X 28
#define MAX_Y 24


/*
 * hex codes for objects in the mapfile
 */

#define SPACE 	0x20	/** space char ' ' **/
#define PILL	0x2e	/** pill '.' **/
#define DASH	0x2d	/** dunno '-' **/
#define PPILL	0x2a	/** PowerPill '*'  **/ 
#define LEFTT	0x3c	/** Left transport '<'  **/
#define RIGHTT	0x3e	/** Right transport '>'  **/
#define GGATE	0x5e	/** Ghost gate '^' **/

#define WALL	0x40	/** wall '@' **/


typedef struct map {

    int map_x;
    int map_y;
    char **mapstore;

} Map;


typedef struct portal {

    int portal_id;
    int portal_xpos;
    int portal_ypos;

} Portal;



Map *readMap(char*);
void displayMap(Map *lp);
int isWall(Map *lp, int x, int y);
int isPill(Map *lp, int x, int y);
int isPowerPill(Map *lp, int x, int y);
int isLeftTransport(Map *lp, int x, int y);
int isRightTransport(Map *lp, int x, int y);
int isGhostGate(Map *lp, int x, int y);
int numOfPortals(Map *lp);
Portal **mapPortalPos(Map *lp, int portal_num);


#endif
