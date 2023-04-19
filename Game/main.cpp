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


	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
					case SDLK_SPACE:
						if (start) {
							mF = mT = -1;
							board.updateDisplay(mF, mT);
							board.botMove();
						}
						else start = true;
					break;
					case SDLK_m:
						muted = muted ? 0 : 1;
					break;
					case SDLK_w:
						board.setWhiteIsBot(!board.getWhiteIsBot());
					break;
					case SDLK_b:
						board.setBlackIsBot(!board.getBlackIsBot());
					break;
					case SDLK_UP:
						if (board.whiteBot.getLevel() < 9 &&
						    board.blackBot.getLevel() < 9) {
							board.whiteBot.setLevel(board.whiteBot.getLevel()+1);
							board.blackBot.setLevel(board.blackBot.getLevel()+1);
						}
					break;
					case SDLK_DOWN:
						if (board.whiteBot.getLevel() > 1 &&
						    board.blackBot.getLevel() > 1) {
							board.whiteBot.setLevel(board.whiteBot.getLevel()-1);
							board.blackBot.setLevel(board.blackBot.getLevel()-1);
						}
					break;
					case SDLK_LEFT:
						board.undoMove();
						board.updateDisplay(mF, mT);
					break;
					case SDLK_BACKSPACE:
						board.restart();
						board.updateDisplay(mF, mT);
					break;
				}
			}
			board.handleInput(mF, mT, &e);
			board.updateDisplay(mF, mT);
		}
		if (start) {
			if ((board.getSide() && board.getWhiteIsBot()) || 
			    (!board.getSide() && board.getBlackIsBot())) {
				board.botMove();
				board.updateDisplay(mF, mT);
			}
		}
		else
			mF = mT = -1;
	}

    
    return 0;
}
