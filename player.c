//
// Created by Astori on 17/01/2017.
//

#include "player.h"
#include "collisions.h"
extern SDL_Texture *loadImage(char *name);
void initPlayer(Entity* player)
{
    player->sprite = loadImage("files/images/wizard_right.png");
    player->x = (300);
    player->y = (100);
    player->dirX = player->dirY = 0;
    player->wait = 0;
    SDL_QueryTexture(player->sprite, NULL, NULL, &player->w, &player->h);
	player->w/=8;
}

void doPlayer()
{
    if (player->wait == 0)
    {
        player->dirX = 0;
        player->dirY += GRAVITY_SPEED;

        if (player->dirY >= MAX_FALL_SPEED)
        {
            player->dirY = MAX_FALL_SPEED;
        }

        if (input.left == 1)
        {
            player->dirX -= PLAYER_SPEED;
        }

        else if (input.right == 1)
        {
            player->dirX += PLAYER_SPEED;
        }

        if (input.jump == 1)
        {
            if (player->onGround == 1)
            {
                player->dirY = -15;
            }
            input.jump = 0;
        }
        checkToMap(player, m);
    }

    if (player->wait > 0)
    {
        player->wait--;

        if (player->wait == 0)
        {
            initPlayer(player);
        }
    }
}