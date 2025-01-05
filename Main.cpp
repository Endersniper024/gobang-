
/*
	开发日志：
	1.创建项目、导入素材
	2.设计项目的框架（设计类）


*/

#include <iostream>

#include "Player.h"
#include "AI.h"
#include "Game.h"
int main() {
	Player player;
	AI ai;
	Board board(15,20,20,35.5714);//长度为15的棋盘
	Game game(&player, &ai, &board);


	game.play(true);
	return 0;
}


