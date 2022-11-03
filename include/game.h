#ifndef JACKPOTWIN_RE_GAME_INC
#define JACKPOTWIN_RE_GAME_INC

#define STATE_GAMING	1
#define STATE_FLASHING	2
#define STATE_END		3

#define MSTATE_LEAVE	0
#define MSTATE_HOVER	1

typedef struct {
	HWND hWinMain;
	
	int state;
	int max, answer, remain;
	int last_max, last_min;
	
	int last_value_old, flash_stage, mouse_state;
	int mouse_xpoint, mouse_ypoint, mouse_xpoint_old, mouse_ypoint_old;
} GAME;

int start(HWND hWnd, GAME *game, CONFIG *config);
int commit(GAME *game, int num);

#endif
