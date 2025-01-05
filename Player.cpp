
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
		//获取鼠标点击消息
		msg = GetMouseMsg();
		//通过board对象 调用判断落子是否有效 以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN) {//左键单击,且为有效点击
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

// 存档方法
void Player::savePlayerState(std::ofstream& outFile) const {
	// 保存与 Player 相关的状态
	//留下接口进一步优化
	int playerScore = 0; 
	outFile.write(reinterpret_cast<const char*>(&playerScore), sizeof(playerScore));
}

// 读档方法
void Player::loadPlayerState(std::ifstream& inFile) {
	// 读取与 Player 相关的状态
	// 留下接口进一步优化
	int playerScore = 0;
	inFile.read(reinterpret_cast<char*>(&playerScore), sizeof(playerScore));
}
