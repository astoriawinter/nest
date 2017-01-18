//
// Created by Astori on 17/01/2017.
//
#ifndef NEST_GRAPHICS_H
#define NEST_GRAPHICS_H
#include "structs.h"
#include <stdio.h>
#include "SDL_image.h"
#endif
SDL_Texture *loadImage(GameState* gs, char *name);
void setLevelBackground(GameState* gs);
void drawImage(SDL_Texture *image, int x, int y, GameState* gs);
void loadSprite(int index, char *name, GameState* gs);
SDL_Texture *getSprite(int index);
void freeSprites();
void loadAllSprites(GameState* gs);
