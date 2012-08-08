#include <afxwin.h>         // MFC core and standard components

#define DLLEXPORT __declspec(dllexport)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CCheckInputDll : public CWinApp {
public:
	CCheckInputDll()  { }
	~CCheckInputDll() { }
} theDll;

#pragma data_seg (".CheckInput")
HHOOK g_hHookKeyboard = NULL;	
HHOOK g_hHookMouse = NULL;	
DWORD	g_dwLastInputTick = 0;
#pragma data_seg ()

/////////////////
// Get tick count of last keyboard or mouse event
//
DLLEXPORT DWORD CheckInputGetLastInputTime()
{
	return g_dwLastInputTick;
}

/////////////////
// Keyboard hook: record tick count
//
LRESULT CALLBACK CheckInputKeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code==HC_ACTION) {
		g_dwLastInputTick = GetTickCount();
	}
	return ::CallNextHookEx(g_hHookKeyboard, code, wParam, lParam);
}

/////////////////
// Mouse hook: record tick count
//
LRESULT CALLBACK CheckInputMouseHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (code==HC_ACTION) {
		g_dwLastInputTick = GetTickCount();
	}
	return ::CallNextHookEx(g_hHookMouse, code, wParam, lParam);
}

//////////////////
// Initialize DLL: install kbd/mouse hooks.
//
DLLEXPORT BOOL CheckInputInit()
{
	if (g_hHookKeyboard == NULL)
	{
		HINSTANCE hInst = theDll.m_hInstance;
		g_hHookKeyboard	= SetWindowsHookEx(WH_KEYBOARD, CheckInputKeyboardHook,   hInst, 0);
		g_hHookMouse 	= SetWindowsHookEx(WH_MOUSE,	CheckInputMouseHook, 	hInst, 0);
		g_dwLastInputTick = GetTickCount(); // init count
	}
	ASSERT(g_hHookKeyboard);
	ASSERT(g_hHookMouse);
	return TRUE;
}

//////////////////
// Terminate DLL: remove hooks.
//
DLLEXPORT void CheckInputTerm()
{
	BOOL bRet1 = UnhookWindowsHookEx(g_hHookKeyboard);
	BOOL bRet2 = UnhookWindowsHookEx(g_hHookMouse);
	ASSERT(bRet1 && bRet2);
}
