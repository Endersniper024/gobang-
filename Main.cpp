
/*
	������־��
	1.������Ŀ�������ز�
	2.�����Ŀ�Ŀ�ܣ�����ࣩ


*/

#include <iostream>

#include "Player.h"
#include "AI.h"
#include "Game.h"
int main() {
	Player player;
	AI ai;
	Board board(15,20,20,35.5714);//����Ϊ15������
	Game game(&player, &ai, &board);


	game.play(true);
	return 0;
}


