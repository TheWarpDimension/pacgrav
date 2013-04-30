#include "lib/universal_include.h"

#include <windows.h>


typedef bool (APIENTRY vsyncSetProc)(int enabled);


void vsyncSet(bool enable)
{
	static vsyncSetProc *swapIntervalExt = (vsyncSetProc*)wglGetProcAddress("wglSwapIntervalEXT");

	if (swapIntervalExt)
		swapIntervalExt(enable);
}
