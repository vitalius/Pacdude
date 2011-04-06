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

#include <map.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Map *readMap( char *filename ) {

 Map *loc_map;
 char **loc_p;
 char *pnt;
 FILE *fp;
 int i, j;

 
 if ((fp = fopen(filename, "r")) == NULL)
  {
	printf("\nfile could not be opened: %s\n",filename);
	exit(2);
  }

 if ((loc_map = malloc(sizeof(Map))) == NULL)
  {
	printf("\nAllocation error!\n");
	exit(2);
  }


 loc_map->map_x = MAX_X +1;
 loc_map->map_y = MAX_Y;


 pnt = malloc(sizeof(char)*MAX_X);
 loc_p = malloc(sizeof(char *)*loc_map->map_y);

  for (i=0; i<loc_map->map_y; i++)
   {

	loc_p[i] = malloc(sizeof(char)*loc_map->map_x);

   }	


	for (i=0; i<loc_map->map_y; i++)
	 {
		
	 fgets(pnt ,loc_map->map_x, fp);
	  	
		
	   for (j=0; j<loc_map->map_x; j++)
	    {	

		loc_p[i][j] = *(pnt + j);

 	    }	

	  fgets(pnt ,loc_map->map_x, fp);
	  	
 	 }

 loc_map->mapstore = loc_p;


 fclose(fp);

 return loc_map;
}


void display_map (Map *lp) {

 int i, j;

	printf("\n\n");	

	for (i=0; i<lp->map_y; i++)
         {
          for (j=0; j<lp->map_x; j++)
           {

                printf("%c", lp->mapstore[i][j]);

           }
	  printf("\n");
         }

	printf("\n\n");


return;
}


int isWall(Map *lp, int x, int y) {


if (lp->mapstore[y][x] == WALL) {
	return 1;
} else {
	return 0;
}

}


int isPill(Map *lp, int x, int y) {

if (lp->mapstore[y][x] == PILL) {
	return 1;
} else {
	return 0;
}

}


int isPowerPill(Map *lp, int x, int y) {

if (lp->mapstore[y][x] == PPILL) {
        return 1;
} else {
        return 0;
}   
    
}


int isLeftTransport(Map *lp, int x, int y) {

if (lp->mapstore[y][x] == LEFTT) {
        return 1;
} else {
        return 0;
}
    
}

int isRightTransport(Map *lp, int x, int y) {
          
if (lp->mapstore[y][x] == RIGHTT) {
        return 1;
} else {        
        return 0;
}           

}


int isGhostGate(Map *lp, int x, int y) {

if (lp->mapstore[y][x] == GGATE) {
        return 1;
} else {
        return 0;
}

}
