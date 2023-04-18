#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "common.h"
#include "board.h"

using namespace std;

Board::Board() : display(this){

    initializerVars();
    emptyBoard();
    initializePiece();
    placePieceDefault();
    setSquarePosition();
    initiallizeBitboard();
    display.setSpriteClips();
    display.setButton();
}

Board::Board(string FEN){

    cout << "Loading FEN" << FEN << "\n";
    initiallizeVars();
    emptyBoard();
    initializePiece();
    setSquarePosition();
    display.setSpriteClips();
    display.setButtons();
    placePiece(FEN);
}

void Board::updateDisplay(const int& mF, const int& mT){
    display.displayBoard(mF, mT);
}

void Board::setSquarePosition(){

    if (!flipped){
        for (int i = 0;i < 64;++i){
            display.square[i].setPos(display.getBoardXStart() + (display.getSqSize() * (i % 8)), display.getBoardYStart() + display.getBoardSize() - (display.getSqSize() * (i / 8 + 1)));
            display.square[i].setSq(i + 1);  
        }
    }
    else {
        for (int i = 0;i < 64;++i) {
			display.square[i].setPos(display.getBoardXStart()+display.getBoardSize() - (display.getSqSize()*(i%8+1)), display.getBoardYStart()+(display.getSqSize()*(i/8)));
			display.squares[i].setSq(i+1);
		}
    }
}

void Board::setPieceOnSquare(){
    for (inti i = 0;i < 64;++i)
        display.square[i].setPiece(board120[from64(i + 1)]);
}

void Board::initializeVars(){

    moveFrom = 0;
    moveTo = 0;
    ply = 0;
    side = WHITE;
    whiteIsBot = false;
    blackIsBot = true;
    whiteCastled = false;
    blackCastled = false;
    castling = 0;
    sideInCheck = 0;
    sideInCheckmate = 0;
    whiteMaterial = 8 * P_VAL + 2 * (R_VAL + B_VAL + N_VAL) + Q_VAL + K_VAL;
    blackMaterial = whiteMaterial;
}

void Board::initiallizeBitboard(){
    int pos, t;
    for (int i = BLACK;i <= WHITE;++i){
        for (int j = bqR - (i * 16);j <= bPh - (i * 16);++j){
            if (piece[p].getAlive()){
                pos = to64(piece[j].getPos()) - 1;
                t = piece[j].getType();
                bb.pieces[i][j] |= bb.sq[pos];
                bb.allPieces[i][j] |= bb.sq[pos]; 
            }
        }
    }   
}

void Board::emptyBoard(){

    for (int i = 0;i < 120;++i)
        if (i < 21 || i > 98)
            board120[i] = invalid;
        else if (i % 10 == 0 || i % 10 == 9)
            board120[i] = invalid;
        else board120[i] = none;
}

void Board::placePiece(unsigned int p, unsigned int sq){
    piece[p].setPos(sq);
    board120[sq] = p;
}

void Board::placePieceDefault(){

    for (int i = A1;i <= H1;++i)
        placePiece(i - A1, i);
    for (int i = A2;i <= A2;++i)
        placePiece(i - A2 + 8, i);
    for (int i = A8;i <= H8;++i)
        placePiece(i - A8 + 16, i);
    for (int i = A7;i <= H7;++i)
        placePiece(i - A7 + 24, i);
}

void Board::initializePiece(){

    int v;

    for (int i = 0; i <= 16; i += 16) {
		piece[wqR+i].setName("Rook");
		piece[wqR+i].setAbbr('R' + 2*i);
		piece[wqR+i].setValue(R_VAL);
		piece[wqR+i].setType(ROOK);

		piece[wqN+i].setName("Knight");
		piece[wqN+i].setAbbr('N' + 2*i);
		piece[wqN+i].setValue(N_VAL);
		piece[wqN+i].setType(KNIGHT);

		piece[wqB+i].setName("Bishop");
		piece[wqB+i].setAbbr('B' + 2*i);
		piece[wqB+i].setValue(B_VAL);
		piece[wqB+i].setType(BISHOP);

		piece[wQ+i].setName("Queen");
		piece[wQ+i].setAbbr('Q' + 2*i);
		piece[wQ+i].setValue(Q_VAL);
		piece[wQ+i].setType(QUEEN);

		piece[wK+i].setName("King");
		piece[wK+i].setAbbr('K' + 2*i);
		piece[wK+i].setValue(K_VAL);
		piece[wK+i].setType(KING);

		piece[wkB+i].setName("Bishop");
		piece[wkB+i].setAbbr('B' + 2*i);
		piece[wkB+i].setValue(B_VAL);
		piece[wkB+i].setType(BISHOP);

		piece[wkN+i].setName("Knight");
		piece[wkN+i].setAbbr('N' + 2*i);
		piece[wkN+i].setValue(N_VAL);
		piece[wkN+i].setType(KNIGHT);

		piece[wkR+i].setName("Rook");
		piece[wkR+i].setAbbr('R' + 2*i);
		piece[wkR+i].setValue(R_VAL);
		piece[wkR+i].setType(ROOK);
	}
	for (int s = 0; s <= 16; s += 16) 
		for (int i = 0; i < 8; i++) { 
			piece[wPa+i+s].setName("Pawn");
			piece[wPa+i+s].setAbbr('P' + 2*s);
			piece[wPa+i+s].setValue(P_VAL);
			piece[wPa+i+s].setType(PAWN);
		}


    for (int i = wqR;i <= wPh;++i)
        piece[i].setColor(WHITE);
    for (int i = bqR;i <= bPh;++i)
        piece[i].setColor(BLACK);
    
    for (int i = wqR;i <= bPh;++i){
        piece[i].freeMoveList();
        v = piece[i].getValue();

        if (v == K_VAL)
            piece[i].setMoveListSize(10);
        else if (v == Q_VAL)
            piece[i].setMoveListSize(27);
        else if (v == R_VAL)
            piece[i].setMoveListSize(14);
        else if (v == B_VAL)
            piece[i].setMoveListSize(13);
        else if (v == N_VAL)
            piece[i].setMoveListSize(8);
        else if (v == P_VAL)
            piece[i].setMoveListSize(4);
        piece[i].setMoveList(new int[piece[i].getMoveListSize()]);
    }
}

void Board::placePieces(std::string FEN){

    unsigned int index = 0, sqCounter = A8, p;
    int wPIndex = wPa, bPIndex = bPa;

    while (FEN[index] != ' '){
        if (FEN[index] == '/')
            sqCounter -= 18;
        else if (isalpha(FEN[index])){
            if (FEN[index] == 'Q')
                p = wQ;
            else if (FEN[index] == 'K')
                p = wK;
            else if (FEN[index] == 'R'){
                if (piece[wqR].getPos())
                    p = wkR;
                else p = wqR;
            }
            else if (FEN[index] == 'N'){
                if (piece[wqN].getPos())
                    p = wkN;
                else p = wqN;
            }
            else if (FEN[index] == 'B')
                if (piece[wqB].getPos())
                    p = wkB;
                else p = wqB;
            else if (FEN[index] == 'P'){
                p = wPIndex;
                ++wPIndex;
            }
            else if (FEN[index] == 'q')
                p = bQ;
            else if (FEN[index] == 'k')
                p = bK;
            else if (FEN[index] == 'r'){
                if (piece[bqR].getPos())
                    p = bkR;
                else p = bqR;
            }
            else if (FEN[index] == 'n'){
                if (piece[bqN].getPos())
                    p = bkN;
                else p = bqN;
            }
            else if (FEN[index] == 'b')
                if (piece[bqB].getPos())
                    p = bkB;
                else p = wqB;
            else if (FEN[index] == 'p'){
                p = bPIndex;
                ++bPIndex;
            } 

            placePiece(p, sqCounter);
            ++sqCounter; 
            
        }
        else sqCounter += FEN[index] - '0';
        ++index;
    }

    whiteMaterial = 0;
    blackMaterial = 0;

    for (int i = wqR;i <= bPh;++i){
        if (!piece[i].getPos())
            piece[i].kill();
        if (!piece[i].getAlive())
            continue;
        
        if (i <= wPh)
            whiteMaterial += piece[i].getValue();
        else blackMaterial += piece[i].getValue();   

        if (piece[i].getValue() == P_VAL){
            if (i <= wPh){
                if (piece[i].getPos() / 10 != 3)
                    piece[i].incrMoved();
            }
            else {
                if (piece[i].getPos() / 10 != 8)
                    piece[i].incrMoved();
            }
        } 
    }

    ++index;
    if (FEN[index] == 'w')
        side = 1;
    else side = 0;

    index += 2;
    piece[wqR].incrMoved();
    piece[wkR].incrMoved();
    piece[bqR].incrMoved();
    piece[bkR].incrMoved();

    while (FEN[index] != ' '){
        if (FEN[index] == 'K')
            piece[wkR].decrMoved();
        else if (FEN[index] == 'Q')
            piece[wqR].decrMoved();
        else if (FEN[index] == 'k')
            piece[bkR].decrMoved();
        else if (FEN[index] == 'q')
            piece[bqR].decrMoved();
        ++index;       
    }

    int epSq = 0;

    ++index; 
    if (FEN[index] != '-'){
        int row, file;
        file = FEN[index] - 'a' + 1;
        ++index;
        row = FEN[index] - '0' + 1;
        epSq = row * 10 + file;
    }
    ++index;

    ++index;
    int halfMoveClock = FEN[index] - '0';
    ++index;
    while (FEN[index] != ' '){
        halfMoveClock = halfMoveClock * 10;
        ++index;
        halfMoveClock += FEN[index] - '0';
        ++index;
    }

    ++index;
    int moves;
    moves = FEN[index] - '0';
    while (index + 1 != FEN.size() && FEN[index + 1] != ' '){
        moves = movés * 10;
        ++index;
        moves += FEN[index] - '0';
    }

    ply = (move - 1) * 2;
    if (!side) 
        ++ply;
    
    moveInfo.push_back({0, epSq, -1, -1, halfMoveClock});

}

void Board::handleInput(int& mF, int& mT, SDL_Event* e){

    display.handleButtons(e);

    int sound = 0;
    for (int i = 0;i < 64;++i){
        display.squares[i].handleEvent(e, mF, mT, side, sound);
        if (sound == 1)
            Mix_PlayChannel(-1, display.mFSound, 0);
        else if (sound == 2)
            Mix_PlayChannel(-1, display.mTSound, 0);
    }

    if (mF != -1 && mT != -1){
        mF = from64(mF);
        mT = from64(mT);
        if (legalMove())
    }
}