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

        int getMoveFrom() const{
            return moveFrom;
        };

        int getMoveTo() const{
            return moveTo;
        };

        int getPly() const {
            return ply;
        };

        bool getSide() const {
            return side;
        };

        int getSideInCheck() const{
            return sideInCheck;
        };

        int getSideInCheckmate() const{
            return sideInCheckmate;
        };

        int getCastling() const {
            return castling;
        };

        int getWhiteMaterial() const{
            return whiteMaterial;
        };

        
        int getBlackMaterial() const{
            return blackMaterial;
        };

        int getFromMoveList(bool s, int i) const;
        int getMoveListSize(bool s) const;
        int getEpSq(int i) const;
        int getPmSq(int i) const;
        int getMoveMade(int i) const;
        int getLastMove() const;
        int getNumMovesmade() const;
        int getPreviousOnMoveTo(int i) const;
        int getPieceMoved(int i) const;
        std::string getFen();
        bool getWhiteIsBot() const{
            return whiteIsBot;
        };
        bool getBlackIsBot() const{
            return blackIsBot;
        }; 
        bool getFlipped() const{
            return flipped;
        };

        void setMove(int mF, int mT) {
            moveFrom = mF;
            moveTo = mT;
        };

        void setPly(int newPly){
            ply = newPly;
        };

        void setSide(bool newSide){
            side = newSide;
        };

        void setCastling(int c){
            castling = c;
        };

        void setSideInCheck(int i){
            sideInCheck = i;
        };

        void setSideInCheckmate(int i){
            sideInCheckmate = i;
        };

        void addToMoveList(bool s, int v);
        void clearMoveList(bool s);
        void setWhiteIsBot(bool b){
            whiteIsBot = b;
        };
        void SetBlackIsBot(bool b){
            blackIsBot = b;
        };
        void flipBoard(){
            flipped = 1 - flipped;
        };

        int& operator[](const int index) const;
        const int& operator[](const int index) const;

        Display display;

    private:
        int board120[120], moveFrom, moveTo, ply;
        int whiteMaterial, blackMaterial, castling;

        int sideInCheck, sideInCheckmate;
        bool side, whiteCastled, blackCastled;
        bool flipped, whiteIsBot, blackIsBot;
        
        Bitboards bb;
};

#endif