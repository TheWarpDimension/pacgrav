#include "lib/universal_include.h"
#include "lib/text_renderer.h"
#include "lib/hi_res_time.h"
#include "app.h"
#include "ghost.h"
#include "maze.h"


void Ghost::Init(double x, double y, GhostCharacter character)
{
    Creature::Init(x, y, CreatureTypeGhost);

	m_mode = GhostModeScatter;
	m_character = character;

	m_speed = 3.25;

	m_eyesOnlySprite.Load("bitmaps/EyesOnly.png", 0);

	m_runAwaySprites[0].Load("bitmaps/RunAwayGhost0.png", 0);
	m_runAwaySprites[1].Load("bitmaps/RunAwayGhost0.png", 0);
	m_runAwaySprites[2].Load("bitmaps/RunAwayGhost0.png", 0);
	m_runAwaySprites[3].Load("bitmaps/RunAwayGhost0.png", 0);

    char *filename = NULL;
    switch (character)
    {
    case GhostCharacterRed:    filename = "bitmaps/RedGhost0.png"; break;
    case GhostCharacterBlue:  filename = "bitmaps/BlueGhost0.png"; break;
    case GhostCharacterOrange: filename = "bitmaps/OrangeGhost0.png"; break;
    case GhostCharacterPink:   filename = "bitmaps/PinkGhost0.png"; break;
    }

 	m_normalSprites[0].Load(filename, 0);
    m_normalSprites[1].Load(filename, 0);
    m_normalSprites[2].Load(filename, 0);
    m_normalSprites[3].Load(filename, 0);

	switch (character)
	{
	case GhostCharacterRed:    filename = "bitmaps/RedEyes1.png"; break;
	case GhostCharacterBlue:  filename = "bitmaps/BlueEyes1.png"; break;
	case GhostCharacterOrange: filename = "bitmaps/OrangeEyes1.png"; break;
	case GhostCharacterPink:   filename = "bitmaps/PinkEyes1.png"; break;
	}

	m_normalEyesSprite.Load(filename, 0);
	ReleaseAssert(m_runAwayEyesSprite.Load("bitmaps/RunAwayEyes1.png", 0), "Couldn't load run away eyes");
}


void Ghost::AdvanceScatterMovement()
{
	m_nextMove = g_maze.getScatterMovement(m_posX+.5, m_posY+.5, m_character);

	if (GetHighResTime() > g_scatterEndTime)
		m_mode = GhostModeNormal;
}


bool Ghost::IsPositionAllowed(int x, int y)
{
	if (g_maze.isPositionAllowed(x, y))
	{
		if (!g_maze.isHomeEntrance(x, y))
			return true;
	}

	return false;
}

// 10¯PAPER 0: BORDER 0: INK 7: BRIGHT 1: CLS                                                                 
// 20¯INPUT "Intelligence ";i                                                                                 
// 30¯REM                                                       Set up ghost 1 co-ords                        
// 40¯LET gx1=INT (RND*255): LET gy1=INT (RND*175)                                                            
// 50¯REM                                                     Set up PcMan(TM) co-ords                        
// 60¯LET px=10: LET py=20                                                                                    
// 70¯REM                              Locate PcMan(TM)                                                       
// 80¯LET gx1d=px-gx1                                                                                         
// 90¯LET gy1d=py-gy1                                                                                         
// 100¯REM                              Inteligence loop                                                       
// 110¯FOR a=1 TO i                                                                                            
// 120¯IF gx1d<0 and abs(gx1d) > abs(gy1d) THEN LET gx1=gx1-1                                                                            
// 130¯IF gx1d>0 THEN LET gx1=gx1+1                                                                            
// 140¯IF gy1d<0 THEN LET gy1=gy1-1                                                                            
// 150¯IF gy1d>0 THEN LET gy1=gy1+1                                                                            
// 160¯PLOT INK 5;gx1,gy1                                                                                      
// 170¯PLOT INK 4;gx1,gy1                                                                                      
// 180¯PLOT INK 2;gx1,gy1                                                                                      
// 190¯PLOT INK 6;px,py                                                                                        
// 200¯PRINT AT 0,0;"px ";px,"py ";py                                                                          
// 210¯REM                                 Player controls                                                     
// 220¯IF INKEY$="a" THEN LET py=py-1                                                                          
// 230¯IF INKEY$="q" THEN LET py=py+1                                                                          
// 240¯IF INKEY$="o" THEN LET px=px-1                                                                          
// 250¯IF INKEY$="p" THEN LET px=px+1                                                                          
// 260¯REM                           Inteligence loop repeat                                                   
// 270¯NEXT a                                                                                                  
// 280¯REM                         Research PcMan(TM) Co-ords again in loop                                    
// 290¯GO TO 80                                                                    

bool Ghost::IsMoveAllowed(Move m)
{
	switch (m)
	{
	case MoveLeft: return IsPositionAllowed(m_posX - 1, m_posY);
	case MoveRight: return IsPositionAllowed(m_posX + 1, m_posY);
	case MoveUp: return IsPositionAllowed(m_posX, m_posY - 1);
	case MoveDown: return IsPositionAllowed(m_posX, m_posY + 1);
	}

	return false;
}


Move Ghost::GetBestMoveTowardsPcMan()
{
	Move nextMove = MoveNone;
	double diffX = g_pcman.m_posX - m_posX;
	double diffY = g_pcman.m_posY - m_posY;

	// Choose a movement from this priority list - i.e. choose 2 if we can't have 1.
	// 1. Move towards pcman in the dimension that he's furthest away
	// 2. Move towards pcman in the other dimension
	// 3. Move in opposite to (2)
	// 4. Move in opposite to (1)

	Move move1 = MoveLeft;
	Move move3 = MoveRight;
	if (diffX > 0)
	{
		move1 = MoveRight;
		move3 = MoveLeft;
	}

	Move move2 = MoveUp;
	Move move4 = MoveDown;
	if (diffY > 0)
	{
		move2 = MoveDown;
		move4 = MoveUp;
	}
	
	if (abs(diffX) < abs(diffY))
	{
		Move moveTmp = move1;
		move1 = move2;
		move2 = moveTmp;
	
		moveTmp = move3;
		move3 = move4;
		move4 = moveTmp;
	}

	if (IsMoveAllowed(move1))
		nextMove = move1;
	else if (IsMoveAllowed(move2))
		nextMove = move2;
	else if (IsMoveAllowed(move3))
		nextMove = move3;
	else if (IsMoveAllowed(move4))
		nextMove = move4;

	return nextMove;
}


void Ghost::AdvanceNormalMovement(Move oldMove)
{
	double diffX = g_pcman.m_posX - m_posX;
	double diffY = g_pcman.m_posY - m_posY;

	// If we are at a junction, then move towards PC man
	if (g_maze.isJunction(m_posX, m_posY)) //stupid capitals and pizx and pizy
	{
		m_nextMove = GetBestMoveTowardsPcMan();
	}
	else if (m_move == MoveNone)
	{
		// *******
		// * X <-
		// *   ***
		if (g_maze.isCorner(m_posX, m_posY))
		{
			m_nextMove = g_maze.getCornerMove(m_posX, m_posY, (Move)oldMove);
		}
//		m_nextMove = GetBestMoveTowardsPcMan();
	}
//     else if (rand() % 1000 == 1)
//     {
// 		double dist = sqrtf(diffX * diffX + diffY * diffY);
// 		
// 		if (dist < 12)
// 		{
// 			m_nextMove = GetBestMoveTowardsPcMan();
// 		}
// 		else
// 		{
// 			switch (m_move)
// 			{
// 			case MoveUp:
// 			case MoveDown:
// 				if (rand() % 2 == 0)
// 					m_nextMove = MoveLeft;
// 				else
// 					m_nextMove = MoveRight; 
// 				break;
// 			case MoveLeft:
// 			case MoveRight:
// 				if (rand() % 2 == 0)
// 				{
// 					if (IsPositionAllowed(m_posX, m_posY-1))
// 						m_nextMove = MoveUp;
// 				}
// 				else
// 				{
// 					if (IsPositionAllowed(m_posX, m_posY+1))
// 						m_nextMove = MoveDown; 
// 				}
// 				break;
// 			}
// 		}
//     }
//     else if (rand() % 2000 == 1)
//     {
//         m_nextMove = rand() % MoveNumMoves;
//     }

	if (g_maze.isHome(m_posX, m_posY))
	{
		m_nextMove = MoveUp;
	}

	if (GetHighResTime() > g_scatterEndTime)
		m_mode = GhostModeScatter;
}


void Ghost::AdvanceEyesOnlyMovement()
{
	if (g_maze.isHome(m_posX, m_posY))
		m_mode = GhostModeScatter;
	
	m_nextMove = g_maze.getEyesOnlyMovement(m_posX+0.5, m_posY+0.5);
}


void Ghost::Advance(float timeDelta)
{
	Move oldMove = m_move;

	// This does the collision detection and sets our move to MoveNone when we 
	// hit a wall
	Creature::Advance(timeDelta);

	double diffX = g_pcman.m_posX - m_posX;
	double diffY = g_pcman.m_posY - m_posY;
	
	if (m_mode == GhostModeScatter)
		AdvanceScatterMovement();
	else if (m_mode == GhostModeNormal || m_mode == GhostModeRunAway)
		AdvanceNormalMovement(oldMove);
	else	
		AdvanceEyesOnlyMovement();

	// Have we collided with pcman?
	if (abs(diffX) < 1.0 && abs(diffY) < 1.0)
	{
		// Yes, should we eat him?
		switch (m_mode)
		{
		case GhostModeNormal:
		case GhostModeScatter:
			if  (g_godMode != 1)
			g_gameMode = ModePcmanDying;
			break;
		case GhostModeRunAway:
			// No, he eats us
			m_mode = GhostModeEyesOnly;
			break;
		}
	}	

	// Has run away mode ended?
	if (g_powerPillEndTime > 0 && GetHighResTime() > g_powerPillEndTime)
		EndPowerPillMode();	
}


void Ghost::Render()
{
	Sprite *bodySprite = NULL;
	Sprite *eyesSprite = NULL;
	int w = g_tileSize * 1.3;
	int h = g_tileSize * 1.3;

	// Choose which sprites to use for the eyes and body
	switch (m_mode)
	{
    case GhostModeScatter:
    case GhostModeNormal:
		bodySprite = &m_normalSprites[abs((int)m_spriteFrame)];
		eyesSprite = &m_normalEyesSprite;
		break;
	case GhostModeRunAway:
		bodySprite = &m_runAwaySprites[abs((int)m_spriteFrame)];
		eyesSprite = &m_runAwayEyesSprite;
		break;
	case GhostModeEyesOnly:
		eyesSprite = &m_eyesOnlySprite;
		break;
    default:
        ReleaseAssert(0, "Couldn't find sprite");
	}

	int x = g_maze.m_offsetX -3 + m_posX * g_tileSize;
	int y = g_maze.m_offsetY -1 + m_posY * g_tileSize;

	// Render the body
	if (bodySprite)
	{
		switch (m_move)
		{
		case MoveLeft:
			bodySprite->Render(x, y, Sprite::ModeNormal, w, h);
			break;
		case MoveRight:
		case MoveNone:
		case MoveUp:
		case MoveDown:
			bodySprite->Render(x, y, Sprite::ModeFlipH, w, h);
			break;
		}
	}

	// Render the eyes
	switch (m_move)
	{
	case MoveLeft:
		eyesSprite->Render(x, y, Sprite::ModeNormal, w, h);
			break;
	case MoveRight:
	case MoveNone:
	case MoveUp:
	case MoveDown:
		eyesSprite->Render(x, y, Sprite::ModeFlipH, w, h);
		break;
	}

	if (g_debugMode)
	{
		if (m_mode == GhostModeScatter)
			g_fixedFont.DrawText(x, y, 14, "S");
		else if (m_mode == GhostModeNormal)
			g_fixedFont.DrawText(x, y, 14, "N");
	}
}	


void Ghost::EndPowerPillMode() 
{ 
	if (m_mode != GhostModeEyesOnly) 
		m_mode = GhostModeScatter; 
}


void Ghost::SetModeRunAway() 
{ 
	if (m_mode != GhostModeEyesOnly) 
		m_mode = GhostModeRunAway; 
}
