#include "lib/universal_include.h"

#include <math.h>
#include <float.h>

#include "lib/debug_utils.h"
#include "lib/hi_res_time.h"
#include "lib/profiler.h"
#include "lib/string_utils.h"
#include "lib/window_manager.h"


#ifdef PROFILER_ENABLED

#define PROFILE_HISTORY_LENGTH  10

Profiler g_profiler;


// ****************************************************************************
//  Class ProfilerObject
// ****************************************************************************

ProfilerObject::ProfilerObject(char const *name)
{
	g_profiler.StartProfile(name);
}


ProfilerObject::~ProfilerObject()
{
	g_profiler.EndProfile();
}


// ****************************************************************************
//  Class ProfiledElement
// ****************************************************************************

ProfiledElement::ProfiledElement(char const *_name, ProfiledElement *_parent)
:   m_currentTotalTime(0.0),
	m_currentNumCalls(0),
	m_lastTotalTime(0.0),
	m_lastNumCalls(0),
	m_longest(DBL_MIN),
	m_shortest(DBL_MAX),
	m_callStartTime(0.0),
	m_historyTotalTime(0.0),
	m_historyNumSeconds(0.0),
	m_historyNumCalls(0),
	m_parent(_parent),
	m_isExpanded(false)
{
    m_name = StringDuplicate(_name);
}


ProfiledElement::~ProfiledElement()
{
}


void ProfiledElement::Start()
{
    m_callStartTime = GetHighResTime();
}


void ProfiledElement::End()
{
	double const timeNow = GetHighResTime();

	m_currentNumCalls++;
	double const duration = timeNow - m_callStartTime;
	m_currentTotalTime += duration;
	
	if (duration > m_longest)
	{
		m_longest = duration;
	}
	if (duration < m_shortest)
	{
		m_shortest = duration;
	}
}


void ProfiledElement::Advance()
{
	m_lastTotalTime = m_currentTotalTime;
	m_lastNumCalls = m_currentNumCalls;
	m_currentTotalTime = 0.0;
	m_currentNumCalls = 0;
	m_historyTotalTime += m_lastTotalTime;
	m_historyNumSeconds += 1.0;
	m_historyNumCalls += m_lastNumCalls;

	for (int i = 0; i < m_children.Size(); ++i)
	{
		if (m_children.ValidIndex(i))
		{
			m_children[i]->Advance();
		}
	}
}


void ProfiledElement::ResetHistory()
{
	m_historyTotalTime = 0.0;
	m_historyNumSeconds = 0.0;
	m_historyNumCalls = 0;
	m_longest = DBL_MIN;
	m_shortest = DBL_MAX;

	for (unsigned int i = 0; i < m_children.Size(); ++i)
	{
		if (m_children.ValidIndex(i))
		{
			m_children[i]->ResetHistory();
		}
	}
}


double ProfiledElement::GetMaxChildTime()
{
	double rv = 0.0;

	short first = m_children.StartOrderedWalk();
	if (first == -1)
	{
		return 0.0;
	}

	short i = first;
	while (i != -1)
	{
		float val = m_children[i]->m_historyTotalTime;
		ProfiledElement *child = m_children[i];
		if (val > rv)
		{
			rv = val;
		}

		i = m_children.GetNextOrderedIndex();
	}

	return rv / m_children[first]->m_historyNumSeconds;
}



// ****************************************************************************
//  Class Profiler
// ****************************************************************************

Profiler::Profiler()
:   m_doFlush(false),
	m_currentElement(NULL),
	m_insideRenderSection(false)
{
	m_rootElement = new ProfiledElement("Root", NULL);
	m_rootElement->m_isExpanded = true;
	m_currentElement = m_rootElement;
	m_endOfSecond = GetHighResTime() + 1.0f;
}


Profiler::~Profiler()
{
}


void Profiler::Initialise()
{
	m_endOfSecond = GetHighResTime() + 1.0f;
}


void Profiler::Advance()
{
	double timeNow = GetHighResTime();
	if (timeNow > m_endOfSecond)
	{
		m_lengthOfLastSecond = timeNow - (m_endOfSecond - 1.0);
		m_endOfSecond = timeNow + 1.0;

		m_rootElement->Advance();
	}
}


void Profiler::RenderStarted()
{
	m_insideRenderSection = true;
}


void Profiler::RenderEnded()
{
	m_insideRenderSection = false;
}


void Profiler::ResetHistory()
{
	m_rootElement->ResetHistory();
}


void Profiler::StartProfile(char const *_name)
{
	ProfiledElement *pe = m_currentElement->m_children.GetData(_name);
	if (!pe)
	{
		pe = new ProfiledElement(_name, m_currentElement);
		m_currentElement->m_children.PutData(_name, pe);
	}

	ReleaseAssert(m_rootElement->m_isExpanded, "Profiler root element has been un-expanded");
	if (m_currentElement->m_isExpanded)
	{
		if (m_doFlush && m_insideRenderSection)
		{
			g_windowManager->Flush();
		}
 		pe->Start();
	}
	m_currentElement = pe;
}


void Profiler::EndProfile()
{
	if (m_currentElement->m_parent->m_isExpanded)
	{
		if (m_doFlush && m_insideRenderSection)
		{
			g_windowManager->Flush();
		}
    
		DebugAssert(m_currentElement != m_rootElement);

		m_currentElement->End();
	}

	m_currentElement = m_currentElement->m_parent;
}


#endif // PROFILER_ENABLED
