#include <stdio.h>
#include "SDL.h"
int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              
    window = SDL_CreateWindow("Game Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              1280,
                              640,
                              0
    );
    SDL_Delay(2000);
    SDL_Quit();
}