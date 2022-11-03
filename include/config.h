#ifndef JACKPOTWIN_RE_CONFIG_INC
#define JACKPOTWIN_RE_CONFIG_INC

typedef struct {
	int diff;
	int xpos, ypos, xscr, yscr;
} CONFIG;

#define INVALID_CONFIG		0x3fffffff

#define DIFF_EASY			1
#define	DIFF_MEDIUM			2
#define DIFF_DIFFICULT		3

int load_config(HWND hWnd, CONFIG *config);
int save_config(HWND hWnd, CONFIG *config);

#endif
