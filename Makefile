all: launch_server.out launch_client.out

launch_server.out: server/server.o server/launch_server.o
	gcc -o launch_server.out server/server.o server/launch_server.o -lpthread

server.o: server/server.c server/server.h
	gcc -o server/server.o -c server/server.c

launch_server.o: server/launch_server.c server/server.h
	gcc -o server/launch_server.o -c server/launch_server.c

launch_client.out: client/client.o client/launch_client.o
	gcc -o launch_client.out client/client.o client/launch_client.o

client.o: client/client.h client/client.c
	gcc -o client/client.o -c client/client.c

launch_client.o: client/client.h client/launch_client.c
	gcc -o client/launch_client.o -c client/launch_client.c

clean_all: clean
	rm *.out;

clean:
	rm client/*.o; rm server/*.o;
