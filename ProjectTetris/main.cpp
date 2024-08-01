#include<iostream>
#include "Tetris.h"
#include <graphics.h>

int main() {
	Tetris game(20, 10, 197, 66, 27);
	game.play();
	return 0;
}
