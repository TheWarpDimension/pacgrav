#ifndef INCLUDED_WINDOW_MANAGER_H
#define INCLUDED_WINDOW_MANAGER_H


#define SCREEN_X (g_windowManager->GetLeft())
#define SCREEN_Y (g_windowManager->GetTop())
#define SCREEN_W (g_windowManager->GetWidth())
#define SCREEN_H (g_windowManager->GetHeight())



class WindowManager
{
public:
	HWND				m_hWnd;
	HDC					m_memDC;
    HGLRC               m_hRC;

	int		            m_left;
	int		            m_top;
	unsigned int		m_width;
	unsigned int		m_height;

	int					m_borderWidth;
	int					m_titleHeight;

	void EnableOpenGL();
	void DisableOpenGL();
	
public:
	WindowManager();
	~WindowManager();

	void SetClipRectangle(int x1, int y1, int w, int h);

	void CreateWin(int x, int y, int _width, int _height, bool _windowed);
	
	inline unsigned int GetLeft() { return m_left; }
	inline unsigned int GetTop() { return m_top; }
	inline unsigned int GetWidth() { return m_width; }
	inline unsigned int GetHeight() { return m_height; }
	inline unsigned int GetBorderWidth() { return m_borderWidth; }
	inline unsigned int GetTitleHeight() { return m_titleHeight; }

	void Flip();
	void Flush();
	
	void NastyPollForMessages();
	
	void CaptureMouse();
	void UncaptureMouse();
	bool IsMouseInWindow();

	bool IsWindowMaximised();
	void SetMaximisedState(bool maximise);

	void ShowMouse();
	void HideMouse();

	void SetWindowTitle(char const *title);
};


extern WindowManager *g_windowManager;


#endif
