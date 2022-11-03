#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#pragma  comment (lib, "comctl32.lib")

#include "../include/main.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/ui.h"
#include "../include/misc.h"
#include "../include/aboutdiag.h"
#include "../include/resources.h"

int init_dialog2(HWND hWinMain);
int message_loop_aboutdlg(void);
LRESULT CALLBACK ProcAboutDialog(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ProcAboutDialogSub(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int fix_windowsize_aboutdlg(HWND hWnd);
int load_controls_aboutdlg(HWND hWnd);
int paint_aboutdlgsub(HWND hWnd);

// Dangerous Design!
#define TABID_ABOUT		0
#define TABID_HELP		1
int g_tabID[2];

int init_aboutdiag(HWND hWinMain)
{
	init_dialog2(hWinMain);
	message_loop_aboutdlg();
	return 0;
}

int init_dialog2(HWND hWinMain)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wndclass;
	HWND hAboutDlg;
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(0, IDC_ARROW);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)ProcAboutDialog;
	wndclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wndclass.lpszClassName = "IcelterUtilities01-AboutDialog";
	RegisterClassEx(&wndclass);
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(0, IDC_ARROW);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)ProcAboutDialogSub;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wndclass.lpszClassName = "IcelterUtilities01-AboutDialog-Sub";
	RegisterClassEx(&wndclass);

	hAboutDlg = CreateWindowEx(0, "IcelterUtilities01-AboutDialog", "About JackpotWin Re",
						  WS_SYSMENU|WS_CAPTION|WS_POPUP|WS_VISIBLE|WS_CHILD,
						  100, 100, 300, 200,
						  hWinMain, NULL, hInstance, NULL);
	ShowWindow(hAboutDlg, SW_SHOWNORMAL);
	UpdateWindow(hAboutDlg);
	
	EnableWindow(hWinMain, FALSE);
	
	return 0;
}

int message_loop_aboutdlg(void)
{
	MSG message;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	while(GetMessage(&message, NULL, 0, 0) != 0) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnregisterClass("IcelterUtilities01-AboutDialog", hInstance);
	UnregisterClass("IcelterUtilities01-AboutDialog-Sub", hInstance);
	return 0;
}

LRESULT CALLBACK ProcAboutDialog(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	HWND hTab, hControl;
	int i;
	
	switch(uMessage) {
		case WM_COMMAND:
			switch(wParam) {
				case IDC_BUTTON1_D2:
					SendMessage(hWnd, WM_CLOSE, NULL, NULL);
					break;
					
				case IDC_BUTTON2_D2:
					ShellExecute(NULL, "open", "https://github.com/icelter02/jackpotwin_re", NULL, NULL, SW_SHOW);
					break;
					
				case IDC_BUTTON3_D2:
					ShellExecute(NULL, "open", "https://icelter02.github.io", NULL, NULL, SW_SHOW);
					break;
			}
			break;
			
		case WM_NOTIFY:
			if(((NMHDR *)lParam)->code == TCN_SELCHANGE) {
				hTab = GetDlgItem(hWnd, IDC_TABCTRL_D2);
				i = SendMessage(hTab, TCM_GETCURSEL, NULL, NULL);
				if(i == g_tabID[TABID_ABOUT]) {
					for(i = IDC_BUTTON2_D2; i <= IDC_ABTD_SUB_D2; i++) {
						hControl = GetDlgItem(hWnd, i);
						ShowWindow(hControl, SW_SHOW);
					}
					
					hControl = GetDlgItem(hWnd, IDC_RICHEDIT_D2);
					ShowWindow(hControl, SW_HIDE);
				} else if(i == g_tabID[TABID_HELP]) {
					for(i = IDC_BUTTON2_D2; i <= IDC_ABTD_SUB_D2; i++) {
						hControl = GetDlgItem(hWnd, i);
						ShowWindow(hControl, SW_HIDE);
					}
					
					hControl = GetDlgItem(hWnd, IDC_RICHEDIT_D2);
					ShowWindow(hControl, SW_SHOW);
				}
			}
			break;
			
		case WM_CREATE:
			fix_windowsize_aboutdlg(hWnd);
			load_controls_aboutdlg(hWnd);

			break;
			
		case WM_CLOSE:
			EnableWindow((HWND)GetWindowLong(hWnd, GWL_HWNDPARENT), TRUE);
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			
			break;
			
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	
	return false;
}

LRESULT CALLBACK ProcAboutDialogSub(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uMessage) {
		case WM_PAINT:
			paint_aboutdlgsub(hWnd);
			break;
			
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}

	return false;
}

int fix_windowsize_aboutdlg(HWND hWnd)
{
	int xWindow, yWindow, xPosition, yPosition;
	RECT rectWindow, rectClient;
	
	GetWindowRect(hWnd, &rectWindow);
	GetClientRect(hWnd, &rectClient);
	
	xWindow = (rectWindow.right - rectWindow.left) - (rectClient.right - rectClient.left) + ABOUTDIAG_XSIZE;
	yWindow = (rectWindow.bottom - rectWindow.top) - (rectClient.bottom - rectClient.top) + ABOUTDIAG_YSIZE;
	xPosition = TopXY(GetSystemMetrics(SM_CXSCREEN), xWindow);
	yPosition = TopXY(GetSystemMetrics(SM_CYSCREEN), yWindow);
	
	SetWindowPos(hWnd, HWND_TOP, xPosition, yPosition, xWindow, yWindow, 0);
	return 0;
}

int load_controls_aboutdlg(HWND hWnd)
{
	HWND hControl;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HFONT hFont;
	HRSRC hResInfo;
	HGLOBAL hResource;
	TC_ITEM tc_item;
	char *sbuf;
	int i;

	const char *fontname = choose_font("Noto Sans CJK Regular", "Tahoma");
	int fontsize = (strcmp(fontname, "Noto Sans CJK Regular") == 0) ? 16 : 15;
	hFont = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
							 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, fontname);
	
	hControl = CreateWindowEx(0, "button", "OK", WS_CHILD|WS_VISIBLE,
							  264, 248, 57, 25,
							  hWnd, (HMENU)IDC_BUTTON1_D2, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "SysTabControl32", "", WS_CHILD|WS_VISIBLE|TCS_FIXEDWIDTH|TCS_RIGHTJUSTIFY|TCS_HOTTRACK|TCS_MULTILINE,
							  8, 8, 314, 233,
							  hWnd, (HMENU)IDC_TABCTRL_D2, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	tc_item.mask = TCIF_TEXT;
	tc_item.pszText = "About";
	i = SendMessage(hControl, TCM_GETITEMCOUNT, NULL, NULL);
	g_tabID[TABID_ABOUT] = SendMessage(hControl, TCM_INSERTITEM, i, (LPARAM)&tc_item);
	
	tc_item.pszText = "Help";
	i = SendMessage(hControl, TCM_GETITEMCOUNT, NULL, NULL);
	g_tabID[TABID_HELP] = SendMessage(hControl, TCM_INSERTITEM, i, (LPARAM)&tc_item);
	
	hControl = CreateWindowEx(0, "button", "This program's github repository", WS_CHILD|WS_VISIBLE,
							  16, 170, 225, 22,
							  hWnd, (HMENU)IDC_BUTTON2_D2, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "button", "Icelter's home page", WS_CHILD|WS_VISIBLE,
							  16, 197, 225, 22,
							  hWnd, (HMENU)IDC_BUTTON3_D2, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	
	hControl = CreateWindowEx(0, "IcelterUtilities01-AboutDialog-Sub", "", WS_CHILD|WS_VISIBLE,
							  16, 40, 297, 125,
							  hWnd, (HMENU)IDC_ABTD_SUB_D2, hInstance, NULL);
							  
	hResInfo = FindResource(hInstance, MAKEINTRESOURCE(RCDATA_HELP), RT_RCDATA);
	hResource = LoadResource(hInstance, hResInfo);
	sbuf = (char *)LockResource(hResource);
	
	hControl = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_MULTILINE,
							  16, 40, 297, 193,
							  hWnd, (HMENU)IDC_RICHEDIT_D2, hInstance, NULL);
	SendMessage(hControl, WM_SETFONT, (WPARAM)hFont, NULL);
	SendMessage(hControl, WM_SETTEXT, NULL, (LPARAM)sbuf);
	ShowWindow(hControl, SW_HIDE);
	
	return 0;
}

int paint_aboutdlgsub(HWND hWnd)
{
	PAINTSTRUCT paint;
	HDC hDevice = BeginPaint(hWnd, &paint);
	HFONT hFont;
	RECT rect;
	char sbuf[MAX_STRLEN];
	
	const char *fontname = choose_font("Noto Sans CJK Regular", "Tahoma");
	int fontsize = (strcmp(fontname, "Noto Sans CJK Regular") == 0) ? 16 : 15;
	hFont = CreateFont(fontsize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
							 ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, fontname);
	
	sprintf(sbuf, "Icelter Utilities #01 - JackpotWin Re\nby Icelter(Ghdgtdgu), 2019-2022\n\nClassical window program, MSVC-I386, Win32-API\nversion_%d, build_%d, build at %04d-%02d-%02d\n\nExterned link:", 
			VER_MAIN, VER_BUILD, BUILD_YEAR, BUILD_MONTH, BUILD_DAY);
			
	GetClientRect(hWnd, &rect);
	SelectObject(hDevice, hFont);
	SetBkMode(hDevice, TRANSPARENT);
	DrawText(hDevice, sbuf, -1, &rect, DT_TABSTOP|DT_LEFT);
	
	EndPaint(hWnd, &paint);
	return 0;
}
