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

bool Init(){ //Khởi tạo

    bool success_ = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        
        success_ = false;
        std::cout << "Không tạo được màn hình" << "\n";
    
    }
    else {

        _window = SDL_CreateWindow("Road To Peace", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        _surface = SDL_GetWindowSurface(_window);
    }

    return success_;
}

void close(){ //Đóng

    SDL_FreeSurface(_surface);
    _surface = NULL;

    SDL_DestroyWindow(_window);
    _window = NULL;

    SDL_Quit();

}

void loadMedia(); //Load ảnh

void optimized_size(); //Mở rộng cửa sổ

int main(int argc, char* args[]){
    
    if (!Init()){
        std::cout << "Đéo load được";
    }
    else {

        bool quit = false;

        SDL_Event e;

        while (!quit){
            
            while(SDL_PollEvent(&e) != 0){

                if (e.type == SDL_QUIT)
                    quit = true;
            }

            SDL_UpdateWindowSurface(_window);
        }


    }

    close();

    return 0;
}