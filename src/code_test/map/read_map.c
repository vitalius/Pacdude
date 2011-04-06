#include "read_map.h"



Map *read_map() {

 Map *loc_map;
 char **loc_p;
 char *pnt;
 FILE *fp;
 int i, j, t;

 
 if ((fp = fopen(MAP_FILE, "r")) == NULL)
  {
	printf("\nerror!\n");
  }

 if ((loc_map = malloc(sizeof(Map))) == NULL)
  {
	printf("\nAllocation error!\n");
  }


 loc_map->map_x = MAX_X;
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


 close(fp);

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


int iswall(Map *lp, int x, int y) {


if ((lp->mapstore[y][x] == 0x20) || (lp->mapstore[y][x] == 0x2e) || (lp->mapstore[y][x] == 0x2d) || (lp->mapstore[y][x] == 0x2a))
	{
		return 0;
	}
else 
	{
		return 1;
	}

}
