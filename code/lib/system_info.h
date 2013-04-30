#ifndef INCLUDED_SYSTEM_INFO_H
#define INCLUDED_SYSTEM_INFO_H


#include "rgb_colour.h"


class SystemInfo
{
public:
	RGBAColour	m_windowBackgroundColour;	// Window background colour
	RGBAColour	m_frameColour;				// "Menu color" in Windows' terminology
	char const	*m_homeDirectory;			// Something like "Documents and Settings/<username>/Application Data/CodeScroll"

	SystemInfo();
};


extern SystemInfo g_systemInfo;


#endif
