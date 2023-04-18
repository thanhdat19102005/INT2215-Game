#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "common.h"
#include "board.h"
#include "ltexture.h"

bool Board::legalMove(int mF, int mT, bool s, bool v){

    bool isInCheck;

    if (!validateMove(mF, mT, s)){
        if (v)
            std::cout << "Invalide Move\n";
        return false;
    }

    movePiece(mF, mT);

    isInCheck = inCheck(s);

    
}