#include <iostream>
#include "common.h"
#include "board.h"

void Board::movePiece(){
    movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT){
    bool s = piece[board120[mF]].getColor();
    int type = piece[board120[mF]].getType();
    int mFVal = piece[board120[mF]].getValue();
    int epExtra = 0;
    int localEpSq = null, localPmSq = null;
    int localPrevOnMoveTo = board120[mT], localHalfMoveClock;

    if (!moveInfo.size())
        localHalfMoveClock = 0;
    else localHalfMoveClock = moveInfo.back().halffMoveClock + 1;

    moveSMade.push_back(mF * 100 + mT);
    
    if (!castling){
        if (mFVal == K_VAl && abs(mF - mT) == 2)
            if (mF < mT)   
                castling = KINGSIDE;
            else castling = QUEENSIDE;
        
        if (mFVal == P_VAL){

            if (moveInfo.size() && mT == moveInfo.back().epSq){
                if (s)
                    epExtra = -10;
                else epExtra = 10;
            }
            else if (abs(mF - mT) == 20){

                if (s)
                    localEpSq = mF + 10;
                else localEpSq = mF - 10;
            }
        }
        else if (mFVal == R_VAL && piece[board120[mF]].getMoved() == 0){

        }
        else if (mFVal == K_VAL && piece[board120[mF]].getMoved() == 0){
            int kRook, qRook;
            if (s)
                kRook = wkR;
            else kRook = bkR;
            if (s)
                qRook = wqR;
            else qRook = bqR;
        }

        if (mFVal == P_VAL && ((s && mT / 10 == 9) || (!s && mT / 10 == 2))){
            bb.pieces[s][QUEEN] |= bb.sq[to64(mT) - 1];
            bb.allPieces[s] |= bb.sq[to64(mT) - 1];

            localPmSq = mT;
            piece[board120[mF]].setValue(Q_VAL);
            piece[board120[mF]].setName("Queen");
            piece[board120[mF]].setType(QUEEN);
            if (s){
                piece[board120[mF]].setAbbr('Q');
                whiteMaterial += Q_VAL - P_VAL;
            }
            else {
                piece[board120[mF]].setAbbr('q');
                blackMaterial += Q_VAL - P_VAL;
            }
            piece[board120[mF]].setPromoted(true);
            
            int* temp = new int[27];
            for (int i = 0;i < 4;++i)
                temp[i] = piece[board120[mF]].getFromMoveList(i);
            piece[board120[mF]].freeMoveList();
            piece[board120[mF]].setMoveList(temp);
            piece[board120[mF]].setMoveListSize(27);
            for (int i = 4;i < 27;++i)
                piece[board120[mF]].setInMoveList(i , 0);
            
        }

        if (board120[mT + epExtra] != -1){
            if (!s)
                whiteMaterial -= piece[board120[mT + epExtra]].getValue();
            else blackMaterial += piece[board120[mT + epExtra]].getValue();
        }

        if (piece[board120[mT + epExtra]].getValue() == R_VAL){
            int eKing;
            if (s)
                eKing = bK;
            else eKing = wK;

        }
        else if (piece[board120[mT + epExtra]].getValue() == K_VAL){
            int eKing;
            if (s)
                eKing = bK;
            else eKing = wK;

            int ekRook, eqRook;
            if (s)  
                eqRook = bqR;
            else eqRook = wqR;
            if (s)
                ekRook = bkR;
            else ekRook = wqR;
            
        }

        bb.pieces[!s][piece[board120[mT + epExtra]].getType()] &= ~bb.sq[to64(mT + epExtra) - 1];
        bb.allPieces[!s] &= ~bb.sq[to64(mT + epExtra) - 1];
        piece[board120[mT + epExtra]].kill();
        piece[board120[mT + epExtra]].setPos(null);
        if (epExtra)
            board120[mT + epExtra] = none;

        localHalfMoveClock = 0;

        bb.pieces[s][piece[board120[mF]].getType()] &= ~bb.sq[to64(mF) - 1];
        bb.allPieces[s] &= ~bb.sq[to64(mF) - 1];
        board120[mT] = board120[mF];
        piece[board120[mT]].setPos(mT);
        piece[board120[mT]].incrMoved();
        board120[mF] = none;

    }
    else {

        if (s)
            whiteCastled = true;
        else blackCastled = true;

        int cExtra[2];

        if (castling == KINGSIDE){
            cExtra[0] = 1;
            cExtra[1] = 3;
        }
        else{
            cExtra[0] = -1;
            cExtra[1] = -4;
            
        }

        board120[mT] = board120[mF];
        piece[board120[mT]].setPos(mT);
        piece[board120[mT]].incrMoved();
        bb.pieces[s][KING] &= ~bb.sq[to64(mF) - 1];
        bb.pieces[s][KING] |= bb.sq[to64(mT) - 1];
        bb.allPieces[s] &= ~bb.sq[to64(mF) -  1];
        bb.allPieces[s] |= bb.sq[to64(mT) - 1];

        board120[mF + cExtra[0]] = board120[mF + cExtra[1]];
        piece[board120[mF+cExtra[0]]].setPos(mF+cExtra[0]);
		piece[board120[mF+cExtra[0]]].incrMoved();
		bb.pieces[s][ROOK] &= ~bb.sq[to64(mF+cExtra[1])-1];
		bb.pieces[s][ROOK] |= bb.sq[to64(mF+cExtra[0])-1];
		bb.allPieces[s] &= ~bb.sq[to64(mF+cExtra[1])-1];
		bb.allPieces[s] |= bb.sq[to64(mF+cExtra[0])-1]; 
        
        board120[mF] = none;
        board120[mF + cExtra[1]] = none;

        castling = 0;
        localHalfMoveClock = 0;
    }

    //add Hashing
    moveInfo.push_back({localPmSq, localEpSq, board120[mT], localPrevOnMoveTo, localHalfMoveClock});
    ++ply;

}

void Board::undoMove(){

    unmovePiece(movesMade.back() / 100, movesMade.back() % 100);
}

void Board::unmovePiece(int mF, int mT) {
	
    int epExtra = 0;
	bool s = piece[board120[mT]].getColor();
	int type = piece[board120[mT]].getType();
	int mTVal = piece[board120[mT]].getValue();

	if (mTVal == K_VAL && abs(mF-mT) == 2){
        if (mF < mT)
            castling = KINGSIDE;
        else castling = QUEENSIZE;
    }
	
	if (!castling) {

		if (mTVal == P_VAL && moveInfo.back().prevOnMoveTo == none) {
			int diffMTMF = abs(mT - mF);
			if (diffMTMF == 11 || diffMTMF == 9) {
				
                if (s)
                    epExtra = -10;
                else epExtra = 10;
			}
		}
		
		if (mT == moveInfo.back().pmSq) {
			
			bb.pieces[s][QUEEN] &= ~bb.sq[to64(mT)-1];
			bb.allPieces[s] &= ~bb.sq[to64(mT)-1];
			
			bb.pieces[s][PAWN] |= bb.sq[to64(mF)-1];
			bb.allPieces[s] |= bb.sq[to64(mF)-1];

			piece[board120[mT]].setType(PAWN);
			piece[board120[mT]].setValue(P_VAL);
			piece[board120[mT]].setName("Pawn");
			if (s) {
				piece[board120[mT]].setAbbr('P');
				whiteMaterial -= Q_VAL - P_VAL;
			}
			else {
				piece[board120[mT]].setAbbr('p');
				blackMaterial -= Q_VAL - P_VAL;
			}
			piece[board120[mT]].setPromoted(false);
	
			int * temp = new int[4]; 
			piece[board120[mT]].freeMoveList();
			piece[board120[mT]].setMoveList(temp);
			piece[board120[mT]].setMoveListSize(4);
		}	
		else { 
			
			bb.pieces[s][piece[board120[mT]].getType()] &= ~bb.sq[to64(mT) - 1];
			bb.pieces[s][piece[board120[mT]].getType()] |= bb.sq[to64(mF) - 1];
			bb.allPieces[s] &= ~bb.sq[to64(mT) - 1];
			bb.allPieces[s] |= bb.sq[to64(mF) - 1];
		}
		
		if (mTVal == R_VAL && piece[board120[mT]].getMoved() == 1) {
			
            int king;
            if (s)
                king = wK;
            else king = bK;

		}
		
		else if (mTVal == K_VAL && piece[board120[mT]].getMoved() == 1) {

            int kRook, qRook;
            if (s)
                kRook = wkR;
            else kRook = bkR;
    
		}
		
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);

		piece[board120[mF]].decrMoved();
	
		board120[mT] = moveInfo.back().prevOnMoveTo;

		if (epExtra){
			board120[mT + epExtra] = mT % 10 - 1;
            if (s == 0)
                board120[mT + epExtra] += wPa;
            else board120[mT + epExtra] += bPa;
        }
		
		if (epExtra || board120[mT] != none) { 
			
			piece[board120[mT + epExtra]].setPos(mT + epExtra);
			piece[board120[mT + epExtra]].unkill();
			if (!s) 
				whiteMaterial += piece[board120[mT + epExtra]].getValue();
			else
				blackMaterial += piece[board120[mT + epExtra]].getValue();
			
			bb.pieces[!s][piece[board120[mT + epExtra]].getType()] |= bb.sq[to64(mT + epExtra)-1];
			bb.allPieces[!s] |= bb.sq[to64(mT + epExtra)-1];

			if (board120[mT] != none && piece[board120[mT]].getValue() == R_VAL && piece[board120[mT]].getMoved() == 0) {
		
				int eKing;
                if (s)
                    eKing = bK;
                else eKing = wK;
			}
			else if (board120[mT] != none && piece[board120[mT]].getValue() == K_VAL && piece[board120[mT]].getMoved() == 0) {

                int ekRook, eqRook;
                if (s){
                    ekRook = bkR;
                    eqRook = bqR;
                }
                else {
                    ekRook = wkR;
                    eqRook = wqR;
                }

			}

		}

	}
	else { 
		if (s) 
            whiteCastled = false;
		else blackCastled = false;
	
		int cExtras[2];
		if (castling == KINGSIDE) {
			cExtras[0] = 1; 
			cExtras[1] = 3; 
			
		}
		else {
			cExtras[0] = -1; 
			cExtras[1] = -4; 
			
		}
		
		bb.pieces[s][KING] &= ~bb.sq[to64(mT) - 1];
		bb.pieces[s][KING] |= bb.sq[to64(mF) - 1];
		bb.allPieces[s] &= ~bb.sq[to64(mT) - 1];
		bb.allPieces[s] |= bb.sq[to64(mF) - 1];
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);
		piece[board120[mF]].decrMoved();
		board120[mT] = none;

		bb.pieces[s][ROOK] &= ~bb.sq[to64(mF + cExtra[0]) - 1];
		bb.pieces[s][ROOK] |= bb.sq[to64(mF + cExtra[1]) - 1];
		bb.allPieces[s] &= ~bb.sq[to64(mF + cExtra[0]) - 1];
		bb.allPieces[s] |= bb.sq[to64(mF + cExtra[1]) - 1];
		board120[mF+cExtra[1]] = board120[mF + cExtra[0]];
		piece[board120[mF+cExtra[1]]].setPos(mF+cExtra[1]);
		piece[board120[mF+cExtra[1]]].decrMoved();	
		board120[mF+cExtra[0]] = none;

		castling = 0;
	}

	movesMade.pop_back();
	moveInfo.pop_back();
	--ply;
}