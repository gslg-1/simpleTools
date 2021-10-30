#pragma once

#include <string>

using namespace Platform;


class tasksObserver
{
private:
	DWORD aProcesses[1024];
	int cProcesses;
	HANDLE threadHandler;
	DWORD threadId;

public:
	tasksObserver();
	~tasksObserver();

	int GetPCount();

	void ClearPIDs();
	void ScanProcesses();

	void GetPName(int tPos, wchar_t* lpwchar);
};

