#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

void RemoteThreadFunc(DWORD pid, DWORD funcaddr) {
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid); //ͨ������id������ȡ���
	DWORD threadId = 0;
	HANDLE hThread = INVALID_HANDLE_VALUE;
	hThread = CreateRemoteThread(hProcess, NULL, 0, funcaddr, NULL, 0, &threadId);
	CloseHandle(hProcess);
	CloseHandle(hThread);
}

int main2() {
	RemoteThreadFunc(23868,0xf61041);//23868����id��0xf61041������ַ
	return 0;
}