#ifndef INCLUDED_THREADING_H
#define INCLUDED_THREADING_H


class CriticalSection
{
private:
	CRITICAL_SECTION m_criticalSectionWin32;
    char const *m_owner;

public:
	CriticalSection();
	~CriticalSection();

    char const *GetOwner() { return m_owner; }
	void Enter(char const *owner);
	void Leave();
};


typedef unsigned long (__stdcall *ThreadProc)(void *data);

// Returns a thread handle, or zero on failure
unsigned StartThread(ThreadProc threadFunc, void *threadData);


#endif
