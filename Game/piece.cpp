#include "common.h"
#include "piece.h"

Piece::Piece(){
    pos = 0;
    moved = 0;
    value = 0;
    moveListSize = 0;
    alive = true;
    promoted = false;
    moveList = NULL;
    name = "";
    abbr = ' ';
}

Piece::~Piece(){
    delete[] moveList;
}

int Piece::getFromMoveList(int i) const{
    return moveList[i];
}

void Piece::setInMoveList(int i, int v){
    moveList[i] = v;
}