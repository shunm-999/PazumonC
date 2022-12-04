#include <stdio.h>
#include <stdlib.h>

/** プロトタイプ宣言 **/
typedef char String[1024];
/** 列挙型宣言 **/
enum
{
	FIRE,
	WATER,
	WIND,
	EARTH,
	LIFE,
	EMPTY
};
/** グローバル定数の宣言 **/
const char ELEMNET_SYMBOLS[] = {'$', '~', '@', '#', '&', ' '};
const int ELEMNET_COLOR[] = {1, 6, 2, 3, 5, 0};
/** 構造体型宣言 **/
typedef struct
{
	char *name;
	int hp;
	int maxHp;
	int element;
	int attack;
	int defence;

} Monster;

Monster slaim = {
	.name = "スライム",
	.hp = 100,
	.maxHp = 100,
	.element = WATER,
	.attack = 10,
	.defence = 5};

Monster goblin = {
	.name = "ゴブリン",
	.hp = 200,
	.maxHp = 200,
	.element = EARTH,
	.attack = 20,
	.defence = 15};

Monster flyingFox = {
	.name = "オオコウモリ",
	.hp = 300,
	.maxHp = 300,
	.element = WIND,
	.attack = 30,
	.defence = 25};

Monster werewolf = {
	.name = "ウェアウルフ",
	.hp = 400,
	.maxHp = 400,
	.element = WIND,
	.attack = 40,
	.defence = 30};

Monster dragon = {
	.name = "ドラゴン",
	.hp = 800,
	.maxHp = 800,
	.element = FIRE,
	.attack = 50,
	.defence = 40};

Monster *monsters[] = {&slaim, &goblin, &flyingFox, &werewolf, &dragon};

typedef struct
{
	Monster **monstersHeadAddress;
	int monstersCount;
} Dungeon;

Dungeon pazumonDungeon = {
	.monstersHeadAddress = &monsters[0],
	.monstersCount = sizeof(monsters) / sizeof(Monster *)};

/** 関数宣言 **/
void printMonsterName(Monster *monster);
void goDungeon(char *playerName);
void doBattle(Monster *monster);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("プレーヤー名を指定してください\n");
		return 1;
	}

	printf("*** Puzzle & Monsters ***\n");
	goDungeon(argv[1]);
	for (int i = 0; i < pazumonDungeon.monstersCount; i++)
	{
		doBattle(monsters[i]);
	}
	printf("*** GAME CLEARED! ***\n");
	printf("倒したモンスターの数%d\n", pazumonDungeon.monstersCount);
	return 0;
}

/** ユーティリティ宣言 **/
void printMonsterName(Monster *monster)
{
	printf("\x1b[3%dm%c%s%c\x1b[39m", ELEMNET_COLOR[monster->element], ELEMNET_SYMBOLS[monster->element], monster->name, ELEMNET_SYMBOLS[monster->element]);
}

void goDungeon(char *playerName)
{
	printf("%sはダンジョンに到着した\n", playerName);
}

void doBattle(Monster *monster)
{
	printMonsterName(monster);
	printf("が現れた！\n");
	printMonsterName(monster);
	printf("を倒した！\n");
}
