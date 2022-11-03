// JackpotWin Re
// by icelter, October 2022 ~ November 2022

#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#pragma  comment (lib, "comctl32.lib")

#include "./include/main.h"
#include "./include/config.h"
#include "./include/game.h"
#include "./include/ui.h"
#include "./include/misc.h"
#include "./include/mainboard.h"
#include "./include/hyperlink.h"
#include "./include/aboutdiag.h"
#include "./include/resources.h"

int init_controls(void);
int init_ui(void);
int message_loop(void);
BOOL CALLBACK ProcWinMain(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	init_controls();
	init_ui();
	message_loop();
	
	return 0;
}

int init_controls(void)
{
	INITCOMMONCONTROLSEX InitControl;
	
	// Common controls
	InitControl.dwSize = sizeof(INITCOMMONCONTROLSEX);
	InitControl.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitControl);
	
	// Custom controls
	init_mainboard();
	init_hyperlink();
	
	return 0;
}

int init_ui(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wndclass;
	HWND hWinMain;
	HMENU hMenu;
	char titlebuf[MAX_STRLEN];
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(0, IDC_ARROW);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)ProcWinMain;
	wndclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wndclass.lpszClassName = "IcelterUtilities01";
	RegisterClassEx(&wndclass);
	
	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(MENU_MAIN));
	sprintf(titlebuf, "JackpotWin Re (build_%d)", VER_BUILD, BUILD_YEAR, BUILD_MONTH, BUILD_DAY);
	hWinMain = CreateWindowEx(0, "IcelterUtilities01", titlebuf,
						  WS_SYSMENU|WS_CAPTION|WS_POPUP|WS_VISIBLE,
						  100, 100, 300, 200,
						  NULL, hMenu, hInstance, NULL);
	ShowWindow(hWinMain, SW_SHOWNORMAL);
	UpdateWindow(hWinMain);
	
	return 0;
}

int message_loop(void)
{
	MSG message;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HACCEL hAccelerator = LoadAccelerators(hInstance, MAKEINTRESOURCE(ACCEL_MAIN));
	
	while(GetMessage(&message, NULL, 0, 0) != 0) {
		if(TranslateAccelerator(message.hwnd, hAccelerator, &message) == 0) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	
	UnregisterClass("IcelterUtilities01", hInstance);
	UnregisterClass("IcelterUtilities01-MainBoard", hInstance);
	UnregisterClass("icelter-hyperlink", hInstance);
	
	return 0;
}

BOOL CALLBACK ProcWinMain(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	static CONFIG config;
	static GAME game;
	int i;
	
	switch(uMessage) {
		case WM_COMMAND:
			wParam = LOWORD(wParam);
			
			switch(wParam) {
				case IDM_EASY:
				case IDM_MEDIUM:
				case IDM_DIFFICULT:
					i = wParam - IDM_EASY + 1;
					if(i != config.diff && 
					   (game.last_value_old == -1 || MessageBox(hWnd, "Are you continue?", "JackpotWin Re", MB_ICONINFORMATION|MB_OKCANCEL) == IDOK)) {
						config.diff = i;
						start(hWnd, &game, &config);
					}
					break;
					
				case IDC_BUTTON1:
					SendMessage(hWnd, WM_COMMAND, IDM_RETRY, NULL);
					break;
					
				case IDM_RETRY:
					if(game.state == STATE_END || 
					   game.last_value_old == -1 || MessageBox(hWnd, "Are you continue?", "JackpotWin Re", MB_ICONINFORMATION|MB_OKCANCEL) == IDOK) {
						start(hWnd, &game, &config);
					}
					break;
					
				case IDM_ABOUT:
					init_aboutdiag(hWnd);
					break;
					
				case IDM_QUIT:
					SendMessage(hWnd, WM_CLOSE, NULL, NULL);
					break;
					
			}
			break;
			
		case WM_CREATE:
			fix_windowsize(hWnd);
			load_controls(hWnd, &game);
			load_config(hWnd, &config);
			start(hWnd, &game, &config);
			break;
			
		case WM_CLOSE:
			if(game.state != STATE_FLASHING) {
				save_config(hWnd, &config);
				DestroyWindow(hWnd);
				PostQuitMessage(0);
			}
				
			break;
		
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	return FALSE;
}
