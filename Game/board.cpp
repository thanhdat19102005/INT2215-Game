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
            
        }
    }
}

void Board::setPieceOnSquare(){
    
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

void Board::initiallizeBitboar(){
    int pos, t;
    for (int i = BLACK;i <= WHITE;++i){
        for (int j = bqR - (i * 16);j <= bPh - (i * 16);++j){
            if (piece[p])
        }
    }   
}