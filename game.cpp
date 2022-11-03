#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "./include/main.h"
#include "./include/config.h"
#include "./include/game.h"
#include "./include/ui.h"

int start(HWND hWnd, GAME *game, CONFIG *config)
{
	static int diff2max[3] = { 15, 30, 50 };
	
	srand((unsigned int)time(NULL));
	
	memset(game, 0, sizeof(GAME));
	
	game->hWinMain = hWnd;
	
	game->state = STATE_GAMING;
	game->max = diff2max[config->diff-1];
	game->answer = rand() % (game->max+1);
	game->remain = 5;
	
	game->last_min = -1;
	game->last_max = game->max+1;
	game->last_value_old = -1;
	
	refresh_mainwin0(game);
	
	return 0;
}

int commit(GAME *game, int num)
{
	if(num == game->answer) {
		game->state = STATE_END;
		refresh_mainwin2(game);
	} else {
		game->state = STATE_FLASHING;
		game->remain--;
		game->last_value_old = num;
		game->flash_stage = 1;
		
		refresh_mainwin3(game);
		lock_all_controls(game->hWinMain);
	}
	
	return 0;
}
