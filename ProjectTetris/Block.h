#pragma once
#include <graphics.h>

#include <vector>
using namespace std;

struct Locate {
	int row;
	int col;
};
class Block
{
public:
	Block();
	void drop();
	void rotate();
	void translation(int offset);
	void draw(int leftMargin, int rightMargin);
	void solidify(vector<vector<int>>& map);


	Block& operator=(const Block& other);

	static IMAGE** getImageType();
	Locate* getSmallBlock();
	bool ifblockInMap(const vector<vector<int>> &map);
	int getBlockType();

private:
	int blockType;
	Locate smallBlocks[4];
	IMAGE *img;

	static IMAGE *imgs[7];
	static int size;

};

