#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
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
const double ELEMENT_BOOST[4][4] = {{1.0, 0.5, 2.0, 1.0}, {2.0, 1.0, 1.0, 0.5}, {0.5, 1.0, 1.0, 2.0}, {1.0, 2.0, 0.5, 1.0}};
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
	int totalDefence;
} Party;

typedef struct
{
	Party *pParty;
	Monster *mMonster;
	int *gemElements;
} BattleField;

typedef struct
{
	int element;
	int startIndex;
	int count;
} BanishInfo;

typedef struct
{
	BanishInfo *banishInfos;
	int count;
} BanishInfoList;

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
bool checkValidCommand(char *input);
void moveGem(int *gemElements, int start, int end);
void swapGem(int *gemElements, int source, int target);
void evaluateGems(BattleField *battleField);
void checkBanishable(int *gemElements, BanishInfoList *banishInfoList);
void banishGems(BattleField *battleField, BanishInfoList *banishInfoList);
int countGems(int *gemElements, int gem);
void shiftGems(int *gemElements);
void spawnGems(int *gemElements);
int calcAttackDamage(BattleField *battleField, BanishInfoList *banishInfoList, int combo);
void doAttack(BattleField *battleField, int attack);

int doRecover(int currentHp, int banishLifeCount, int combo);

void onEnemyTurn(BattleField *battleField);
void doEnemyAttack(BattleField *battleField, int attack);
int calcEnemyAttackDamage(BattleField *battleField);

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
	int totalDefence = 0;
	for (int i = 0; i < monstersCount; i++)
	{
		totalHp += monstersHeadAddress[i]->hp;
		totalDefence += monstersHeadAddress[i]->defence;
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

	String input = {};
	while (checkValidCommand(input) == false)
	{
		printf("コマンド？＞ \n");
		scanf("%1023s%*[^\n]%*c", input);
	}
	int start = input[0] - 65;
	int end = input[1] - 65;
	moveGem(battleField->gemElements, start, end);
	evaluateGems(battleField);
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

bool checkValidCommand(char *input)
{
	char input1 = input[0];
	char input2 = input[1];

	// 強制終了コマンド
	if (input1 == 'Z' && input2 == 'Z')
	{
		exit(0);
		return false;
	}
	if (input1 < 65 || 78 < input1)
	{
		return false;
	}
	if (input2 < 65 || 78 < input2)
	{
		return false;
	}
	if (input1 == input2)
	{
		return false;
	}
	return true;
}

void moveGem(int *gemElements, int start, int end)
{
	printGems(gemElements, MAX_GEMS);

	if (start == end)
	{
		return;
	}

	int startTmp = start;
	int endTmp = end;

	while (startTmp != endTmp)
	{
		if (startTmp > endTmp)
		{
			swapGem(gemElements, startTmp, startTmp - 1);
			printGems(gemElements, MAX_GEMS);
			startTmp--;
		}
		else
		{
			swapGem(gemElements, startTmp, startTmp + 1);
			printGems(gemElements, MAX_GEMS);
			startTmp++;
		}
	}
}

void swapGem(int *gemElements, int source, int target)
{
	int s = gemElements[source];
	int t = gemElements[target];

	gemElements[source] = t;
	gemElements[target] = s;
}

void evaluateGems(BattleField *battleField)
{

	int combo = 1;

	while (true)
	{
		BanishInfo banishInfos[4] = {};
		BanishInfoList banishInfoList = {
			.banishInfos = &banishInfos[0],
			.count = 0};

		// 宝石の消滅判定
		checkBanishable(battleField->gemElements, &banishInfoList);

		// 宝石の消滅
		if (banishInfoList.count < 1)
		{
			return;
		}
		banishGems(battleField, &banishInfoList);

		// 攻撃処理
		int attack = calcAttackDamage(battleField, &banishInfoList, combo);
		doAttack(battleField, attack);

		// HP回復処理
		for (int i = 0; i < banishInfoList.count; i++)
		{
			BanishInfo banishInfo = banishInfos[i];
			if (banishInfo.element != LIFE)
			{
				continue;
			}

			int recoveredHp = doRecover(battleField->pParty->totalHp, banishInfo.count, combo);

			if (recoveredHp > battleField->pParty->totalHp)
			{
				battleField->pParty->totalHp = battleField->pParty->totalMaxHp;
			}
			else
			{
				battleField->pParty->totalHp = recoveredHp;
			}
		}

		// 宝石補充処理
		if (countGems(battleField->gemElements, EMPTY) > 0)
		{
			shiftGems(battleField->gemElements);
			spawnGems(battleField->gemElements);
		}
		combo++;
	}
}

void checkBanishable(int *gemElements, BanishInfoList *banishInfoList)
{
	int elementTmp = -1;
	int loopingCount = 0;

	for (int i = 0; i < MAX_GEMS; i++)
	{
		int iElement = gemElements[i];

		if (elementTmp == iElement)
		{
			// 連続
			loopingCount++;
		}
		else
		{
			// 不連続
			if (loopingCount >= 3)
			{
				BanishInfo banishInfo = {
					.element = elementTmp,
					.startIndex = i - loopingCount,
					.count = loopingCount};
				banishInfoList->banishInfos[banishInfoList->count] = banishInfo;
				banishInfoList->count++;
			}
			loopingCount = 1;
		}

		elementTmp = iElement;
	}

	if (loopingCount >= 3)
	{
		BanishInfo banishInfo = {
			.element = elementTmp,
			.startIndex = 13 - (loopingCount - 1),
			.count = loopingCount};
		banishInfoList->banishInfos[banishInfoList->count] = banishInfo;
		banishInfoList->count++;
	}
}

void banishGems(BattleField *battleField, BanishInfoList *banishInfoList)
{

	BanishInfo *banishInfos = banishInfoList->banishInfos;

	for (int i = 0; i < banishInfoList->count; i++)
	{
		BanishInfo banishInfo = banishInfos[i];

		for (int m = banishInfo.startIndex; m < banishInfo.startIndex + banishInfo.count; m++)
		{
			battleField->gemElements[m] = EMPTY;
		}
	}
	printGems(battleField->gemElements, MAX_GEMS);
}

int countGems(int *gemElements, int gem)
{
	int count = 0;
	for (int i = 0; i < MAX_GEMS; i++)
	{
		if (gemElements[i] == gem)
		{
			count++;
		}
	}
	return count;
}

void shiftGems(int *gemElements)
{
	int gemElementsTmp[14] = {};
	int count = 0;
	for (int i = 0; i < MAX_GEMS; i++)
	{
		gemElementsTmp[i] = EMPTY;
	}
	for (int i = 0; i < MAX_GEMS; i++)
	{
		int gem = gemElements[i];
		if (gem != EMPTY)
		{
			gemElementsTmp[count] = gem;
			count++;
		}
	}
	for (int i = 0; i < MAX_GEMS; i++)
	{
		gemElements[i] = gemElementsTmp[i];
	}
	printGems(gemElements, MAX_GEMS);
}

void spawnGems(int *gemElements)
{
	for (int i = 0; i < MAX_GEMS; i++)
	{
		int gem = gemElements[i];
		if (gem == EMPTY)
		{
			gemElements[i] = rand() % 5;
		}
	}
}

void doAttack(BattleField *battleField, int attack)
{
	battleField->mMonster->hp -= attack;
	printf("ダミー攻撃で%dのダメージを与えた\n", attack);
}

int calcAttackDamage(BattleField *battleField, BanishInfoList *banishInfoList, int combo)
{
	double percent = ((rand() % 3) + 9) / 10;
	int attack = 0;
	Party party = *(battleField->pParty);

	for (int i = 0; i < banishInfoList->count; i++)
	{
		BanishInfo banishInfo = banishInfoList->banishInfos[i];

		for (int m = 0; m < battleField->pParty->monstersCount; m++)
		{
			Monster *partyMonster = battleField->pParty->monstersHeadAddress[m];
			if (partyMonster->element == banishInfo.element)
			{
				// 宝石と属性が一致しているモンスターが攻撃する
				double elementCorrection = ELEMENT_BOOST[partyMonster->element][battleField->mMonster->element];
				int partyMonsterAttack = (partyMonster->attack - battleField->mMonster->defence) * elementCorrection * pow(1.5, (banishInfo.count - 3 + combo)) * percent;
				if (partyMonsterAttack > 0)
				{
					attack += partyMonsterAttack;
				}
				else
				{
					attack += 1;
				}
			}
		}
	}

	if (attack < 1)
	{
		attack = 1;
	}
	return attack;
}

int doRecover(int currentHp, int banishLifeCount, int combo)
{
	double percent = ((rand() % 3) + 9) / 10;
	int recoverHp = 20 * pow(1.5, (banishLifeCount - 3 + combo)) * percent;
	return currentHp + recoverHp;
}

void onEnemyTurn(BattleField *battleField)
{
	printf("【%sのターン】\n", battleField->mMonster->name);
	int attack = calcEnemyAttackDamage(battleField);
	doEnemyAttack(battleField, attack);
}

void doEnemyAttack(BattleField *battleField, int attack)
{
	battleField->pParty->totalHp -= attack;
	printf("%dのダメージを受けた\n", attack);
}

int calcEnemyAttackDamage(BattleField *battleField)
{

	double percent = ((rand() % 3) + 9) / 10;

	int attack = (battleField->mMonster->attack - battleField->pParty->totalDefence);

	if (attack < 1)
	{
		attack = 1;
	}
	return attack;
}