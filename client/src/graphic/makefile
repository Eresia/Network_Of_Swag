EXEC = game

all : $(EXEC)

main.o : $(EXEC).h main.c
	gcc -c -Wall main.c `sdl2-config --cflags --libs`

test.o : test.c
	gcc -c -Wall test.c

$(EXEC).o : $(EXEC).h $(EXEC).c
	gcc -c -Wall $(EXEC).c `sdl2-config --cflags --libs`

$(EXEC) : main.o $(EXEC).o test.o
	gcc main.o $(EXEC).o test.o -Wall -o $(EXEC) `sdl2-config --cflags --libs`

clean :
	rm *.o $(EXEC)
