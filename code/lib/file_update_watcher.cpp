#include "lib/universal_include.h"

#include <sys/stat.h>

#include "lib/file_update_watcher.h"
#include "lib/hi_res_time.h"
#include "lib/string_utils.h"


FileWatch::FileWatch(char const *p)
:	m_fullPath(StringDuplicate(p)),
	m_lastModTime(0),
	m_useCount(1)
{
}


FileWatch::~FileWatch() 
{ 
	SafeArrayDelete(m_fullPath);
}


//*****************************************************************************
// Class FileUpdateListener
//*****************************************************************************

unsigned long __stdcall FileWatcherProc(void *data)
{
	FileUpdateWatcher *watcher = (FileUpdateWatcher*)data;
	
	while (!watcher->m_aboutToExit)
	{
		// Get the modification time for all the files
        watcher->m_criticalSection.Enter("FileWatcherProc1");
		watcher->m_loopStartTime = GetHighResTime();
		for (int i = 0; i < watcher->m_watches.Size(); ++i)
		{
			FileWatch *watch = watcher->m_watches[i];

			struct stat fileStat;
            watcher->m_criticalSection.Leave();

            if (watcher->m_aboutToExit)
                break;

            int err = stat(watch->m_fullPath, &fileStat);

            watcher->m_criticalSection.Enter("FileWatcherProc2");
			if (err == 0) 
            {
                watch->m_lastModTime = fileStat.st_mtime;
            }
		}
        watcher->m_criticalSection.Leave();

		while (!watcher->m_aboutToExit)
		{
			// Remove any watches that need removing
			watcher->m_criticalSection.Enter("FileWatcherProc3");
			while (!watcher->m_aboutToExit && watcher->m_removeList.Size() > 0)
			{
                // Find the watch
				char *path = watcher->m_removeList[0];
				int watchIndex = watcher->FindWatchIndex(path);
				FileWatch *watch = watcher->m_watches[watchIndex];

				// Reduce count of people using this watch
				watch->m_useCount--;

				// If no one is left using it, then delete it
				if (watch->m_useCount <= 0)
				{
					watcher->m_watches.RemoveData(watchIndex);
					delete watch;
				}

				// Remove item from removeList
				watcher->m_removeList.RemoveData(0);

				// Free the item
				delete [] path;
			}
			watcher->m_criticalSection.Leave();

			// Is it time to for the next round of updates?
			float timeNow = GetHighResTime();
			if (timeNow > watcher->m_loopStartTime + 1.0f)
				break;	// Yes

			// We've got time left, go to sleep for a bit
			Sleep(10);
		}
	}

    watcher->m_aboutToExit = false;
	return 0;
}


FileUpdateWatcher g_fileUpdateWatcher;


FileUpdateWatcher::FileUpdateWatcher()
{
	m_threadRunning = false;
}


FileUpdateWatcher::~FileUpdateWatcher()
{
    m_aboutToExit = true;
    while (m_threadRunning && m_aboutToExit)
        Sleep(10);
}


void FileUpdateWatcher::Initialise()
{
	unsigned threadHandle = StartThread(FileWatcherProc, this);
    ReleaseAssert(threadHandle, "FileUpdateWatcher::Initialise couldn't create thread");
	m_threadRunning = true;
}


int FileUpdateWatcher::FindWatchIndex(char const *path)
{
	// Must be inside the critical section before calling this function
	for (int i = 0; i < m_watches.Size(); ++i)
	{
		FileWatch *watch = m_watches[i];
		if (stricmp(watch->m_fullPath, path) == 0)
		{
			return i;
		}
	}
	
	return -1;
}


void FileUpdateWatcher::StartWatch(char const *filePath)
{
	m_criticalSection.Enter("FileUpdateWatcher::StartWatch");
	int watchIndex = FindWatchIndex(filePath);
	if (watchIndex != -1)
	{
		// Watch already exists, just increment the use count
		FileWatch *watch = m_watches[watchIndex];
		watch->m_useCount++;
        m_criticalSection.Leave();
		return;
	}

	FileWatch *watch = new FileWatch(filePath);
	m_watches.PutDataAtEnd(watch);
	m_criticalSection.Leave();
}


bool FileUpdateWatcher::EndWatch(char const *path)
{
	m_criticalSection.Enter("FileUpdateWatcher::EndWatch");
	int watchIndex = FindWatchIndex(path);
	if (watchIndex == -1)
    {
        m_criticalSection.Leave();
		return false;
    }

	m_removeList.PutData(StringDuplicate(path));
	m_criticalSection.Leave();
	return true;
}


unsigned FileUpdateWatcher::GetModTime(char const *filePath)
{
    unsigned rv = 0;

//     float watchDogBiteTime = m_loopStartTime + 10.0f;
//     ReleaseAssert(GetHighResTime() < watchDogBiteTime, "FileUpdateWatcher locked out by %s", m_criticalSection.GetOwner());

	m_criticalSection.Enter("FileUpdateWatcher::GetModTime");
	int watchIndex = FindWatchIndex(filePath);
	if (watchIndex != -1)
    {
        FileWatch *watch = m_watches[watchIndex];
        rv = watch->m_lastModTime;
    }
    m_criticalSection.Leave();

	return rv;
}
