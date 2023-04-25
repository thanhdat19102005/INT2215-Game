#include <iostream>
#include "common.h"
#include "board.h"

void Board::movePiece() {
	movePiece(moveFrom, moveTo);
}

void Board::movePiece(int mF, int mT) {
	bool s = piece[board120[mF]].getColor();
	int type = piece[board120[mF]].getType();
	int  mFVal = piece[board120[mF]].getValue(), epExtra = 0;
	int localEpSq = null, localPmSq = null;
	int localPrevOnMoveTo = board120[mT], localHalfMoveClock;
	
	localHalfMoveClock = moveInfo.size() == 0 ? 0 : moveInfo.back().halfMoveClock+1;

	movesMade.push_back(mF*100+mT);
	
	if (moveInfo.size() && moveInfo.back().epSq != 0) {
		zobrist.key ^= zobrist.enPassant[moveInfo.back().epSq%10-1];
	}

	if (mFVal == K_VAL && abs(mF-mT) == 2)
		castling = mF < mT ? KINGSIDE : QUEENSIDE;
	
	if (!castling) {	
		zobrist.key ^= zobrist.piece[type][s][to64(mF)-1];

		if (mFVal == P_VAL) {
			if (moveInfo.size() && mT == moveInfo.back().epSq)
				epExtra = s ? -10 : 10;
			else if (abs(mF-mT) == 20) {
				localEpSq = s ? mF+10 : mF-10;
				zobrist.key ^= zobrist.enPassant[mF%10-1];
			}
			localHalfMoveClock = 0;
		}
		else if (mFVal == R_VAL && piece[board120[mF]].getMoved() == 0) {
			int king = s ? wK : bK;
			if (piece[king].getAlive() && piece[king].getMoved() == 0) {
				//Queenside
				if (king - board120[mF] > 0) {
					zobrist.key ^= zobrist.castling[s][1];
				}
				//Kingside
				else {
					zobrist.key ^= zobrist.castling[s][0];
				}
			}
		}
		else if (mFVal == K_VAL && piece[board120[mF]].getMoved() == 0) {
			int kRook = s ? wkR : bkR;
			int qRook = s ? wqR : bqR;
			//Kingside
			if (piece[kRook].getAlive() && piece[kRook].getMoved() == 0) {
				zobrist.key ^= zobrist.castling[s][0];
			}
			//Queenside
			if (piece[qRook].getAlive() && piece[qRook].getMoved() == 0) {
				zobrist.key ^= zobrist.castling[s][1];
			}
		}

		if (mFVal == P_VAL && ((s && mT/10 == 9) || (!s && mT/10 == 2))) {
			zobrist.key ^= zobrist.piece[3][s][to64(mT)-1];
			bb.pieces[s][QUEEN] |= bb.sq[to64(mT)-1];
			bb.allPieces[s] |= bb.sq[to64(mT)-1];

			localPmSq = mT;
			piece[board120[mF]].setValue(Q_VAL);
			piece[board120[mF]].setName("Queen");
			piece[board120[mF]].setType(QUEEN);
			if (s) {
				piece[board120[mF]].setAbbr('Q');
				whiteMaterial += Q_VAL - P_VAL;
			}
			else {
				piece[board120[mF]].setAbbr('q');
				blackMaterial += Q_VAL - P_VAL;
			}
			piece[board120[mF]].setPromoted(true);

			int * temp = new int[27];
			for (int i = 0; i < 4; i++)
				temp[i] = piece[board120[mF]].getFromMoveList(i);
			piece[board120[mF]].freeMoveList();
			piece[board120[mF]].setMoveList(temp);
			piece[board120[mF]].setMoveListSize(27);
			for (int i = 4; i < 27; i++ )
				piece[board120[mF]].setInMoveList(i, 0);
		}
		else {
			zobrist.key ^= zobrist.piece[type][s][to64(mT)-1];
			bb.pieces[s][piece[board120[mF]].getType()] |= bb.sq[to64(mT)-1];
			bb.allPieces[s] |= bb.sq[to64(mT)-1];
		}

		if (board120[mT+epExtra] != none) {
			if (!s) 
				whiteMaterial -= piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial -= piece[board120[mT+epExtra]].getValue();

			if (piece[board120[mT+epExtra]].getValue() == R_VAL) {
				int eKing = s ? bK : wK;
				if (piece[eKing].getAlive() && piece[eKing].getMoved() == 0) {
					int eRook = board120[mT+epExtra];
					if (piece[eRook].getAlive() && piece[eRook].getMoved() == 0) {
						//Queenside
						if ((eKing - eRook) > 0) {
							zobrist.key ^= zobrist.castling[!s][1];
						}
						//Kingside
						else {
							zobrist.key ^= zobrist.castling[!s][0];
						}
					}
				}
			}
			else if (piece[board120[mT+epExtra]].getValue() == K_VAL) {
				int eKing = s ? bK : wK;
				int ekRook = s ? bkR : wkR;
				int eqRook = s ? bqR : wqR;
				if (piece[eKing].getMoved() == 0) {
					//Kingside
					if (piece[ekRook].getAlive() && piece[ekRook].getMoved() == 0) {
						zobrist.key ^= zobrist.castling[!s][0];
					}
					//Queenside
					if (piece[eqRook].getAlive() && piece[eqRook].getMoved() == 0) {
						zobrist.key ^= zobrist.castling[!s][1];
					}
				}
			}

			zobrist.key ^= zobrist.piece[piece[board120[mT+epExtra]].getType()][!s][to64(mT+epExtra)-1];
			bb.pieces[!s][piece[board120[mT+epExtra]].getType()] &= ~bb.sq[to64(mT+epExtra)-1];
			bb.allPieces[!s] &= ~bb.sq[to64(mT+epExtra)-1];
			piece[board120[mT+epExtra]].kill();
			piece[board120[mT+epExtra]].setPos(null);
			if (epExtra != 0)
				board120[mT+epExtra] = none;
	
			localHalfMoveClock = 0;
		}
	
		bb.pieces[s][piece[board120[mF]].getType()] &= ~bb.sq[to64(mF)-1];
		bb.allPieces[s] &= ~bb.sq[to64(mF)-1];
		board120[mT] = board120[mF];
		piece[board120[mT]].setPos(mT);
		piece[board120[mT]].incrMoved();
		board120[mF] = none;
	}
	else {
		if (s) whiteCastled = true;
		else blackCastled = true;

		int cExtras[2];
		if (castling == KINGSIDE) {
			cExtras[0] = 1;  
			cExtras[1] = 3;  

			zobrist.key ^= zobrist.castling[s][0];
			if (s) {
				if (piece[wqR].getAlive() &&
				    piece[wqR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[WHITE][1];
				}
			}
			else {
				if (piece[bqR].getAlive() &&
				    piece[bqR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[BLACK][1];
				}
			}
		}
		else {
			cExtras[0] = -1; 
			cExtras[1] = -4;
			zobrist.key ^= zobrist.castling[s][1];
			if (s) {
				if (piece[wkR].getAlive() &&
				    piece[wkR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[WHITE][0];
				}
			}
			else {
				if (piece[bkR].getAlive() &&
				    piece[bkR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[BLACK][0];
				}
			}
		}
		zobrist.key ^= zobrist.piece[KING][s][to64(mF)-1];	      //Remove king
		zobrist.key ^= zobrist.piece[KING][s][to64(mT)-1]; 	      //Place king
		zobrist.key ^= zobrist.piece[ROOK][s][to64(mF+cExtras[1])-1]; //Remove rook
		zobrist.key ^= zobrist.piece[ROOK][s][to64(mF+cExtras[0])-1]; //Place rook
	
		board120[mT] = board120[mF];		 	    
		piece[board120[mT]].setPos(mT); 
		piece[board120[mT]].incrMoved();
		bb.pieces[s][KING] &= ~bb.sq[to64(mF)-1];
		bb.pieces[s][KING] |= bb.sq[to64(mT)-1];
		bb.allPieces[s] &= ~bb.sq[to64(mF)-1];
		bb.allPieces[s] |= bb.sq[to64(mT)-1];

		board120[mF+cExtras[0]] = board120[mF+cExtras[1]];  
		piece[board120[mF+cExtras[0]]].setPos(mF+cExtras[0]);
		piece[board120[mF+cExtras[0]]].incrMoved();
		bb.pieces[s][ROOK] &= ~bb.sq[to64(mF+cExtras[1])-1];
		bb.pieces[s][ROOK] |= bb.sq[to64(mF+cExtras[0])-1];
		bb.allPieces[s] &= ~bb.sq[to64(mF+cExtras[1])-1];
		bb.allPieces[s] |= bb.sq[to64(mF+cExtras[0])-1];

		board120[mF] = none;				    
		board120[mF+cExtras[1]] = none;		    
		castling = 0;
		localHalfMoveClock = 0;
	}

	zobrist.key ^= zobrist.side;
	moveInfo.push_back({localPmSq, localEpSq, board120[mT],
			    localPrevOnMoveTo, localHalfMoveClock, zobrist.key});
	ply++;
}

void Board::unmovePiece() {
	unmovePiece(movesMade.back()/100, movesMade.back()%100);
}

void Board::unmovePiece(int mF, int mT) {
	int epExtra = 0;
	bool s = piece[board120[mT]].getColor();
	int type = piece[board120[mT]].getType();
	int mTVal = piece[board120[mT]].getValue();

	if (moveInfo.back().epSq != 0) {
		zobrist.key ^= zobrist.enPassant[moveInfo.back().epSq%10-1];
	}

	if (moveInfo.size() > 1 && moveInfo[moveInfo.size()-2].epSq != 0) {
		zobrist.key ^= zobrist.enPassant[moveInfo[moveInfo.size()-2].epSq%10-1];
	}

	if (mTVal == K_VAL && abs(mF-mT) == 2)
			castling = mF < mT ? KINGSIDE : QUEENSIDE;

	if (!castling) {

		if (mTVal == P_VAL && moveInfo.back().prevOnMoveTo == none) {
			int diffMTMF = abs(mT - mF);
			if (diffMTMF == 11 || diffMTMF == 9) {
				epExtra = s ? -10 : 10;
			}
		}
		if (mT == moveInfo.back().pmSq) {
			zobrist.key ^= zobrist.piece[type][s][to64(mT)-1];
			bb.pieces[s][QUEEN] &= ~bb.sq[to64(mT)-1];
			bb.allPieces[s] &= ~bb.sq[to64(mT)-1];
			zobrist.key ^= zobrist.piece[5][s][to64(mF)-1];
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
			zobrist.key ^= zobrist.piece[type][s][to64(mT)-1];
			zobrist.key ^= zobrist.piece[type][s][to64(mF)-1];
			bb.pieces[s][piece[board120[mT]].getType()] &= ~bb.sq[to64(mT)-1];
			bb.pieces[s][piece[board120[mT]].getType()] |= bb.sq[to64(mF)-1];
			bb.allPieces[s] &= ~bb.sq[to64(mT)-1];
			bb.allPieces[s] |= bb.sq[to64(mF)-1];
		}
		if (mTVal == R_VAL && piece[board120[mT]].getMoved() == 1) {
			int king = s ? wK : bK;
			if (piece[king].getAlive() && piece[king].getMoved() == 0) {
				//Queenside
				if ((king - board120[mT]) > 0) {
					zobrist.key ^= zobrist.castling[s][1];
				}
				//Kingside
				else {
					zobrist.key ^= zobrist.castling[s][0];
				}
			}
		}
		else if (mTVal == K_VAL && piece[board120[mT]].getMoved() == 1) {
			int kRook = s ? wkR : bkR;
			int qRook = s ? wqR : bqR;
			//Kingside
			if (piece[kRook].getAlive() && piece[kRook].getMoved() == 0) {
				zobrist.key ^= zobrist.castling[s][0];
			}
			//Queenside
			if (piece[qRook].getAlive() && piece[qRook].getMoved() == 0) {
				zobrist.key ^= zobrist.castling[s][1];
			}
		}
		
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);

		piece[board120[mF]].decrMoved();
	
		board120[mT] = moveInfo.back().prevOnMoveTo;

		if (epExtra != 0)
			board120[mT+epExtra] = mT%10 - 1 + (!s ? wPa : bPa);
		
		if (epExtra != 0 || board120[mT] != none) { 
			piece[board120[mT+epExtra]].setPos(mT+epExtra);
			piece[board120[mT+epExtra]].unkill();
			if (!s) 
				whiteMaterial += piece[board120[mT+epExtra]].getValue();
			else
				blackMaterial += piece[board120[mT+epExtra]].getValue();
			zobrist.key ^= zobrist.piece[piece[board120[mT+epExtra]].getType()][!s][to64(mT+epExtra)-1];
			bb.pieces[!s][piece[board120[mT+epExtra]].getType()] |= bb.sq[to64(mT+epExtra)-1];
			bb.allPieces[!s] |= bb.sq[to64(mT+epExtra)-1];

			if (board120[mT] != none &&
			    piece[board120[mT]].getValue() == R_VAL && 
			    piece[board120[mT]].getMoved() == 0) {
				int eKing = s ? bK : wK;
				if (piece[eKing].getAlive() && piece[eKing].getMoved() == 0) {
					if ((eKing - board120[mT]) > 0) {
						zobrist.key ^= zobrist.castling[!s][1];
					}
					else {
						zobrist.key ^= zobrist.castling[!s][0];
					}
				} 
			}
			else if (board120[mT] != none &&
				 piece[board120[mT]].getValue() == K_VAL &&
				 piece[board120[mT]].getMoved() == 0) {
				int ekRook = s ? bkR : wkR;
				int eqRook = s ? bqR : wqR;
				if (piece[ekRook].getAlive() && piece[ekRook].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[!s][0];
				}
				if (piece[eqRook].getAlive() && piece[eqRook].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[!s][1];
				}
			}

		}

	}
	else { 
		if (s) whiteCastled = false;
		else blackCastled = false;
	
		int cExtras[2];
		if (castling == KINGSIDE) {
			cExtras[0] = 1; 
			cExtras[1] = 3; 
			zobrist.key ^= zobrist.castling[s][0];
			if (s) {
				if (piece[wqR].getAlive() && piece[wqR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[WHITE][1];
				}
			}
			else {
				if (piece[bqR].getAlive() && piece[bqR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[BLACK][1];
				}
			}
		}
		else {
			cExtras[0] = -1; 
			cExtras[1] = -4; 
			zobrist.key ^= zobrist.castling[s][1];
			if (s) {
				if (piece[wkR].getAlive() && piece[wkR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[WHITE][0];
				}
			}
			else {
				if (piece[bkR].getAlive() && piece[bkR].getMoved() == 0) {
					zobrist.key ^= zobrist.castling[BLACK][0];
				}
			}
		}
		
		//Update zobrist key
		zobrist.key ^= zobrist.piece[4][s][to64(mT)-1]; 	   //Remove king
		zobrist.key ^= zobrist.piece[4][s][to64(mF)-1]; 	   //Place king
		zobrist.key ^= zobrist.piece[0][s][to64(mF+cExtras[0])-1]; //Remove rook
		zobrist.key ^= zobrist.piece[0][s][to64(mF+cExtras[1])-1]; //Place rook
	
		//Move the king back
		bb.pieces[s][KING] &= ~bb.sq[to64(mT)-1];
		bb.pieces[s][KING] |= bb.sq[to64(mF)-1];
		bb.allPieces[s] &= ~bb.sq[to64(mT)-1];
		bb.allPieces[s] |= bb.sq[to64(mF)-1];
		board120[mF] = board120[mT];
		piece[board120[mF]].setPos(mF);
		piece[board120[mF]].decrMoved();
		board120[mT] = none;

		//Move the rook back
		bb.pieces[s][ROOK] &= ~bb.sq[to64(mF+cExtras[0])-1];
		bb.pieces[s][ROOK] |= bb.sq[to64(mF+cExtras[1])-1];
		bb.allPieces[s] &= ~bb.sq[to64(mF+cExtras[0])-1];
		bb.allPieces[s] |= bb.sq[to64(mF+cExtras[1])-1];
		board120[mF+cExtras[1]] = board120[mF+cExtras[0]];
		piece[board120[mF+cExtras[1]]].setPos(mF+cExtras[1]);
		piece[board120[mF+cExtras[1]]].decrMoved();	
		board120[mF+cExtras[0]] = none;

		castling = 0;
	}

	zobrist.key ^= zobrist.side;
	movesMade.pop_back();
	moveInfo.pop_back();
	ply--;
}
