#include "lib/universal_include.h"

#include "lib/threading.h"


CriticalSection::CriticalSection()
{
	InitializeCriticalSection(&m_criticalSectionWin32);
}


CriticalSection::~CriticalSection()
{
	DeleteCriticalSection(&m_criticalSectionWin32);
}


void CriticalSection::Enter(char const *owner)
{
	EnterCriticalSection(&m_criticalSectionWin32);
    m_owner = owner;
}


void CriticalSection::Leave()
{
	LeaveCriticalSection(&m_criticalSectionWin32);
    m_owner = NULL;
}


unsigned StartThread(ThreadProc threadFunc, void *threadData)
{
	DWORD threadId;
	HANDLE threadHandle = CreateThread(NULL, 0, threadFunc, threadData, 0, &threadId);
    return (unsigned)threadHandle;
}
