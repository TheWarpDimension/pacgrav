#include "lib/universal_include.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <math.h>
#include "app.h"

#include "hi_res_time.h"


#define USE_PENTIUM_COUNTER 0


#ifdef USE_PENTIUM_COUNTER
 static unsigned int g_ticksPerSec = 0;
#endif
static double g_tickInterval = -1.0;
static double g_lastGetHighResTime = 0.0;
static double g_timeShift = 0.0;


#pragma warning (push)
#pragma warning (disable : 4035)	// disable no return value warning
inline  unsigned int GetPentiumCounter()
{
	__asm
	{
		_emit 0x0F		        // Call RDTSC (Read time stamp counter) this will
		_emit 0x31				// put a 64 bit clock cycle count in edx;eax
					
		shr eax,6				// 64 bits are too much and the resolution is too
								// high, so throw away the bottom 6 bits of eax
		shl edx,26				// Now throw away everything other than the top 6 bits of edx
		or eax,edx				// Shove the edx data into the gap in eax
								// Return value in eax, which apparently VC is happy with
	}
}
#pragma warning (pop)


inline double GetLowLevelTime();


void InitialiseHighResTime()
{
	// Start be getting the frequency the Performance Counter uses.
	// We need to use the Performance Counter to calibrate the Pentium
	// Counter, if we are going to use it.
    LARGE_INTEGER count;
    QueryPerformanceFrequency(&count);
    g_tickInterval = 1.0 / (double)count.QuadPart;
	g_timeShift = GetLowLevelTime();

#if USE_PENTIUM_COUNTER
	// Get start times
	QueryPerformanceCounter(&count);
	unsigned int pentiumCounterStart = GetPentiumCounter();
	double perfCounterStart = count.QuadPart * g_tickInterval;

	// Wait for roughly one second
	Sleep(1000);
	
	// Get end times
	QueryPerformanceCounter(&count);
	unsigned int pentiumCounterEnd = GetPentiumCounter();
	double perfCounterEnd = count.QuadPart * g_tickInterval;
	
	double perfCounterTicks = perfCounterEnd - perfCounterStart;
	double perfCounterTime = perfCounterTicks;
	double pentiumTicks = pentiumCounterEnd - pentiumCounterStart;
       
	g_ticksPerSec = pentiumTicks / perfCounterTime;
	g_tickInterval = 1.0 / (double)g_ticksPerSec;
#endif
}


inline double GetLowLevelTime()
{
    if (g_tickInterval < 0.0)
        InitialiseHighResTime();

#if USE_PENTIUM_COUNTER
    static double wraps = 0.0;		// Each time the counter wraps, we increment this value by the wrap period
    static unsigned int lastCount = 0;
    unsigned int newCount = GetPentiumCounter();
    
    // Has the counter wrapped?
    if (newCount < lastCount)
    {
        wraps += g_tickInterval * pow(2, 32);
    }
    lastCount = newCount;
    
    return (double)newCount * g_tickInterval + wraps;
#else
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return (double)count.QuadPart * g_tickInterval;
#endif
}


double GetHighResTime()
{
	double timeNow = GetLowLevelTime();
    timeNow -= g_timeShift;

    if( timeNow > g_lastGetHighResTime + 1.0f )
    {
        g_timeShift += timeNow - g_lastGetHighResTime;
        timeNow -= timeNow - g_lastGetHighResTime;
    }

    g_lastGetHighResTime = timeNow;
    return timeNow;
}
