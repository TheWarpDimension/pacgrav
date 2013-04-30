#include "lib/universal_include.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug_utils.h"


void DebugOut(char *_fmt, ...)
{
    char buf[512];
    va_list ap;
    va_start (ap, _fmt);
    vsprintf(buf, _fmt, ap);
    fprintf(stderr, "%s\n", buf);
}


void ReleaseAssert(bool _condition, char const *_fmt, ...)
{
	if (!_condition)
	{
		char buf[512];
		va_list ap;
		va_start (ap, _fmt);
		vsprintf(buf, _fmt, ap);
        fprintf(stderr, "%s\n", buf);
		exit(-1);
	}
}


void PrintMemoryLeaks()
{
}

    
void GenerateBlackBox()
{
}
    

