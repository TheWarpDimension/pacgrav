#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H


void DebugOut(char *fmt, ...);


void ReleaseAssert(bool _condition, char const *_fmt, ...);
void ReleaseWarn(char const *_fmt, ...);

#ifdef _DEBUG
  #ifdef _WIN32
	#define DebugAssert(x) if(!(x)) { _CrtDbgBreak(); }
  #else
    #define DebugAssert(x) assert(x)
  #endif
#else
	#define DebugAssert(x)
#endif

    
#endif
