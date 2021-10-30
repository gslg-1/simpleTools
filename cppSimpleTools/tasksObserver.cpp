#include "pch.h"
#include "tasksObserver.h"

#include <windows.h>
#include <psapi.h>

DWORD WINAPI scanRuningTasksThread(LPVOID lpParam);

DWORD WINAPI scanRuningTasksThread(LPVOID lpParam)
{
	tasksObserver* handler = (tasksObserver*)lpParam;
	int TaskID = 0;
	while (1)
	{
		handler->SetFirstTask("Task(" + TaskID + ")");
		TaskID++;
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

}

int tasksObserver::GetPCount()
{
	return pos;
}
void tasksObserver::ClearPIDs()
{
	for (; pos > 0; pos--)
	{
		aProcesses[pos] = 0;
	}
	aProcesses[pos] = 0;
}

void tasksObserver::SetPID( DWORD pID)
{
	if (pos < (sizeof(aProcesses) / sizeof(aProcesses[0])) )
	{
		aProcesses[pos] = pID;
		pos++;
	}
}

String^ tasksObserver::GetPName(int tPos)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	String^ output;
	if (tPos < pos)
	{
		HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[tPos]);
		if (hProc != NULL)
		{

			HMODULE hMod;
			DWORD cbNeeded;

			if ( EnumProcessModules( aProcesses[tPos], &hMod, sizeof(hMod), 
				 &cbNeeded) )
			{
				GetModuleBaseName(aProcesses[tPos], hMod, szProcessName,
								   sizeof(szProcessName)/sizeof(TCHAR) );
			}
		}
	}
	output = toString(szProcessName);
	return output;
}

