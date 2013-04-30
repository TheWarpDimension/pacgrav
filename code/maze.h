#ifndef INCLUDED_MAZE_H
#define INCLUDED_MAZE_H


#include "lib/darray.h"
#include "lib/sprite.h"
#include "creature.h"
#include "ghost.h"


class Maze
{
private:
	char *m_data;
	char *m_eyeMap;
	char *m_scatterMapRed;
	char *m_scatterMapGreen;
	char *m_scatterMapPink;
	char *m_scatterMapOrange;
	char *m_breadCrumbs;
	DArray <Move> m_movesForTarget;
	int m_shortestRouteLen;
	int m_targetX;
	int m_targetY;
	Sprite m_tiles[16];

	bool canMove(int x, int y);
	int searchForTarget(int sx, int sy, int tx, int ty, int depth);
	Move getMoveForTarget(int sx, int sy, int tx, int ty);

public:
	int m_width;
	int m_height;
	int m_offsetX;
	int m_offsetY;

	Maze();
	void init();

	Sprite *getCellSprite(int x, int y);

	bool isWall(int x, int y);
	bool isPill(int x, int y);
	bool isCherry(int x, int y);
	bool isPowerPill(int x, int y);
	bool isHome(int x, int y);
	bool isHomeEntrance(int x, int y);
	bool isJunction(int x, int y);
	bool isCorner(int x, int y);
	Move getCornerMove(int x, int y, Move inputMove);

	Move getEyesOnlyMovement(int x, int y);
	Move getScatterMovement(int x, int y, GhostCharacter character);

	void eatPill(int x, int y);
	bool isComplete();
	bool isPositionAllowed(double fx, double fy);
	bool edgePassed(double fx, double oldFx);

	int getPacStartPosX();
	int getPacStartPosY();
};


extern Maze g_maze;


#endif
