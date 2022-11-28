#include <Windows.h>

typedef void(*hookOn)();
typedef void(*hookOff)();
void main() {

	HMODULE hmod = LoadLibraryA("GlobalHookDll.dll");
	hookOn hook_on = (hookOn)GetProcAddress(hmod, "OnHook");
	hookOff hook_off = (hookOff)GetProcAddress(hmod, "OffHook");
	hook_on();

	system("pause");

	hook_off();
}