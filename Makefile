CC=gcc
FLAGS=-O2 -Wall -pedantic
LIB=-lncurses
SRCDIR=./src
INCDIR=./src/include
LIBDIR=./lib
BUILDDIR=./build
OBJF=$(BUILDDIR)/pacman

$(OBJF): $(SRCDIR)/obj/*.o $(SRCDIR)/pacdude.o
	@ echo " ";
	@ echo " >>> Linking a executable <<<"; 
	$(CC) $(LIB) -I$(INCDIR) $(SRCDIR)/obj/*.o $(SRCDIR)/pacdude.o -o $(OBJF) 
	@ echo " "; 
	@ echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	@ echo "Type 'make run' to run the program."; 
	@ echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
	@ echo " ";

$(SRCDIR)/obj/*.o: $(SRCDIR)/obj/*.c
	@ echo " ";
	@ echo " >>> Building objects <<<";
	cd $(SRCDIR)/obj/; \
	make
	@ echo " >>> DONE <<<"

$(SRCDIR)/pacdude.o: $(SRCDIR)/pacdude.c
	@ echo " "
	@ echo " >>> Building final object <<<";
	$(CC) -c $(FLAGS) -I$(INCDIR) $(SRCDIR)/pacdude.c -o $(SRCDIR)/pacdude.o
	@ echo " >>> DONE <<<"

clean:
	@ echo " "
	@ echo " >>> Removing object/build files <<<";
	rm -f  ./$(SRCDIR)/pacdude.o
	rm -f ./$(OBJF); \
	cd $(SRCDIR)/obj/; \
	make clean
	@ echo " >>> DONE <<<";
    
run:
	./$(OBJF) ./maps/PACMAP.TXT
