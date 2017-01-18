//
// Created by Astori on 17/01/2017.
//

#include "player.h"
extern void drawImage(SDL_Texture *, int, int,  GameState*);
extern SDL_Texture *getSprite(int);
void initPlayer()
{
	player.sprite = getSprite(PLAYER_SPRITE);
	player.x = SCREEN_WIDTH / 2;
	player.y = SCREEN_HEIGHT / 2;
    SDL_QueryTexture(player.sprite, NULL, NULL, &player.w, &player.h);
}

void doPlayer()
{
	if (input.up == 1)
	{
		player.y -= 3;

		if (player.y < 0)
		{
			player.y = 0;
		}
	}

	if (input.down == 1)
	{
		player.y += 3;


		if (player.y + player.h >= SCREEN_HEIGHT)
		{
			player.y = SCREEN_HEIGHT - (player.h + 1);
		}
	}

	if (input.left == 1)
	{
		player.x -= 3;

		if (player.x < 0)
		{
			player.x = 0;
		}
	}

	if (input.right == 1)
	{
		player.x += 3;

		if (player.x + player.w >= SCREEN_WIDTH)
		{
			player.x = SCREEN_WIDTH - (player.w + 1);
		}
	}
}

void drawPlayer(GameState* gs)
{
	drawImage(player.sprite, player.x, player.y, gs);
}

