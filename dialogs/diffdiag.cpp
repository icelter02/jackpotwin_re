#include <stdio.h>
#include <windows.h>

#include "../include/main.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/ui.h"
#include "../include/misc.h"	
#include "../include/resources.h"
#include "../include/diffdiag.h"
#include "../include/hyperlink.h"

int init_dialog1(int *diff);
BOOL CALLBACK ProcDiffDialog(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int message_loop_diffdlg(void);
int fix_windowsize_diffdlg(HWND hWnd);
int load_controls_diffdlg(HWND hWnd);

int query_difficulty(void)
{
	static int diff = 1;
	
	init_dialog1(&diff);
	message_loop_diffdlg();
	
	return diff;
}

int init_dialog1(int *diff)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wndclass;
	HWND hDiffDlg;
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(0, IDC_ARROW);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)ProcDiffDialog;
	wndclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wndclass.lpszClassName = "IcelterUtilities01-DifficultyDialog";
	RegisterClassEx(&wndclass);

	hDiffDlg = CreateWindowEx(0, "IcelterUtilities01-DifficultyDialog", "Choose your difficult",
						  WS_SYSMENU|WS_CAPTION|WS_POPUP|WS_VISIBLE,
						  100, 100, 300, 200,
						  NULL, NULL, hInstance, diff);
	ShowWindow(hDiffDlg, SW_SHOWNORMAL);
	UpdateWindow(hDiffDlg);
	
	return 0;
}

int message_loop_diffdlg(void)
{
	MSG message;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	while(GetMessage(&message, NULL, 0, 0) != 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnregisterClass("IcelterUtilities01-DifficultyDialog", hInstance);
	return 0;
}

BOOL CALLBACK ProcDiffDialog(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	static int *diff = (int *)(((CREATESTRUCT *)lParam)->lpCreateParams);
	HWND hControl;
	
	switch(uMessage) {
		case WM_COMMAND:
			switch(wParam) {
				case IDC_BUTTON1_D1:
					SendMessage(hWnd, WM_CLOSE, NULL, NULL);
					break;
					
				default:
					hControl = GetDlgItem(hWnd, wParam);
					*diff = wParam - IDC_RADIO1_D1 + 1;
			}
			break;
			
		case WM_CREATE:
			fix_windowsize_diffdlg(hWnd);
			load_controls_diffdlg(hWnd);
			
			*diff = 1;
			break;
			
		case WM_CLOSE:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			break;
			
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	
	return false;
}

int fix_windowsize_diffdlg(HWND hWnd)
{
	int xWindow, yWindow, xPosition, yPosition;
	RECT rectWindow, rectClient;
	
	GetWindowRect(hWnd, &rectWindow);
	GetClientRect(hWnd, &rectClient);
	
	xWindow = (rectWindow.right - rectWindow.left) - (rectClient.right - rectClient.left) + DIFFDIAG_XSIZE;
	yWindow = (rectWindow.bottom - rectWindow.top) - (rectClient.bottom - rectClient.top) + DIFFDIAG_YSIZE;
	xPosition = TopXY(GetSystemMetrics(SM_CXSCREEN), xWindow);
	yPosition = TopXY(GetSystemMetrics(SM_CYSCREEN), yWindow);
	
	SetWindowPos(hWnd, HWND_TOP, xPosition, yPosition, xWindow, yWindow, 0);
	return 0;
}

int load_controls_diffdlg(HWND hWnd)
{
	HWND hControl;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HFONT hFont;

	const char *fontname = choose_font("Noto Sans CJK Regular", "Tahoma");
	int fontsize = (strcmp(fontname, "Noto Sans CJK Regular") == 0) ? 16 : 15;
	hFont = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
							 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, fontname);
	
	hControl = CreateWindowEx(0, "button", "", WS_VISIBLE|WS_CHILD|BS_GROUPBOX,
							  8, 8, 219, 99,
							  hWnd, (HMENU)IDC_GROUPBOX_D1, hInstance, NULL);
							  
	hControl = CreateWindowEx(0, "button", "Easy (0 ~ 15)", WS_VISIBLE|WS_CHILD|WS_TABSTOP|WS_GROUP|BS_AUTORADIOBUTTON,
							  16, 24, 185, 25,
							  hWnd, (HMENU)IDC_RADIO1_D1, hInstance, NULL);
	SendMessage(hControl, BM_SETCHECK, BST_CHECKED, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "button", "Medium (0 ~ 30)", WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_AUTORADIOBUTTON,
							  16, 48, 185, 25,
							  hWnd, (HMENU)IDC_RADIO2_D1, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "button", "Difficult (0 ~ 50)", WS_VISIBLE|WS_CHILD|WS_TABSTOP|BS_AUTORADIOBUTTON,
							  16, 72, 185, 25,
							  hWnd, (HMENU)IDC_RADIO3_D1, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "button", "OK", WS_VISIBLE|WS_CHILD,
							  171, 112, 57, 25,
							  hWnd, (HMENU)IDC_BUTTON1_D1, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);

	return 0;
}
