#ifndef INCLUDED_APP_H
#define INCLUDED_APP_H


#include "pcman.h"
#include "ghost.h"


class App
{
public:	
	App();
	void	MainLoop();
};


typedef enum
{
	ModeMainMenu,
	ModeInGame,
	ModePcmanDying,
	ModeGameOverScreen
} Mode;


extern App		*g_app;
extern double	g_advanceTime;
extern int		g_score;
extern int		g_tileSize;
extern PcMan	g_pcman;
extern int		g_gameMode;
extern double	g_powerPillEndTime;
extern double	g_scatterEndTime;
#define numGhosts 4
extern Ghost	g_ghosts[numGhosts];
extern bool     g_debugMode;
extern bool     g_godMode;
extern bool     g_slowmo;
extern bool		g_paused;

#endif
