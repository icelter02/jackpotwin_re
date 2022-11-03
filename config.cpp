#include <stdio.h>
#include <windows.h>

#include "./include/main.h"
#include "./include/config.h"
#include "./include/diffdiag.h"
#include "./include/misc.h"

int load_config(HWND hWnd, CONFIG *config)
{
	int xscr, yscr;
	RECT rectWindow;
	
	config->diff = GetPrivateProfileInt("Icelter", "Difficulty", INVALID_CONFIG, "./jackpotwin.ini");
	config->xpos = GetPrivateProfileInt("Icelter", "XPosition", INVALID_CONFIG, "./jackpotwin.ini");
	config->ypos = GetPrivateProfileInt("Icelter", "YPosition", INVALID_CONFIG, "./jackpotwin.ini");
	config->xscr = GetPrivateProfileInt("Icelter", "XScreen", INVALID_CONFIG, "./jackpotwin.ini");
	config->yscr = GetPrivateProfileInt("Icelter", "YScreen", INVALID_CONFIG, "./jackpotwin.ini");
	
	if(config->diff == INVALID_CONFIG) {
		config->diff = query_difficulty();
	}
	
	xscr = GetSystemMetrics(SM_CXSCREEN);
	yscr = GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(hWnd, &rectWindow);
	
	if(config->xscr != INVALID_CONFIG && config->yscr != INVALID_CONFIG && config->xpos != INVALID_CONFIG && config->ypos != INVALID_CONFIG) {
		if(config->xscr == xscr && config->yscr == yscr) {
			if(config->xpos < xscr && config->xpos+(rectWindow.right-rectWindow.left) > 0) {
				if(config->ypos < yscr && config->ypos+(rectWindow.bottom-rectWindow.top) > 0) {
					SetWindowPos(hWnd, HWND_TOP, config->xpos, config->ypos, 0, 0, SWP_NOSIZE);
					return 0;
				}
			}
		}
	}
	
	config->xscr = xscr;
	config->yscr = yscr;
	config->xpos = TopXY(xscr, (rectWindow.right-rectWindow.left));
	config->ypos = TopXY(yscr, (rectWindow.bottom-rectWindow.top));
	
	SetWindowPos(hWnd, HWND_TOP, config->xpos, config->ypos, 0, 0, SWP_NOSIZE);
	return 0;
}

int save_config(HWND hWnd, CONFIG *config)
{
	int xscr, yscr;
	char sbuf[MAX_STRLEN];
	RECT rectWindow;
	
	xscr = GetSystemMetrics(SM_CXSCREEN);
	yscr = GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(hWnd, &rectWindow);
	
	if(config->xscr != xscr) {
		config->xscr = xscr;
	}
	
	if(config->yscr != yscr) {
		config->yscr = yscr;
	}
	
	config->xpos = rectWindow.left;
	config->ypos = rectWindow.top;
	
	sprintf(sbuf, "%d\0", config->diff);
	WritePrivateProfileString("Icelter", "Difficulty", sbuf, "./jackpotwin.ini");
	sprintf(sbuf, "%d\0", config->xpos);
	WritePrivateProfileString("Icelter", "XPosition", sbuf, "./jackpotwin.ini");
	sprintf(sbuf, "%d\0", config->ypos);
	WritePrivateProfileString("Icelter", "YPosition", sbuf, "./jackpotwin.ini");
	sprintf(sbuf, "%d\0", config->xscr);
	WritePrivateProfileString("Icelter", "XScreen", sbuf, "./jackpotwin.ini");
	sprintf(sbuf, "%d\0", config->yscr);
	WritePrivateProfileString("Icelter", "YScreen", sbuf, "./jackpotwin.ini");
	
	return 0;
}
