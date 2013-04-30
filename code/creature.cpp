#include "lib/universal_include.h"
#include "creature.h"

#include "lib/hi_res_time.h"
#include "lib/sprite.h"
#include "app.h"
#include "maze.h"


void Creature::Init(double x, double y, CreatureType creatureType)
{
	m_posX = x;
	m_posY = y;
	m_posOldX = 1.0;
	m_posOldY = 1.0;
	m_move = MoveLeft;
	m_nextMove = MoveNone;	
	m_speed = 4.25;

	m_spriteFrame = 0;

	m_creatureType = creatureType;
}


void Creature::Advance(float timeDelta)
{
	Move oldMove = m_move;

	switch (m_move)
	{
	case MoveRight:
		m_posX += timeDelta * m_speed;		
		
		// Have we hit a wall?
		if (!g_maze.isPositionAllowed(m_posX + 1, m_posY))
		{
			m_posX = floor(m_posX);
			m_move = MoveNone;
		}
		
		if (m_posX >= g_maze.m_width - 1.1)
			m_posX = 0.1;

		// Consider requested move
		if (m_nextMove == MoveLeft)
		{
			m_move = m_nextMove;
		}
		else if (g_maze.edgePassed(m_posX, m_posOldX))
		{
			if (m_nextMove == MoveUp && g_maze.isPositionAllowed(m_posX, m_posY - 1))
			{
				m_posX = floor(m_posX);
				m_move = MoveUp;
			}
			else if (m_nextMove == MoveDown && g_maze.isPositionAllowed(m_posX, m_posY + 1))
			{
				m_posX = floor(m_posX);
				m_move = MoveDown;
			}
		}
		break;
	case MoveLeft:
		m_posX -= timeDelta * m_speed;
		
		// Have we hit a wall?
		if (!g_maze.isPositionAllowed(m_posX, m_posY))
		{
			m_posX = ceil(m_posX);
			m_move = MoveNone;
		}

		if (m_posX <= 0.1)
			m_posX = g_maze.m_width - 1.1;

		// Consider requested move
		if (m_nextMove == MoveRight)
		{
			m_move = m_nextMove;
		}
		else if (g_maze.edgePassed(m_posX, m_posOldX))
		{
			if (m_nextMove == MoveUp && g_maze.isPositionAllowed(m_posX + 1, m_posY - 1))
			{
				m_posX = ceil(m_posX);
				m_move = MoveUp;
			}
			else if (m_nextMove == MoveDown && g_maze.isPositionAllowed(m_posX + 1, m_posY + 1))
			{
				m_posX = ceil(m_posX);
				m_move = MoveDown;
			}
		}
		break;
	case MoveUp:
		m_posY -= timeDelta * m_speed;

		// Have we hit a wall?
		if (!g_maze.isPositionAllowed(m_posX, m_posY))
		{
			m_posY = ceil(m_posY);
			m_move = MoveNone;
		}

		// Consider requested move
		if (m_nextMove == MoveDown)
		{
			m_move = m_nextMove;
		}
		else if (g_maze.edgePassed(m_posY, m_posOldY))
		{
			if (m_nextMove == MoveLeft && g_maze.isPositionAllowed(m_posX - 1, m_posY + 1))
			{
				m_posY = ceil(m_posY);
				m_move = MoveLeft;
			}
			else if (m_nextMove == MoveRight && g_maze.isPositionAllowed(m_posX + 1, m_posY + 1))
			{
				m_posY = ceil(m_posY);
				m_move = MoveRight;
			}
		}
		break;
	case MoveDown:
		m_posY += timeDelta * m_speed;

		// Have we hit a wall?
		if (!g_maze.isPositionAllowed(m_posX, m_posY + 1))
		{
			m_posY = floor(m_posY);
			m_move = MoveNone;
		}

		// Consider requested move
		if (m_nextMove == MoveUp)
		{
			m_move = m_nextMove;
		}
		else if (g_maze.edgePassed(m_posY, m_posOldY))
		{
			if (m_nextMove == MoveLeft && g_maze.isPositionAllowed(m_posX - 1, m_posY))
			{
				m_posY = floor(m_posY);
				m_move = MoveLeft;
			}
			else if (m_nextMove == MoveRight && g_maze.isPositionAllowed(m_posX + 1, m_posY))
			{
				m_posY = floor(m_posY);
				m_move = MoveRight;
			}
		}
		break;
	case MoveNone:
		if (m_nextMove == MoveUp && g_maze.isPositionAllowed(m_posX, m_posY - 1))
			m_move = MoveUp;
		else if (m_nextMove == MoveDown && g_maze.isPositionAllowed(m_posX, m_posY + 1))
			m_move = MoveDown;
		else if (m_nextMove == MoveLeft && g_maze.isPositionAllowed(m_posX - 1, m_posY))
			m_move = MoveLeft;
		else if (m_nextMove == MoveRight && g_maze.isPositionAllowed(m_posX + 1, m_posY))
			m_move = MoveRight;
		break;
	}
		
	m_posOldX = m_posX;
	m_posOldY = m_posY;

	m_spriteFrame += timeDelta * 30.0;
	if (m_spriteFrame >= 4.0)
		m_spriteFrame = -3.0;
}
