#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

DWORD WINAPI threadFunc(LPVOID lpThreadParameter) {
	for (int i = 0; i < 5; i++)
	{
		printf("�߳�%d����\n",i+1);
		Sleep(500);
	}
}

int main1() {
	printf("0x%x\n", threadFunc);//��ӡ��������ַ��ΪԶ���߳���׼��
	HANDLE hThread = CreateThread(NULL, 0, threadFunc, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	printf("��ͨ�̴߳�����ɣ�\n");
	getchar();
	return 0;
}