#ifndef INCLUDED_PROFILER_H
#define INCLUDED_PROFILER_H

// This is a hierarchical profiler. It is intended to be used in an interactive program
// where the top level function is an infinite loop. Typically one iteration of that
// loop will advance a bunch of objects and render a bunch of objects, and possibly do
// some other stuff.

// The idea is that you insert PROFILE_THIS_FUNCTION into your top level function and 
// any other functions you are interested in. The total time spent in each profiled
// function on each iteration will be recorded. It will then be possible to display

#include "lib/sorting_hash_table.h"

#define PROFILER_ENABLED 1
#ifdef PROFILER_ENABLED


class ProfilerObject
{
public:
	ProfilerObject(char const *name);
	~ProfilerObject();
};


//*****************************************************************************
// Class ProfiledElement
//*****************************************************************************

class ProfiledElement
{
public:
	// Values used for accumulating the profile for the current second
	double				m_currentTotalTime;		// In seconds
	int					m_currentNumCalls;

	// Values used for storing the profile for the previous second
    double				m_lastTotalTime;		// In seconds
    int					m_lastNumCalls;

	// Values used for storing history data (accumulation of all m_lastTotalTime
	// and m_lastNumCalls values since last reset)
	double				m_historyTotalTime;		// In seconds
	double				m_historyNumSeconds;
	int					m_historyNumCalls;

	// Values used for storing the longest and shortest duration spent inside
	// this elements profile. These values are reset when the history is reset
	double				m_shortest;
	double				m_longest;

	double				m_callStartTime;
    char				*m_name;

	SortingHashTable	<ProfiledElement *> m_children;
	ProfiledElement		*m_parent;

	bool				m_isExpanded;			// Bit of data that a tree view display can use

public:
    ProfiledElement	 (char const *_name, ProfiledElement *_parent);
    ~ProfiledElement ();
	
	void				Start			();
	void				End				();
	void				Advance			();
	void				ResetHistory	();

	double				GetMaxChildTime	();
};


//*****************************************************************************
// Class Profiler
//*****************************************************************************

class Profiler
{
private:
	bool				m_insideRenderSection;	// Used to decide whether to do a glFinish for each call to EndProfile
    ProfiledElement		*m_currentElement;		// Stores the currently active profiled element

public:
	ProfiledElement		*m_rootElement;
	bool				m_doFlush;
	double				m_endOfSecond;
	double				m_lengthOfLastSecond;	// Will be somewhere between 1.0 and (1.0 + g_advanceTime)

    Profiler();
    ~Profiler();

	void				Initialise		();
	void				Advance			();

	void				RenderStarted	();
	void				RenderEnded		();

    void				StartProfile	(char const *_name);
    void				EndProfile		();

	void				ResetHistory	();
}; 

#define PROFILE_BLOCK(name) ProfilerObject profilerObject(name);
#define PROFILE_RENDER_STARTED g_profiler.RenderStarted();
#define PROFILE_RENDER_ENDED g_profiler.RenderEnded();
#define PROFILER_INITIALISE g_profiler.Initialise();
#define PROFILER_ADVANCE g_profiler.Advance();

extern Profiler g_profiler;

#else // PROFILER_ENABLED
	#define PROFILE_BLOCK(name)
	#define PROFILE_RENDER_STARTED
	#define PROFILE_RENDER_ENDED
	#define PROFILER_INITIALISE
	#define PROFILER_ADVANCE
#endif // PROFILER_ENABLED


#endif

