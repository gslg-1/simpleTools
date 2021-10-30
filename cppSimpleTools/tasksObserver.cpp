#include "pch.h"
#include "tasksObserver.h"
#include <windows.h>
#define PSAPI_VERSION 2
#include <psapi.h>
#include <string>

DWORD WINAPI scanRuningTasksThread(LPVOID lpParam);

DWORD WINAPI scanRuningTasksThread(LPVOID lpParam)
{
	tasksObserver* handler = (tasksObserver*)lpParam;
	unsigned int TaskID = 12000;
	while (1)
	{
		handler->ScanProcesses();
		Sleep(500);
	}
	return 0;
}

tasksObserver::tasksObserver()
{
	ClearPIDs();
	threadHandler = CreateThread(
		NULL,
		0,
		scanRuningTasksThread,
		(LPVOID)this,
		0,
		&threadId);
}
tasksObserver::~tasksObserver()
{
	/*nothing to do*/
}

int tasksObserver::GetPCount()
{
	return this->cProcesses;
}
void tasksObserver::ClearPIDs()
{
	memset((void*)(this->aProcesses), 0, this->cProcesses);
	this->cProcesses = 0;
}

void tasksObserver::ScanProcesses()
{
	DWORD cbNeeded;
	/*Get Process IDs*/
	if (!EnumProcesses(this->aProcesses, sizeof(this->aProcesses), &cbNeeded))
	{
		/*Error -> Clear Handler*/
		this->ClearPIDs();
	}
	this->cProcesses = cbNeeded / sizeof(DWORD);
}

void tasksObserver::GetPName(int tPos, wchar_t * lpwchar)
{
	wchar_t szProcessName[MAX_PATH] = TEXT("<unknown>");
	if (tPos < this->cProcesses)
	{
		HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[tPos]);
		if (hProc != NULL)
		{
			HMODULE hMod;
			DWORD cbNeeded;
			///if ( EnumProcessModules( hProc, &hMod, sizeof(hMod), &cbNeeded) )
			///{
				GetModuleBaseNameW( hProc, NULL, szProcessName,
								   sizeof(szProcessName)/sizeof(TCHAR) );
			///}
		}
	}
	wmemcpy( lpwchar, (const wchar_t *)szProcessName, sizeof(szProcessName) );
}
