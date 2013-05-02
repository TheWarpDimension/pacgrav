#include "lib/universal_include.h"

#include <math.h>
#include <stdio.h>

#include "lib/binary_stream_readers.h"
#include "lib/debug_utils.h"
#include "lib/drawing_primitives.h"
#include "lib/hi_res_time.h"
#include "lib/input.h"
#include "lib/rgb_colour.h"
#include "lib/text_renderer.h"
#include "lib/sprite.h"
#include "sound/sound_system.h"
#include "lib/window_manager.h"

#include "sound/sound_stream_decoder.h"

#include "app.h"
#include "ghost.h"
#include "maze.h"
#include "pcman.h"


App         *g_app = NULL;
double      g_advanceTime;
int         g_score = 0;
int         g_tileSize = 25;
PcMan       g_pcman;
int         g_gameMode = ModeInGame;
Sprite      g_gameOverSprite;
Sprite		g_mazeBlockSprite;
Sprite		g_mazePowerpillSprite;
Sprite		g_mazePillSprite;
Sprite		g_mazeCherrySprite;
double      g_powerPillEndTime = 0.0;
double      g_scatterEndTime = 0.0;
Ghost		g_ghosts[numGhosts];
bool		g_debugMode = false;


App::App()
{
}


void App::MainLoop()
{
    g_gameOverSprite.Load("bitmaps/GameOver.png", 0);
	g_mazeBlockSprite.Load("bitmaps/Block.png", 0);
	g_mazePillSprite.Load("bitmaps/Pill.png", 0);
	g_mazeCherrySprite.Load("bitmaps/Cherry.png", 0);
	g_mazePowerpillSprite.Load("bitmaps/Powerpill.png", 0);

	g_pcman.Init(g_maze.getPacStartPosX(),g_maze.getPacStartPosY());
	g_ghosts[0].Init(14.0, 11.0, GhostCharacterRed);
	g_ghosts[1].Init(13.0, 12.0, GhostCharacterBlue);
	g_ghosts[2].Init(14.0, 12.0, GhostCharacterOrange);
	g_ghosts[3].Init(15.0, 12.0, GhostCharacterPink);

	double lastTime = GetHighResTime();

	g_scatterEndTime = GetHighResTime() + 9.0;

	while (!g_keys[KEY_ESC] && !g_inputManager->m_windowClosed)
	{
		// Read user input
		g_inputManager->Advance();
		g_inputManager->PollForMessages();

		double timeNow = GetHighResTime();
		double timeDelta = timeNow - lastTime;
        if (timeDelta > 0.2)
            timeDelta = 0.2;
		if (g_keys[KEY_F5])
			timeDelta *= 0.1;
		lastTime = timeNow;

		if (g_keyDowns[KEY_F4])
			g_debugMode = !g_debugMode;

		if (g_gameMode == ModeInGame)
		{
			// ****************
			// Advance the NPCs
			// ****************
			
			// Set desired move for PC man
			if (g_keys[KEY_RIGHT] || g_keys [KEY_D])
				g_pcman.m_nextMove = MoveRight;
			if (g_keys[KEY_LEFT] || g_keys [KEY_A])
				g_pcman.m_nextMove = MoveLeft;
			if (g_keys[KEY_UP] || g_keys [KEY_W])
				g_pcman.m_nextMove = MoveUp;
			if (g_keys[KEY_DOWN] || g_keys [KEY_S])
				g_pcman.m_nextMove = MoveDown;

			g_pcman.Advance(timeDelta);
			for (int i = 0; i < numGhosts; i++)
				g_ghosts[i].Advance(timeDelta);

			if (g_powerPillEndTime > 0 && timeNow > g_powerPillEndTime)
				g_powerPillEndTime = 0.0;

			if (timeNow > g_scatterEndTime)
				g_scatterEndTime = timeNow + 9.0;
		}
		else if (g_gameMode == ModePcmanDying)
		{
			g_pcman.m_numLives--;
			if (g_pcman.m_numLives <= 0)
				g_gameMode = ModeGameOverScreen;
			else
				g_gameMode = ModeInGame;

			g_pcman.m_posX = g_maze.getPacStartPosX();
			g_pcman.m_posY = g_maze.getPacStartPosY();
			g_pcman.m_move = MoveLeft;
			g_pcman.m_nextMove = MoveLeft;
		}

		// Clear background to black
		DrawFilledBox(0, 0, g_windowManager->m_width, g_windowManager->m_height, 0, 0, 0);

		if (g_gameMode != ModeGameOverScreen)
		{
			// Render maze
			for (int y = 0; y < g_maze.m_height; y++)
			{
				for(int x = 1; x < g_maze.m_width - 1; x++)
				{
					int tx = g_maze.m_offsetX + x * g_tileSize;
					int ty = g_maze.m_offsetY + y * g_tileSize;
					if (g_maze.isWall(x, y))
					{
						Sprite *sprite = g_maze.getCellSprite(x, y);
						sprite->Render(tx, ty, Sprite::ModeNormal);
					}
					else if (g_maze.isPill(x, y))
					{
						int w = 12;
						int h = 12;
						int x = tx + (g_tileSize - w) / 2;
						int y = ty + (g_tileSize - h) / 2;
						g_mazePillSprite.Render(x, y, Sprite::ModeNormal, w, h);
					}
					else if (g_maze.isCherry(x, y))
					{
						g_mazeCherrySprite.Render(tx - g_tileSize * 0.5, ty - g_tileSize * 0.5, 
												  Sprite::ModeNormal, 
												  g_tileSize * 1.32, g_tileSize * 1.32);
					}
					else if (g_maze.isPowerPill(x, y))
					{
						if ((int)(GetHighResTime() * 2) & 1)
							g_mazePowerpillSprite.Render(tx, ty, Sprite::ModeNormal);
 					}
				} 
			}

			// Render the NPCs
			if (g_gameMode == ModeInGame || g_gameMode == ModePcmanDying)
				g_pcman.Render();

			if (g_gameMode == ModeInGame)
			{
				g_ghosts[GhostCharacterRed].Render();
				g_ghosts[GhostCharacterBlue].Render();
				g_ghosts[GhostCharacterOrange].Render();
 				g_ghosts[GhostCharacterPink].Render();
			}
			
			DrawFilledBox(g_maze.m_offsetX -1, g_maze.m_offsetY - 37+ g_tileSize * 13, g_tileSize, g_tileSize*2, 0, 0, 0);
			DrawFilledBox(g_maze.m_offsetX +1 + g_tileSize * (g_maze.m_width - 1), g_maze.m_offsetY - 37 + g_tileSize * 13, g_tileSize, g_tileSize*2, 0, 0, 0);

			// Draw score
			//SetTextColour(RGBAColour(255,225,225));
			if ((int)(GetHighResTime() * 2) & 1)
    			g_fixedFont.DrawText(45, 7, DEF_FONT_SIZE, "1 UP");
			char buf[10];
			sprintf(buf, "%d", g_score);
			g_fixedFont.DrawText(45, 27, DEF_FONT_SIZE, buf);

			// Draw FPS meter
//			g_fixedFont.DrawText();

			// Draw pcman lives
			for (int i = 0; i < g_pcman.m_numLives; i++)
				g_pcman.m_sprite[0].Render(g_maze.m_offsetX + g_tileSize + i * 10, (g_maze.m_height + 2) * g_tileSize, Sprite::ModeNormal);
			
			// Have we finished the level?
			if (g_maze.isComplete())
				g_fixedFont.DrawText(200, 200, DEF_FONT_SIZE, "// Finished //");
		}
		else
		{
			g_gameOverSprite.Render(0, 0, Sprite::ModeNormal, 760, 690);
		//	g_soundSystem.PlayOnce("sounds/GameOverYeah.ogg");
		}
        
        g_soundSystem.Advance();

		// Flip and sleep //
		
		g_windowManager->Flip();
		SleepEx(1, true);
	}
}


void AppMain()
{
	g_app = new App();

	// WindowManager	
	g_windowManager = new WindowManager;
	g_windowManager->CreateWin(100, 10, 760, 690, true);
//	g_windowManager->CreateWin(100, 100, 1024, 768, false);
//	ShowCursor(false);

    // Sound System

	// InputManager
	g_inputManager = new InputManager;
	
	// Fonts
	g_fixedFont.Initialise("bitmaps/speccy_font.png");
 
	g_maze.init();
//	if (g_maze.isWall(x,y) == false)

	g_app->MainLoop();
}
