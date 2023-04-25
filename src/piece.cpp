#include "common.h"
#include "piece.h"

Piece::Piece() {
	pos = moved = value = moveListSize = 0;
	alive = true;
	promoted = false;
	moveList = NULL;
	name = "";
	abbr = ' ';
}

Piece::~Piece() {
	delete [] moveList;
}

int Piece::getFromMoveList(int i) const {
	assert (i > -1 && i < moveListSize);
	return moveList[i];
}

void Piece::setInMoveList(int i, int v) {
//	assert (i > -1 && i < moveListSize);
	moveList[i] = v;
}
