#include "lib/universal_include.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "lib/message_dialog.h"
#include "debug_utils.h"
#include "window_manager.h"


void DebugOut(char *_fmt, ...)
{
    char buf[512];
    va_list ap;
    va_start (ap, _fmt);
    vsprintf(buf, _fmt, ap);
    OutputDebugString(buf);
}


void ReleaseAssert(bool _condition, char const *_fmt, ...)
{
    static bool noAlready = false;
	if (!_condition && !noAlready)
	{
		char buf[1024];
		va_list ap;
		va_start (ap, _fmt);
		vsprintf(buf, _fmt, ap);
		strcat(buf, "\nDo you want to quit?");
        int result = MessageDialog("Serious Error", buf, MsgDlgTypeYesNo);
#ifdef _DEBUG
		_CrtDbgBreak();
#else
        if (result == MsgDlgRtnCode_Yes)
		    exit(-1);
        else
            noAlready = true;
#endif
	}
}


void ReleaseWarn(char const *_fmt, ...)
{
	char buf[512];
	va_list ap;
	va_start (ap, _fmt);
	vsprintf(buf, _fmt, ap);
	MessageDialog("Warning", buf, MsgDlgTypeOk);
#ifdef _DEBUG
	_CrtDbgBreak();
#endif
}
