#include "Tetris.h"
#include <stdlib.h>
#include "Block.h"
#include <time.h>
#include<graphics.h>
#include<easyx.h>
#include<conio.h>
#include<fstream>


#include<mmsystem.h>
#include <iostream>
#pragma comment(lib,"winmm.lib")

#undef UNICODE
#undef _UNICODE

#define MAX_LEVEL 5
#define RECORED_FILE "record.txt"
//const int SPEED_NORMAL = 500;
const int SPEED_NORMAL[MAX_LEVEL] = { 500 ,300,150,100,80 };
const int SPEED_MEDIUM = 200;
const int SPEED_QUICK = 50;



Tetris::Tetris(int row, int col, int left, int top, int blockSize)
{
	this->row = row;
	this->col = col;
	this->leftMargin = left;
	this->topMargin = top;
	this->blockSize = blockSize;
	
	for (int i = 0; i < row; i++) {
		vector <int> mapRow;
		for (int j = 0; j < col; j++) {
			mapRow.push_back(0);
		}
		map.push_back(mapRow);
	}
}




void Tetris::init()
{
	ifUpdateWindow = 1;
	delay = SPEED_NORMAL[0];
	srand(time(NULL));

	initgraph(704,631);
	
	loadimage(&backgroundImage,"./res/bg2.png");

	char data[20][10];
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			map[i][j] = 0;
		}
	}
	score = 0;
	lineCount = 0;
	level = 1;
	ifstream file(RECORED_FILE);
	if (!file.is_open()) {
		cout << RECORED_FILE << "Failed to open" << endl;
		highestScore = 0;
	}
	else {
		file >> highestScore;
	}
	file.close();
	ifGameOver = 0;
}

void Tetris::play()
{
	Tetris::init();

	nextBlock = new Block;
	currBlock = nextBlock;
	nextBlock = new Block;

	int count = 0;
	while (true)
	{
		KeyEvent();

		count += getDelay();
		if (count > delay) {
			count = 0;
			ifUpdateWindow = 1;
			drop();
			
			system("cls");
		}
		if (ifUpdateWindow) {
			ifUpdateWindow = 0;
			updateWindow();
			clearLine();
		}
		if (ifGameOver) {
			saveScore();

			displayOver();

			system("pause");
			init();
		}
	}
}

void Tetris::KeyEvent()
{
	unsigned char ch;
	bool ifRotate = 0;
	int dx = 0;
	if (_kbhit()) {
		ch=_gettch();

		if (ch == 224) {
			ch = _getch();
			switch (ch)
			{
			case 72:
				ifRotate = 1;
				break;
			case 80:
				delay = SPEED_QUICK;
				break;
			case 75://left
				dx = -1;
				break;
			case 77://right
				dx = 1;
				break;
			default:
				break;
			}
		}
		
	}
	if (ifRotate) {
		rotate();
		ifUpdateWindow = 1;
	}
	if (dx != 0) {
		translation(dx);
		ifUpdateWindow = 1;
	}
}

void Tetris::updateWindow()
{
	
	//const char add[] = "./res/bg2.png";
	//DWORD address=add;

	putimage(0, 0, &backgroundImage);
	//test
	//Block block;
	//block.draw(100, 65);
	IMAGE** imgs = Block::getImageType();
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (map[i][j] == 0) {continue;}
			int x = j * blockSize + leftMargin;
			int y = i * blockSize + topMargin;
			putimage(x, y, imgs[map[i][j] - 1]);
		}
	}
	currBlock->draw(leftMargin, topMargin);
	nextBlock->draw(520, topMargin);
	drawScore();
	EndBatchDraw();
}

void Tetris::drop()
{
	bakBlock = *currBlock;
	currBlock->drop();
	if (!currBlock->ifblockInMap(map)) {
		bakBlock.solidify(map);
		delete(currBlock);
		currBlock = nextBlock;
		nextBlock = new Block;
		overCheck();
	}
	delay = SPEED_NORMAL[level-1];

}

void Tetris::clearLine()
{
	int lines = 0;
	int k = row - 1;
	for (int i = row - 1; i >= 0; i--) {
		int cnt = 0;
		for (int j = 0; j < col; j++) {
			if (map[i][j])cnt++;
			map[k][j] = map[i][j];
		}
		if (cnt < col)k--;
		else {
			lines++;
		}
	}
	if (lines) {
		mciSendString("play res/clearSound.mp3", 0, 0, 0);
		ifUpdateWindow= 1;
		score += 10 * lines;

		level = (score + 99) / 100;
		if (level > MAX_LEVEL) {
			ifGameOver = 1;
		}


		lineCount += lines;
	}
}

//first return 0
//then return delay time from last call 
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void Tetris::translation(int offset)
{
	bakBlock = *currBlock;
	currBlock->translation(offset);
	if (!currBlock->ifblockInMap(map)) {
		*currBlock=bakBlock;
	}

}

void Tetris::rotate()
{
	if (currBlock->getBlockType() == 7) {
		return;
	}
	bakBlock = *currBlock;
	currBlock->rotate();
	if (!currBlock->ifblockInMap(map)) {
		*currBlock = bakBlock;
	}
}

void Tetris::drawScore()
{
	char scoreText[32];
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	
	setcolor(RGB(180, 180, 180));

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 34;
	f.lfWeight = 32;
	f.lfQuality = ANTIALIASED_QUALITY;
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	gettextstyle(&f);

	setbkmode(TRANSPARENT);

	outtextxy(501, 530, scoreText);


	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);
	int xpos = 109;
	int ypos = 519;
	outtextxy(xpos, ypos, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", level);
	int xpos1 = 109;
	int ypos1 = 588;
	outtextxy(xpos1, ypos1, scoreText);

	sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
	int xpos2 = 500;
	int ypos2 = 588;
	outtextxy(xpos2, ypos2, scoreText);

}

void Tetris::overCheck()
{
	ifGameOver = (currBlock->ifblockInMap(map) == false);
		
}

void Tetris::saveScore()
{
	if (score > highestScore) {
		highestScore = score;
		ofstream file(RECORED_FILE);
		file <<highestScore;
		file.close();
	}
}

void Tetris::displayOver()
{
	//gameover music

	if (level <= MAX_LEVEL) {
		//shows lose over image
	}
	else {
		//shows win over image
	}

}
