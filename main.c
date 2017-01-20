
#include "main.h"
int main(int argc, char *argv[]) {
    unsigned int frameLimit = SDL_GetTicks() + 16;
    initPlayer();
    drawPlayer();
    map("files/map/map.tmx");
        while (1) {
            getInput();
            doPlayer();
        }
    return 0;
}
