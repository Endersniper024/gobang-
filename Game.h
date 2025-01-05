
#pragma once
#include "Player.h"
#include "AI.h"
#include "Board.h"
#include <string>


class Game
{
public:
	Game(Player* player, AI* ai, Board* board);
	// ���ò˵�
	void play(bool resetBoard);

	// �浵����
	void saveGame(const std::string& filename) const;

	// ��������
	bool loadGame(const std::string& filename);
//������ݳ�Ա
private:
	Player* player;
	AI* ai;
	Board* board;
	bool skipAiTurn;
	
};


