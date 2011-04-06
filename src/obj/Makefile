
CC=gcc

CFLAGS=-O2 -Wall -pedantic

COF=-c 

INCDIR=../include
I = -I

all: map.o ghosts.o video.o pacman.o misc.o

map.o: map.c $(INCDIR)/map.h $(INCDIR)/misc.h
	$(CC) $(COF) $(CFLAGS) $I$(INCDIR) map.c

ghosts.o: ghosts.c $(INCDIR)/ghosts.h
	$(CC) $(COF) $(CFLAGS) $I$(INCDIR) ghosts.c

video.o: video.c $(INCDIR)/video.h
	$(CC) $(COF) $(CFLAGS) $I$(INCDIR) video.c

pacman.o: pacman.c $(INCDIR)/pacman.h
	$(CC) $(COF) $(CFLAGS) $I$(INCDIR) pacman.c

misc.o: misc.c $(INCDIR)/misc.h $(INCDIR)/video.h $(INCDIR)/map.h $(INCDIR)/ghosts.h $(INCDIR)/pacman.h $(INCDIR)/map.h
	$(CC) $(COF) $(CFLAGS) $I$(INCDIR) misc.c

clean:
	rm -rf ./*.o
