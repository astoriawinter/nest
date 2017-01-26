//
// Created by Astori on 23/01/2017.
//

#include "collisions.h"
#define MAX_MAP_X 71
#define MAX_MAP_Y 20
void checkToMap(Entity *player, Map* m)
{
	int i, x1, x2, y1, y2;
	unsigned int gid, gid_2;
	player->onGround = 0;
    /* ladders collision */
    x1 = (player->x + player->dirX + player->w/2) / TILE_SIZE;
    y2 = (player->y + player->dirY + player->h/2) / TILE_SIZE;
    gid = gid_clear_flags(m->map_lad->content.gids[(y2 * m->map_m->width) + x1]);
    if (player->y < 610) {
        if (m->map_m->tiles[gid] != NULL) {
            player->onLadder = 1;
        } else {
            player->onLadder = 0;
        }
    }
    /* coins */
    y2 = player->y / TILE_SIZE + 2;
    gid = gid_clear_flags(m->map_obj->content.gids[(y2 * m->map_m->width) + x1]);
    if (player->y < 600) {
        if (m->map_m->tiles[gid] != NULL) {
            m->map_obj->content.gids[(y2 * m->map_m->width) + x1] = 0;
            player->collectedCoins++;
            redraw_tile(m->map_m, x1, y2);
        }
    }

    /* doors */
    gid = gid_clear_flags(m->map_door->content.gids[(y2 * m->map_m->width) + x1]);
    if (player->y < 600) {
        if (m->map_m->tiles[gid] != NULL) {
            if (player->collectedCoins > 15)
                player->accomplished = 1;
        }
    }

    /* map collision */
	i = player->h > TILE_SIZE ? TILE_SIZE : player->h;
	for (;;)
	{
		x1 = (player->x + player->dirX) / TILE_SIZE;
		x2 = (player->x + player->dirX + player->w - 1) / TILE_SIZE;
		y1 = (player->y) / TILE_SIZE;
		y2 = (player->y + i - 1) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y) {
            if (player->dirX > 0) {
                gid = gid_clear_flags(m->map_col->content.gids[(y1 * m->map_m->width) + x2]);
                gid_2 = gid_clear_flags(m->map_col->content.gids[(y2 * m->map_m->width) + x2]);

                    if (m->map_m->tiles[gid] != NULL || m->map_m->tiles[gid_2] != NULL) {
                        player->x = x2 * TILE_SIZE;
                        player->x -= player->w + 1;
                        player->dirX = 0;
                }
            } else if (player->dirX < 0) {
                gid = gid_clear_flags(m->map_col->content.gids[(y1 * m->map_m->width) + x1]);
                gid_2 = gid_clear_flags(m->map_col->content.gids[(y2 * m->map_m->width) + x1]);

                if (m->map_m->tiles[gid] != NULL || m->map_m->tiles[gid_2] != NULL) {
                    player->x = (x1 + 1) * TILE_SIZE;
                    player->dirX = 0;
                }
            }
        }
		if (i == player->h)
		{
			break;
		}

		i += TILE_SIZE;
		if (i > player->h)
		{
			i = player->h;
		}
	}
	i = player->w > TILE_SIZE ? TILE_SIZE : player->w;

	for (;;)
	{
		x1 = (player->x) / TILE_SIZE;
		x2 = (player->x + i) / TILE_SIZE;
		y1 = (player->y + player->dirY) / TILE_SIZE;
		y2 = (player->y + player->dirY + player->h) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			if (player->dirY > 0)
			{
                x1 = (player->x + player->dirX + player->w/3) / TILE_SIZE;
                x2 = (player->x + player->dirX + player->w*2/3 - 1) / TILE_SIZE;

				gid = gid_clear_flags(m->map_col->content.gids[(y2*m->map_m->width)+x1]);
				gid_2 = gid_clear_flags(m->map_col->content.gids[(y2*m->map_m->width)+x2]);

				if (m->map_m->tiles[gid_2] != NULL || m->map_m->tiles[gid] != NULL)
				{
					player->y = y2 * TILE_SIZE;
					player->y -= player->h;
					player->dirY = 0;
					player->onGround = 1;
				}
			}

			else if (player->dirY < 0)
			{
				gid = gid_clear_flags(m->map_col->content.gids[(y2*m->map_m->width)+x1]);
				gid_2 = gid_clear_flags(m->map_col->content.gids[(y1*m->map_m->width)+x2]);
				if (m->map_m->tiles[gid_2] != NULL || m->map_m->tiles[gid] != NULL)
				{
					player->y = (y1 + 1) * TILE_SIZE;
					player->dirY = 0;
				}
			}
		}

		if (i == player->w)
		{
			break;
		}
		i += TILE_SIZE;
		if (i > player->w)
		{
			i = player->w;
		}
	}

	player->x += player->dirX;
	player->y += player->dirY;
	if (player->x < 0)
	{
		player->x = 0;
	}
    if (player->y < 0)
    {
        player->y = 0;
    }
	else if (player->x + player->w >= m->map_m->width*TILE_SIZE)
	{
		player->x =  m->map_m->width*TILE_SIZE - player->w - 1;
	}

	if (player->y > m->map_m->height*TILE_SIZE)
	{
		player->thinkTime = 60;
	}
}