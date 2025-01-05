
#include "AI.h"
#include <algorithm>
#include <random>
#define INF 1000000

void AI::init(Board* board, bool resetScore)
{
	this->board = board;
	int gradeSize = board->getGradeSize();
	if (resetScore) {
		for (int i = 0; i < gradeSize; i++) {
			std::vector<int> row;
			for (int j = 0; j < gradeSize; j++) {
				row.push_back(0);
			}
			score.push_back(row);
		}
	}
}

void AI::go()
{
	ChessPos pos=tk();
	Sleep(700);
	board->placeStone(&pos, WHITE_CHESS);
}

// 存档方法
void AI::saveGameState(std::ofstream& outFile) const {
	int gradeSize = board->getGradeSize();
	for (const auto& row : score) {
		outFile.write(reinterpret_cast<const char*>(row.data()), gradeSize * sizeof(int));
	}
}

// 读档方法
void AI::loadGameState(std::ifstream& inFile) {
	int gradeSize = board->getGradeSize();
	score.resize(gradeSize, std::vector<int>(gradeSize));
	for (auto& row : score) {
		inFile.read(reinterpret_cast<char*>(row.data()), gradeSize * sizeof(int));
	}
}

void AI::evaluate()
{	
	int playerNum = 0;
	int aiNum = 0;
	int emptyNum = 0;

	int size = board->getGradeSize();
	//clear
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			score[i][j] = 0;
		}
	}

	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
		{
			// 空白点就算
			if (row >= 0 && col >= 0 && board->getChessData(row,col) == 0)
			{
				// 遍历周围四个方向
				int directs[4][2] = {
					{1, 0},
					{1, 1},
					{0, 1},
					{-1, 1}
				};
				for (int k = 0; k < 4; k++) {
					int x = directs[k][0];
					int y = directs[k][1];

					// 重置
					playerNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// 对玩家的评分（正反两个方向）
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y,col + i * x) == 1) // 玩家棋子
						{
							playerNum++;
						}
						else if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == 0) // 空白位
						{
							emptyNum++;
							break;
						}
						else  // 对方棋子或出边界
							break;
					}

					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 1) // 玩家棋子
						{
							playerNum++;
						}
						else if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 0) // 空白位
						{
							emptyNum++;
							break;
						}
						else  // 对方棋子或出边界
							break;
					}

					// 玩家评分
					if (playerNum == 1)                      // 杀二
						score[row][col] += 10;
					else if (playerNum == 2)                 // 杀三
					{
						if (emptyNum == 1)
							score[row][col] += 30;
						else if (emptyNum == 2)
							score[row][col] += 40;
					}
					else if (playerNum == 3)                 // 杀四
					{
						if (emptyNum == 1)
							score[row][col] += 60;
						else if (emptyNum == 2)
							score[row][col] += 200;
					}
					else if (playerNum == 4)                 // 杀五
						score[row][col] += 20000;

					// 进行一次清空
					emptyNum = 0;

					// 对AI评分
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == -1) // AI棋子
						{
							aiNum++;
						}
						else if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == 0) // 空白位
						{
							emptyNum++;
							break;
						}
						else  // 玩家棋子或出边界
							break;
					}

					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == -1) // AI棋子
						{
							aiNum++;
						}
						else if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 0) // 空白位
						{
							emptyNum++;
							break;
						}
						else  // 玩家棋子或出边界
							break;
					}

					// AI评分
					if (aiNum == 0)                        // 普通下子
						score[row][col] += 5;
					else if (aiNum == 1)                   // 活二
						score[row][col] += 10;
					else if (aiNum == 2)
					{
						if (emptyNum == 1)                // 死三
							score[row][col] += 25;
						else if (emptyNum == 2)
							score[row][col] += 50;  // 活三
					}
					else if (aiNum == 3)
					{
						if (emptyNum == 1)                // 死四
							score[row][col] += 55;
						else if (emptyNum == 2)
							score[row][col] += 300; // 活四
					}
					else if (aiNum >= 4)
						score[row][col] += 30000;   // 活五，应该具有最高优先级
				}
			}
		}
	
}




ChessPos AI::tk()
{
	evaluate();
	int maxScore = -1;
	std::vector<ChessPos> maxPoints;
	maxPoints.push_back(-1);
	int size = board->getGradeSize();

	// 寻找最大分数并存储相关位置
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (board->getChessData(row, col) == 0) {
				int currentScore = score[row][col];
				if (currentScore > maxScore) {
					// 发现更高的分数，更新最大分数并清空位置列表
					maxScore = currentScore;
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if (currentScore == maxScore) {
					// 分数相同，添加到位置列表
					maxPoints.push_back(ChessPos(row, col));
				}
			}
		}
	}

	// 使用C++11的随机数生成器，更加高效和准确
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, maxPoints.size() - 1);

	// 随机选择一个位置
	return maxPoints[dis(gen)];

}