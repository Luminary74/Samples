// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    _declspec(dllexport) void OnHook();
    _declspec(dllexport) void OffHook();

#ifdef __cplusplus
}
#endif // __cplusplus

HHOOK g_hook = NULL;
HINSTANCE instance = NULL;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        instance = hModule;
        MessageBoxA(0, "life is a fucking movie", "life is a fucking movie", 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

LRESULT CALLBACK getMsgProc(int code, WPARAM wparam, LPARAM lparam) {

    return  CallNextHookEx(g_hook, code, wparam, lparam);
}

void OnHook() {
    g_hook = SetWindowsHookEx(WH_GETMESSAGE, getMsgProc, instance, 0);

}

void OffHook() {

    UnhookWindowsHookEx(g_hook);
}