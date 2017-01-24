//
// Created by Astori on 17/01/2017.
//

#include "player.h"
#include "collisions.h"
extern SDL_Texture *loadImage(char *name);
void initPlayer(Entity* player)
{
    player->sprite_r = loadImage("files/images/wizard_right.png");
    player->sprite_l = loadImage("files/images/wizard_left.png");
    player->sprite_f = loadImage("files/images/wizard_face.png");
    player->x = (300);
    player->y = (300);
    player->dirX = player->dirY = 0;
    player->thinkTime = 0;
    player->collectedCoins = 0;
    SDL_QueryTexture(player->sprite_l, NULL, NULL, &player->w, &player->h);
    SDL_QueryTexture(player->sprite_r, NULL, NULL, &player->w, &player->h);
    SDL_QueryTexture(player->sprite_f, NULL, NULL, &player->w, &player->h);
	player->w/=8;
}

void doPlayer()
{
    if (player->thinkTime == 0)
    {
        player->dirX = 0;
        if (!player->onLadder)
            player->dirY += GRAVITY_SPEED;
        else
            player->dirY = 0;

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
        if (input.up == 1)
        {
            if (player->onLadder == 1)
            {
                player->dirY = -3*PLAYER_SPEED;
            }
            input.up = 0;
        }
        if (input.down == 1)
        {
            if (player->onLadder == 1)
            {
                player->dirY = 3*PLAYER_SPEED;
            }
            input.down = 0;
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

    if (player->thinkTime > 0)
    {
        player->thinkTime--;

        if (player->thinkTime == 0)
        {
            initPlayer(player);
        }
    }
}