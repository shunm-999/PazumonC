#include <stdio.h>

/** プロトタイプ宣言 **/
typedef char String[1024];
/** 列挙型宣言 **/
/** グローバル定数の宣言 **/
char *monsters[] = {"スライム", "ゴブリン", "オオコウモリ", "ウェアウルフ", "ドラゴン", NULL};
/** 構造体型宣言 **/
void goDungeon(char *playerName);
void doBattle(char *monsterName);
/** 関数宣言 **/
int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("プレーヤー名を指定してください\n");
		return 1;
	}

	printf("*** Puzzle & Monsters ***\n");
	goDungeon(argv[1]);
	for (int i = 0; monsters[i]; i++)
	{
		doBattle(monsters[i]);
	}
	printf("*** GAME CLEARED! ***\n");
	return 0;
}

/** ユーティリティ宣言 **/
void goDungeon(char *playerName)
{
	printf("%sはダンジョンに到着した\n", playerName);
}

void doBattle(char *monsterName)
{
	printf("%sが現れた！\n", monsterName);
	printf("%sを倒した！\n", monsterName);
}
