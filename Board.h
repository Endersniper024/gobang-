
#pragma once
#include <graphics.h>
#include <vector>
#include <fstream>
#include <iostream>

struct ChessPos {
    int row;
    int col;
    ChessPos(int r = 0, int c = 0) : row(r), col(c) {};
};

typedef enum {
    WHITE_CHESS = -1,//白棋
    BLACK_CHESS = 1,
} chess_kind_t;

class Board {
public:
    Board(int gradeSize, int marginX, int marginY, float chessSize);
    //初始化
    void init(bool resetBoard);
    bool clickBoard(int x, int y, ChessPos* pos);
    int clickMenu(int x, int y);
    void placeStone(ChessPos* pos, chess_kind_t kind);
    void ctrlMenu(int opt);
    int getGradeSize(); //获取棋盘大小 13 15 19；
    int getBoard(ChessPos* pos);
    int getBoard(int row, int col);
    bool checkOver();
    bool playerFlag;
    // 存档与读档方法
    void saveToFile(std::ofstream& outFile) const;
    void loadFromFile(std::ifstream& inFile);
    bool rst;
    bool sv;
    bool ld;
    
private:
    IMAGE blackImg; //黑棋棋子
    IMAGE whiteImg; //白棋棋子
    int window_wid;
    int window_hei;
    
    int gradeSize; //棋盘大小
    int margin_x; //棋盘左侧边界
    int margin_y; //棋盘顶部边界
    float chessSize; //棋子的大小（棋盘小方格的大小）

    //存储当前棋局的棋子分布数据 0：空白 1：黑子 -1：白子
    std::vector<std::vector<int>> boardMap;

    //表示现在该谁下棋
     //true->black false->white

    ChessPos lastPos;

    void updateBoard(ChessPos* pos);
    bool isFive();
    bool isOverLong();
    bool isFourFour();
    bool isThreeThree();
    bool win();
};
