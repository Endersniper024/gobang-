#pragma once
#include "Board.h"
class Player
{
public:
	void init(Board* board);
	void go();
	// �浵���������
	void savePlayerState(std::ofstream& outFile) const;
	void loadPlayerState(std::ifstream& inFile);

private:
	Board *board;

};

