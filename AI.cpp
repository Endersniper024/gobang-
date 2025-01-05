
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

// �浵����
void AI::saveGameState(std::ofstream& outFile) const {
	int gradeSize = board->getGradeSize();
	for (const auto& row : score) {
		outFile.write(reinterpret_cast<const char*>(row.data()), gradeSize * sizeof(int));
	}
}

// ��������
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
			// �հ׵����
			if (row >= 0 && col >= 0 && board->getChessData(row,col) == 0)
			{
				// ������Χ�ĸ�����
				int directs[4][2] = {
					{1, 0},
					{1, 1},
					{0, 1},
					{-1, 1}
				};
				for (int k = 0; k < 4; k++) {
					int x = directs[k][0];
					int y = directs[k][1];

					// ����
					playerNum = 0;
					aiNum = 0;
					emptyNum = 0;

					// ����ҵ����֣�������������
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y,col + i * x) == 1) // �������
						{
							playerNum++;
						}
						else if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == 0) // �հ�λ
						{
							emptyNum++;
							break;
						}
						else  // �Է����ӻ���߽�
							break;
					}

					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 1) // �������
						{
							playerNum++;
						}
						else if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 0) // �հ�λ
						{
							emptyNum++;
							break;
						}
						else  // �Է����ӻ���߽�
							break;
					}

					// �������
					if (playerNum == 1)                      // ɱ��
						score[row][col] += 10;
					else if (playerNum == 2)                 // ɱ��
					{
						if (emptyNum == 1)
							score[row][col] += 30;
						else if (emptyNum == 2)
							score[row][col] += 40;
					}
					else if (playerNum == 3)                 // ɱ��
					{
						if (emptyNum == 1)
							score[row][col] += 60;
						else if (emptyNum == 2)
							score[row][col] += 200;
					}
					else if (playerNum == 4)                 // ɱ��
						score[row][col] += 20000;

					// ����һ�����
					emptyNum = 0;

					// ��AI����
					for (int i = 1; i <= 4; i++) {
						if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == -1) // AI����
						{
							aiNum++;
						}
						else if (row + i * y >= 0 && row + i * y < size &&
							col + i * x >= 0 && col + i * x < size &&
							board->getChessData(row + i * y, col + i * x) == 0) // �հ�λ
						{
							emptyNum++;
							break;
						}
						else  // ������ӻ���߽�
							break;
					}

					for (int i = 1; i <= 4; i++) {
						if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == -1) // AI����
						{
							aiNum++;
						}
						else if (row - i * y >= 0 && row - i * y < size &&
							col - i * x >= 0 && col - i * x < size &&
							board->getChessData(row - i * y, col - i * x) == 0) // �հ�λ
						{
							emptyNum++;
							break;
						}
						else  // ������ӻ���߽�
							break;
					}

					// AI����
					if (aiNum == 0)                        // ��ͨ����
						score[row][col] += 5;
					else if (aiNum == 1)                   // ���
						score[row][col] += 10;
					else if (aiNum == 2)
					{
						if (emptyNum == 1)                // ����
							score[row][col] += 25;
						else if (emptyNum == 2)
							score[row][col] += 50;  // ����
					}
					else if (aiNum == 3)
					{
						if (emptyNum == 1)                // ����
							score[row][col] += 55;
						else if (emptyNum == 2)
							score[row][col] += 300; // ����
					}
					else if (aiNum >= 4)
						score[row][col] += 30000;   // ���壬Ӧ�þ���������ȼ�
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

	// Ѱ�����������洢���λ��
	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {
			if (board->getChessData(row, col) == 0) {
				int currentScore = score[row][col];
				if (currentScore > maxScore) {
					// ���ָ��ߵķ��������������������λ���б�
					maxScore = currentScore;
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if (currentScore == maxScore) {
					// ������ͬ����ӵ�λ���б�
					maxPoints.push_back(ChessPos(row, col));
				}
			}
		}
	}

	// ʹ��C++11������������������Ӹ�Ч��׼ȷ
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, maxPoints.size() - 1);

	// ���ѡ��һ��λ��
	return maxPoints[dis(gen)];

}