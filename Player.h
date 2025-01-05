#pragma once
#include "Board.h"
class Player
{
public:
	void init(Board* board);
	void go();
	// ¥Êµµ”Î∂¡µµ∑Ω∑®
	void savePlayerState(std::ofstream& outFile) const;
	void loadPlayerState(std::ifstream& inFile);

private:
	Board *board;

};

