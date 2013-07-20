#include "lib/universal_include.h"

#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod.hpp"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod_errors.h"

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
bool		g_godMode = false;
bool		g_slowmo = false;
bool		g_paused = false;
bool		g_killMode = false;

//ERROR CODE START
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
		ReleaseAssert( false, "FMOD_MANAGER SAYS - FMOD HAS CRASHED, BLAME BONCEY'S FMOD CODE.");
    }
}
//ERROR CODE END

//FMOD CODE

App::App()
{
}


void App::MainLoop()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	FMOD::System     *system;
    FMOD::Sound      *sound1, *sound2, *sound3;
    FMOD::Channel    *channel = 0;
    FMOD_RESULT       result;
    int               key;
    unsigned int      version;

    /*
        Create a System object and initialize.
    */
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        ReleaseAssert( false, "OUTDATED FMOD_MANAGER SAYS - FMOD IS OUTDATED. OR NEEDS TO BE UPDATED IN OUR CODE...");
    }

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);
	/// PUT SOUNDS AFTER ME!
	result = system->createSound("../sounds/goy.wav", FMOD_HARDWARE, 0, &sound1);
    ERRCHECK(result);
	
	result = sound1->setMode(FMOD_LOOP_OFF);
    ERRCHECK(result);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		lastTime = timeNow;

		if (g_keyDowns[KEY_F4])
			g_debugMode = !g_debugMode;

		if (g_keyDowns[KEY_F3])
			g_godMode = !g_godMode;

		if (g_keyDowns[KEY_F5])
			g_slowmo = !g_slowmo;

		if (g_keyDowns[KEY_P])
			g_paused = !g_paused;
		
		if (g_keyDowns[KEY_CONTROL, KEY_K])
			g_killMode = !g_killMode;
//SOUNDTEST
		if (g_keyDowns[KEY_CONTROL, KEY_T])
			result = system->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel);

		if (g_slowmo != 0)
			timeDelta *= 0.1;

		if (g_killMode != 0)
			g_gameMode = ModePcmanDying;

		if (g_paused != 0)
			timeDelta *= 0.0;


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

			//Pause code

			//Are any debug features on?
			if (g_godMode != 0)
				g_fixedFont.DrawText(150, 25, DEF_FONT_SIZE, "GOD MODE");
			if (g_debugMode != 0)
				g_fixedFont.DrawText(300, 25, DEF_FONT_SIZE, "DEBUG MODE");
			if (g_slowmo != 0)
				g_fixedFont.DrawText(475, 25, DEF_FONT_SIZE, "MATRIX MODE");
			if (g_paused != 0)
				g_fixedFont.DrawText(342.5, 345, DEF_FONT_SIZE, "PAUSED");
			if (g_killMode != 0)
				g_fixedFont.DrawText(0, 345, DEF_FONT_SIZE, "YOU SHOULD BE DEAD NOW");
		}
		else
		{
			g_gameOverSprite.Render(0, 0, Sprite::ModeNormal, 760, 690);
			g_soundSystem.PlayOnce("sounds/Game Over Yeah.wav");
						//	g_soundSystem.PlayOnce("sounds/Game Over Yeah.ogg");
		}

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
