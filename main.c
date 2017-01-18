
#include "main.h"
int main(int argc, char *argv[]) {
    unsigned int frameLimit = SDL_GetTicks() + 16;
    GameState gameState;
    initSDL(&gameState);
    setLevelBackground(&gameState);
    loadAllSprites(&gameState);
    initPlayer();
    while (1)
    {
        getInput();
        doPlayer();
        draw(&gameState);
        delay(frameLimit);
        frameLimit = SDL_GetTicks() + 16;
    }
    return 0;
}