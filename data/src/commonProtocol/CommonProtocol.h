#ifndef COMMON_PROTOCOL
#define COMMON_PROTOCOL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "../map/Map.h"
#include "../player/Player.h"

#define MARGE 3

#define SIZE_MESSAGE_MAX 10000
#define FORBIDEN_CHAR " \n"

//Bot never used
typedef enum Move Move;
enum Move {BOT=2, LEFT=4, RIGHT=6, TOP=8};

typedef struct FallData FallData;
struct FallData{
	Player* player;
	block** map;
};

bool canGoToBlock(int, int, int, int, block**);
bool canAccesBlock(int, int, int, int, block**, bool);

#endif
