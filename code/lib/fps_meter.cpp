#include "lib/universal_include.h"

#include "fps_meter.h"
#include "hi_res_time.h"


FpsMeter g_fpsMeter;


FpsMeter::FpsMeter()
:	m_fps(0),
	m_framesThisSecond(0)
{
	m_endOfSecond = GetHighResTime() + 1.0;
}


void FpsMeter::Advance()
{
    m_framesThisSecond++;

    double currentTime = GetHighResTime();
    if (currentTime > m_endOfSecond)
    {
        if (currentTime > m_endOfSecond + 2.0)
        {
            m_endOfSecond = currentTime + 1.0;
        }
        else
        {
            m_endOfSecond += 1.0;
        }
        m_fps = m_framesThisSecond;
        m_framesThisSecond = 0;
    }
	else if (m_endOfSecond > currentTime + 2.0)
    {
        m_endOfSecond = currentTime + 1.0;
    }
}
