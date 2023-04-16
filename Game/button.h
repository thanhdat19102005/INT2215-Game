#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

class Board;

class Button{

    public:

        Button();

        bool handleEvent(SDL_Event* e, int& sound);

        int getX() const {
            return pos.x;
        };
        int getY() const{
            return pos.y;
        };
        int getW() const{
            return w;
        };
        int getH() const{
            return h;
        };
        
        bool getInside() const{
            return inside;
        };
        bool getClicking() const{
            return clicking;
        };
        void setInside(bool b){
            inside = b;
        };
        void setClicking(bool b){
            clicking = b;
        };
        void setButt(int b){
            butt = b;
        };
        void setPos(int x, int y);
        void setW(int x){
            if (x > 0)
                w = x;
            else w = 0;

        };
        void setH(int x){
            if (x > 0)
                h = x;
            else h = 0;
        };
        void setSize(int x, int y);
        void setBoardPtr(Board* b){
            boardPtr = b;
        };

    private:
        Board* boardPtr;
        int butt;
        int w, h;
        bool inside, clicking;
        SDL_Point pos;
};

#endif