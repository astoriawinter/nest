//
// Created by Astori on 20/01/2017.
//

#ifndef NEST_MAP_H
#define NEST_MAP_H
#include <stdio.h>
#include <tmx.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <SDL_image.h>
#include "player.h"
#define fatal_error2(str) { fputs(str, stderr);  return NULL; }
#define DISPLAY_H 660
#define DISPLAY_W 1280
#include "structs.h"
#include <stdio.h>
#include "SDL_image.h"
#endif //NEST_MAP_H

void grawGate();
SDL_Texture *loadImage(char *name);
void loadSprite(int index, char *name);
void set_color(int color);
void* sdl_img_loader(const char *path);
void redraw_tile(tmx_map *map, unsigned int x, unsigned int y);
void draw_polyline(double **points, double x, double y, int pointsc);
void draw_polygon(double **points, double x, double y, int pointsc);
void draw_objects(tmx_object_group *objgr, Map* m);
unsigned int gid_clear_flags(unsigned int gid);
void draw_layer(tmx_map *map, tmx_layer *layer);
void draw_image_layer(tmx_image *img) ;
SDL_Texture* render_map(Map *map);
int map(char* string);
void init();
int gameLoop(char* string, Entity* player, Map* m);
void render(Entity* player, Map* m);
void drawMap(Map* m);
void drawImage(Entity* player);
int restartGame();
void quit();
