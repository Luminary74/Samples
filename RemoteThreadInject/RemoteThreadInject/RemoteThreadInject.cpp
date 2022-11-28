// inject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
		printf("打开进程失败");
		return;
	}

	//线程函数的地址是LoadLibary的地址
	HMODULE hModule = LoadLibrary(TEXT("Kernel32.dll"));
	if (!hModule)
	{
		printf("加载kernel32失败");
		CloseHandle(hProcess);
		return;
	}

	PLPADLIBRARY LoadLibaryAddr = NULL;
	LoadLibaryAddr = (PLPADLIBRARY)GetProcAddress(hModule, "LoadLibraryA");

	if (!LoadLibaryAddr)
	{
		printf("查找loadlibrary失败");
		CloseHandle(hProcess);
		return;
	}

	//在目标进程申请一块内存存放DLL名字
	PVOID pAllocAddr = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	if (!pAllocAddr)
	{
		printf("分配内存失败");
		CloseHandle(hProcess);
		return;
	}

	CHAR szModuleName[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, szModuleName, MAX_PATH);
	DWORD pos = FindSep(szModuleName);
	if (pos == 0)
	{
		printf("路径中未找到 '\'");
		return;
	}
	szModuleName[pos] = 0;
	SIZE_T dwRet = 0;
	sprintf_s(szModuleName, MAX_PATH, "./RemoteInjectDll.dll", szModuleName);
	WriteProcessMemory(hProcess, pAllocAddr, szModuleName, MAX_PATH, &dwRet);

	if (!dwRet)
	{
		//获取错误码
		CloseHandle(hProcess);
		VirtualFreeEx(hProcess, pAllocAddr, 0, MEM_RELEASE);
		printf("写进程内存失败");
		return;
	}

	//传入的参数就是模块名的地址
	HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibaryAddr, pAllocAddr, 0, 0);
	if (hThread)
	{
		printf("注入成功\n");
	}
	else
	{
		printf("注入失败\n");
	}
	CloseHandle(hThread);
	return;
}

int main()
{
	if (!EnableDebugPriv())
	{
		printf("提权失败\n");
		getchar();
		return 0;
	}

	printf("输入注入的进程id\n");
	DWORD processId = 0;
	scanf_s("%d", &processId);
	InjectDll(processId);
	Sleep(5000);
	system("pause");
	return 0;
}


