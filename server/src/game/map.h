typedef enum {NONE=0, IRON=41, WOOD=43, DIRT=42, STONE=47} BlockType;
typedef enum {CAVE=40, SKY=44} Background;
typedef enum {false, true} Boolean;

typedef struct {
	BlockType type;
	Background back;
} block;

/*
const block _SKY = {NONE, SKY, false};
const block _CAVE = {NONE, CAVE, false};
const block _DIRT = {DIRT, SKY, true};
const block _WOOD = {WOOD, SKY, true};
const block _STONE = {STONE, CAVE, true};
const block _IRON = {IRON, CAVE, true};
*/

block **getMapFromFile(char* filePath);
void displayMap(block **map);
void displayMapPlayer(block **map, int player[]);

block **removeBlock(block **map, int x, int y);
block **addBlock(block **map, int x, int y, block newBlock);


/*
0:sky
1:dirt
2:stone
3:wood
4:iron
5:cave
*/
