typedef enum {AIR, DIRT, STONE, WOOD, IRON} blocType;
typedef enum {BLUE=44, GREEN=42, GRAY=47, BROWN=43, ORANGE=41} Color;
typedef enum {false, true} Boolean;

typedef struct {
	blocType type;
	Color col;
	Boolean solid;
} bloc;

/*
0:air
1:dirt
2:stone
3:wood
4:iron
*/
