// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <stdio.h>

PVOID OldProc;

typedef  int (WINAPI* _fun)(HWND, LPCTSTR, LPCTSTR, UINT);
int HookMsg();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		HookMsg();
		return true;
    case DLL_THREAD_ATTACH:
		HookMsg();
		return true;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


int WINAPI  NewProc(HWND, LPCTSTR buffer, LPCTSTR, UINT)
{
	TCHAR NewMsg[MAX_PATH];
	char szBuffer[260];
	wsprintfW(NewMsg, L"HOOK的信息框 原消息：%s", buffer);
	WideCharToMultiByte(CP_ACP, 0, NewMsg, -1,szBuffer, 260, NULL, 0);
	wprintf_s(NewMsg);
	MessageBoxA(0, szBuffer, 0, 0);
	return 1;
}

int HookMsg()
{
	HANDLE hProcess = GetCurrentProcess();
	HMODULE hDll = LoadLibraryW(L"User32.dll");
	OldProc = MessageBoxW;// (PVOID)GetProcAddress(hDll, "MessageBoxW");
	BYTE code[5];
	code[0] = 0xE9;
	*(DWORD*)(code + 1) = (DWORD)NewProc - (DWORD)OldProc - 5;
	WriteProcessMemory(hProcess, OldProc, code, 5, 0);
	MessageBoxW(0, L"123321", 0, 0);
	return 0;
}
