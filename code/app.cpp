#include "lib/universal_include.h"

#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod.hpp"
#include "C:/Program Files (x86)/FMOD SoundSystem/FMOD Programmers API Windows/api/inc/fmod_errors.h"
#include "lib/SDL/SDL.h"
#include "lib/binary_stream_readers.h"
#include "lib/debug_utils.h"
#include "lib/drawing_primitives.h"
#include "lib/hi_res_time.h"
#include "lib/input.h"
#include "lib/rgb_colour.h"
#include "lib/text_renderer.h"
#include "lib/sprite.h"
#include "lib/window_manager.h"


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



//ERROR CODE START
void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		ReleaseAssert( false, "FMOD_MANAGER SAYS - FMOD HAS CRASHED, YOU BLAME BONCEY'S FMOD CODE OR YOU CAN BLAME YOURSELF IF YOU PUT A SOUND IN WRONG OR SOMETHING.");
	}
}













	////////////////////////////////////////////////
	//FMOD CODE DERIVED FROM BONCEY'S FMOD_MANAGER//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FMOD::System *bsystem;																					//
	FMOD_SPEAKERMODE speakerMode;																			//
	FMOD_CAPS caps;																							//
	FMOD::Sound      *spawn, *gameover, *title;																//
	FMOD::Channel    *channel = 0;																			//
	FMOD_RESULT       result;																				//
	int               key;																					//
	unsigned int      version;																				//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	Create a System object and initialize.
	*/
	
	void play(FMOD::Sound *s, FMOD::Channel *c) { FMOD_RESULT result = bsystem->playSound(FMOD_CHANNEL_FREE, s, false, &c); ERRCHECK(result); }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////














//ERROR CODE END

App::App()
{
}


void App::MainLoop()
{

	//
	result = FMOD::System_Create(&bsystem);
	ERRCHECK(result);

	result = bsystem->getVersion(&version);
	ERRCHECK(result);

	result = bsystem->init(32, FMOD_INIT_NORMAL, 0);
	ERRCHECK(result);
	/// PUT SOUNDS AFTER ME!
	result = bsystem->createSound("sounds/Spawn.wav", FMOD_HARDWARE, 0, &spawn);
	ERRCHECK(result);

	result = bsystem->createSound("sounds/GOY.wav", FMOD_HARDWARE, 0, &gameover);
	ERRCHECK(result);

	result = bsystem->createSound("sounds/Title.wav", FMOD_HARDWARE, 0, &title);
	ERRCHECK(result);

	result = spawn->setMode(FMOD_LOOP_OFF);
	ERRCHECK(result);  

	result = gameover->setMode(FMOD_LOOP_OFF);
	ERRCHECK(result);  

	result = title->setMode(FMOD_LOOP_OFF);
	ERRCHECK(result);  

	///
	play(spawn, channel);
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
		{
			if (g_debugMode == true)
				g_godMode = !g_godMode;
		}

		if (g_keyDowns[KEY_F5])
		{
			if (g_debugMode == true)
				g_slowmo = !g_slowmo;
		}

		if (g_keyDowns[KEY_P])
			g_paused = !g_paused;

		if (g_keyDowns[KEY_CONTROL, KEY_K])
		{
			if (g_debugMode == true)
				g_gameMode = ModePcmanDying;
		}
		//SOUNDTEST
		if (g_keyDowns[KEY_CONTROL, KEY_T])
		{
			if (g_debugMode == true)
			{
				play(spawn, channel);
				play(gameover,channel);
				play(title, channel);
			}
		}

		if (g_slowmo != 0)
			timeDelta *= 0.1;

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
			// Play get you ghosts sound

			// Render the NPCs
			if (g_gameMode == ModeInGame || g_gameMode == ModePcmanDying)
			{
				g_pcman.Render();
			}

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
				g_fixedFont.DrawText(200, 200, DEF_FONT_SIZE, "COMPLETED");

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
		}
		else
		{
			g_gameOverSprite.Render(0, 0, Sprite::ModeNormal, 760, 690);
		}
		if (g_gameMode == ModeGameOverScreen)
		{
			result = bsystem->playSound(FMOD_CHANNEL_FREE, gameover, false, &channel);
			ERRCHECK(result);
		}
		//if (g_gameMode == ModeInGame

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
