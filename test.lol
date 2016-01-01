Gameloop.c: In function ‘launch_gameloop’:
Gameloop.c:10:10: warning: unused variable ‘map’ [-Wunused-variable]
  block **map = getMapFromFile("static.map");
          ^
Server.c: In function ‘getPlayerFromFile’:
Server.c:176:9: warning: unused variable ‘i’ [-Wunused-variable]
     int i=0;
         ^
game.c: In function ‘waitEvent’:
game.c:59:8: warning: unused variable ‘iron’ [-Wunused-variable]
   bloc iron = {IRON, ORANGE, true};
        ^
game.c:57:8: warning: unused variable ‘stone’ [-Wunused-variable]
   bloc stone = {STONE, GRAY, true};
        ^
game.c:56:8: warning: unused variable ‘dirt’ [-Wunused-variable]
   bloc dirt = {DIRT, GREEN, true};
        ^
gcc: error: /home/eresia/Documents/L3/Reseau/Network_Of_Swag/client/bin/main.o: No such file or directory
make: *** [client.out] Error 1
