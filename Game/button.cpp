#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "button.h"
#include "board.h"
#include "display.h"
#include "common.h"

Button::Button(){

    pos.x = 0;pos.y = 0;
    w = 0;h = 0;
    inside = false;
    clicking = false;
    butt = 0;
    boardPtr = NULL;
}

void Button::setPos(int x, int y){

    pos.x = x;
    pos.y = y;
}

void Button::setSize(){
    setW(x);
    setY(y);
}

bool Button::handleEvent(SDL_Event *e, int& sound){
    int x, y;
    SDL_GetMouseState(&x, &y);
    inside = true;
    if (x < pos.x)
        inside = false;
    else if (x > pos.x + w)
        inside = false;
    else if (y < pos.y)
        inside = false;
    else if (y > pos.y + h)
        inside = false;
    
    if (inside){
        switch(e->type){
            case SDL_MOUSEBUTTONDOWN:
            clicking = true;
            
            if (butt == 0)
                boardPtr->restart();
                return true;

            if (butt == 1){
                boardPtr->undoMove();
            }
            else if (butt == 2 && !start)
                boardPtr->setWhiteIsBot(false);
            else if (butt == 3 && !start)
                boardPtr->setWhiteIsBot(true);
            else if (butt == 4 && !start)
                boardPtr->setBlackIsBot(true);
            else if (butt >= 6 && butt <= 14 && boardPtr->getWhiteIsBot() && !start())
                boardPtr->whiteBot.setLevel(butt - 5);
            else if (butt >= 15 && butt <= 23 && boardPtr->getBlackIsBot() && !start)
                boardPtr->blackBot.setlevel(butt-14);
            else if (butt == 24 && !start)
                boardPtr->flipBoard();
            else if (butt == 25)
                start = true;
            break;
            case SDL_MOUSEBUTTONUP:
            clicking = false;
            break;
        }

    }
    return false;
}