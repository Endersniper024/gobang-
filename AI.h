
#pragma once
#include "Board.h"


class AI {
public:
    void init(Board* board,bool resetScore);
    void go();

    // ¥Êµµ”Î∂¡µµ∑Ω∑®
    void saveGameState(std::ofstream& outFile) const;
    void loadGameState(std::ifstream& inFile);
private:
    Board* board;
    std::vector<std::vector<int>> score;

    void evaluate();
    ChessPos tk();
};

