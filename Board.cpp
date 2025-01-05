#include "Board.h"
#include<conio.h>
#include<graphics.h>
#include<mmsystem.h>
#include<easyx.h>
#pragma comment(lib,"winmm.lib")

void putimagePNG(int x, int y, IMAGE* picture) //x为载入图片的X坐标，y为Y坐标
{
	// 变量初始化
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}
Board::Board(int gradeSize, int marginX, int marginY, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = marginX;
	this->margin_y = marginY;
	this->chessSize = chessSize;
	playerFlag = BLACK_CHESS;

	for (int i = 0; i < gradeSize; i++) {
		std::vector<int> row;
		for (int j = 0; j < gradeSize; j++) {
			row.push_back(0);
		}
		boardMap.push_back(row);
	}

}

void Board::init(bool resetBoard) {
	// 创建游戏窗口
	window_wid = 936;
	window_hei = 536;
	initgraph(window_wid, window_hei);

	// 显示棋盘图片
	loadimage(0, "res/棋盘3 - 副本.jpg");

	// 播放开始提示音
	mciSendString("play res/start.wav", 0, 0, 0);

	loadimage(&blackImg, "res/black.png", chessSize, chessSize, true);
	loadimage(&whiteImg, "res/white.png", chessSize, chessSize, true);

	// 如果需要重置棋盘数据
	if (resetBoard) {
		for (int i = 0; i < gradeSize; i++) {
			for (int j = 0; j < gradeSize; j++) {
				boardMap[i][j] = 0;
			}
		}
		playerFlag = true;
	}
}


bool Board::clickBoard(int x, int y, ChessPos* pos)
{
	int col = (x - margin_x) / chessSize;
	int row = (y - margin_y) / chessSize;
	int leftTopPosx = margin_x + chessSize * col;
	int leftTopPosy = margin_y + chessSize * row;
	int offset = chessSize * 0.4;
	
	
	bool ret = false;
	do {
		//top left
		int len1 = (x - leftTopPosx) * (x - leftTopPosx) + (y - leftTopPosy) * (y - leftTopPosy);
		if (len1 < offset * offset) {
			pos->row = row;
			pos->col = col;
			if (boardMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}
		//top right
		int x2 = leftTopPosx + chessSize;
		int y2 = leftTopPosy;
		len1 = (x - x2) * (x - x2) + (y - y2) * (y - y2);
		if (len1 < offset * offset) {
			pos->row = row;
			pos->col = col+1;
			if (boardMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}

		int x3 = leftTopPosx;
		int y3 = leftTopPosy + chessSize;
		len1 = (x - x3) * (x - x3) + (y - y3) * (y - y3);
		if (len1 < offset * offset) {
			pos->row = row + 1;
			pos->col = col;
			if (boardMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}


		int x4 = leftTopPosx + chessSize;
		int y4 = leftTopPosy + chessSize;
		len1 = (x - x4) * (x - x4) + (y - y4) * (y - y4);
		if (len1 < offset * offset) {
			pos->row = row + 1;
			pos->col = col + 1;
			if (boardMap[pos->row][pos->col] == 0) {
				ret = true;
			}
			break;
		}
		
	} while (0);
	
	return ret;
}

int Board::clickMenu(int x, int y)
{
	int bt1lt[] = { 622,157 }, bt2lt[] = { 622,228 }, bt3lt[] = { 622,295 }, bt4lt[] = {622,364};
	int bt1rd[] = {822,202}, bt2rd[] = {822,272}, bt3rd[] = {822,339}, bt4rd[] = {822,407};
	if(x>=bt1lt[0]&&x<=bt1rd[0]&&y>=bt1lt[1]&&y<=bt1rd[1]){
		return 1;
	}
	if(x>=bt2lt[0]&&x<=bt2rd[0]&&y>=bt2lt[1]&&y<=bt2rd[1]){
		return 2;
	}
	if(x>=bt3lt[0]&&x<=bt3rd[0]&&y>=bt3lt[1]&&y<=bt3rd[1]){
		return 3;
	}
	if(x>=bt4lt[0]&&x<=bt4rd[0]&&y>=bt4lt[1]&&y<=bt4rd[1]){
		return 4;
	}

	return 0;
}


void Board::placeStone(ChessPos* pos, chess_kind_t kind)
{
	mciSendString("play res/down8.wav", 0, 0, 0);

	int x = margin_x + pos->col * chessSize - 0.5 * chessSize;
	int y = margin_y + pos->row * chessSize - 0.5 * chessSize;

	if (kind == WHITE_CHESS) {
		putimagePNG(x, y, &whiteImg);
	}
	else {
		putimagePNG(x, y, &blackImg);
	}

	updateBoard(pos);

}

void Board::ctrlMenu(int opt)
{
	if (opt == 1) {
		rst = true;
	}if (opt == 2) {
		sv = true;
	}if (opt == 3) {
		ld = true;
	}
	if (opt == 4) {
		closegraph();
		exit(0);
	}
}

int Board::getGradeSize()
{
	return gradeSize;
}

int Board::getChessData(ChessPos* pos)
{
	return boardMap[pos->row][pos->col];
}

int Board::getChessData(int row, int col)
{
	return boardMap[row][col];
}

bool Board::checkOver()
{
	if (rst == true) {
		return true;
	}
	if (win()) {
		Sleep(1500);
		if (playerFlag == false) {
			loadimage(0, "res/胜利.jpg", window_wid, window_hei, true);
		}
		else {
			loadimage(0, "res/失败.jpg", window_wid, window_hei, true);
		}
		Sleep(3000);
		return true;
		
	}
	if (isOverLong()||isFourFour()||isThreeThree()) {
		Sleep(1500);
		if (playerFlag == false) {
			loadimage(0, "res/失败.jpg", window_wid, window_hei, true);
		}
		else {
			loadimage(0, "res/胜利.jpg", window_wid, window_hei, true);
		}
		Sleep(3000);
		return true;
	}
	return false;
}
//存档方法
void Board::saveToFile(std::ofstream& outFile) const {
	outFile.write(reinterpret_cast<const char*>(&gradeSize), sizeof(gradeSize));
	outFile.write(reinterpret_cast<const char*>(&margin_x), sizeof(margin_x));
	outFile.write(reinterpret_cast<const char*>(&margin_y), sizeof(margin_y));
	outFile.write(reinterpret_cast<const char*>(&chessSize), sizeof(chessSize));
	outFile.write(reinterpret_cast<const char*>(&playerFlag), sizeof(playerFlag));
	outFile.write(reinterpret_cast<const char*>(&lastPos), sizeof(lastPos));

	for (const auto& row : boardMap) {
		outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
	}
}

// 读档方法
void Board::loadFromFile(std::ifstream& inFile) {
	inFile.read(reinterpret_cast<char*>(&gradeSize), sizeof(gradeSize));
	inFile.read(reinterpret_cast<char*>(&margin_x), sizeof(margin_x));
	inFile.read(reinterpret_cast<char*>(&margin_y), sizeof(margin_y));
	inFile.read(reinterpret_cast<char*>(&chessSize), sizeof(chessSize));
	inFile.read(reinterpret_cast<char*>(&playerFlag), sizeof(playerFlag));
	inFile.read(reinterpret_cast<char*>(&lastPos), sizeof(lastPos));

	boardMap.resize(gradeSize, std::vector<int>(gradeSize));
	for (auto& row : boardMap) {
		inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
	}

	// 重新绘制棋子
	for (int i = 0; i < gradeSize; i++) {
		for (int j = 0; j < gradeSize; j++) {
			if (boardMap[i][j] != 0) {
				int x = margin_x + j * chessSize - 0.5 * chessSize;
				int y = margin_y + i * chessSize - 0.5 * chessSize;
				if (boardMap[i][j] == BLACK_CHESS) {
					putimagePNG(x, y, &blackImg);
				}
				else if (boardMap[i][j] == WHITE_CHESS) {
					putimagePNG(x, y, &whiteImg);
				}
			}
		}
	}
}
void Board::updateBoard(ChessPos* pos)
{
	lastPos = *pos;
	boardMap[pos->row][pos->col] = playerFlag ? BLACK_CHESS : WHITE_CHESS;
	playerFlag = !playerFlag;

}

bool Board::isFive()
{
	int dr[] = { 0,1,1,-1 };
	int dc[] = { 1,0,1,1 };
	int row = lastPos.row;
	int col = lastPos.col;
	int inicolor = boardMap[row][col];
	int size = getGradeSize();
	for (int i = 0; i <= 3; i++) {
		int cnt = 1;
		int nr = row + dr[i], nc = col + dc[i];
		//计算正向连续的棋子数
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr += dr[i];
			nc += dc[i];
		}
		//计算反向连续的棋子数
		nr = row - dr[i], nc = col - dc[i];
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr -= dr[i];
			nc -= dc[i];
		}
		if (cnt == 5) return true;
	}
	return false;
}

bool Board::isOverLong()
{
	int dr[] = { 0,1,1,-1 };
	int dc[] = { 1,0,1,1 };
	int row = lastPos.row;
	int col = lastPos.col;
	int inicolor = BLACK_CHESS;
	int size = getGradeSize();
	for (int i = 0; i <= 3; i++) {
		int cnt = 1;
		int nr = row + dr[i], nc = col + dc[i];
		//计算正向连续的棋子数
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr += dr[i];
			nc += dc[i];
		}
		//计算反向连续的棋子数
		nr = row - dr[i], nc = col - dc[i];
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr -= dr[i];
			nc -= dc[i];
		}
		if (cnt > 5) return true;
	}
	return false;
}

bool Board::isFourFour()
{
	int dr[] = { 0,1,1,-1 };
	int dc[] = { 1,0,1,1 };
	int row = lastPos.row;
	int col = lastPos.col;
	int inicolor = BLACK_CHESS;
	int size = getGradeSize();
	int fcnt = 0;
	for (int i = 0; i <= 3; i++) {
		int cnt = 1;
		int nr = row + dr[i], nc = col + dc[i];
		//计算正向连续的棋子数
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr += dr[i];
			nc += dc[i];
		}
		bool openEnd1 = nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == 0;

		//计算反向连续的棋子数
		nr = row - dr[i], nc = col - dc[i];
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr -= dr[i];
			nc -= dc[i];
		}

		bool openEnd2 = nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == 0;
		if (cnt == 4 && openEnd1 && openEnd2) fcnt++;
	}
	if (fcnt >= 2) return true;
	else return false;
}

bool Board::isThreeThree()
{
	int dr[] = { 0,1,1,-1 };
	int dc[] = { 1,0,1,1 };
	int row = lastPos.row;
	int col = lastPos.col;
	int inicolor = BLACK_CHESS;
	int size = getGradeSize();
	int tcnt = 0;
	for (int i = 0; i <= 3; i++) {
		int cnt = 1;
		int nr = row + dr[i], nc = col + dc[i];
		//计算正向连续的棋子数
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr += dr[i];
			nc += dc[i];
		}
		bool openEnd1 = nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == 0;

		//计算反向连续的棋子数
		nr = row - dr[i], nc = col - dc[i];
		while (nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == inicolor) {
			cnt++;
			nr -= dr[i];
			nc -= dc[i];
		}

		bool openEnd2 = nr >= 0 && nr < size && nc >= 0 && nc < size && boardMap[nr][nc] == 0;
		if (cnt == 3 && openEnd1 && openEnd2) tcnt++;
	}
	if (tcnt >= 2) return true;
	else return false;
}


bool Board::win(){
	if (isFive() && !isOverLong()){
		return true;
	}
	return false;
}

