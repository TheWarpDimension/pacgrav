#include "lib/universal_include.h"
#include "pcman.h"

#include "lib/hi_res_time.h"
#include "lib/sprite.h"
#include "app.h"
#include "maze.h"


void PcMan::Init(double x, double y)
{
    Creature::Init(x, y, CreatureTypePcMan);

    m_speed = 4.25;
	m_numLives = 3;

	m_sprite[0].Load("bitmaps/pcman0.png", 0);
	m_sprite[1].Load("bitmaps/pcman1.png", 0);
	m_sprite[2].Load("bitmaps/pcman2.png", 0);
	m_sprite[3].Load("bitmaps/pcman3.png", 0);
	m_spriteFrame = 0;
}


void PcMan::Advance(float timeDelta)
{
	if (m_move != MoveNone)
		m_lastMove = m_move;

    Creature::Advance(timeDelta);

    if (g_maze.isPill(m_posX + 0.5, m_posY + 0.5))
	{
		g_maze.eatPill(m_posX + 0.5, m_posY + 0.5);
		g_score += 10;
	}
	else if (g_maze.isPowerPill(m_posX + 0.5, m_posY + 0.5))
	{
		g_maze.eatPill(m_posX + 0.5, m_posY + 0.5);
		g_score += 50;
		g_powerPillEndTime = GetHighResTime() + 9.0;

		for (int i = 0; i < numGhosts; i++)
			g_ghosts[i].SetModeRunAway();
	}
}


void PcMan::Render()
{
	float zoomAmount = 1.5;
	int x = g_maze.m_offsetX - 0.5 * (zoomAmount - 1.0) * g_tileSize + m_posX * g_tileSize + 1;
	int y = g_maze.m_offsetY - 0.5 * (zoomAmount - 1.0) * g_tileSize + m_posY * g_tileSize + 1;
	int w = g_tileSize * zoomAmount;
	int h = g_tileSize * zoomAmount;

	switch (m_lastMove)
	{
	case MoveLeft:
		m_sprite[abs((int)m_spriteFrame)].Render(x, y, Sprite::ModeFlipH, w, h);
		break;
	case MoveRight:
	case MoveNone:
		m_sprite[abs((int)m_spriteFrame)].Render(x, y, Sprite::ModeNormal, w, h);
		break;
	case MoveUp:
		m_sprite[abs((int)m_spriteFrame)].Render(x, y, Sprite::ModeRotate270, w, h);
		break;
	case MoveDown:
		m_sprite[abs((int)m_spriteFrame)].Render(x, y, Sprite::ModeRotate90, w, h);
		break;
	}
}
