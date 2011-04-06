#include "read_map.h"


int main(int argc, char *argv[]) {
 
 
 Map *main_p;
 int i, j;
   
 main_p = read_map();
 display_map(main_p);

 if ((j = iswall(main_p, 5, 4)) == 1)
	{
		printf("\n\nIs a wall\n");
	}
 else
	{
		printf("\nThis is not a wall\n");
	}
        

        
 return 0;
}
