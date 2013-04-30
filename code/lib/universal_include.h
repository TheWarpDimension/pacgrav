#ifndef INCLUDED_UNIVERSAL_INCLUDE_H
#define INCLUDED_UNIVERSAL_INCLUDE_H

#define APPLICATION_NAME "Pc Man!"

#define ENABLE_OPENGL

#ifdef _WIN32
	// 4018 - signed/unsigned mismatch
	// 4244 - possible loss of data unsigned long to unsigned char
	// 4267 - possible loss of data converting from size_t to int
	// 4530 - exception handler used but unwind semantics are not enabled
	// 4800 - forcing value to bool (performance warning)
	#pragma warning(disable: 4018 4244 4267 4530 4800)
	#define for if (0) ; else for
	
	// Defines that will enable you to double click on a #pragma message
	// in the Visual Studio output window.
	#define MESSAGE_LINENUMBERTOSTRING(linenumber)	#linenumber
	#define MESSAGE_LINENUMBER(linenumber)			MESSAGE_LINENUMBERTOSTRING(linenumber)
	#define MESSAGE(x) message (__FILE__ "(" MESSAGE_LINENUMBER(__LINE__) "): "x) 
	
	#include <crtdbg.h>

    #define _WIN32_WINNT 0x500
	#include <windows.h>

	#define stricmp _stricmp   

	typedef __int64 int64;
#endif

#ifdef TARGET_OS_LINUX                                                
	#include <ctype.h>                                                    
	#define stricmp strcasecmp                                            
	#define strnicmp strncasecmp                                          
	#define __stdcall                                                     
	template<class T> inline T min (T a, T b) { return (a < b) ? a : b; };
	template<class T> inline T max (T a, T b) { return (a > b) ? a : b; };
	inline char * strlwr(char *s) {                                       
	  char *p = s;                                                        
	  for (char *p = s; *p; p++)                                          
		*p = tolower(*p);                                                 
	  return s;                                                           
	}                                                                     
	inline char * strupr(char *s) {                                       
	  char *p = s;                                                        
	  for (char *p = s; *p; p++)                                          
		*p = toupper(*p);                                                 
	  return s;                                                           
	}                                                                     
	#include <unistd.h>                                                   
	#define Sleep sleep                                                   
#endif

#include <stdio.h>
#include <math.h>

#include "lib/debug_utils.h"
#include "lib/profiler.h"

#define SafeDelete(a) delete (a); (a) = NULL;
#define SafeArrayDelete(a) delete [] (a); (a) = NULL;

#endif
