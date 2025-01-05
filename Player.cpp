
#include "Player.h"

void Player::init(Board* board)
{
	this->board = board;
	

}

void Player::go()
{
	MOUSEMSG msg;
	ChessPos pos;
	
	msg.x, msg.y;


	while (1) {
		//��ȡ�������Ϣ
		msg = GetMouseMsg();
		//ͨ��board���� �����ж������Ƿ���Ч �Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN) {//�������,��Ϊ��Ч���
			if (msg.x <= 532 && board->clickBoard(msg.x, msg.y, &pos)) {
				board->placeStone(&pos, BLACK_CHESS);
				break;
			}
			if (msg.x > 532 && board->clickMenu(msg.x, msg.y)) {
				board->ctrlMenu(board->clickMenu(msg.x, msg.y));
				break;
			}
		}

	}

	//printf("%d %d\n", pos.row, pos.col);


}

// �浵����
void Player::savePlayerState(std::ofstream& outFile) const {
	// ������ Player ��ص�״̬
	//���½ӿڽ�һ���Ż�
	int playerScore = 0; 
	outFile.write(reinterpret_cast<const char*>(&playerScore), sizeof(playerScore));
}

// ��������
void Player::loadPlayerState(std::ifstream& inFile) {
	// ��ȡ�� Player ��ص�״̬
	// ���½ӿڽ�һ���Ż�
	int playerScore = 0;
	inFile.read(reinterpret_cast<char*>(&playerScore), sizeof(playerScore));
}
