#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
enum
{
	MAX_GEMS = 14
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
	int totalHp;
	int totalMaxHp;
} Party;

typedef struct
{
	Party *pParty;
	Monster *mMonster;
	int *gemElements;
} BattleField;

Dungeon pazumonDungeon = {
	.monstersHeadAddress = &monsters[0],
	.monstersCount = sizeof(monsters) / sizeof(Monster *)};

/** 関数宣言 **/
Party organizeParty(char *playerName, Monster **monstersHeadAddress, int monstersCount);
void showParty(Party *playerParty);
void fillGems(BattleField *battleField);
void printMonsterName(Monster *monster);
int goDungeon(char *playerName);
void doBattle(Party *playerParty, Monster *monster);
void onPlayerTurn(BattleField *battleField);
void showBattleField(BattleField *battleField);
void printGems(int *gemElements, int gemCount);
void printGem(int gem);
void doAttack(BattleField *battleField);
void onEnemyTurn(BattleField *battleField);
void doEnemyAttack(BattleField *battleField);

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("プレーヤー名を指定してください\n");
		return 1;
	}

	srand((unsigned)time(NULL));

	char *playerName = argv[1];

	printf("*** Puzzle & Monsters ***\n");

	int defeatedMonsterCount = goDungeon(playerName);

	printf("*** GAME CLEARED! ***\n");
	printf("倒したモンスターの数%d\n", defeatedMonsterCount);
	return 0;
}

/** ユーティリティ宣言 **/

int goDungeon(char *playerName)
{
	Monster *playerMonsters[] = {&suzaku, &seiryu, &byakko, &gennbu};
	Party playerParty = organizeParty(playerName, &playerMonsters[0], sizeof(playerMonsters) / sizeof(Monster *));

	int defeatedMonsterCount = 0;
	printf("%sはダンジョンに到着した\n", playerParty.playerName);
	showParty(&playerParty);
	for (int i = 0; i < pazumonDungeon.monstersCount; i++)
	{
		doBattle(&playerParty, monsters[i]);
		if (playerParty.totalHp > 0)
		{
			defeatedMonsterCount++;
			printf("%sはさらに奥へと進んだ\n", playerParty.playerName);
			printf("                                 \n");
			printf("=================================\n");
		}
		else
		{
			printf("%sはダンジョンから逃げ出した\n", playerParty.playerName);
			printf("                                 \n");
			printf("=================================\n");
			return defeatedMonsterCount;
		}
	}
	printf("%sはダンジョンに制覇した\n", playerParty.playerName);
	return defeatedMonsterCount;
}

Party organizeParty(char *playerName, Monster **monstersHeadAddress, int monstersCount)
{
	int totalHp = 0;
	for (int i = 0; i < monstersCount; i++)
	{
		totalHp += monstersHeadAddress[i]->hp;
	}
	Party party = {
		.playerName = playerName,
		.monstersCount = monstersCount,
		.monstersHeadAddress = monstersHeadAddress,
		.totalHp = totalHp,
		.totalMaxHp = totalHp};
	return party;
}

void doBattle(Party *playerParty, Monster *monster)
{
	int gemElements[14] = {};
	BattleField battleField = {
		.pParty = playerParty,
		.mMonster = monster,
		.gemElements = &gemElements[0]};
	fillGems(&battleField);

	printf("                                 \n");
	printMonsterName(monster);
	printf("が現れた！\n");
	while (playerParty->totalHp > 0 && monster->hp > 0)
	{
		onPlayerTurn(&battleField);
		onEnemyTurn(&battleField);
	}
	printMonsterName(monster);
	printf("を倒した！\n");
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

void fillGems(BattleField *battleField)
{
	for (int i = 0; i < MAX_GEMS; i++)
	{
		int gem = rand() % 5;
		battleField->gemElements[i] = gem;
	}
}

void printMonsterName(Monster *monster)
{
	printf("\x1b[3%dm%c%s%c\x1b[39m", ELEMNET_COLOR[monster->element], ELEMNET_SYMBOLS[monster->element], monster->name, ELEMNET_SYMBOLS[monster->element]);
}

void onPlayerTurn(BattleField *battleField)
{
	printf("【%sのターン】\n", battleField->pParty->playerName);
	showBattleField(battleField);
	doAttack(battleField);
}

void showBattleField(BattleField *battleField)
{
	printf("--------------------------------\n");
	printf("                                \n");
	printf("           ");
	printMonsterName(battleField->mMonster);
	printf("          \n");
	printf("           HP= %d / %d          \n", battleField->mMonster->hp, battleField->mMonster->maxHp);
	for (int i = 0; i < battleField->pParty->monstersCount; i++)
	{
		printMonsterName(battleField->pParty->monstersHeadAddress[i]);
		printf(" ");
	}
	printf("\n");
	printf("           HP= %d / %d          \n", battleField->pParty->totalHp, battleField->pParty->totalMaxHp);
	printf("                                \n");
	printf("                                \n");
	printf("--------------------------------\n");
	printGems(battleField->gemElements, MAX_GEMS);
	printf("--------------------------------\n");
}

void printGems(int *gemElements, int gemCount)
{
	for (int i = 0; i < gemCount; i++)
	{
		printf(" ");
		printf("%c", 65 + i);
	}
	printf("\n");

	for (int i = 0; i < gemCount; i++)
	{
		printf(" ");
		printGem(gemElements[i]);
	}
	printf("\n");
}

void printGem(int gem)
{
	printf("\x1b[4%dm%c\x1b[49m", ELEMNET_COLOR[gem], ELEMNET_SYMBOLS[gem]);
}

void doAttack(BattleField *battleField)
{
	battleField->mMonster->hp -= 80;
	printf("ダミー攻撃で80のダメージを与えた\n");
}

void onEnemyTurn(BattleField *battleField)
{
	printf("【%sのターン】\n", battleField->mMonster->name);
	doEnemyAttack(battleField);
}

void doEnemyAttack(BattleField *battleField)
{
	battleField->pParty->totalHp -= 20;
	printf("20のダメージを受けた\n");
}
