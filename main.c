
#include "main.h"
int main(int argc, char *argv[]) {
    unsigned int frameLimit = SDL_GetTicks() + 16;
    player = malloc(sizeof(Entity));
    m = malloc(sizeof(Map));
    init();
    initPlayer(player);
    gameLoop("files/map/map.tmx", player, m);
    return 0;
}
