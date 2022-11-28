#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

DWORD WINAPI threadFunc(LPVOID lpThreadParameter) {
	for (int i = 0; i < 5; i++)
	{
		printf("线程%d开启\n",i+1);
		Sleep(500);
	}
}

int main1() {
	printf("0x%x\n", threadFunc);//打印出函数地址，为远程线程做准备
	HANDLE hThread = CreateThread(NULL, 0, threadFunc, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	printf("普通线程创建完成！\n");
	getchar();
	return 0;
}