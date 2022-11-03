#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#pragma  comment (lib, "comctl32.lib")

#include "./include/main.h"
#include "./include/config.h"
#include "./include/game.h"
#include "./include/mainboard.h" 
#include "./include/ui.h"
#include "./include/resources.h"

BOOL CALLBACK FontEnumProc(LPLOGFONT logfont, LPTEXTMETRIC textmetric, int iType, LPARAM lParam);

const char *choose_font(const char *font_name, const char *default_name)
{
	HDC hDevice;
	LOGFONT logfont;
	static bool isExist = false;
	
	hDevice = GetDC(NULL);
	
	memset(&logfont, 0, sizeof(LOGFONT));
	logfont.lfCharSet = DEFAULT_CHARSET;
	strcat(logfont.lfFaceName, font_name);
	
	EnumFontFamiliesEx(hDevice, &logfont, (FONTENUMPROC)FontEnumProc, (LPARAM)&isExist, 0);
	
	ReleaseDC(NULL, hDevice);
	
	return (isExist == true) ? font_name : default_name;
}

BOOL CALLBACK FontEnumProc(LPLOGFONT logfont, LPTEXTMETRIC textmetric, int iType, LPARAM lParam)
{
	bool *isExist = (bool *)lParam;
	*isExist = true;
	
	return false;
}

int fix_windowsize(HWND hWnd)
{
	int xWindow, yWindow;
	RECT rectWindow, rectClient;
	
	GetWindowRect(hWnd, &rectWindow);
	GetClientRect(hWnd, &rectClient);
	
	xWindow = (rectWindow.right - rectWindow.left) - (rectClient.right - rectClient.left) + MAINWIN_XSIZE;
	yWindow = (rectWindow.bottom - rectWindow.top) - (rectClient.bottom - rectClient.top) + MAINWIN_YSIZE;
	
	SetWindowPos(hWnd, HWND_TOP, 0, 0, xWindow, yWindow, SWP_NOMOVE);
	return 0;
}

int load_controls(HWND hWnd, GAME *game)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND hControl;
	HFONT hFont1, hFont2;
	
	// Create fonts
	const char *fontname = choose_font("Noto Sans CJK Regular", "Tahoma");
	int fontsize = (strcmp(fontname, "Noto Sans CJK Regular") == 0) ? 18 : 16;
	hFont1 = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
							 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, fontname);
							 
	fontsize = (strcmp(fontname, "Noto Sans CJK Regular") == 0) ? 20 : 17;
	hFont2 = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
							 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, fontname);
	
	// Create Controls
	hControl = CreateWindowEx(0, "IcelterUtilities01-MainBoard", "",
							  WS_VISIBLE|WS_CHILD, 8, 8, MAINBOARD_XSIZE, MAINBOARD_YSIZE,
							  hWnd, (HMENU) IDC_MAINBOARD, hInstance, game);

	hControl = CreateWindowEx(0, "static", "SAMPLE TEXT",
							  WS_VISIBLE|WS_CHILD, 8, 65, 240, 24,
							  hWnd, (HMENU) IDC_LABEL1, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont2, NULL);
	
	hControl = CreateWindowEx(0, "button", "Retry",
							  WS_VISIBLE|WS_CHILD, 256, 65, 56, 24,
							  hWnd, (HMENU) IDC_BUTTON1, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont1, NULL);
	
	hControl = CreateStatusWindow(WS_CHILD|WS_VISIBLE, "SAMPLE TEXT", hWnd, IDC_STATUSBAR);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont1, NULL);
	
	return 0;
}

int refresh_mainwin0(GAME *game)
{
	static int diff2max[3] = { 15, 30, 50 };
	
	HMENU hMenu;
	HWND hControl;
	char sbuf[MAX_STRLEN];
	int diff;
	
	hControl = GetDlgItem(game->hWinMain, IDC_LABEL1);
	SetWindowText(hControl, "Please select your number.");
	
	sprintf(sbuf, "Lives remaining: %d", game->remain);
	hControl = GetDlgItem(game->hWinMain, IDC_STATUSBAR);
	SendMessage(hControl, SB_SETTEXT, (WPARAM)SBT_NOBORDERS, (LPARAM)sbuf);
	
	for(diff = 0; diff < 3; diff++) {
		if(diff2max[diff] == game->max) {
			break;
		}
	}
	
	hMenu = GetMenu(game->hWinMain);
	CheckMenuRadioItem(hMenu, IDM_EASY, IDM_DIFFICULT, IDM_EASY+diff, MF_BYCOMMAND);
	
	hControl = GetDlgItem(game->hWinMain, IDC_MAINBOARD);
	RedrawWindow(hControl, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
	
	return 0;
}

int refresh_mainwin1(GAME *game)
{
	HWND hControl;
	char sbuf[MAX_STRLEN];
	int num = point2num(game->mouse_xpoint, game->mouse_ypoint, game->last_max, game->last_min, game->max);
	
	if(game->state == STATE_GAMING && num != -1) {
		sprintf(sbuf, "Your choosed number is: %d", num);
		hControl = GetDlgItem(game->hWinMain, IDC_STATUSBAR);
		SendMessage(hControl, SB_SETTEXT, (WPARAM)SBT_NOBORDERS, (LPARAM)sbuf);
	}

	return 0;
}

int refresh_mainwin2(GAME *game)
{
	HWND hControl;
	char sbuf[MAX_STRLEN];

	hControl = GetDlgItem(game->hWinMain, IDC_LABEL1);
	if(game->remain != 0) {
		SetWindowText(hControl, "You win!");
	} else {
		SetWindowText(hControl, "You lose!");
	}
	
	sprintf(sbuf, "Lives remaining: %d", game->remain);
	hControl = GetDlgItem(game->hWinMain, IDC_STATUSBAR);
	SendMessage(hControl, SB_SETTEXT, (WPARAM)SBT_NOBORDERS, (LPARAM)sbuf);
	
	hControl = GetDlgItem(game->hWinMain, IDC_MAINBOARD);
	RedrawWindow(hControl, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);

	return 0;
}

int refresh_mainwin3(GAME *game)
{
	HWND hControl;
	char sbuf[MAX_STRLEN];

	hControl = GetDlgItem(game->hWinMain, IDC_LABEL1);
	if(game->last_value_old < game->answer) {
		SetWindowText(hControl, "It's smaller than the answer!");
	} else if(game->last_value_old > game->answer) {
		SetWindowText(hControl, "It's bigger than the answer!");
	}
	
	sprintf(sbuf, "Lives remaining: %d", game->remain);
	hControl = GetDlgItem(game->hWinMain, IDC_STATUSBAR);
	SendMessage(hControl, SB_SETTEXT, (WPARAM)SBT_NOBORDERS, (LPARAM)sbuf);
	
	hControl = GetDlgItem(game->hWinMain, IDC_MAINBOARD);
	SetTimer(hControl, FLASH_TIMER_ID, 333, NULL);
	RedrawWindow(hControl, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
	
	return 0;
}

int lock_all_controls(HWND hWnd)
{
	HWND hControl;
	HMENU hMenu;
	int i;
	
	hControl = GetDlgItem(hWnd, IDC_BUTTON1);
	EnableWindow(hControl, false);
	
	hMenu = GetMenu(hWnd);
	for(i = IDM_EASY; i <= IDM_RETRY; i++) {
		EnableMenuItem(hMenu, i, MF_GRAYED);
	}
	
	return 0;
}

int unlock_all_controls(HWND hWnd)
{
	HWND hControl;
	HMENU hMenu;
	int i;
	
	hControl = GetDlgItem(hWnd, IDC_BUTTON1);
	EnableWindow(hControl, true);
	
	hMenu = GetMenu(hWnd);
	for(i = IDM_EASY; i <= IDM_RETRY; i++) {
		EnableMenuItem(hMenu, i, MF_ENABLED);
	}
	
	return 0;
}
