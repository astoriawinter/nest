//
// Created by Astori on 17/01/2017.
//

#include "render.h"
extern void drawPlayer(GameState* gs);
void draw(GameState* gs)
{
    SDL_RenderClear(gs->renderer);
    drawPlayer(gs);
    SDL_Delay(1);
}

void delay(unsigned int frameLimit)
{
    unsigned int ticks = SDL_GetTicks();
    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}
