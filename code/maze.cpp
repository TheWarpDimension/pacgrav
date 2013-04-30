#include "lib/universal_include.h"

#include "maze.h"
#include <math.h>
#include <string.h>


Maze g_maze;

#include "lib/hi_res_time.h"
Maze::Maze()
{
	m_data = strdup(
		" *************************** "//0
		" *............*............* "
		" *.****.*****.*.*****.****.* "
		" *O*  *.*   *.*.*   *.*  *O* "
		" *.****.*****.*.*****.****.* "
		" *.........................* "
		" *.****.*.*********.*.****.* "
		" *......*.....*.....*......* "
		" ******.*****.*.*****.****** " 
		"      *.*...........*.*      "//9
		"      *.*.***HHH***.*.*      "//10
		"*******.*.*xxxxxxx*.*.*******"
		"       ...*xxxxxxx*...       "
		"*******.*.*********.*.*******"
		"      *.*.....M.....*.*      "
		" ******.*.*********.*.****** "//15
		" *............*............* "
		" *.****.*****.*.*****.****.* "
		" *O...*.......C.......*...O* " 
		" ****.*.*.*********.*.*.**** "//19
		" *......*.....*.....*......* "//20
		" *.**********.*.**********.* "
		" *.........................* "
		" *************************** ");
	   //012345678901234567890123456789

    m_width = 29;
    m_height = 24;
    
    m_breadCrumbs = new char [29 * 24];
    int steps = getMoveForTarget(2, 22, 26, 16);    


//         " **************************** "//1
//         " *............**............* "
//         " *.****.*****.**.*****.****.* "
//         " *O*  *.*   *.**.*   *.*  *O* "
//         " *.****.*****.**.*****.****.* "
//         " *..........................* "
//         " *.****.**.********.**.****.* "
//         " *.****.**.********.**.****.* "
//         " *......**....**....**......* "
//         " ******.*****.**.*****.****** "//10 
//         "      *.*****.**.*****.*      " 
//         "      *.**          **.*      "
//         "*******.** ***  *** **.*******"
//         "       .   *xxxxxx*   .       "
//         "*******.** ******** **.*******"
//         "      *.**          **.*      "
//         " ******.**.********.**.****** "
//         " *............**............* "
//         " *.****.*****.**.*****.****.* "
//         " *.****.*****.**.*****.****.* "//20
//         " *O..**........C.......*...O* " 
//         " ***.**.**.********.**.*.**** "
//         " *......**....**....**......* "
//         " *.**********.**.**********.* "
//         " *..........................* "
//         " **************************** ");
// 	   //123456789012345678901234567890

	m_eyeMap = strdup(
		" *************************** "
		" *DLLRRDLLRRRD*DLLRRRDLLRRD* "
		" *D****D*****D*D*****D****D* "
		" *D*  *D*   *D*D*   *D*  *D* "
		" *D****D*****D*D*****D****D* "
		" *DLLRRDLDLLLLLRRRRDLDLLRRD* "
		" *D****D*D*********D*D****D* "
		" *RRRRRD*RRRRD*DLLLL*DLLLLL* "
		" ******D*****D*D*****D****** " 
		"      *D*RRRRDDDLLLL*D*      "//10
		"      *D*U***DDD***U*D*      "
		"*******D*U*  DDD  *U*D*******"
		"       RRU*  TTT  *ULL       "
		"*******U*U*********U*U*******"
		"      *U*ULLLLLRRRRU*U*      "
		" ******U*U*********U*U****** "
		" *RRRRRURULLLL*RRRRURULLLLL* "
		" *U****U*****U*U*****U****U* "
		" *ULLL*ULLLRRURULLRRRU*RRRU* " 
		" ****U*U*U*********U*U*U**** "//20
		" *RRRURU*ULLLL*RRRRULULULLL* "
		" *U**********U*U**********U* "
		" *ULLLLLLRRRRURULLLLLRRRRRU* "
		" *************************** ");
	   //12345678901234567890123456789
	
	m_scatterMapRed = strdup(               // Top right target
		" *************************** "
		" *DRRRRDRRRRRD*RRRRRRRRRRRD* "
		" *D****D*****D*U*****U****D* "
		" *D*  *D*   *D*U*   *U*  *D* "
		" *D****D*****D*U*****U****D* "
		" *RRRRRRRRRRRRRURRRRRULLLLL* "
		" *U****U*U*********U*U****U* "
		" *URRRRU*ULLLD*RRRRU*URRRRU* "
		" ******U*****D*U*****U****** "
		"      *U*RRRRRRULLLL*U*      "
		"      *U*U***RUU***D*U*      "
		"*******U*U*RRRULLL*D*U*******"
		" RRRRRRURU*RRRULLL*RRULLLLLL "
		"*******U*U*********U*U*******"
		"      *U*URRRRRRRRRU*U*      "
		" ******U*U*********U*U****** "
		" *RRRRRURULLLL*RRRRURULLLLL* "
		" *U****U*****U*U*****U****U* "
		" *ULLL*URRRRRURULLLLLU*RRRU* "
		" ****U*U*U*********U*U*U**** "
		" *RRRRRU*LLLLL*RRRRU*ULULLL* "
		" *U**********U*U**********U* "
		" *ULLLLLLLLLLULURRRRRRRRRRU* "
		" *************************** ");

	m_scatterMapGreen = strdup(
		" *************************** "     // Bottom left target
		" *DLLRRDLLRRRD*DLLRRRDLLRRD* "
		" *D****D*****D*D*****D****D* "
		" *D*  *D*   *D*D*   *D*  *D* "
		" *D****D*****D*D*****D****D* "
		" *RRRRRDLLLLLLLLLLLLLLLLLLD* "
		" *D****D*D*********D*D****D* "
		" *RRRRRD*RRRRD*DLLLL*DLLLLL* "
		" ******D*****D*D*****D****** "
		"      *D*DLLLLLLRRRD*D*      "
		"      *D*D***UUU***D*D*      "
		"*******D*D*RRULLLL*D*D*******"
		" RRRRRRDRD*RRULLLL*DLDLLLLLL "
		"*******D*D*********D*D*******"
		"      *D*DLLLLRRRRRD*D*      "
		" ******D*D*********D*D****** "
		" *DRRRRDLLRRRD*DRRRRRDRRRRD* "
		" *D****D*****D*D*****D****D* "
		" *RRRD*DLLRRRRRRRRRDRD*DLLL* "
		" ****D*D*U*********D*D*D**** "
		" *DLLLLL*ULLLL*DLLLL*RRRRRD* "
		" *D**********U*D**********D* "
		" *RRRRRRRRRRRULLLLLLLLLLLLL* "
		" *************************** ");

	m_scatterMapOrange = strdup(
		" *************************** "     // Bottom right target
		" *DRRRRDLLLLLD*DLLLLLDLLLLD* "
		" *D****D*****D*D*****D****D* "
		" *D*  *D*   *D*D*   *D*  *D* "
		" *D****D*****D*D*****D****D* "
		" *RRRRRDLLLLLLLLLLLLLLLLLLL* "
		" *D****D*U*********U*U****U* "
		" *RRRRRD*ULLLL*RRRRU*LLLLLL* "
		" ******D*****U*U*****U****** "
		"      *D*RRRRRRRRRRD*U*      "
		"      *D*U***UUU***D*U*      "
		"*******D*U*RRRRULL*D*U*******"
		" RRRRRRDRU*RRRRULL*RRDLLLLLL "
		"*******D*U*********U*D*******"
		"      *D*ULLLLRRRRRU*D*      "
		" ******D*U*********U*D****** "
		" *DRRRRDLLLLLD*RRRRRRDLLLLL* "
		" *D****D*****D*D*****D****U* "
		" *LLLD*DRRRRRRRRRRRDLL*RRRU* "
		" ****D*D*U*********D*U*U**** "
		" *DLLLLL*ULLLL*DLLLL*ULLLLL* "
		" *D**********U*D**********U* "
		" *RRRRRRRRRRRRRRRRRRRRRRRRU* "
		" *************************** ");

	m_scatterMapPink = strdup(              // Top left target
		" *************************** "
		" *DLLLLLLLLLLL*DLLLLLDLLLLD* "
		" *D****U*****U*D*****D****D* "
		" *D*  *U*   *U*D*   *D*  *D* "
		" *D****U*****U*D*****D****D* "
		" *RRRRRULLLLLULLLLLLLLLLLLL* "
		" *U****U*U*********U*U****U* "
		" *URRRRU*ULLLL*RRRRU*ULLLLU* "
		" ******U*****U*U*****U****** "
		"      *U*RRRRULLLLLL*U*      "
		"      *U*U***UUU***U*U*      "
		"*******U*U*RRRULLL*U*U*******"
		" RRRRRRULL*RRRULLL*ULULLLLLL "
		"*******U*U*********U*U*******"
		"      *U*ULLLLRRRRRU*U*      "
		" ******U*U*********U*U****** "
		" *RRRRRULULLLL*RRRRULULLLLL* "
		" *U****U*****U*U*****U****U* "
		" *ULLL*ULLRRRULULLLLLU*LLLU* "
		" ****U*U*U*********U*U*U**** "
		" *RRRURU*ULLLU*URRRU*ULULLL* "
		" *U**********U*U**********U* "
		" *URRRRRRRRRRULULLLLLLLLLLU* "
		" *************************** ");

	m_offsetX = 20;
	m_offsetY = 50;
}


void Maze::init()
{
	m_tiles[1].Load("bitmaps/Half_LedgeH.png", 90);
	m_tiles[2].Load("bitmaps/Half_LedgeV.png", 0);
	m_tiles[3].Load("bitmaps/Corner.png", 0);
	m_tiles[4].Load("bitmaps/Half_LedgeH.png", 0);
	m_tiles[5].Load("bitmaps/Ledge.png", 0);
	m_tiles[6].Load("bitmaps/Corner.png", 90);
	m_tiles[7].Load("bitmaps/CrossingH.png", 0);
	m_tiles[8].Load("bitmaps/Half_LedgeV.png", 180);
	m_tiles[9].Load("bitmaps/Corner.png", 270);
	m_tiles[10].Load("bitmaps/LedgeV.png", 0);
	m_tiles[11].Load("bitmaps/CrossingV.png", 90);
	m_tiles[12].Load("bitmaps/Corner.png", 180);
	m_tiles[13].Load("bitmaps/CrossingH.png", 180);
	m_tiles[14].Load("bitmaps/CrossingV.png", 0);
}

/*
" *************************** "//0
" *............*............* "
" *.****.*****.*.*****.****.* "
" *O*  *.*   *.*.*   *.*  *O* "
" *.****.*****.*.*****.****.* "
" *.........................* "
" *.****.*.*********.*.****.* "
" *......*.....*.....*......* "
" ******.*****.*.*****.****** " 
"      *.*...........*.*      "//9
"      *.*.***HHH***.*.*      "//10
"*******.*.*xxxxxxx*.*.*******"
     N
    W E
	 S

	 X
   X X X = 1111
     X

	 X
   X X  = 1011

// NESW 
// 0000 = 0 = dot in middle
// 0001 = 1 = half line from west to middle
// 0010 = 2 = half line from south to middle
// 0011 = 3 = corner sw filled
// 0100 = 4 = half line from east to middle
// 0101 = 5 = straight line horizontal (unfilled either side)
// 0110 = 6 = corner 
// 0111 = 7 = straight line horizontal south filled Checked T Junction
// 1000 = 8 = half line from north to middle
// 1001 = 9 = corner (nw filled)
// 1010 = 10= vertical line (unfilled)
// 1011 = 11= straight line vertical (west filled)
// 1100 = 12= corner (ne filled)
// 1101 = 13= straight line horizontal (north filled)
// 1110 = 14= straight line vertical (east filled)Checked T Junction
// 1111 = 15= empty (filled)
*/
Sprite *Maze::getCellSprite(int x, int y)
{
	if (isWall (x,y) == true)
	{
		int NESW = 0;
		if (isWall (x-1,y) == true)
			NESW = 1;

		if (isWall (x,y+1) == true)
			NESW = NESW + 2;

		if (isWall (x+1,y) == true)
			NESW = NESW + 4;
		
		if (isWall (x,y-1) == true)
			NESW = NESW + 8;

		return &m_tiles[NESW];
	}
	else
	{
		// The cell at x,y is not filled, so we always render nothing
		return &m_tiles[0];
	}
}


bool Maze::isWall(int x, int y)
{
	if (x < 0 || y < 0 || x >= m_width || y >= m_height)
		return false;

	if (m_data[y * m_width + x] == '*')
		return true;
	else
		return false;
}


bool Maze::isPill(int x, int y)
{
	if (m_data[y * m_width + x] == '.')
		return true;
	else
		return false;
}

bool Maze::isCherry(int x, int y)
{
	if (m_data[y * m_width + x] == 'M')
		return true;
	else
		return false;
}

bool Maze::isPowerPill(int x, int y)
{
	if (m_data[y * m_width + x] == 'O')
		return true;
	else
		return false;
}


bool Maze::isHome(int x, int y)
{
	if (m_data[y * m_width + x] == 'x')
		return true;
	else
		return false;
}


bool Maze::isHomeEntrance(int x, int y)
{
	if (m_data[y * m_width + x] == 'H')
		return true;
	else
		return false;
}


bool Maze::isJunction(int x, int y)
{
	int numDirectionsAllowed = 0;
	if (isWall(x+1, y) == false)	// If cell to right is not a wall...
		numDirectionsAllowed++;
	if (isWall(x, y+1) == false)
		numDirectionsAllowed++;
	if (isWall(x-1, y) == false)
		numDirectionsAllowed++;
	if (isWall(x, y-1) == false)
		numDirectionsAllowed++;
	if (numDirectionsAllowed >=3)
		return true;
	else
		return false;
}


bool Maze::isCorner(int x, int y)
{
	// If we can go left and right, then it is not a corner
	if ((isWall(x+1, y) == false) && (isWall(x-1, y) == false))
		return false;
	
	// If we can go up and down, then it is not a corner
	if ((isWall(x, y+1) == false) && (isWall(x, y-1) == false))
		return false;

	return true;
}


Move Maze::getCornerMove(int x, int y, Move inputMove)
{
	if (isWall(x - 1, y) == false && inputMove != MoveRight)
		return MoveLeft;
	if (isWall(x + 1, y) == false && inputMove != MoveLeft)
		return MoveRight;
	if (isWall(x , y - 1) == false && inputMove != MoveUp)
		return MoveUp;
	if (isWall(x, y + 1) == false && inputMove != MoveDown)
		return MoveDown;

	return MoveNone;
}


bool Maze::canMove(int x, int y)
{
    if (x < 1 || x >= (m_width - 1) || y < 1 || y >= (m_height - 1))
        return false;
	if (isWall(x, y))
		return false;
	if (m_breadCrumbs[y * m_width + x] == '*')
		return false;
	return true;
}


int Maze::searchForTarget(int sx, int sy, int tx, int ty, int depth)
{
    depth++;
    if (depth > 80 || depth >= m_shortestRouteLen)
        return -1;

	// Have we reached the target?
	if (sx == tx && sy == ty)
	{
		m_shortestRouteLen = depth;
//		m_movesForTarget.Empty();
		return 0;
	}
	
    // Place breadcrumb
    m_breadCrumbs[sy * m_width + sx] = '*';
    
    int shortest = 9999;
	Move bestMove = MoveNone;
	if (canMove(sx - 1, sy))
	{
		 int steps = searchForTarget(sx - 1, sy, tx, ty, depth);
		 if (steps >= 0)
		 {
             shortest = steps;
			 bestMove = MoveLeft;
		 }
	}

	if (canMove(sx + 1, sy))
	{
		 int steps = searchForTarget(sx + 1, sy, tx, ty, depth);
		 if (steps >= 0 && steps < shortest)
		 {
             shortest = steps;
			 bestMove = MoveRight;
		 }
	}

	if (canMove(sx, sy - 1))
	{
		 int steps = searchForTarget(sx, sy - 1, tx, ty, depth);
		 if (steps >= 0 && steps < shortest)
		 {
             shortest = steps;
			 bestMove = MoveUp;
		 }
	}

	if (canMove(sx, sy + 1))
	{
		 int steps = searchForTarget(sx, sy + 1, tx, ty, depth);
		 if (steps >= 0 && steps < shortest)
		 {
             shortest = steps;
			 bestMove = MoveDown;
		 }
	}

    m_breadCrumbs[sy * m_width + sx] = ' ';

    if (bestMove != MoveNone)
	{
//		m_movesForTarget.Push(bestMove);
        return shortest + 1;
	}

    return -1;
}


Move Maze::getMoveForTarget(int sx, int sy, int tx, int ty)
{
	double startTime = GetHighResTime();
	memset(m_breadCrumbs, ' ', m_width * m_height);
	m_movesForTarget.Empty();
	m_shortestRouteLen = 9999;
	m_targetX = tx;
	m_targetY = ty;
	int steps = searchForTarget(sx, sy, tx, ty, 0);

// 	double endTime = GetHighResTime();
// 	double dur = endTime - startTime;
// 	char buf[10];
// 	sprintf(buf, "%0.05f", dur);
// 	OutputDebugStr(buf);
	return MoveNone;
}


void Maze::eatPill(int x, int y)
{
	char *tmp = &(m_data[y * m_width + x]);
	*tmp = ' ';
}


Move Maze::getScatterMovement(int x, int y, GhostCharacter character)
{
	char *scatterMap = NULL;
	switch (character)
	{
	case GhostCharacterRed: scatterMap = m_scatterMapRed; break;
	case GhostCharacterBlue: scatterMap = m_scatterMapGreen; break;
	case GhostCharacterOrange: scatterMap = m_scatterMapOrange; break;
	case GhostCharacterPink: scatterMap = m_scatterMapPink; break;
	}

	char temp = scatterMap[y * m_width + x];
	if (temp == 'L')
		return MoveLeft;
	else if (temp == 'R')
		return MoveRight;
	else if (temp == 'U')
		return MoveUp;
	else if (temp == 'D')
		return MoveDown;
	else
		return MoveNone;
}


Move Maze::getEyesOnlyMovement(int x, int y)
{
	char temp = m_eyeMap[y * m_width + x];
	if (temp == 'L')
		return MoveLeft;
	else if (temp == 'R')
		return MoveRight;
	else if (temp == 'U')
		return MoveUp;
	else if (temp == 'D')
		return MoveDown;
	else
		return MoveNone;
}


bool Maze::isComplete()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (isPill(x, y))
				return false;
		}
	}

	return true;
}


bool Maze::isPositionAllowed(double fx, double fy)
{
	int x = (int)floor(fx);
	int y = (int)floor(fy);
	
	if (x < 0 || x >= m_width)
		return false;
	
	if (isWall(x, y))
		return false;
	return true;
}


bool Maze::edgePassed(double fx, double oldFx)
{
	int x = (int)floor(fx);
	int oldX = (int)floor(oldFx);
	
	if (oldX != x)
		return true;
	
	return false;
}


int Maze::getPacStartPosX()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (m_data[y * m_width + x] == 'C')
				return x;
		}
	}

    return 0;
}


int Maze::getPacStartPosY()
{
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (m_data[y * m_width + x] == 'C')
				return y;
		}
	}

    return 0;
}