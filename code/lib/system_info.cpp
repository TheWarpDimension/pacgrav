#include "lib/universal_include.h"

#include <stdlib.h>
#include <string>

#include "filesys_utils.h"
#include "string_utils.h"
#include "system_info.h"


SystemInfo g_systemInfo;


SystemInfo::SystemInfo()
:	m_frameColour(200, 200, 200),
	m_windowBackgroundColour(255, 255, 255)
{
	// Get window background colour
	DWORD winCol = GetSysColor(COLOR_WINDOW);
	m_windowBackgroundColour.r = winCol & 0xff;
	m_windowBackgroundColour.g = (winCol & 0xff00) >> 8;
	m_windowBackgroundColour.b = (winCol & 0xff0000) >> 16;
	m_windowBackgroundColour.a = 255;

	// Get window frame colour
	DWORD buttonCol = GetSysColor(COLOR_BTNFACE);
	m_frameColour.r = buttonCol & 0xff;
	m_frameColour.g = (buttonCol & 0xff00) >> 8;
	m_frameColour.b = (buttonCol & 0xff0000) >> 16;
	m_frameColour.a = 255;

	// Find the directory that should contain the user's config files
	// Usually "C:\documents and settings\user\application data\codescroll" on windows
	char buf[MAX_PATH + 1];
	DWORD result = ExpandEnvironmentStrings("%APPDATA%", buf, MAX_PATH);
	ReleaseAssert(result != 0,
		"APPDATA environment variable not set.\n"
		"On a normal Windows machine this is set to something like\n"
		"c:\\documents and settings\\application data\n"
		"Please set it and try again");

	std::string path = buf;
	path += "/CodeTrowel";
	CanonicalisePath(path);
	m_homeDirectory = StringDuplicate(path.c_str());

	if (!FileExists(m_homeDirectory))
	{
    	bool result = CreateDirectory(m_homeDirectory);
	    ReleaseAssert(result, "Home directory '%s' does not exist and could not be created");
	}
}
