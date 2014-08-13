#include "SDL.h"

int main(int argc,char *argv[]){
	if(SDL_Init(SDL_INT_VIDEO) < 0){
		return -1;
	}
	
	SDL_Surface *screen=SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	if(screen == NULL){
		SDL_Quit();
		return -1;
	}

	DL_Rect box;
	box.x = 160;
	box.y = 120;
	box.w = 320;
	box.h = 240;
	SDL_FillRect(screen, &box, 0xff2B4B65);
	SDL_UpdateRect(screen,0,0,0,0);

	SDL_Event event;
	int done = 0;
	while(!done){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE){
						done = 1;
					}
					break;
				case SDL_QUIT:
					done = 1;
					break;
			}
		}
	}

	SDL_Quit();

	return 0;
}

