
#pragma once
#include "Player.h"
#include "AI.h"
#include "Board.h"
#include <string>


class Game
{
public:
	Game(Player* player, AI* ai, Board* board);
	// 设置菜单
	void play(bool resetBoard);

	// 存档方法
	void saveGame(const std::string& filename) const;

	// 读档方法
	bool loadGame(const std::string& filename);
//添加数据成员
private:
	Player* player;
	AI* ai;
	Board* board;
	bool skipAiTurn;
	
};


