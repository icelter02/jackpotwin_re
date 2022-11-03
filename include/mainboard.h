#ifndef JACKPOTWIN_RE_MAINBOARD_INC
#define JACKPOTWIN_RE_MAINBOARD_INC

#define MAINBOARD_XSIZE 304
#define MAINBOARD_YSIZE	50

#define FLASH_TIMER_ID	2000

int init_mainboard(void);
int point2num(int x, int y, int lmax, int lmin, int num);
int refresh_mainboard(HWND hWnd, GAME *game);

#endif
