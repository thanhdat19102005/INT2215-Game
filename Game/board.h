#ifndef BOARH_H
#define BOARD_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "bitboard.h"
#include "display.h"

using namespace std;

class Board {

    public:
        
        Board();
        ~Board();
        Board(string FEN);
        void initiallizeVars();
        void emptyBoard();
        void botMove();
        void placePiece(unsigned int p, unsigned int sq);
        void placePieceDefault();
        void initializePiece();
        void initiallizeBitboard();
        void handleInput(int& mF, int& mT, SDL_Event* e);
        void setSquarePosition();
        void setPieceOnSquare();
        void changeTurn();
        void undoMove();
        void restart();
        void updateDisplay(const int& mF, const int& mT);

        Display display;

    private:
        int board120[120];

};

#endif