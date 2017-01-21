//
// Created by Astori on 17/01/2017.
//
#ifndef NEST_STRUCTS_H
#define NEST_STRUCTS_H
#include "SDL.h"
#include "tmx.h"

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
typedef struct Map
{
    SDL_Texture *map_bmp;
    tmx_map *map_m;
    SDL_Rect *map_rect;
}Map;
enum
{
    PLAYER_SPRITE,
    MAX_SPRITES
};
Control input;
Entity* player;
Map* m;
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#endif
