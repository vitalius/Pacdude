#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_FILE "./mapfile"

#define MAX_X 21
#define MAX_Y 8

typedef struct map {

        int map_x;
        int map_y;
        char **mapstore;

}Map;


Map *read_map(void);
void display_map(Map *lp);
int iswall(Map *lp, int x, int y);
