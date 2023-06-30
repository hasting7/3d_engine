#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "engine.h"
#include "linear.h"


int main(int argc, char *argv[]) {

    	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        	printf("Could not init SDL: %s\n", SDL_GetError());
        	exit(1);
    	}	

    	SDL_Window *window = SDL_CreateWindow(
        	NAME,
        	SDL_WINDOWPOS_CENTERED,
        	SDL_WINDOWPOS_CENTERED,
        	WIDTH,
        	HEIGHT,
		SDL_WINDOW_OPENGL
    	);

    	if (!window) {
        	printf("Failed to init window: %s\n", SDL_GetError());
        	exit(1);
    	}

	SDL_Renderer *render = SDL_CreateRenderer(window,-1,0);

	init();
	
	// main loop
    	bool should_quit = false;
    	while (!should_quit) {

		// stuff to do
		
		SDL_RenderClear(render);

		draw(render);

		SDL_SetRenderDrawColor(render, bg.r, bg.g, bg.b, 255);
    	SDL_RenderPresent(render);

		// key shit
        	SDL_Event event;

        	while (SDL_PollEvent(&event)) {
			if (event.key.keysym.sym == 'q' || event.type == SDL_QUIT) {
				should_quit = true;
			}
			if (event.type == SDL_KEYDOWN) {
				char key = event.key.keysym.sym;
				key_press(key);
			} else {
				events(event);
			}
         	}
    	}
	SDL_Quit();
   	return 0;
}
