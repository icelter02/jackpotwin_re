#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#pragma  comment (lib, "comctl32.lib")

#include "../include/main.h"
#include "../include/config.h"
#include "../include/game.h"
#include "../include/mainboard.h"
#include "../include/ui.h"
#include "../include/misc.h"

BOOL CALLBACK ProcMainBoard(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int paint_mainboard(HWND hWnd, GAME *game);

int init_mainboard(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC) ProcMainBoard;
	wndclass.lpszClassName = "IcelterUtilities01-MainBoard";
	
	RegisterClassEx(&wndclass);
	
	return 0;
}

BOOL CALLBACK ProcMainBoard(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	static GAME *game = (GAME *)(((CREATESTRUCT *)lParam)->lpCreateParams);
	static TRACKMOUSEEVENT tme;
	static int old_num, num;
	
	switch(uMessage) {
		case WM_MOUSEMOVE:
			_TrackMouseEvent(&tme);
			
			game->mouse_state = MSTATE_HOVER;
			
			if(game->state == STATE_GAMING) {
				game->mouse_xpoint_old = game->mouse_xpoint;
				game->mouse_ypoint_old = game->mouse_ypoint;
				game->mouse_xpoint = LOWORD(lParam);
				game->mouse_ypoint = HIWORD(lParam);
			
				num = point2num(game->mouse_xpoint, game->mouse_ypoint, game->last_max, game->last_min, game->max);
				if(num != old_num) {
					old_num = num;
					if(num == -1) {
						refresh_mainwin0(game);
					} else {
						refresh_mainwin1(game);
					}
				}
				
				refresh_mainboard(hWnd, game);
			}
			break;
			
		case WM_SETCURSOR:
			num = point2num(game->mouse_xpoint, game->mouse_ypoint, game->last_max, game->last_min, game->max);
			if(game->state == STATE_GAMING && num != -1) {
				SetCursor(LoadCursor(NULL, IDC_CROSS));
			} else {
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
			break;
					
		case WM_PAINT:
			paint_mainboard(hWnd, game);
			break;
			
		case WM_TIMER:
			if(game->flash_stage >= 1 && game->flash_stage <= 6) {
				game->flash_stage++;
			} else if(game->flash_stage == 7) {
				game->flash_stage = 0;
				
				if(game->last_value_old < game->answer) {
					game->last_min = game->last_value_old;
				} else if(game->last_value_old > game->answer) {
					game->last_max = game->last_value_old;
				}
				
				unlock_all_controls(game->hWinMain);
				if(game->remain == 0) {
					game->state = STATE_END;
					refresh_mainwin2(game);
				} else {
					game->state = STATE_GAMING;
					refresh_mainwin0(game);
				}
				
				KillTimer(hWnd, FLASH_TIMER_ID);
			}
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
			
			break;
			
		case WM_LBUTTONDOWN:
			num = point2num(game->mouse_xpoint, game->mouse_ypoint, game->last_max, game->last_min, game->max);
			if(game->state == STATE_GAMING && num != -1) {
				commit(game, num);
			}
			break;
			
		case WM_MOUSELEAVE:
			game->mouse_state = MSTATE_LEAVE;
			
			if(game->state == STATE_GAMING) {
				refresh_mainwin0(game);
			}
			break;

		case WM_CREATE:
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.hwndTrack = hWnd;
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 1;
			
			old_num = -1;
			
			break;
			
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
 	}
	return FALSE;
}

int paint_mainboard(HWND hWnd, GAME *game)
{
	static long rgbtable0[3] = {
		RGB(255, 0, 32), RGB(240, 200, 20), RGB(0, 255, 32)
	};
	
	PAINTSTRUCT paint;
	HDC hDevice = BeginPaint(hWnd, &paint);
	HBRUSH hBrush0, hBrush1, hBrush2, hBrush3;
	HPEN hPen0, hPen1, hPen2;
	RECT rect;
	long rgb0, i, j, k, num;

	hBrush0 = CreateSolidBrush(RGB(16, 16, 16));
	if(game->remain > 3) {
		rgb0 = rgbtable0[2];
	} else if(game->remain <= 3 && game->remain > 1) {
		rgb0 = rgbtable0[1];
	} else if(game->remain <= 1) {
		rgb0 = rgbtable0[0];
	}
	hBrush1 = CreateSolidBrush(rgb0);
	hBrush2 = CreateSolidBrush(RGB(16, 64, 255));
	hBrush3 = CreateSolidBrush(RGB(255, 0, 32));
	
	hPen0 = CreatePen(PS_SOLID, 1, RGB(16, 16, 16));
	hPen1 = CreatePen(PS_SOLID, 1, RGB(255, 0, 32));
	hPen2 = CreatePen(PS_SOLID, 2, RGB(0, 255, 32));
	
	// Background
	GetWindowRect(hWnd, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;
	FillRect(hDevice, &rect, hBrush0);
	
	// HP Bar
	i = rect.bottom;
	j = rect.right;
	rect.right -= 2;
	rect.left = 2;
	rect.right = rect.left + ((rect.right-rect.left)*game->remain/5);
	
	rect.top = 2;
	rect.bottom = 4;
	FillRect(hDevice, &rect, hBrush1);
	
	// Background 2
	rect.top = 7;
	rect.bottom = i-2;
	rect.right = j-2;
	FillRect(hDevice, &rect, hBrush2);
	
	// Limit min and max, Or Flash
	i = rect.right;
	if(game->state == STATE_FLASHING && game->last_value_old < game->answer && game->flash_stage % 2 == 1) {
		k = game->last_value_old+1;
	} else {
		k = game->last_min+1;
	}
	rect.right = rect.left + ((rect.right-rect.left)*k/(game->max+1));
	FillRect(hDevice, &rect, hBrush3);
	
	rect.right = i;
	j = rect.left;
	if(game->state == STATE_FLASHING && game->last_value_old > game->answer && game->flash_stage % 2 == 1) {
		k = game->last_value_old;
	} else {
		k = game->last_max;
	}
	rect.left += ((rect.right-rect.left)*k/(game->max+1));
	FillRect(hDevice, &rect, hBrush3);
	rect.left = j;
	
	// Lines
	SelectObject(hDevice, hPen0);
	for(i = 0; i < game->max+1; i++) {
		if(i % 5 == 4) {
			k = 41;
		} else {
			k = 45;
		}
		
		j = rect.left+((rect.right-rect.left)*(i+1)/(game->max+1));
		MoveToEx(hDevice, j, k, NULL);
		LineTo(hDevice, j, 48);
	}
	
	// Red or Green Line
	if((game->state == STATE_GAMING && game->mouse_state == MSTATE_HOVER)) {
		num = point2num(game->mouse_xpoint, game->mouse_ypoint, game->last_max, game->last_min, game->max);
		if(num != -1) {
			SelectObject(hDevice, hPen1);
			MoveToEx(hDevice, game->mouse_xpoint, 7, NULL);
			LineTo(hDevice, game->mouse_xpoint, 48);
		}
	} else if ((game->state == STATE_END && game->remain != 0)) {
		SelectObject(hDevice, hPen2);
		MoveToEx(hDevice, game->mouse_xpoint, 7, NULL);
		LineTo(hDevice, game->mouse_xpoint, 47);
	}

	DeleteObject(hBrush0);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
	
	DeleteObject(hPen0);
	DeleteObject(hPen1);
	DeleteObject(hPen2);
	
	EndPaint(hWnd, &paint);
	return 0;
}

int point2num(int x, int y, int lmax, int lmin, int max)
{
	if(x < 2+((MAINBOARD_XSIZE-4)*(lmin+1)/(max+1)+1) || x > MAINBOARD_XSIZE-(2+(MAINBOARD_XSIZE-4)*(max-lmax+1)/(max+1)+1)) {
		return -1;
	}
	
	if(y < 6 || y > MAINBOARD_YSIZE-2) {
		return -1;
	}
	
	return (x-2)*(max+1)/(MAINBOARD_XSIZE-4);
}

int refresh_mainboard(HWND hWnd, GAME *game)
{
	RECT rect;
	
	rect.top = 7;
	rect.bottom = 48;
	rect.left = game->mouse_xpoint;
	rect.right = rect.left+1;
	RedrawWindow(hWnd, &rect, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
	
	rect.left = game->mouse_xpoint_old;
	rect.right = rect.left+1;
	RedrawWindow(hWnd, &rect, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
	
	return 0;
}

