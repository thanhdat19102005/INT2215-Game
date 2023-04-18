#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include "button.h"
#include "ltexture.h"
#include "square.h"

using namespace std;

class Display {

    friend class Board;

    public:
        Display(Board* b);
        ~Display();

        void displayBoard(const int& mF = -1, const int& mT = -1);
        void setButton();
        void setSpriteClips();
        void displayBotText();
        void updateText();
        void drawButtons();
        void handleButtons(SDL_EVent* e);


        bool init_SDL();
        bool loadMedia();
        void close_SDL();
        SDL_Texture* loadTexture(string path);

        int getBoardXStart(){
            return BXSTART;
        };
        int getBoardYStart(){
            return BYSTART;
        };
        int getSqSize() {
            return SQ_SIZE;
        };
        int getBoardSize(){
            return B_SIZE;
        };

        Square squares[64];

    private:
        Board* boardPtr;
        const static int B_SIZE = 600, SQ_SIZE = 75, BXSTART = 50, BYSTART = 50;
        const static int SCREEN_W = 1250;
		const static int SCREEN_H = 700;

        SDL_Window* Window;
        SDL_Renderer* renderer;
        TTF_Font* Garamond26, *Garamond28, *Cicero22, *Cicero26;
        Mix_Chunk* mTSound, *mFSound;
        SDL_Rect spriteClip[12], buttonClip[6], titleTextClip[28];
        Button button[26];
        LTexture spriteSheetTexture, buttonTexture, titleTexture, titleTextTexture, turnText, checkText, moveText, rankText, fileText;
        SDL_Color textColor;
        string rankStr, fileStr;
};

extern Mix_Chunk* mFSound, *mTSound;

#endif