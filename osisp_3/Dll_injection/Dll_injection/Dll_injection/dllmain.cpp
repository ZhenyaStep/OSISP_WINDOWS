// dllmain.cpp: определяет точку входа для приложения DLL.
#include "stdafx.h"

#define DLL_IMPORT __declspec(dllimport)

extern "C" DLL_IMPORT void _stdcall PrintStr();

static int count = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
	if (count == 0)
		PrintStr();
	count++;
    return TRUE;
}

