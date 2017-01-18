//
// Created by Astori on 17/01/2017.
//
#ifndef NEST_STRUCTS_H
#define NEST_STRUCTS_H
#include "SDL.h"
typedef struct GameState {
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *sheetTexture;
    SDL_Texture *backgroundTexture;
}GameState;

typedef struct Sprites
{
    SDL_Texture *image;
} Sprites;

typedef struct Entity
{
    int x, y;
    int w, h;
    SDL_Texture *sprite;
} Entity;

typedef struct Control
{
    int up, down, left, right;
} Control;
enum
{
    PLAYER_SPRITE,
    MAX_SPRITES
};
Control input;
Entity player;
Sprites sprite[MAX_SPRITES];
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#endif
