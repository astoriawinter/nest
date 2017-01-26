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
    float x, y, dirX, dirY;
    int w, h, onGround, thinkTime, onLadder, collectedCoins;
    SDL_Texture *sprite_l;
    SDL_Texture *sprite_r;
    SDL_Texture *sprite_f;
} Entity;
typedef  struct Hud
{
    SDL_Texture* Message;
    SDL_Surface* surfaceMessage;
    SDL_Rect* Message_rect;
    int coins;
}Hud;

typedef struct Control
{
    int up, down, left, right, jump;
} Control;
typedef struct Map
{
    SDL_Texture *map_bmp;
    tmx_map *map_m;
    SDL_Rect *map_rect;
    tmx_layer *map_col;
    tmx_layer *map_lad;
    tmx_layer *map_obj;
}Map;
enum
{
    PLAYER_SPRITE,
    MAX_SPRITES
};
Control input;
Entity* player;
Map* m;
Hud* hud;
#define TILE_SIZE 32
#define SCROLL_SPEED 8
#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 20
#define PLAYER_SPEED 4
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640
#endif
