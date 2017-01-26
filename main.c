
#include "main.h"
int main(int argc, char *argv[]) {
    unsigned int frameLimit = SDL_GetTicks() + 16;
    player = malloc(sizeof(Entity));
    m = malloc(sizeof(Map));
    hud = malloc(sizeof(Hud));
    hud->Message_rect = NULL;
    hud->surfaceMessage = NULL;
    hud->Message = NULL;
    init();
    initPlayer(player);
    gameLoop("files/map/map.tmx", player, m);
    return 0;
}
