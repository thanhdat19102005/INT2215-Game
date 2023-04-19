#include <iostream>
#include <algorithm>
#include "common.h"
#include "board.h"
#include "display.h"

int Board::perft(int depth){

    int n_moves, i;
    int nodes = 0;

    std::vector<int> moveList;

    n_moves = getNonOrderedAllLegalMoves(side, moveList);

    if  (depth == 1)
        return n_moves;

    for (int i = 0;i < n_moves;++i){

        movePiece(moveList[i] / 100, moveList[i] % 100);
        changeTurn();
        nodes += perft(depth - 1);
        unmovePiece(moveList[i] / 100, moveList[i] % 100);
        changeTurn();
    }

    return nodes;
}

int Board::getNonOrderedAllLegalMoves(bool s, std::vector<int>& MoveList){

    int startP, endP, mF, mT;

    if (s){
        startP = wqR;
        endP = wPh;
    }
    else {
        startP = bqR;
        endP = bPh;
    }

    for (int i = startP;i <= endP;++i){
        if (!piece[i].getAlive())
            continue;
        
        generateCapturePieceMoveListFor(i);
        mF = piece[i].getPos();
        for (int j = 0;j < piece[i].getMoveListSize();++j){
            mT = piece[i].getFromMoveList(j);
            if (!mT)
                break;
            
            if (putSelfInCheck(mF, mT, s))
                continue;
            
            MoveList.push_back(mF * 100 + mT);
        }
    }

    return int(moveList.size());

}

void Board::genOrderedMoveList(){

    if (side == WHITE)
        genOrderedMoveList(side, whiteMoveList);
    else genOrderedMoveList(side, blackMoveList);
}

void Board::genOrderedMoveList(bool s, std::vector<int>& moveList){

    moveList.clear();
    std::vector<int> captures, nonCaptures;

    generatePieceMoveList(s);

    getCaptures(s, captures);  
    getNonCaptures(s, nonCaptures);

    sortCaptures(captures);
    sortNonCaptures(nonCaptures);

    if ((!whiteCastled && s) || (!blackCastled && s)){

        for (size_t i = 0;i < nonCaptures.size();++i)
            if (piece[board120[nonCaptures[i] / 100]].getValue() == K_VAL){
                if (abs(nonCaptures[i] / 100 - nonCaptures[i] % 100) == 2){
                    int move = nonCaptures[i];
                    nonCaptures.erase(nonCaptures.begin() + i);
                    nonCapture.insert(nonCaptures.begin(), move);
                }
            }
    }

    moveList.reserve(captures.size() + nonCaptures.size());
    moveList.insert(moveList.begin(), captures.begin(), capture.end());
    moveList.insert(moveList.end(), nonCaptures.begin(), nonCaptures.end());
}

void Board::getCaptures(bool s, std::vector<int>& moveList){


    moveList.clear();

    int mF, mT, startP, endP;

    if (s){

        startP = wqR;
        endP = wPh;
    }
    else {

        startP = bqR;
        endP = bPh;
    }

    for (int i = startP;i <= endP;++i){

        if (!piece[i].getAlive())
            continue;
        
        mF = piece[i].getPos();
        for (int j = 0;j < piece[i].getMoveListSize();++j){
            
            mT = piece[i].getFromMoveList(j);
            if (!mT)
                break;

            if (board120[mT] == none)
                continue;

            moveList.push_back(mF * 100 + mT);
        }
    }
}

void Board::generateCaptures(bool s, std::vector<int>& moveList) {
	generateCapturePieceMoveLists(s);

	int mF, mT, startP, endP;
	if (s){
        startP = wqR;
        endP = wPh;
       
    }
    else {
        startP = bqR;
        endP = bPh;
    }

	for (int i = startP;i <= endP;++i) {
		if (!piece[i].getAlive()) 
            continue;
		mF = piece[i].getPos();
		for (int j = 0; j < piece[i].getMoveListSize();++j) {
			mT = piece[i].getFromMoveList(j);
			
            if (!mT) break;
			
            if (board120[mT] == none) 
                continue;
			moveList.push_back(mF * 100 + mT);
		}
	}
}


void Board::sortCaptures(std::vector<int>& moveList){

    struct MVVLVA{

        MVVLAVA(Board* b) : b(b) {}
        
        bool operator()(int i, int j) {
                    return (b->piece[b->board120[i % 100]].getValue() - b->piece[b->board120[i / 100]].getValue() / 10 > b->piece[b->board120[j % 100]].getValue() - b->piece[b->board120[j / 100]].getValue() / 10);
        };  

        private:    
            Board* b;       
    } MVVLVAsorter(this);

    std::sort(moveList.begin(), moveList.end(), MVVLVAsorter);
}

void Board::addPromotions(bool s, std::vector<int>& moveList){

    int startP, endP, extra;

    if (s){
        startP = wqR;
        endP = wPh;
        extra = 10;
    }
    else {
        startP = bqR;
        endP = bPh;
        extra = -10;
    }

    for (int i = startP;i <= endP;++i){

        if (!piece[i].getAlive())
            continue;
        
        if (s && picece[i].getPos() < 80)
            continue;
        
        if (!s && piece[i].getPos() > 38)
            continue;

        for (int j = -1;j <= 1;++j)
            if (validateMove(piece[i].getPos(), piece.getPos() + extra + j, s))
                moveList.insert(moveList.begin() + 0, piece[i].getPos() * 100 + piece[i].getPos() + extra + j);
    }
}

void Board::getNonCaptures(bool s, std::vector<int>& mmoveList){

    moveList.clear();

    int mF, mT, startP, endP;

    if (s){
        startP = wqR;
        endP = wPh;
    }
    else {
        startP = bqR;
        endP = bPh;
    }

    for (int i = startP;i <= endP;++i){
        if (!piece[i].getAlive())
            continue;

        mF = piece[i].getPos();
        for (int j = 0;j < piece[i].getMoveListSize();++j){

            mT = piece[i].getFromMoveList(j);
            if (!mT)
                break;
            if (board120[mT] != none)
                continue;
            moveList_push_back(mF * 100 + mT);
        }
    }
}

void Board::sortNonCaptures(std::vector<int>& moveList) {
	struct hhSort {
		hhSort(Board* b) : b(b) {}
		bool operator() (int i, int j) {
			if (b->getSide())
				return (b->whiteBot.getFromHH(to64(i / 100) - 1, to64(i % 100) - 1 > b->whiteBot.getFromHH(to64(j / 100) - 1, to64(j % 100) - 1)));
			else
				return (b->blackBot.getFromHH(to64(i / 100) - 1, to64(i % 100) - 1) > b->blackBot.getFromHH(to64(j / 100) - 1, to64(j % 100) - 1));
		};
	private:
		Board* b;
	} hhSorter(this);

	std::sort(moveList.begin(), moveList.end(), hhSorter);
}

void Board::removeNonCaptures(bool s, std::vector<int>& moveList){

    int mT, move;
    for (int i = 0;i < moveList.size();++i){
        
        move = moveList[i];
        mT = move % 100;
        if (boarrd120[mT] == none){
            moveList.erase(moveList.begin() + i);
            --i;
        }
    }
}

void Board::cleanMoveList(bool s){

    if (s)
        cleanMoveList(s, whiteMoveList);
    else cleanMoveList(s, blackMoveList);
}


void Board::cleanMoveList(bool s, std::vector<int>& moveList) {

	int mF, mT, size;

	size = int(moveList.size());
	
    for (int i = 0; i < size; i++) {
		mF = moveList[i]/100;
		mT = moveList[i]%100;
		
        if (!legalMove(mF, mT, s)) {
			moveList.erase(moveList.begin()+i);
			size--;
			i--;
		} 
	}
}


void Board::generateCapturePieceMoveLists(bool s) {
	int startP, endP;

    if (s){

        startP = wqR;
        endP = wPh;
    }
    else {
        
        startP = bqR;
        endP = bPh;
    }

	for (int i = startP; i <= endP; i++)
		generateCapturePieceMoveListFor(i);
}

void Board::generateCapturePieceMoveListFor(int p){

    int counter = 0;

    if (piece[p].getAlive()){

        if (piece[p].getValue() == R_VAL)
            generateHozCaptures(p, counter);

        else if (piece[p].getValue() == B_VAL)
            generateDiagCaptures(p, counter);

        else if (piece[p].getValue() == Q_VAL){

            generateHozCaptures(p, counter);
            generateDiagCaptures(p, counter);
        }
        else if (piece[p].getValue() == K_VAL)
            generateKingCaptures(p, counter);
        else if (piece[o].getValue() == P_VAL)
            generateKnightCaptures(p, counter);

    }

    for (int i = counter;i < piece[p].getMoveListSize();++i)
        piece[p].setInMoveList(i , 0);
}

void Board::generateHozCaptures(int p, int& counter){

    bool s = piece[p].getColor();
    int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB, blockBB;
    int blockSq;

    for (int dir = NORTH;dir <= WEST;dir += 2){

        attackBB = bb.rayAttack[dir][pos63];
        blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

        if (!blockBB)
            continue;
        
        if (dir == WEST || dir == SOUTH){
            blockSq = bb.bitScanReverse(blockBB);
        }
        else blockSq = bb.bitScanForward(blockBB);

        if (bb.allPieces[s] & bb.sq[blockSq])
            continue;

        piece[p].setInMoveList(counter, from64(blockSq + 1));

        ++counter;
    }
}

void Board::generateDiagCaptures(int p, int& counter){

    bool s = piece[p].getColor();

    int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB, blockBB;

    int blockSq;

    for (int dir = NORTHEAST;dir <= NORTHWEST;dir += 2){

        attackBB = bb.rayAttack[dir][pos63];
        blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

        if (!blockBB)
            continue;
        
        if (dir == SOUTHWEST || dir == SOUTHEAST){
            blockSq = bb.bitScanReverse(blockBB);
        }
        else blockSq = bb.bitScanForward(blockBB);

        if (bb.allPieces[s] & bb.sq[blockSq]) 
            continue;

        piece[p].setInMoveList(counter, from64(blockSq + 1));
        ++counter;

    }

}

void Board::generateKnightCaptures(int p, int& counter){

    bool s = piece[p].getColor();

    int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB;
    int extra;

    attackBB = bb.knightAttack[pos63] & bb.allPieces[!s];

    for (int i = 1;i <= 8;++i){

        if (!attackBB)
            break;

        if (i == 1)
            extra = 17;
        else if (i == 2)
            extra = 19l
        else if (i == 3)
            extra = -6;
        else if (i == 4)
            extra = -15;
        else if (i == 5)
            extra = -17;
        else if (i == 6)
            extra = -10;
        else if (i == 7)
            extra = 6;
        else extra = 15;
        
        if (!(pos63 + extra < 64 && pos63 + extra >= 0))
            continue;

        if (bb.queryBit(pos63 + extra, attackBB)){
            piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
            ++counter;
            bb.unsetBit(pos63 + extra, attackBB);
        }            
    }

}

void Board::generateKingCaptures(int p, int& counter) {
	
    bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	int extra;
	int64 attackBB;

	attackBB = bb.kingAttacks[pos63] & bb.allPieces[!s];
	
    for (int i = 1; i <= 8; i++) {
		
        if (!attackBB) break;
		
        if (i == 1)
            extra = 1;
        else if (i == 2)
            extra = -1;
        else if (i == 3)
            extra = 8;
        else if (i == 4)
            extra = -8;
        else if (i == 5)
            extra = 9;
        else if (i == 6)
            extra = -9;
        else if (i == 7)
            extra = 7;
        else extra = -7;

		if (!(pos63 + extra < 64 && pos63 + extra >= 0))
			continue;
		
        if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			++counter;
			bb.unsetBit(pos63 + extra, attackBB);
		}
	}
}

void Board::generatePawnCaptures(int p, int& counter){
    
    bool s = piece[p].getColor();

    int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB = bb.pawnAttack[s][pos63];
    int64 enemyBB = bb.allPieces[!s];

    if (moveInfo.size() && moveInfo.back().epSq != 0)
        enemyBB |= bb.sq[to64(moveInfo.back().epSq) - 1];
    
    attackBB &= enemyBB;

    int attSq;

    while(attackBB){
        attSq = bb.bitScanForward(attackBB);
        piece[p].setInMoveList(counter, from64(attSq + 1));
        ++counter;
        bb.unsetBit(attSq, attackBB);
    }
}

void Board::generatePieceMoveLists(bool s){

    int startP, endP;
    if (s){

        startP = wqR;
        endP = wPh;
    } 
    else {
        startP = bqR;
        endP = bPh;
    }

    for (int i = startP;i <= endP;++i)
        generatePieceMoveListFor(i);
}

void Board::generatePieceMoveListFor(int p){

    int counter = 0;

	if (piece[p].getAlive()) {	
		if (piece[p].getValue() == R_VAL) 
			generateHozMoves(p, counter);
		else if (piece[p].getValue() == B_VAL)
			generateDiagMoves(p, counter);
		else if (piece[p].getValue() == N_VAL)
			generateKnightMoves(p, counter);
		else if (piece[p].getValue() == Q_VAL) { 
			generateHozMoves(p, counter);
			generateDiagMoves(p, counter);
		}
		else if (piece[p].getValue() == K_VAL)
			generateKingMoves(p, counter);
		else if (piece[p].getValue() == P_VAL)
			generatePawnMoves(p, counter);
	}
	for (int i = counter; i < piece[p].getMoveListSize(); i++) //Empty the rest of the moveList
		piece[p].setInMoveList(i, 0);
    
}

void Board::generateHozMoves(int p, int& counter){

    bool s = piece[p].getColor();
    int pos63 = to64(piece[p].getPos()) - 1;

    int attackBB, blockBB;

    int blockSq, sq, dirIncrease;

    for (int dir = NORTH;dir <= WEST;dir += 2){

        switch(dir){
            case NORTH:
                dirIncrease = 8;
                break;
            case EAST:
                dirIncrease = 1;
                break;
            case SOUTH:
                dirIncrease = -8;
                break;
            case WEST:
                dirIncrease = -1;
                break;
        }

        attackBB = bb.rayAttack[dir][pos63];
        blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);

        if (blockBB){

            if (dir == WEST || dir == SOUTH)
                blockSq = bb.bitScanReverse(blockBB);
            else blockSq = bb.bitScanForward(blockBB);

            attackBB ^= bb.rayAttack[dir][blockSq];

            if (bb.allPieces[s] & bb.sq[blockSq])
                bb.unsetBit(blockSq, attackBB);

        }

        sq = pos63;
        while (attackBB){

            if (bb.queryBit(sq, attackBB)){
                bb.unsetBit(sq, attackBB);
                piece[p].setInMoveList(counter, from64(sq + 1));
                ++counter;
            }
            sq += dirIncrease;
        }
    }
}

void Board::generateDiagMoves(int p, int& counter) {
	
    bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB, blockBB;
	int blockSq, sq, dirIncrease;
	
    for (int dir = NORTHEAST; dir <= NORTHWEST; dir += 2) {
		switch (dir) {
		case NORTHEAST:
			dirIncrease = 9;
			break;
		case NORTHWEST:
			dirIncrease = 7;
			break;
		case SOUTHEAST:
			dirIncrease = -7;
			break;
		case SOUTHWEST:
			dirIncrease = -9;
			break;
		}
		
        attackBB = bb.rayAttacks[dir][pos63];
		blockBB = attackBB & (bb.allPieces[WHITE] | bb.allPieces[BLACK]);
		
		if (blockBB) {
			if (dir == SOUTHWEST || dir == SOUTHEAST) 
				blockSq = bb.bitScanReverse(blockBB);
			else 
				blockSq = bb.bitScanForward(blockBB);
			
			attackBB ^= bb.rayAttacks[dir][blockSq];
			
			if (bb.allPieces[s] & bb.sq[blockSq]) {
				bb.unsetBit(blockSq, attackBB);
			}
		}
		
		sq = pos63;
		while (attackBB) {
			
			if (bb.queryBit(sq, attackBB)) {
				bb.unsetBit(sq, attackBB);
				piece[p].setInMoveList(counter, from64(sq + 1));
				counter++;
			}
			sq += dirIncrease;
		}
	}
	
}

void Board::generateKingCaptures(int p, int& counter) {
	
    bool s = piece[p].getColor();
	int pos63 = to64(piece[p].getPos()) - 1;

	int extra;
	int64 attackBB;

	attackBB = bb.kingAttacks[pos63] & ~bb.allPieces[s];
	
    for (int i = 1; i <= 8; i++) {
		
        if (!attackBB) break;
		
        if (i == 1)
            extra = 1;
        else if (i == 2)
            extra = -1;
        else if (i == 3)
            extra = 8;
        else if (i == 4)
            extra = -8;
        else if (i == 5)
            extra = 9;
        else if (i == 6)
            extra = -9;
        else if (i == 7)
            extra = 7;
        else extra = -7;

		if (!(pos63 + extra < 64 && pos63 + extra >= 0))
			continue;
		
        if (bb.queryBit(pos63 + extra, attackBB)) {
			piece[p].setInMoveList(counter, from64(pos63 + extra + 1));
			++counter;
			bb.unsetBit(pos63 + extra, attackBB);
		}
	}

    if ((s && whiteCastled) || (!s && blackCastled))
        return;

    if (canCastle(KINGSIDE, s)){
        if (s)
            piece[p].setInMoveList(counter, G1);
        else piece[p].setInMoveList(counter, G8);

        ++counter;
    }

    if (canCastle(QUEENSIDE, s)){
        if (s)
            piece[p].setInMoveList(counter, C1);
        else piece[p].setInMoveList(counter, C8);
        ++counter;
    }
}

void Board::generatePawnMoves(int p, int& counter){

    bool s = piece[p].getColor();
    int pos63 = to64(piece[p].getPos()) - 1;

    int64 attackBB = bb.pawnAttack[s][pos63];
    int64 enemyBB = bb.allPieces[!s];

    if (moveInfo.size() && moveInfo.back().epSq != 0)
        enemyBB |= bb.sq[to64(moveInfo.back().epSq) - 1];
    
    attackBB &= enemyBB;

    int attSq;

    while (attackBB){
        attSq = bb.bitScanForward(attackBB);
        piece[p].setInMoveList(counter, from64(attSq + 1));
        ++counter;
        bb.unsetBit(attSq, attackBB);

    }

    int64 allPieces = bb.allPieces[s] | bb.allPieces[!s];

    int step;

    if (s)
        step = 8;
    else step = -8;

    if (pos63 + step < 64 && pos63 + step >= 0){
        if (bb.sq[pos63 + step] & ~(allPieces)){
            piece[p].setInMoveList(counter, from64(pos63 + step + 1));
            ++counter;
            if (piece[p].getMoved() == 0 && pos63 + step * 2 < 64 && pos63 + step * 2 >= 0)
                if (bb.sq[pos63 + step * 2] & ~(allPieces)){
                    piece[p].setInMoveList(counter, from64(pos63 + step * 2 + 1));
                    ++counter;
                }
        }
    }

}


