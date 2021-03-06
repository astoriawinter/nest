
#include <stdio.h>
#include "map.h"
#include "input.h"
#include "SDL_ttf.h"
#define fatal_error2(str) { fputs(str, stderr);  return NULL; }

static SDL_Renderer *ren = NULL;
static SDL_Window *win = NULL;
TTF_Font* Sans = NULL;
        SDL_Texture *loadImage(char *name)
{
    IMG_Init(IMG_INIT_JPG);
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface * temp = IMG_Load(name);
    SDL_Texture * image;
    image = SDL_CreateTextureFromSurface(ren, temp);
    SDL_FreeSurface(temp);
    return image;
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

void draw_objects(tmx_object_group *objgr, Map* m) {
    SDL_Rect rect;
    set_color(objgr->color);
    tmx_object *head = objgr->head;
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
            } else if (head->shape == S_TILE) {
            }
        }
        head = head->next;
    }
}

unsigned int gid_clear_flags(unsigned int gid) {
    return gid & TMX_FLIP_BITS_REMOVAL;
}

void redraw_tile(tmx_map *map, unsigned int x, unsigned int y) {
    tmx_tileset *ts;
    tmx_image *im;
    SDL_Rect srcrect, dstrect;
    SDL_Texture* tileset;
    unsigned int gid;
    tmx_layer *layers = m->map_m->ly_head;
    SDL_SetRenderTarget(ren, m->map_bmp);
    while (layers) {
        if (layers->visible && layers->type == L_LAYER) {
            gid = gid_clear_flags(layers->content.gids[(y*map->width)+x]);
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im = map->tiles[gid]->image;
                srcrect.x = map->tiles[gid]->ul_x;
                srcrect.y = map->tiles[gid]->ul_y;
                srcrect.w = dstrect.w = ts->tile_width;
                srcrect.h = dstrect.h = ts->tile_height;
                dstrect.x = x*ts->tile_width;  dstrect.y = y*ts->tile_height;
                if (im) {
                    tileset = (SDL_Texture*)im->resource_image;
                }
                else {
                    tileset = (SDL_Texture*)ts->image->resource_image;
                }
                SDL_RenderCopy(ren, tileset, &srcrect, &dstrect);
            }
        }
        layers = layers->next;
    }
    SDL_SetRenderTarget(ren, NULL);
}

void draw_layer(tmx_map *map, tmx_layer *layer) {
    unsigned long i, j;
    unsigned int gid;
    float op;
    unsigned int f;
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

SDL_Texture* render_map(Map *m) {
    SDL_Texture *res;
    tmx_layer *layers = m->map_m->ly_head;
    int w, h;
    w = m->map_m->width  * m->map_m->tile_width;
    h = m->map_m->height * m->map_m->tile_height;
    if (!(res = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h)))
    fatal_error2(SDL_GetError());
    SDL_SetRenderTarget(ren, res);
    set_color(m->map_m->backgroundcolor);
    SDL_RenderClear(ren);
    while (layers) {
        if (layers->visible) {
            if (layers->type == L_OBJGR) {
                draw_objects(layers->content.objgr, m);
            } else if (layers->type == L_IMAGE) {
                draw_image_layer(layers->content.image);
            } else if (layers->type == L_LAYER) {
                draw_layer(m->map_m, layers);
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
void init()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    win = SDL_CreateWindow("Nest",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              DISPLAY_W,
                              DISPLAY_H,
                              0
    );
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}
void clear(SDL_TimerID timer_id, Map* m)
{
    tmx_map_free(m->map_m);
    free(m->map_rect);
    SDL_RemoveTimer(timer_id);
    SDL_DestroyTexture(m->map_bmp);
}
void quit(){
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
void map_load(char * string, Map* m)
{
    tmx_layer *layers;
    tmx_img_load_func = (void* (*)(const char*))sdl_img_loader;
    tmx_img_free_func = (void  (*)(void*))      SDL_DestroyTexture;
    m->map_m = tmx_load(string);
    layers = m->map_m->ly_head;
    while (layers)
    {
        if (!(strcmp(layers->name, "collisions")))
            m->map_col = layers;
        if (!(strcmp(layers->name, "ladders")))
            m->map_lad = layers;
        if (!(strcmp(layers->name, "objects")))
            m->map_obj = layers;
        if (!(strcmp(layers->name, "door")))
            m->map_door = layers;
        layers = layers->next;
    }
}
void map_render(Map* m)
{
    m->map_bmp = render_map(m);
}
void drawMap(Map* m){
    SDL_RenderCopy(ren, m->map_bmp, NULL, m->map_rect);
}

void drawText(Hud* hud){
    char buf[8] = {0};
    SDL_Color White = {255, 215, 0};
    if (hud->Message != NULL) {
        SDL_DestroyTexture(hud->Message);
        hud->Message = NULL;
    }
        sprintf(buf, "COINS %d", player->collectedCoins);
        hud->surfaceMessage = TTF_RenderText_Solid(Sans, buf, White);
        hud->Message = SDL_CreateTextureFromSurface(ren, hud->surfaceMessage);
        if (!hud->Message_rect) {
            hud->Message_rect = malloc(sizeof(SDL_Rect));
            hud->Message_rect->x = 1100;
            hud->Message_rect->y = 0;
            hud->Message_rect->w = 120;
            hud->Message_rect->h = 50;
        }
    SDL_RenderCopy(ren, hud->Message, NULL, hud->Message_rect);
    SDL_FreeSurface(hud->surfaceMessage);
    hud->surfaceMessage = NULL;
}
void render(Entity* player, Map* m)
{
    SDL_RenderClear(ren);
    drawMap(m);
    drawImage(player);
}
void drawImage(Entity* player) {
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite_n = (ticks / 75) % 8;
    SDL_Texture *sprite;
    SDL_Rect dest;
    dest.x = player->x;
    dest.y = player->y;
    if (player->dirX < 0) {
        sprite = player->sprite_l;
    }
    else {
        sprite = player->sprite_r;
    }
    if (player->dirX < 0.001 && player->dirX > -0.001) {
        sprite = player->sprite_f;
        sprite_n = 0;
    }
    if (player->onLadder) {
        sprite = player->sprite_f;
        sprite_n = (ticks / 300) % 4 + 4;
    }
    SDL_QueryTexture(sprite, NULL, NULL, &dest.w, &dest.h);
    int w = dest.w/8;
    SDL_Rect srcRect_ledges = {sprite_n*w , 0, w, dest.h};
    SDL_Rect Rect_ledges = { player->x, player->y, w, dest.h};
    SDL_RenderCopy(ren, sprite, &srcRect_ledges, &Rect_ledges);
}
void loadWater(){
    if (m->water == NULL)
    m->water = loadImage("files/images/water.png");
    int w, h, j;
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite_n = (int) (SCREEN_HEIGHT - ((ticks - m->l_start_ticks) / 250)) > 0 ? SCREEN_HEIGHT - ((ticks - m->l_start_ticks) / 250) : 0;
    SDL_QueryTexture(m->water, NULL, NULL, &w, &h);
    SDL_Rect srcRect_ledges = {0 , 0, w, h};
    SDL_Rect Rect_ledges = { 0, sprite_n, w, h};
    SDL_RenderCopy(ren, m->water, &srcRect_ledges, &Rect_ledges);

    for (j=0; j<m->map_m->width; j++) {
        m->map_col->content.gids[( ((sprite_n / TILE_SIZE) + 1) *m->map_m->width)+j] = 0;
        m->map_lad->content.gids[( ((sprite_n / TILE_SIZE) + 1) *m->map_m->width)+j] = 0;
    }
}
void drawGate(){
    if (m->gate == NULL)
        m->gate = loadImage("files/art/gate.png");
    int w, h, j;
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite_n = (ticks / 300) % 5;
    SDL_QueryTexture(m->gate, NULL, NULL, &w, &h);
    w = w/5;
    SDL_Rect srcRect_ledges = {sprite_n*(w) , 0, w, h};
    SDL_Rect Rect_ledges = {1025, 65, w, h};
    SDL_RenderCopy(ren, m->gate, &srcRect_ledges, &Rect_ledges);
}
void cameraRoll()
{
    int x_delta;
    x_delta = DISPLAY_W - m->map_rect->w;
    if (player->dirX > 0)
        m->map_rect->x += 4;
    else if (player->dirX < 0)
        m->map_rect->x += -4;
    if (x_delta > 0) {
        m->map_rect->x = x_delta/2;
    } else {
        if (m->map_rect->x  < x_delta) m->map_rect->x  = x_delta;
        if (m->map_rect->x  > 0) m->map_rect->x  = 0;
    }
}

int restartGame(){
    initPlayer(player);
    m->l_start_ticks = SDL_GetTicks();
    return gameLoop("files/map/map2.tmx", player, m);
}

int gameLoop(char* string, Entity* player, Map* m){
    SDL_Event event;
    SDL_TimerID timer_id;
    if (!Sans)
        Sans = TTF_OpenFont("files/ttf/open-sans.regular.ttf", 30);
    map(string);
    m->map_rect = malloc(sizeof(SDL_Rect));
    m->map_rect->w = m->map_m->width  * m->map_m->tile_width;
    m->map_rect->h = m->map_m->height * m->map_m->tile_height;
    m->map_rect->x = 0;  m->map_rect->y = 0;
    map_render(m);
    while (doPlayer() == 0 && player->accomplished == 0)
    {
        if(getInput() == 1){
            clear(timer_id, m);
            return 0;
        }
        render(player, m);
        loadWater();
        if (player->win == 1)
            drawGate();
        drawText(hud);
        SDL_RenderPresent(ren);
    }
    clear(timer_id, m);
    return player->accomplished ? 0 : 1;
}

int map(char* string) {
    map_load(string, m);
    return 0;
}

