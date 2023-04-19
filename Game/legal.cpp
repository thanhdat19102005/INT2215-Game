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
    unmovePiece(mF, mT);

    if (isInCheck){
        if (v)
            std::cout << "Illegal Move\n";
        return false;
    }

    return true;
    
}

bool Board::putSelfInCheck(int mF, int mT, bool s){

    bool isInCheck;

    movePiece(mF, mT);
    isInCheck = incheck(s);
    unmovePiece(mF, mT);

    return isInCheck;    
}

bool Board::checkCheck(bool s){

    if (s)
        return checkCheck(s, whiteMoveList);
    else return checkCheck(s, blackMoveList);

}

bool Board::checkCheck(bool s, std::vector<int>& moveList){

    sideInCheck = 0;
    sideInCheckmate = 0;

    if (inCheck(s)){

        if (s)
            sideInCheck = 1;
        else sideInCheck = 2;

        cleanMoveList(s, moveList); //movegen.cpp
        if (!moveList.size()){
            if (s)
                sideInCheckmate = 1;
            else sideInCheckmate = 2;
        }
    }
    return false;
}

bool Board::inCheck(bool s) const{

    if (s && !piece[wK].getAlive())
        return true;
    if (!s && !piece[bK].getAlive())
        return true;

    int kPos, kPos63, blockSq;

    int64 kingRay, blockBB;

    if (s)
        kPos = piece[wK].getPos();
    else kPos = piece[bK].getPos();

    kPos63 = to64(kPos) - 1;

    if (bb.pieces[!s][KNIGHT] & bb.knightAttacks[kPos63])
        return true;
    
    for (int i = NORTH;i <= NORTHWEST;++i){
        
        kingRay = bb.rayAttack[i][kPos63];
        blockBB = kingRay & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

        if (!blockBB)
            continue;

        if (i == NORTH || i ==  NORTHEAST || i == NORTHWEST || i == EAST)
            blockSq = bb.bitScanForward(blockBB);
        else if (i == SOUTH || i == SOUTHEAST || d == SOUTHWEST || i == WEST)
            blockSq = bb.bitScanReverse(blockBB);

        if (bb.sq[blockSq] & bb.allPieces[s])
            continue;

        switch (piece[board120[from64(blockSq + 1)]].getType()){
            case QUEEN:
                return true;
            case ROOK:
                if (i % 2 == 0) 
                    return true;
                else continue;
            case BISHOP:
                if (i % 2 == 1)
                    return true;
                else continue;
            case PAWN:
                if (bb.pawnAttack[!s][blockSq] && bb.sq[kPos63])
                    return true;
                else continue;
            case KING:
                if (bb.kingAttack[blockSq] & bb.sq[kPos63])
                    return true;
                else continue;
            default:
                continue;
        }

    }

    return false;
}

bool Board::validateMove(int mF, int mT, bool s){

    int onMF = board120[mF], onMT = board120[mT], value = piece[onMF].getValue();

    if (onMF == none || mT == null || board120[mT] == invalid)
        return false;
    
    if (onMT != none && piece[onMF].getColor() == piece[onMT].getColor())
        return false;
    if (piece[onMF].getColor() != s)
        return false;

    if (value == R_VAL) 
        return validateHozMove(mF, mT);
    else if (value == N_VAL)
        return validateKnightMove(mF, mT);
    else if (value == B_VAL)
        return validateDiagMove(mF, mT);
    else if (value == Q_VAL)
        return (validateHozMove(mF, mT) | (validateDiagMove(mF, mT)));
    else if (value == K_VAL)
        return validateKingMove(mF, mT, s);
    else if (value == P_VAL)
        return validatePawnMove(mF, mT, s);

    return false;

}

bool Board::validatePawnMove(int mF, int mT, bool s) const{

    int extra;

    if (s)
        extra = 10;
    else extra = -10;

    int onMF = board120[mF], onMT = board120[mT];

    int diff = abs(mF - mT);

    if (board120[mT] == invalid)
        return false;
    
    if ((s && mF > mT) || (!s && mF < mT))
        return false;
    
    if (diff == 10 && onMT = none)
        return true;

    if (diff == 20 && onMT == none){
        if (board120[mF + extra] == none && piece[onMF].getMoved() == 0)
            return true;
        else return false;

    }

    if (diff == 9 || diff == 11){
        if (onMT == none){
            if (moveInfo.size() > 1 && mT == moveInfo.back().epSq)
                if ((s && moveInfo.back().epSq > H5) || (!s && moveInfo.back().epSq < A4))
                    return true;
            return false;
        } 
        if (s != piece[onMT].getColor())
            return true;
        else return false;
    }
    return false;

}

bool Board::validateHozMove(int mF, int mT) const{

    int small, big;

    if (mF > mT){
        small = mT;
        big = mF;
    }
    else {
        small = mF;
        big = mT;
    }

    if ((mT - mF) % 10 == 0){
        for (int i = small + 10;i < big;i += 10)
            if (board120[i] != none)
                return false;
        return true;
    }
    else if (mF / 10 == mT / 10){
        for (int i = small + 1;i < big;++i)
            if (board120[i] != none)
                return false;
        return true;
    }
    else return false;
}

bool Board::validateDiagMove(int mF, int mT) const{

    int temp, small, big;

    if (mF > mT){
        small = mT;
        big = mF;
    }
    else {
        small = mF;
        big = mT;
    }

    int diff = big - small;

    if (diff % 11 == 0)
        temp = 11;
    else if (diff % 9 == 0)
        temp = 9;
    else temp = 0;

    if (!temp) 
        return false;
    for (int j = small + temp;j < big;j += temp)
        if (board120[j] != none)
            return false;
    
    return true;

}

bool Board::validateKnightMove(int mF, int mT) const {
	int diff = abs(mF - mT);

	if (diff == 8 || diff == 12 || diff == 19 || diff == 21)
		return true;
	return false;
}

bool Board::validateKingMove(int mF, int mT, bool s) {
	int diff = abs(mF - mT);
	
	if (diff == 1 || diff == 10 || diff == 9 || diff == 11)
		return true;
	if ((mF - mT) == -2) {	
		if (canCastle(KINGSIDE, s)) {
			return true;
		}
	}
	if ((mF - mT) == 2) {  
		if (canCastle(QUEENSIDE, s)) {
			return true;
		}
	}
	return false;
}

bool Board::canCastle(int dir, bool s){

    int k, r, c;

    if (s){
        k = wK;
        if (dir == KINGSIDE)
            r = wkR;
        else r = wqR;
    }
    else {
        if (dir == KINGSIDE)
            r = bkR;
        else r = bqR;
    }

    if (dir === KINGSIDE)
        c = 1;
    else c = -1;

    if (!(piece[k].getAlive() && piece[r].getAlive()))
        return false;
    if (piece[k].getMoved() || piece[r].getMoved())
        return false;

    for (int i = 1;i <= 2;++i)
        if (board120[piece[k].getPos() + i * c] != none)
            return false;
    if (dir == QUEENSIDE)
        if (board120[piece[k].getPos() - 3] != none)
            return false;

    movePiece(piece[k].getPos(), piece[k].getPos() + c);
    if (inCheck(s)){
        unmovePiece();
        return false;
    }

    unmovePiece();

    return true;

}

int Board::drawCheck() const{

    int lastPly = int(movesMade.size());

    if (lastPly < 3)
        return false;
    
    int count = 0;

    //for (int )

    return count;
}

