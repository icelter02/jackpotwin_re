#ifndef JACKPOTWIN_RE_UI_INC
#define JACKPOTWIN_RE_UI_INC

#define MAINWIN_XSIZE	320
#define MAINWIN_YSIZE	120

const char *choose_font(const char *font_name, const char *default_name);
int fix_windowsize(HWND hWnd);
int load_controls(HWND hWnd, GAME *game);

int refresh_mainwin0(GAME *game);
int refresh_mainwin1(GAME *game);
int refresh_mainwin2(GAME *game);
int refresh_mainwin3(GAME *game);

int lock_all_controls(HWND hWnd);
int unlock_all_controls(HWND hWnd);

#endif