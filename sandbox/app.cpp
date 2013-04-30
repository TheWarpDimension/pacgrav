// We need to #include a load of things that Andrew wrote, so we can open a window and stuff
#include "lib/universal_include.h"

#include <math.h>
#include <stdio.h>

#include <gl/gl.h>

#include "lib/debug_utils.h"
#include "lib/drawing_primitives.h"
#include "lib/hi_res_time.h"
#include "lib/input.h"
#include "lib/rgb_colour.h"
#include "lib/text_renderer.h"
#include "lib/sprite.h"
#include "lib/window_manager.h"

#include "app.h"


// Ignore this bit
App *g_app = NULL;
App::App()
{
}

// And explanation of the Heap and the Stack.
// 
// Func A calls B and B calls C and then B calls D
// 
// Func A creates an int called a1
// Func A calls B
// Func B creates an int called b1
// Func B calls C
// Func C creates an int called c1
// Func C returns - program flow goes back to func B
// Func B calls func D
// Func D creates an int called d1
// 
// void funcA()
// {
// 	int a1;
// 	funcB();
// }
// 
// void funcB()
// {
// 	int b1;
// 	funcC();
// 	funcD();
// }
//
// C allocates all these variables on the "stack".
// The stack is just like an big array
// Func asks for enough "stack space" to store a1.
//
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13
// <-a1--> <-b1--> <-d1-->
//
// The stack is only big enough for about 10000 ints
// To get at all the rest of the memory you need to use the "heap".
// The heap is basically the rest of memory. But unlike the nicely
// organised stack, the heap is a unorganised and you need to keep
// track of where all the variables are yourself. That is hard.

// This function will be called once we've opened a window, see below.
void DoStarField()
{
	const int numstars = 2000;			// Numstars = Number Of Stars
	// Create variables x,y,c
	float x[numstars];
	float y[numstars];
	int c[numstars];

	// Firstsequence = initialise values to x,y,c in a quantity of numstars 
	// Velocity is determined by the brightness/colour of the star (distance)
	for (int firstsequence = 0; firstsequence < numstars; firstsequence++)
	{
		// % is the remainder operator
		x[firstsequence] = rand() % 1920;
		y[firstsequence] = rand() % 1080;
		c[firstsequence] = rand() % (255-8) + 8;
	}

	// Create a variable to store the time. This will be used by the
	// FPS meter. Double is a 64-bit float.
	double startTime = GetHighResTime();

	// Loop forever. Each iteration of this loop draws one frame
	while (!g_keys[KEY_ESC] && !g_inputManager->m_windowClosed)
	{
		// Read user input
		g_inputManager->Advance();
		g_inputManager->PollForMessages();

		// Clear background to black
		DrawFilledBox(0, 0, g_windowManager->m_width, g_windowManager->m_height, 0 ,0, 0);

		// Calculate the FPS
		double endTime = GetHighResTime();
		double frameTime = endTime - startTime;
		startTime = endTime;

		// Calculate "movement factor" for this frame, based on the FPS
		
		
		float movementFactor = 0.025 * 60.0 * frameTime;

		for (int star =0; star < numstars; star++)
		{
			// converts the c to a floating point number rather than int
			float temp = c[star];
			
			// Move the star along
			x[star] = x[star] - (temp * movementFactor);
			
			DrawFilledBox(x[star], y[star], 2, 2, c[star], c[star], c[star]);
			
			// Test if star has gone off the side of the screen
			if (x[star] <=0)
			{
				x[star]=1920;
				y[star] =  rand () % 700;
				c[star] =  rand () % (255-64) + 64;
			}
		}

		// Draw the FPS meter
		g_fixedFont.DrawText(4, 2, 14, "FPS = %.1f", 1.0 / (frameTime));

		// Transfer the back buffer to the screen and go to sleep 
		// for 1ms
		g_windowManager->Flip();
		SleepEx(1, true);
	}
}


// This is where your program starts
void AppMain()
{
	// We always need to create an "app". This is just the way I've written my library code
	g_app = new App();

	// We always need to create a window manager if we want a window
	g_windowManager = new WindowManager;

	// Now tell the windowmanager to open a window
	g_windowManager->CreateWin(100, 100, 800, 600, false);

	// Create an InputManager that we will use to get keyboard and mouse input from the user
	g_inputManager = new InputManager;

	// Fonts
	g_fixedFont.Initialise("../bitmaps/speccy_font.png");
 
	DoStarField();
}
