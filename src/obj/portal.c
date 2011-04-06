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

#include "map.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int numOfPortals(Map *lp) {

int numPortRet;
int i, j;

numPortRet = 0;
 

 for(i=0; i<lp->map_y; i++)
  {
   for(j=0; j<lp->map_x; j++)
    {

	if ((lp->mapstore[i][j] == LEFTT) || (lp->mapstore[i][j] == RIGHTT))
	 {
		numPortRet +=1;
	 }	

   }
  }



return numPortRet;

}


Portal **mapPortalPos(Map *lp, int portal_num) {

Portal **port_map;
int tranv_pm;
int i, j, t; 

 tranv_pm = 0;


 if ((port_map = malloc(sizeof(Portal *)*portal_num)) == NULL)
  {
	printf("Alloc Error..");
  }

 for(t=0; t<portal_num; t++)
  {
 	port_map[t] = malloc(sizeof(Portal));
  }


 for(i=0; i<lp->map_y; i++)
  {
   for(j=0; j<lp->map_x; j++)
    {


  	if ((lp->mapstore[i][j] == LEFTT) || (lp->mapstore[i][j] == RIGHTT))
         {                

		port_map[tranv_pm]->portal_xpos = j;
		port_map[tranv_pm]->portal_ypos = i;

		port_map[tranv_pm]->portal_id = tranv_pm;

		tranv_pm += 1;
		
         }

    }
  }


return port_map;
}
