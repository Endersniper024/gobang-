
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
    WHITE_CHESS = -1,//����
    BLACK_CHESS = 1,
} chess_kind_t;

class Board {
public:
    Board(int gradeSize, int marginX, int marginY, float chessSize);
    //��ʼ��
    void init(bool resetBoard);
    bool clickBoard(int x, int y, ChessPos* pos);
    int clickMenu(int x, int y);
    void placeStone(ChessPos* pos, chess_kind_t kind);
    void ctrlMenu(int opt);
    int getGradeSize(); //��ȡ���̴�С 13 15 19��
    int getBoard(ChessPos* pos);
    int getBoard(int row, int col);
    bool checkOver();
    bool playerFlag;
    // �浵���������
    void saveToFile(std::ofstream& outFile) const;
    void loadFromFile(std::ifstream& inFile);
    bool rst;
    bool sv;
    bool ld;
    
private:
    IMAGE blackImg; //��������
    IMAGE whiteImg; //��������
    int window_wid;
    int window_hei;
    
    int gradeSize; //���̴�С
    int margin_x; //�������߽�
    int margin_y; //���̶����߽�
    float chessSize; //���ӵĴ�С������С����Ĵ�С��

    //�洢��ǰ��ֵ����ӷֲ����� 0���հ� 1������ -1������
    std::vector<std::vector<int>> boardMap;

    //��ʾ���ڸ�˭����
     //true->black false->white

    ChessPos lastPos;

    void updateBoard(ChessPos* pos);
    bool isFive();
    bool isOverLong();
    bool isFourFour();
    bool isThreeThree();
    bool win();
};
