#ifndef INCLUDED_APP_H
#define INCLUDED_APP_H


class App
{
public:	
	App();
	void	MainLoop();
};


extern App *g_app;
extern double g_advanceTime;

#endif
