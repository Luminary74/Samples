#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

void RemoteThreadFunc(DWORD pid, DWORD funcaddr) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); //通过进程id，来获取句柄
	DWORD threadId = 0;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	hThread = CreateRemoteThread(hProcess, NULL, 0, funcaddr, NULL, 0, &threadId);
	CloseHandle(hProcess);
	CloseHandle(hThread);
}

int main2() {
	RemoteThreadFunc(23868,0xf61041);//23868进程id，0xf61041函数地址
	return 0;
}