#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "common.h"
#include "board.h"
#include "display.h"

using namespace std;

bool quit = false, start = false, muted = true;

int main(int argc, char* args[]){
    
    SDL_Event e;

    while (!quit){

        while (SDL_PollEvent(&e)){

            if (e.type = SDL_QUIT)
                quit = true;
            // else (e.type == SDL_KEYDOWN){

            //     // switch (e.key.keysym.sym){

            //     // }


            // }
            
        }
    }

    
    return 0;
}
