// Icelter Common Control [Demo]
// Hyperlink (icelter-hyperlink)
// 
// Custom Message:
// HYLM_SETLINK: Set the link which you click it
// wParam = (const char *) a string.
//
// HYLM_GETLINK: Get the link which you click it
// wParam = (char *) a buffer of string.
// lParam = (int *) the size of the buffer.
//
// Data Struct:
// Offset 0: the mouse state
// Offset 4: control's font handle
// Offset 8: link string

#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#pragma  comment (lib, "comctl32.lib")

#include "../include/hyperlink.h"

#define MOUSE_LEAVE		0
#define MOUSE_HOVER		1
#define MOUSE_CLICK		2

#define OFFSET_MOUSE	0
#define OFFSET_FONT		4
#define OFFSET_LINKPTR	8

#define MAX_STRLEN_		256

LRESULT CALLBACK ProcHyperlink(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
int paint_hyperlink(HWND hWnd);
int set_tme(HWND hWnd);

int init_hyperlink(void)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wndclass;
	
	memset(&wndclass, 0, sizeof(WNDCLASSEX));
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.hInstance = hInstance;
	wndclass.style = CS_VREDRAW|CS_HREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)ProcHyperlink;
	wndclass.cbWndExtra = 3*sizeof(long);
	wndclass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wndclass.lpszClassName = "icelter-hyperlink";
	
	RegisterClassEx(&wndclass);
	
	return 0;
}

LRESULT CALLBACK ProcHyperlink(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	char *link_str, *link_buf;
	int len;
	
	switch(uMessage) {
		case WM_MOUSEMOVE:
			set_tme(hWnd);
			SetWindowLong(hWnd, OFFSET_MOUSE, MOUSE_HOVER);
			break;
			
		case WM_PAINT:
			paint_hyperlink(hWnd);
			break;
			
		case WM_LBUTTONDOWN:
			SetWindowLong(hWnd, OFFSET_MOUSE, MOUSE_CLICK);
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
			break;
			
		case WM_LBUTTONUP:
			SetWindowLong(hWnd, OFFSET_MOUSE, MOUSE_HOVER);
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
			
			link_str = (char *)GetWindowLong(hWnd, OFFSET_LINKPTR);
			ShellExecute(NULL, "open", link_str, NULL, NULL, SW_SHOW);
			break;
			
		case WM_MOUSEHOVER:
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
			break;
	
		case WM_MOUSELEAVE:
			SetWindowLong(hWnd, OFFSET_MOUSE, MOUSE_LEAVE);
			RedrawWindow(hWnd, NULL, NULL, RDW_ERASE|RDW_INVALIDATE|RDW_ERASENOW);
			break;
			
		case HYLM_SETLINK:
			SetWindowLong(hWnd, OFFSET_LINKPTR, wParam);
			break;
			
		case HYLM_GETLINK:
			link_str = (char *)GetWindowLong(hWnd, OFFSET_LINKPTR);
			link_buf = (char *)wParam;
			len = strlen(link_str);
			
			if(lParam <= len) {
				strcat(link_buf, link_str);
			} else {
				strncat(link_buf, link_str, lParam);
				link_buf[lParam] = 0;
			}
			
			return (len <= lParam) ? len : lParam;
			break;

		case WM_SETFONT:
			SetWindowLong(hWnd, OFFSET_FONT, wParam);
			break;
			
		case WM_GETFONT:
			return GetWindowLong(hWnd, OFFSET_FONT);
			break;
			
		case WM_CREATE:
			set_tme(hWnd);
			SetWindowLong(hWnd, OFFSET_LINKPTR, NULL);
			SetWindowLong(hWnd, OFFSET_MOUSE, MOUSE_LEAVE);
			break;
			
		default:
			return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	
	return FALSE;
}

int paint_hyperlink(HWND hWnd)
{
	static long rgb2table[3] = {
		RGB(75, 149, 198), RGB(1, 61, 115), RGB(82, 82, 136)
	};
	
	PAINTSTRUCT paint;
	HDC hDevice = BeginPaint(hWnd, &paint);
	long mouse_state, rgb;
	HFONT hFont;
	char sbuf[MAX_STRLEN_];
	RECT rect;
	
	mouse_state = GetWindowLong(hWnd, OFFSET_MOUSE);
	rgb = rgb2table[mouse_state-MOUSE_LEAVE];
	hFont = (HFONT)GetWindowLong(hWnd, OFFSET_FONT);
	
	memset(sbuf, 0, MAX_STRLEN_);
	GetWindowText(hWnd, sbuf, MAX_STRLEN_);

	SetBkMode(hDevice, TRANSPARENT);
	SetTextColor(hDevice, rgb);
	SelectObject(hDevice, hFont);
	GetClientRect(hWnd, &rect);
	DrawText(hDevice, sbuf, -1, &rect, DT_SINGLELINE|DT_TABSTOP|DT_LEFT);

	EndPaint(hWnd, &paint);
	return 0;
}

int set_tme(HWND hWnd)
{
	TRACKMOUSEEVENT tme;
	
	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.hwndTrack = hWnd;
	tme.dwFlags = TME_LEAVE|TME_HOVER;
	tme.dwHoverTime = 1;
	
	_TrackMouseEvent(&tme);
	return 0;
}
