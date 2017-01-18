//
// Created by Astori on 17/01/2017.
//
#include "graphics.h"
SDL_Texture *loadImage(GameState* gs, char *name)
{
    IMG_Init(IMG_INIT_JPG);
    SDL_Surface * temp = IMG_Load(name);
    SDL_Texture * image;
    image = SDL_CreateTextureFromSurface(gs->renderer, temp);
    SDL_FreeSurface(temp);
    return image;
}
void setLevelBackground(GameState* gs){
    gs->backgroundTexture = loadImage(gs, "files/images/sky.jpg");
}

void drawImage(SDL_Texture *image, int x, int y, GameState* gs)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(gs->renderer, gs->backgroundTexture, NULL, NULL);
    SDL_RenderCopy(gs->renderer, image, NULL, &dest);
    SDL_RenderPresent(gs->renderer);
}

void loadSprite(int index, char *name, GameState* gs)
{
    if (index >= MAX_SPRITES || index < 0)
    {
        printf("Invalid index for sprite! Index: %d Maximum: %d\n", index, MAX_SPRITES);

        exit(1);
    }

    sprite[index].image = loadImage(gs, name);

    if (sprite[index].image == NULL)
    {
        exit(1);
    }
}

SDL_Texture *getSprite(int index)
{
    if (index >= MAX_SPRITES || index < 0)
    {
        printf("Invalid index for sprite! Index: %d Maximum: %d\n", index, MAX_SPRITES);

        exit(1);
    }

    return sprite[index].image;
}

void freeSprites()
{
    int i;

    for (i=0;i<MAX_SPRITES;i++)
    {
        if (sprite[i].image != NULL)
        {
            SDL_DestroyTexture(sprite[i].image);
        }
    }
}

void loadAllSprites(GameState* gs)
{
    loadSprite(PLAYER_SPRITE, "files/images/firefly.jpg", gs);
}
