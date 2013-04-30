#include "lib/universal_include.h"

#include <string>
#include "lib/dir_walker.h"
#include "lib/filesys_utils.h"


class DirStackFrame
{
public:
	DirectoryList *m_directoryList;
	int i;

public:
	DirStackFrame(char const *fullPath)
	:	i(0)
	{
		m_directoryList = new DirectoryList(fullPath);
	}

	~DirStackFrame() 
	{ 
		delete m_directoryList; 
	}
};


// ****************************************************************************
// Class DirectoryWalker
// ****************************************************************************

DirectoryWalker::DirectoryWalker(char const *path, bool recursive)
:	m_recursive(recursive)
{
	m_frame = new DirStackFrame(path);
}


DirectoryWalker::~DirectoryWalker()
{
	m_dirStack.EmptyAndDelete();
}


char const *DirectoryWalker::GetErrorDescription()
{
	return m_frame->m_directoryList->m_error;
}


DirectoryItem *DirectoryWalker::GetNextItem()
{
	DirectoryItem *rv = NULL;

	while (rv == NULL)
	{
		if (m_frame->m_directoryList->m_error)
		{
			// Something went wrong - tell the user
			ReleaseAssert(0, "DirectoryWalker::GetNextItem: %s", m_frame->m_directoryList->m_error);
			delete m_frame;
			m_frame = m_dirStack.Pop();
			return NULL;
		}

		// Have we finished with this m_frame?
		while (m_frame->i >= m_frame->m_directoryList->m_items.Size())
		{
			delete m_frame;
			m_frame = m_dirStack.Pop();

			if (!m_frame)
			{
				return NULL;
			}
		}

		DirectoryItem *item = m_frame->m_directoryList->m_items[m_frame->i];
		
		// Construct item's full path
		std::string fullPath = m_frame->m_directoryList->m_fullPath;
		fullPath += "/";
		fullPath += item->m_name;

		if (item->m_isDirectory && m_recursive)
		{
			DirStackFrame *newFrame = new DirStackFrame(fullPath.c_str());
			m_dirStack.Push(newFrame);
		}
		else
		{
			rv = item;
		}
		
		m_frame->i++;
	}

	return rv;
}


char const *DirectoryWalker::GetCurrentPath()
{
	if (m_frame)
	{
		return m_frame->m_directoryList->m_fullPath;
	}

	return NULL;
}
