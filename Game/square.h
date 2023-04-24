#ifndef SQUARE_H
#define SQUARE_H

#include <SDL.h>

class LTexture;

enum PieceSprite{

    noPiece = -1, wRook = 0, wKnight, wBishop, wKing, wPawn, wQueen,
    bKing, bKnight, bRook, bQueen, bBishop, bPawn
};

class Square{

    public:
        Square();
        ~Square();

        void handleEvent(SDL_Event* e, int& mF, int& mT, const bool& s,int& sound);
        int getX() const {
            return pos.x;
        };
        int getY() const{
            return pos.y;
        };
        bool getDragging() const{
            return dragging;
        };
        int getPiece() const{
            return piece;
        };
        int setPos(int x, int y);
        void setPiece(int p){
            piece = p;
        };
        void setSq(int i){
            square = i;
        };
        void setDragging(bool x){
            dragging = x;
        };

        private: 
            bool dragging;
            int square, piece;
            SDL_Point pos;
};

#endif