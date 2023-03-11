#include <iostream>
#include <SDL.h>
#include <cmath>
#include <string>
#include <SDL_image.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

bool Init();

SDL_Window* _window = NULL;
SDL_Surface *_surface = NULL;

bool Init(){

    bool success_ = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        
        success_ = false;
        std::cout << "Không tạo được màn hình" << "\n";
    
    }
    else {

        _window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT);
        _surface = SDL_Get
    }
}

int main(int argc, char* args[]){
    
    

    return 0;
}