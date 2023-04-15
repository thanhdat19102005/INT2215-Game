#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include "common.h"
#include "board.h"

using namespace std;

Board::Board(string FEN){

    cout << "Loading FEN" << FEN << "\n";
    initiallizeVars();
    emptyBoard();
    initializePiece();
    setSquarePosition();
    placePiece(FEN);
    
}

void Board::updateDisplay(const int& mF, const int& mT){

}