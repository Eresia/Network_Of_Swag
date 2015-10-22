all: server

server: server.o launch_server.o
	gcc -o server server.o launch_server.o

server.o: server.c server.h
	gcc -o server.o -c server.c

launch_server.o: launch_server.c server.h
	gcc -o launch_server.o -c launch_server.c
