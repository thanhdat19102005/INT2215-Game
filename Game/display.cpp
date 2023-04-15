#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "board.h"
#include "common.h"
#include "display.h"

using namespace std;

// Display::Display(Board* b) : boardPtr(b){

//     textColor = { 0, 0, 0};
//     sideFlag = !boardPtr->gétSide();
//     for (int i = 0;i < 26;++i)
//         button[]
// }

bool Display::init_SDL(){

    bool success = true;
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        cout << "SDL could not initialize! SDL Error:\n" << SDL_GetError();
		success = false;
    }
    else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
            cout << "Linear texture filtering not enabled!";
        
        Window = SDL_CreateWindow("Battle Of Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if (!Window)
            success = false;
        else {

            renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (!renderer)
                success = false;
            else {
                SDL_SetRenderDrawColor(renderer,0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;

                if (!(IMG_Init(imgFlags) & imgFlags))
                    success = false;

                if (TTF_Init() == -1)
                    success = false;
                
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                    success = false;
            }
        }
    }

    return success;
}

bool Display::loadMedia(){

    bool success = true;
    Garamond28 = TTF_OpenFont("../res/Garamond.ttf", 28);
	Cicero22 = TTF_OpenFont("../res/Cicero.ttf", 22);
	Cicero26 = TTF_OpenFont("../res/Cicero.ttf", 26);
	Garamond26 = TTF_OpenFont("../res/Garamond.ttf", 26);
	
    if (Garamond28 == NULL || Garamond26 == NULL)
		success = false;
	if (Cicero26 == NULL || Cicero22 == NULL)
		success = false;

    mFSound = Mix_LoadWAV("res/moveFrom.wav");
    mTSound = Mix_LoadWAV("res/moveTo.wav");

    if (mFSound == NULL || mTSound == NULL)
        success = false;

    SDL_Surface* icon = IMG_Load("../res/icon.png");
    SDL_SetWindowIcon(Window, icon);

    return success;

}