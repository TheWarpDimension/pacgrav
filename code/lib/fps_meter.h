#ifndef INCLUDED_FPS_METER
#define INCLDUED_FPS_METER


class FpsMeter
{
public:
	unsigned int	m_fps;

private:
	unsigned int	m_framesThisSecond;
	double			m_endOfSecond;

public:
	FpsMeter();

	void Advance();
};


extern FpsMeter g_fpsMeter;

#endif
