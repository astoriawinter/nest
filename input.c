//
// Created by Astori on 17/01/2017.
//

#include "input.h"
void delay(unsigned int frameLimit) {
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks) {
		return;
	}
	if (frameLimit > ticks + 16) {
		SDL_Delay(16);
	} else {
		SDL_Delay(frameLimit - ticks);
	}
}
int getInput()
{
	SDL_Event event;
    //const Uint8 *state = SDL_GetKeyboardState(NULL);
	unsigned int frameLimit = SDL_GetTicks() + 16;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					return 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_UP:
							input.up = 1;
							break;
						case SDLK_DOWN:
							input.down = 1;
							break;
						case SDLK_SPACE:
							input.jump = 1;
							break;
						case SDLK_LEFT:
							input.left = 1;
							break;
						case SDLK_RIGHT:
							input.right = 1;
							break;
						case SDLK_ESCAPE:
							exit(0);
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_UP:
							input.up = 0;
							break;

						case SDLK_DOWN:
							input.down = 0;
							break;

						case SDLK_LEFT:
							input.left = 0;
							break;
						case SDLK_RIGHT:
							input.right = 0;
							break;

						default:
							break;
					}
					break;

			}
		}
		delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
	return 0;
}
