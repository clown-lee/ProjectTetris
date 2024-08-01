#pragma once
#include <vector>
#include<easyx.h>
#include "Block.h"

using namespace std;
class Tetris
{
public:
	Tetris(int row, int col, int left, int top, int blockSize);
	void init();
	void play();

private:
	void KeyEvent();
	void updateWindow();
	void drop();
	void clearLine();

	int getDelay(); //unit ms 
	void translation(int offset);
	void rotate();
	void drawScore();
	void overCheck();
	void saveScore();
	void displayOver();

private:
	int delay;
	bool ifUpdateWindow;
	vector<vector<int>> map;
	int row; int col;
	int leftMargin;
	int topMargin;
	int blockSize;
	IMAGE backgroundImage;

	Block* currBlock;
	Block* nextBlock;
	Block bakBlock;

	int score;
	int level;
	int lineCount;
	int highestScore;
	bool ifGameOver;
};

