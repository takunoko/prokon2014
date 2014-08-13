#include <stdlib.h>
#include <stdio.h>

#include "SDL.h"

int main(int argc,char *argv[]){
	SDL_Window *window;
	SDL_Event event;

	int done;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s\n", SDL_GetError());
		return (1);
	}

	/* Set 640x480 video mode */
	window = SDL_CreateWindow("CheckKeys Test",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,640, 480, 0);
	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create 640x480 window: %s\n",SDL_GetError());
		exit(2);
	}
	/* Creating the context creates the view, which we need to show keyboard */
	/* OpenGLを使うためのうんぬんを生成 */
	//SDL_GL_CreateContext(window);

	SDL_StartTextInput();

	done = 0;
	while(!done){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_MOUSEBUTTONDOWN:
				/* Any button press quits the app... */
			case SDL_QUIT:
				/* Command + Q ? */
				done = 1;
				break;
			default:
				break;
		}
	}
	SDL_Quit();

	return 0;
}

