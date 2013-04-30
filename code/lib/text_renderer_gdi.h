#ifndef TEXT_RENDERER_GDI_H
#define TEXT_RENDERER_GDI_H


#include "lib/rgb_colour.h"


// ****************************************************************************
//  Class TextRenderer
// ****************************************************************************

class TextRenderer
{
protected:
	HFONT		m_fontHandle;
	bool		m_inFastSection;
	bool		m_fixedWidth;
	int			m_avgCharWidth;
	int			m_charHeight;	// Height of characters, in pixels.

	void SetupDrawingState	();
	void ClearDrawingState	();

    int DrawTextLowLevel   (float x, float y, char const *text, int numChars);

public:
	void Initialise			(char const *_fontname, int size);
	void RefreshHandles		();	// Call this when a new device context has been created

	void FastBegin			();
	void FastEnd			();

	int DrawChar			(float x, float y, char c);
    int DrawTextSimple		(float x, float y, char const *text, int maxChars = 9999);	// Returns text length in pixels
    int DrawTextSimpleWithNulls(float x, float y, char const *text, int maxChars);	// Returns text length in pixels
    int DrawText			(float x, float y, char const *text, ...);	// Like simple but with variable args
    int DrawTextRight		(float x, float y, char const *text, ...);	// Like above but with right justify
    int DrawTextCentre		(float x, float y, char const *text, ...);	// Like above but with centre justify
	
	int	 GetTextHeight		() { return m_charHeight; }
    int	 GetTextWidth		(char const *text, int len=999999);
    int  GetAvgCharWidth    () { return m_avgCharWidth; }
};


void SetTextColour(RGBAColour const &c);


extern TextRenderer g_fixedFont;
extern TextRenderer g_propFont;


#endif
