#include <windows.h>

#include "./include/main.h"
#include "./include/misc.h"

typedef struct {
	HWND hWnd;
	DWORD process_id;
} EnumWindowsArg;

HWND get_hwnd(void);
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

int panic(const char *errmsg)
{
	HWND hWnd;
	
	#ifdef _DEBUG
	hWnd = get_hwnd();
	MessageBox(hWnd, errmsg, "Error", MB_ICONHAND);
	ExitProcess(1);
	#endif
	
	return 0;
}

HWND get_hwnd(void)
{
	EnumWindowsArg ewa;
	
	memset(&ewa, 0, sizeof(ewa));
	ewa.process_id = GetCurrentProcessId();
	ewa.hWnd = NULL;
	
	EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
	return ewa.hWnd;
}

BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	EnumWindowsArg *ewa = (EnumWindowsArg *) lParam;
	DWORD process_id;
	static char classname[MAX_STRLEN];
	
	GetWindowThreadProcessId(hWnd, &process_id);
	if(ewa->process_id = process_id) {
		// 显然, 在一个进程下不会只有一个窗口
		// 所以我们要判断类名, 来确认是不是我们创建的窗口
		memset(classname, 0, MAX_STRLEN);
		GetClassName(hWnd, (char *)classname, MAX_STRLEN);
		
		if(strcmp(classname, "IcelterUtilities01") == 0) {
			ewa->hWnd = hWnd;
			return false;
		}
	}
	
	return true;
}

int TopXY(int a, int b)
{
	return (a >> 1) - (b >> 1);
}

