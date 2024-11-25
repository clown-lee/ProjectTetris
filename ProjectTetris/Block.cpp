#include "Block.h"
#include<stdlib.h>

IMAGE* Block::imgs[7] = { NULL,0 };
int Block::size = 27;
Block::Block()
{
	if (imgs[0] == NULL) {
		IMAGE imgTmp;
		loadimage(&imgTmp, "./res/tiles.png");

		SetWorkingImage(&imgTmp);
		for (int i = 0; i < 7; i++) {
			imgs[i] = new IMAGE;
			getimage(imgs[i], i * size, 0, size, size);
		}
		SetWorkingImage();

	}
	int blocks[7][4] = {
		1,3,5,7, //I
		2,4,5,7,//Z1
		3,5,4,6,//Z2
		3,5,4,7,//T
		2,3,5,7,//L
		3,5,7,6,//J
		2,3,4,5,//square
	};

	blockType= 1 + rand() % 7;
	//blocks[type][4];
	//init blocks
	//the location of generated blocks
	for (int i = 0; i < 4; i++) {
		int value = blocks[blockType - 1][i];
		smallBlocks[i].row = value / 2;
		smallBlocks[i].col = value % 2;
	}
	img = imgs[blockType - 1];
}

void Block::drop()
{
	for (int i = 0; i < 4; i++) {
		smallBlocks[i].row++;
	}
}

void Block::rotate()
{
	Locate central = smallBlocks[1];

	for (int i = 0; i < 4; i++) {
		Locate bak = smallBlocks[i];
		smallBlocks[i].col = central.col - bak.row + central.row;
		smallBlocks[i].row = central.row + bak.col - central.col;
	}


}


void Block::translation(int offset)
{
	for (int i = 0; i < 4; i++)
	{
		smallBlocks[i].col += offset;
	}
}

void Block::draw(int leftMargin, int topMargin)
{
	for (int i = 0; i < 4; i++) {
		int x = leftMargin + smallBlocks[i].col * size;
		int y = topMargin + smallBlocks[i].row * size;
		putimage(x, y, img);
	}
}

void Block::solidify(vector<vector<int>>& map)
{
	for (int i = 0; i < 4; i++) {
		map[smallBlocks[i].row][smallBlocks[i].col] = blockType;
	}
	
}

Block &Block::operator=(const Block &other)
{
	if (this == &other)return *this;

	this->blockType = other.blockType;

	for (int i = 0; i < 4; i++) {
		this->smallBlocks[i] = other.smallBlocks[i];
	}
	return *this;
}
 
IMAGE** Block::getImageType()
{

	return imgs;
}

Locate* Block::getSmallBlock()
{
	return nullptr;
}

bool Block::ifblockInMap(const vector<vector<int>>& map)
{
	int rows = map.size();
	int cols = map[0].size();
	for (int i = 0; i < 4; i++) {
		if (smallBlocks[i].col < 0 || smallBlocks[i].col>= cols ||
			smallBlocks[i].row<0 ||smallBlocks[i].row>= rows||
			map[smallBlocks[i].row][smallBlocks[i].col]
			) {
			return false;
		}
	}
	return true;
}

int Block::getBlockType()
{
	return this->blockType;
}
