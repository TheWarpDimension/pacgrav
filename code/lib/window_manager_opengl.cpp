#include "lib/universal_include.h"

#ifdef ENABLE_OPENGL

#include <limits.h>
#include <gl/glu.h>
#include <windows.h>
#include <stdlib.h>	// For exit()

#include "lib/input.h"
#include "lib/math_utils.h"
#include "lib/vertical_sync.h"
#include "lib/window_manager_opengl.h"

#include "app.h"

static HINSTANCE g_hInstance;

#define WH_KEYBOARD_LL 13


WindowManager *g_windowManager = NULL;

// Create pointers that will be initialized to point to member variables inside
// the window manager object when it is constructed.
static int *s_left = NULL;
static int *s_top = NULL;
static unsigned int *s_width = NULL;
static unsigned int *s_height = NULL;


#define FULLSCREEN_WINDOW_STYLE (WS_POPUPWINDOW | WS_VISIBLE)
#define WINDOWED_WINDOW_STYLE (FULLSCREEN_WINDOW_STYLE | (WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX))


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
 		case WM_SETCURSOR:
// 			if (g_windowManager->IsMouseInWindow())
// 			{
// 				// Ignore set cursor messages, otherwise Windows will restore the
// 				// cursor to the arrow bitmap every time the mouse moves
// 				return 0;
// 			}
// 			else
// 			{
				return DefWindowProc(hWnd, message, wParam, lParam);
//			}	
			
			
		case WM_MOVE:
			if (wParam == 0)
			{
				*s_left = lParam & 0xffff;
				*s_left -= g_windowManager->GetBorderWidth();
				*s_top = lParam >> 16;
				*s_top -= g_windowManager->GetBorderWidth() +
						  g_windowManager->GetTitleHeight();
			}
			break;

		case WM_NCCALCSIZE:
		{
			bool maxed = g_windowManager->IsWindowMaximised();
			double aspectRatio = double(*s_width) / double(*s_height);
			if (wParam == 0)
			{
				// This branch is taken on startup
				RECT *rect = (RECT *)lParam;
				*s_width = rect->right - rect->left;
				*s_height = rect->bottom - rect->top;
				aspectRatio = double(*s_width) / double(*s_height);
			}
			else
			{
				// This branch is taken otherwise
				NCCALCSIZE_PARAMS *csp = (NCCALCSIZE_PARAMS*)lParam;
				*s_width = csp->lppos->cx;
				*s_height = csp->lppos->cy;
			}
			*s_width -= 2 * g_windowManager->GetBorderWidth();
			*s_height -= 2 * g_windowManager->GetBorderWidth();
			*s_height -= g_windowManager->GetTitleHeight();

			// Attempt to preserve the old aspect ratio
			double newAspectRatio = double(*s_width) / double(*s_height);
			double xOffset = 0;
			double yOffset = 0;
			if (newAspectRatio > aspectRatio)
			{
				int oldW = *s_width;
				*s_width = *s_height * aspectRatio;
				xOffset = (oldW - *s_width) / 2;
			}
			else
			{
				int oldH = *s_height;
				*s_height = *s_width * aspectRatio;
				yOffset = (oldH - *s_height) / 2;
			}
			
			glViewport(xOffset, yOffset, *s_width, *s_height);
			return DefWindowProc( hWnd, message, wParam, lParam );
			break;
		}

		case WM_CLOSE:
			g_inputManager->m_windowClosed = true;
			return 0;

		default:
			if (!g_inputManager || g_inputManager->EventHandler(message, wParam, lParam) == -1)
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
	}

	return 0;
}


WindowManager::WindowManager()
:	m_left(0),
	m_top(0),
	m_width(0),
	m_height(0)
{
	s_left = &m_left;
	s_top = &m_top;
	s_width = &m_width;
	s_height = &m_height;
}


WindowManager::~WindowManager()
{
	DisableOpenGL();
	DestroyWindow(m_hWnd);
}


void WindowManager::SetClipRectangle(int x, int y, int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int top = SCREEN_H - y;
	glViewport(x, top-h, w, h);
	gluOrtho2D(x, x + w, y+h, y);
}


void WindowManager::EnableOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// Get the device context (DC)
	m_memDC = GetDC( m_hWnd );
	
	// Set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 0;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( m_memDC, &pfd );
	SetPixelFormat( m_memDC, format, &pfd );
	
	// Create and enable the render context (RC)
	m_hRC = wglCreateContext( m_memDC );
	wglMakeCurrent( m_memDC, m_hRC );

	glDisable	(GL_TEXTURE_2D);
    glDisable   (GL_CULL_FACE);
    glDisable   (GL_DEPTH_TEST);
    glDepthMask (false);
	glDisable	(GL_BLEND);

    SetClipRectangle(0, 0, m_width, m_height);
}


void WindowManager::DisableOpenGL()
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( m_hRC );
	ReleaseDC( m_hWnd, m_memDC );
}


void WindowManager::CreateWin(int x, int y, int width, int height, bool _windowed)
{
	clamp(width, 320, 3000);
	clamp(height, 240, 2300);
	m_width = width;
	m_height = height;

	// Register window class
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = APPLICATION_NAME;
	RegisterClass( &wc );

	unsigned int windowStyle = FULLSCREEN_WINDOW_STYLE;

	if (_windowed)
	{
		windowStyle = WINDOWED_WINDOW_STYLE;

		RECT windowRect = { 0, 0, width, height };
		AdjustWindowRect(&windowRect, windowStyle, false);
		m_borderWidth = ((windowRect.right - windowRect.left) - width) / 2;
		m_titleHeight = ((windowRect.bottom - windowRect.top) - height) - m_borderWidth * 2;
		width += m_borderWidth * 2;
		height += m_borderWidth * 2 + m_titleHeight;

		HWND desktopWindow = GetDesktopWindow();
		RECT desktopRect;
		GetWindowRect(desktopWindow, &desktopRect);
		clamp(x, 0, desktopRect.right);
		clamp(y, 0, desktopRect.bottom);
	}
	else
	{
		DEVMODE devmode;
		devmode.dmSize = sizeof(DEVMODE);
		devmode.dmBitsPerPel = 32;
		devmode.dmPelsWidth = width;
		devmode.dmPelsHeight = height;
		devmode.dmDisplayFrequency = 60;
		devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
		ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
		x = 0;
		y = 0;
		m_borderWidth = 1;
		m_titleHeight = 0;
	}

	// Create main window
	m_hWnd = CreateWindow( 
		wc.lpszClassName, wc.lpszClassName, 
		windowStyle,
		x, y, width, height,
		NULL, NULL, 0/*g_hInstance*/, NULL );

	// Set the window's title bar icon
	HANDLE hIcon = LoadImageA(g_hInstance, "#101", IMAGE_ICON, 16, 16, 0);
    if(hIcon)
    {
        SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }

	// Enable OpenGL for the window
	EnableOpenGL();

	vsyncSet(true);
}


void WindowManager::Flip()
{
	SwapBuffers(m_memDC);
}


void WindowManager::Flush()
{
	glFinish();
}


void WindowManager::NastyPollForMessages()
{
	MSG msg;
	while ( PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE) ) 
	{
		// handle or dispatch messages
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	} 
}


void WindowManager::CaptureMouse()
{
	SetCapture(m_hWnd);
}


void WindowManager::UncaptureMouse()
{
	ReleaseCapture();
}


bool WindowManager::IsMouseInWindow()
{
	POINT p;
	GetCursorPos(&p);

	RECT r;
	GetWindowRect(m_hWnd, &r);

	// Compensate for window border and title bar
	r.left += m_borderWidth;
	r.right -= m_borderWidth;
	r.top += m_borderWidth + m_titleHeight;
	r.bottom -= m_borderWidth;

	if (p.x < r.left)	return false;
	if (p.y < r.top)	return false;
	if (p.x > r.right)	return false;
	if (p.y > r.bottom) return false;

	return true;
}


bool WindowManager::IsWindowMaximised()
{
	WINDOWPLACEMENT winPlacement;
	winPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(m_hWnd, &winPlacement);
	return winPlacement.showCmd == SW_SHOWMAXIMIZED;
}


void WindowManager::SetMaximisedState(bool maximise)
{
	if (maximise)
	{
		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}
	else
	{
		ShowWindow(m_hWnd, SW_RESTORE);
	}
}


void WindowManager::ShowMouse()
{
	ShowCursor(true);
}


void WindowManager::HideMouse()
{
	ShowCursor(false);
}


void WindowManager::SetWindowTitle(char const *title)
{
	SetWindowText(m_hWnd, title);
}


void AppMain();

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, 
				   LPSTR _cmdLine, int _iCmdShow)
{
	g_hInstance = _hInstance;
	
	g_windowManager	= new WindowManager();

	AppMain();
	
	return WM_QUIT;
}

#endif // ENABLE_OPENGL
