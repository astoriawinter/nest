
#include <stdio.h>
#include "map.h"

#define fatal_error(str)  { fputs(str, stderr); goto errquit; }
#define fatal_error2(str) { fputs(str, stderr);  return NULL; }


static SDL_Renderer *ren = NULL;

SDL_Texture *loadImage(char *name)
{
    IMG_Init(IMG_INIT_JPG);
    SDL_Surface * temp = IMG_Load(name);
    SDL_Texture * image;
    image = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return image;
}

void drawImage(SDL_Texture *image, int x, int y)
{
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 300) % 8;
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
    int w = dest.w/8;
    SDL_Rect srcRect_ledges = {sprite*w , 0, w, dest.h};
    SDL_Rect Rect_ledges = { x, y, w, dest.h};
    SDL_RenderCopy(ren, image,  & srcRect_ledges, & Rect_ledges);
}

void loadSprite(int index, char *name)
{
    if (index >= MAX_SPRITES || index < 0)
    {
        printf("Invalid index for sprite! Index: %d Maximum: %d\n", index, MAX_SPRITES);

        exit(1);
    }

    sprite[index].image = loadImage(name);

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

void loadAllSprites()
{
    loadSprite(PLAYER_SPRITE, "files/images/wizard_right.png");
}

void set_color(int color) {
    unsigned char r, g, b;

    r = (color >> 16) & 0xFF;
    g = (color >>  8) & 0xFF;
    b = (color)       & 0xFF;

    SDL_SetRenderDrawColor(ren, r, g, b, SDL_ALPHA_OPAQUE);
}

void* sdl_img_loader(const char *path) {
    return IMG_LoadTexture(ren, path);
}

void draw_polyline(double **points, double x, double y, int pointsc) {
    int i;
    for (i=1; i<pointsc; i++) {
        SDL_RenderDrawLine(ren, x+points[i-1][0], y+points[i-1][1], x+points[i][0], y+points[i][1]);
    }
}

void draw_polygon(double **points, double x, double y, int pointsc) {
    draw_polyline(points, x, y, pointsc);
    if (pointsc > 2) {
        SDL_RenderDrawLine(ren, x+points[0][0], y+points[0][1], x+points[pointsc-1][0], y+points[pointsc-1][1]);
    }
}

void draw_objects(tmx_object_group *objgr) {
    SDL_Rect rect;
    set_color(objgr->color);
    tmx_object *head = objgr->head;
    /* FIXME line thickness */
    while (head) {
        if (head->visible) {
            if (head->shape == S_SQUARE) {
                rect.x =     head->x;  rect.y =      head->y;
                rect.w = head->width;  rect.h = head->height;
                SDL_RenderDrawRect(ren, &rect);
            } else if (head->shape  == S_POLYGON) {
                draw_polygon(head->points, head->x, head->y, head->points_len);
            } else if (head->shape == S_POLYLINE) {
                draw_polyline(head->points, head->x, head->y, head->points_len);
            } else if (head->shape == S_ELLIPSE) {
                /* FIXME: no function in SDL2 */
            }
        }
        head = head->next;
    }
}

unsigned int gid_clear_flags(unsigned int gid) {
    return gid & TMX_FLIP_BITS_REMOVAL;
}

void draw_layer(tmx_map *map, tmx_layer *layer) {
    unsigned long i, j;
    unsigned int gid;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    SDL_Rect srcrect, dstrect;
    SDL_Texture* tileset;
    op = layer->opacity;
    for (i=0; i<map->height; i++) {
        for (j=0; j<map->width; j++) {
            gid = gid_clear_flags(layer->content.gids[(i*map->width)+j]);
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                srcrect.x = map->tiles[gid]->ul_x;
                srcrect.y = map->tiles[gid]->ul_y;
                srcrect.w = dstrect.w = ts->tile_width;
                srcrect.h = dstrect.h = ts->tile_height;
                dstrect.x = j*ts->tile_width;  dstrect.y = i*ts->tile_height;
                /* TODO Opacity and Flips */
                if (im) {
                    tileset = (SDL_Texture*)im->resource_image;
                }
                else {
                    tileset = (SDL_Texture*)ts->image->resource_image;
                }
                SDL_RenderCopy(ren, tileset, &srcrect, &dstrect);
            }
        }
    }
}

void draw_image_layer(tmx_image *img) {
    SDL_Rect dim;

    dim.x = dim.y = 0;
    SDL_QueryTexture((SDL_Texture*)img->resource_image, NULL, NULL, &(dim.w), &(dim.h));

    SDL_RenderCopy(ren, (SDL_Texture*)img->resource_image, NULL, &dim);

}

SDL_Texture* render_map(tmx_map *map) {
    SDL_Texture *res;
    tmx_layer *layers = map->ly_head;
    int w, h;

    w = map->width  * map->tile_width;  /* Bitmap's width and height */
    h = map->height * map->tile_height;

    if (!(res = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h)))
    fatal_error2(SDL_GetError());
    SDL_SetRenderTarget(ren, res);

    set_color(map->backgroundcolor);
    SDL_RenderClear(ren);

    while (layers) {
        if (layers->visible) {
            if (layers->type == L_OBJGR) {
                draw_objects(layers->content.objgr);
            } else if (layers->type == L_IMAGE) {
                draw_image_layer(layers->content.image);
            } else if (layers->type == L_LAYER) {
                draw_layer(map, layers);
            }
        }
        layers = layers->next;
    }

    SDL_SetRenderTarget(ren, NULL);
    return res;
}

Uint32 timer_func(Uint32 interval, void *param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

int map(char* string) {
    tmx_map *map = NULL;
    SDL_Window *win;
    SDL_Event e;
    SDL_Texture *map_bmp;
    SDL_Rect map_rect;
    SDL_TimerID timer_id;
    int x_delta, y_delta;
    int key_state[2] = {0, 0};

    if (!(win = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DISPLAY_W, DISPLAY_H, SDL_WINDOW_SHOWN)))
    fatal_error(SDL_GetError());

    if (!(ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
    fatal_error(SDL_GetError());

    SDL_EventState(SDL_MOUSEMOTION, SDL_DISABLE);

    tmx_img_load_func = (void* (*)(const char*))sdl_img_loader;
    tmx_img_free_func = (void  (*)(void*))      SDL_DestroyTexture;

    if (!(map = tmx_load(string))) fatal_error(tmx_strerr());

    map_rect.w = map->width  * map->tile_width;
    map_rect.h = map->height * map->tile_height;
    map_rect.x = 0;  map_rect.y = 0;

    x_delta = DISPLAY_W - map_rect.w;
    y_delta = DISPLAY_H - map_rect.h;

    if (!(map_bmp = render_map(map)))
    fatal_error(SDL_GetError());

    timer_id = SDL_AddTimer(30, timer_func, NULL);

    while (SDL_WaitEvent(&e)){

        if (e.type == SDL_QUIT) break;

        else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_RIGHT: key_state[0] = 0; break;
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_DOWN:  key_state[1] = 0; break;
            }
        }

        else if (e.type == SDL_KEYDOWN) {

            if (e.key.keysym.scancode == SDL_SCANCODE_Q) break;
            switch (e.key.keysym.scancode) {
                case SDL_SCANCODE_LEFT:  key_state[0] =  4; break;
                case SDL_SCANCODE_RIGHT: key_state[0] = -4; break;
                case SDL_SCANCODE_UP:    key_state[1] =  4; break;
                case SDL_SCANCODE_DOWN:  key_state[1] = -4; break;
            }
        }

        else if (e.type == SDL_USEREVENT) {

            map_rect.x += key_state[0];
            map_rect.y += key_state[1];
            if (x_delta > 0) {
                map_rect.x = x_delta/2;
            } else {
                if (map_rect.x < x_delta) map_rect.x = x_delta;
                if (map_rect.x > 0) map_rect.x = 0;
            }
            if (y_delta > 0) {
                map_rect.y = y_delta/2;
            } else {
                if (map_rect.y < y_delta) map_rect.y = y_delta;
                if (map_rect.y > 0) map_rect.y = 0;
            }

            SDL_RenderClear(ren);
            SDL_RenderCopy(ren, map_bmp, NULL, &map_rect);
            SDL_RenderPresent(ren);
        }
    }

    tmx_map_free(map);
    SDL_RemoveTimer(timer_id);
    SDL_DestroyTexture(map_bmp);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;

    errquit:
    SDL_Quit();
    return -1;
}

