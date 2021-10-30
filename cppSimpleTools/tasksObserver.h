#pragma once

#include "string"

using namespace Platform;


class tasksObserver
{
private:
	DWORD aProcesses[1024];
	int pos;
	HANDLE threadHandler;
	DWORD threadId;

public:
	tasksObserver();
	~tasksObserver();

	int GetPCount();

	void ClearPIDs();
	void SetPID( DWORD pID);

	String^ GetPName(int tPos);
};

