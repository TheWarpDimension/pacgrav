#ifndef INCLUDED_DIR_WALKER_H
#define INCLUDED_DIR_WALKER_H


#include "lib/darray.h"
#include "lib/filesys_utils.h" // For DirectoryItem


class DirStackFrame;


class DirectoryWalker
{
private:
	DirStackFrame *m_frame;
	DArray <DirStackFrame *> m_dirStack;
	bool m_recursive;

public:
	DirectoryWalker(char const *path, bool recursive);
	~DirectoryWalker();

	char const *GetErrorDescription();
	DirectoryItem *GetNextItem();
	char const *GetCurrentPath();
};


#endif
