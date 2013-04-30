#ifndef INCLUDE_FILE_UPDATE_WATCHER_H
#define INCLUDE_FILE_UPDATE_WATCHER_H

#include "lib/llist.h"
#include "lib/threading.h"


// This module provides a way to monitor files constantly to see if/when 
// their modification timestamp changes. Calls to fstat (or similar) are
// performed on a background thread once every second for each file. It
// is then possible to query the modification timestamp for a watched file
// without having to worry about fstat possibly blocking for many seconds.


//*****************************************************************************
// Class FileWatch
//*****************************************************************************

// Stores data about a file that we are watching
class FileWatch
{
public:
	char		*m_fullPath;
	unsigned	m_lastModTime;
	int			m_useCount;						// Number of times StartWatch was called for this file

	FileWatch(char const *p);
	~FileWatch();
};


//*****************************************************************************
// Class FileUpdateWatcher
//*****************************************************************************

class FileUpdateWatcher
{
public:
	LList <FileWatch *>	m_watches;
	CriticalSection		m_criticalSection;
	LList <char *>		m_removeList;
	float				m_loopStartTime;		// Only ever used by background thread
    bool                m_aboutToExit;
	bool				m_threadRunning;

public:
	FileUpdateWatcher();
    ~FileUpdateWatcher();
	void Initialise();
	
    int FindWatchIndex(char const *path);
    
    void StartWatch(char const *filePath);
	bool EndWatch(char const *filePath);
	unsigned GetModTime(char const *filePath);
};


extern FileUpdateWatcher g_fileUpdateWatcher;


#endif
