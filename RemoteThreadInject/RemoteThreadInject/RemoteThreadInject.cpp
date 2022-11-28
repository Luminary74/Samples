// inject.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//
#include<Windows.h>
#include <iostream>

typedef HMODULE(WINAPI* PLPADLIBRARY)(LPCSTR lpLibFileName);

BOOL  EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{

		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}

DWORD FindSep(char* String)
{
	DWORD i = 0;
	DWORD pos = 0;
	while (String[i])
	{

		if (String[i] == '\\')
		{
			pos = i;
		}
		i++;
	}
	return pos;
}

VOID InjectDll(DWORD ProcessId)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessId);

	if (!hProcess)
	{
		printf("�򿪽���ʧ��");
		return;
	}

	//�̺߳����ĵ�ַ��LoadLibary�ĵ�ַ
	HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	if (!hModule)
	{
		printf("����kernel32ʧ��");
		CloseHandle(hProcess);
		return;
	}

	PLPADLIBRARY LoadLibaryAddr = NULL;
	LoadLibaryAddr = (PLPADLIBRARY)GetProcAddress(hModule, "LoadLibraryA");

	if (!LoadLibaryAddr)
	{
		printf("����loadlibraryʧ��");
		CloseHandle(hProcess);
		return;
	}

	//��Ŀ���������һ���ڴ���DLL����
	PVOID pAllocAddr = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!pAllocAddr)
	{
		printf("�����ڴ�ʧ��");
		CloseHandle(hProcess);
		return;
	}

	CHAR szModuleName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	DWORD pos = FindSep(szModuleName);
	if (pos == 0)
	{
		printf("·����δ�ҵ� '\'");
		return;
	}
	szModuleName[pos] = 0;
	SIZE_T dwRet = 0;
	sprintf_s(szModuleName, MAX_PATH, "./RemoteInjectDll.dll", szModuleName);
	WriteProcessMemory(hProcess, pAllocAddr, szModuleName, MAX_PATH, &dwRet);

	if (!dwRet)
	{
		//��ȡ������
		CloseHandle(hProcess);
		VirtualFreeEx(hProcess, pAllocAddr, 0, MEM_RELEASE);
		printf("д�����ڴ�ʧ��");
		return;
	}

	//����Ĳ�������ģ�����ĵ�ַ
	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibaryAddr, pAllocAddr, 0, 0);
	if (hThread)
	{
		printf("ע��ɹ�\n");
	}
	else
	{
		printf("ע��ʧ��\n");
	}
	CloseHandle(hThread);
	return;
}

int main()
{
	if (!EnableDebugPriv())
	{
		printf("��Ȩʧ��\n");
		getchar();
		return 0;
	}

	printf("����ע��Ľ���id\n");
	DWORD processId = 0;
	scanf_s("%d", &processId);
	InjectDll(processId);
	Sleep(5000);
	system("pause");
	return 0;
}


