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

// === 敵モンスター ===

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

// === 味方モンスター ===

Monster suzaku = {
	.name = "朱雀",
	.hp = 150,
	.maxHp = 150,
	.element = FIRE,
	.attack = 25,
	.defence = 10};

Monster seiryu = {
	.name = "青龍",
	.hp = 150,
	.maxHp = 150,
	.element = WIND,
	.attack = 15,
	.defence = 10};

Monster byakko = {
	.name = "白虎",
	.hp = 150,
	.maxHp = 150,
	.element = EARTH,
	.attack = 20,
	.defence = 5};

Monster gennbu = {
	.name = "玄武",
	.hp = 150,
	.maxHp = 150,
	.element = WATER,
	.attack = 20,
	.defence = 15};

typedef struct
{
	Monster **monstersHeadAddress;
	int monstersCount;
} Dungeon;

typedef struct
{
	char *playerName;
	Monster **monstersHeadAddress;
	int monstersCount;
} Party;

Dungeon pazumonDungeon = {
	.monstersHeadAddress = &monsters[0],
	.monstersCount = sizeof(monsters) / sizeof(Monster *)};

/** 関数宣言 **/
Party organizeParty(char *playerName, Monster **monstersHeadAddress, int monstersCount);
void showParty(Party *playerParty);
void printMonsterName(Monster *monster);
void goDungeon(Party *playerParty);
void doBattle(Party *playerParty, Monster *monster);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("プレーヤー名を指定してください\n");
		return 1;
	}

	char *playerName = argv[1];
	Monster *playerMonsters[] = {&suzaku, &seiryu, &byakko, &gennbu};
	Party playerParty = organizeParty(playerName, &playerMonsters[0], sizeof(playerMonsters) / sizeof(Monster *));

	printf("*** Puzzle & Monsters ***\n");

	goDungeon(&playerParty);

	printf("*** GAME CLEARED! ***\n");
	printf("倒したモンスターの数%d\n", pazumonDungeon.monstersCount);
	return 0;
}

/** ユーティリティ宣言 **/
Party organizeParty(char *playerName, Monster **monstersHeadAddress, int monstersCount)
{
	Party party = {
		.playerName = playerName,
		.monstersHeadAddress = monstersHeadAddress,
		.monstersCount = monstersCount};
	return party;
}

void showParty(Party *playerParty)
{
	printf("<パーティ構成>--------------------\n");
	for (int i = 0; i < playerParty->monstersCount; i++)
	{
		Monster *monster = playerParty->monstersHeadAddress[i];
		printf("\x1b[3%dm%c%s%c\x1b[39m ", ELEMNET_COLOR[monster->element], ELEMNET_SYMBOLS[monster->element], monster->name, ELEMNET_SYMBOLS[monster->element]);
		printf("HP = %d 攻撃 = %d 防御 = %d\n", monster->hp, monster->attack, monster->defence);
	}
	printf("--------------------------------\n");
}

void printMonsterName(Monster *monster)
{
	printf("\x1b[3%dm%c%s%c\x1b[39m", ELEMNET_COLOR[monster->element], ELEMNET_SYMBOLS[monster->element], monster->name, ELEMNET_SYMBOLS[monster->element]);
}

void goDungeon(Party *playerParty)
{
	printf("%sはダンジョンに到着した\n", playerParty->playerName);
	showParty(playerParty);
	for (int i = 0; i < pazumonDungeon.monstersCount; i++)
	{
		doBattle(playerParty, monsters[i]);
	}
	printf("%sはダンジョンに制覇した\n", playerParty->playerName);
}

void doBattle(Party *playerParty, Monster *monster)
{
	printf("                                 \n");
	printMonsterName(monster);
	printf("が現れた！\n");
	printMonsterName(monster);
	printf("を倒した！\n");
	printf("%sはさらに奥へと進んだ\n", playerParty->playerName);
	printf("                                 \n");
	printf("=================================\n");
}
