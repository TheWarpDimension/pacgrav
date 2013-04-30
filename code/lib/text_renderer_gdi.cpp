#include "lib/universal_include.h"

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/debug_utils.h"
#include "lib/drawing_primitives.h"
#include "lib/rgb_colour.h"
#include "lib/string_utils.h"
#include "lib/window_manager.h"

#include "text_renderer.h"


TextRenderer g_fixedFont;
TextRenderer g_propFont;


// ****************************************************************************
// Private Functions
// ****************************************************************************

void TextRenderer::SetupDrawingState()
{
	SelectObject(g_windowManager->m_memDC, m_fontHandle); 
}


void TextRenderer::ClearDrawingState()
{
}


// ****************************************************************************
// Public Functions
// ****************************************************************************

void TextRenderer::Initialise(char const *fontName, int size)
{
	int scaledSize = -MulDiv(size, GetDeviceCaps(g_windowManager->m_memDC, LOGPIXELSY), 72);

 	m_fontHandle = CreateFont(scaledSize, 0, 0, 0, FW_NORMAL, false, false, false, 
 							  ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
 							  DEFAULT_QUALITY, DEFAULT_PITCH,
 							  fontName);
	ReleaseAssert(m_fontHandle != NULL, "Couldn't find Windows font '%s'", fontName);

	RefreshHandles();
	
	TEXTMETRIC textMetrics;
	GetTextMetrics(g_windowManager->m_memDC, &textMetrics);
	m_charHeight = textMetrics.tmHeight;
	m_avgCharWidth = textMetrics.tmAveCharWidth;
	if (textMetrics.tmAveCharWidth == textMetrics.tmMaxCharWidth)
		m_fixedWidth = true;
	else
		m_fixedWidth = false;
}


void TextRenderer::RefreshHandles()
{
	SetupDrawingState();
	SetBkMode(g_windowManager->m_memDC, OPAQUE);
}


void TextRenderer::FastBegin()
{
	m_inFastSection = true;
	SetupDrawingState();
}


void TextRenderer::FastEnd()
{
	m_inFastSection = false;
	ClearDrawingState();
}


int TextRenderer::DrawChar(float x, float y, char c)
{
	if (!m_inFastSection) SetupDrawingState();

	char buf[] = {c};
	int width;
	if (m_fixedWidth)
	{
		width = m_avgCharWidth;
	}
	else
	{
		SIZE size;
		GetTextExtentPoint32(g_windowManager->m_memDC, buf, 1, &size);
		width = size.cx;
	}
	
	TextOut(g_windowManager->m_memDC, x, y, buf, 1);

	if (!m_inFastSection) ClearDrawingState();

	return width;
}


int TextRenderer::DrawTextLowLevel(float x, float y, char const *text, int numCharsToDraw)
{
    bool res = TextOut(g_windowManager->m_memDC, x, y, text, numCharsToDraw);

    int width;
    if (m_fixedWidth)
    {
        width = m_avgCharWidth * numCharsToDraw;
    }
    else
    {
        SIZE size;
        GetTextExtentPoint32(g_windowManager->m_memDC, text, numCharsToDraw, &size);
        width = size.cx;
    }
    
    return width;
}


int TextRenderer::DrawTextSimple(float x, float y, char const *text, int maxChars)
{
    if (!m_inFastSection) SetupDrawingState();
    
    char const *c = text;
    int width = 0;
    while (maxChars && *c != '\0')
    {
        // Find the first unprintable char, if there are any
        int fragLen = 0;
        while (fragLen < maxChars)
        {
            if ((unsigned)c[fragLen] < 32)
                break;
            fragLen++;
        }
        
        width += DrawTextLowLevel(x + width, y, c, fragLen);
        maxChars -= fragLen;
        c += fragLen;
        
        while (*c > 0 && *c < 32 && maxChars > 0)
        {
            DrawOutlineBox(x + width + 1, y + 1, m_avgCharWidth - 2, m_charHeight - 2, RGBAColour(178,178,178));
            width += m_avgCharWidth;
            c++;
            maxChars--;
        }
    }
    
    if (!m_inFastSection) ClearDrawingState();
    
    return width;
}


int TextRenderer::DrawTextSimpleWithNulls(float x, float y, char const *text, int len)
{
    if (!m_inFastSection) SetupDrawingState();
    
    char const *c = text;
    int width = 0;
    while (len)
    {
        // Find the first unprintable char, if there are any
        int fragLen = 0;
        while (fragLen < len)
        {
            if ((unsigned)c[fragLen] < 32)
                break;
            fragLen++;
        }
        
        width += DrawTextLowLevel(x + width, y, c, fragLen);
        len -= fragLen;
        c += fragLen;
        
        while (*c < 32 && len > 0)
        {
            DrawOutlineBox(x + width + 1, y + 1, m_avgCharWidth - 2, m_charHeight - 2, RGBAColour(178,178,178));
            width += m_avgCharWidth;
            c++;
            len--;
        }
    }
    
    if (!m_inFastSection) ClearDrawingState();
    
    return width;
}


int TextRenderer::DrawText( float _x, float _y, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);
    return DrawTextSimple( _x, _y, buf );
}


int TextRenderer::DrawTextRight( float _x, float _y, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);

    float width = GetTextWidth(buf);
    return DrawTextSimple( _x - width, _y, buf );
}


int TextRenderer::DrawTextCentre( float _x, float _y, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);

    float width = GetTextWidth(buf);
    return DrawTextSimple( _x - width/2, _y, buf );
}


int TextRenderer::GetTextWidth(char const *text, int len)
{
	len = min(strlen(text), len);
	if (m_fixedWidth)
	{
		return len * m_avgCharWidth;
	}
	else
	{
        SIZE size = { 0, 0 };
		if (!m_inFastSection) SetupDrawingState();
        
        char const *c = text;
        int width = 0;
        while (len)
        {
            // Find the first unprintable char, if there are any
            int fragLen = 0;
            while (fragLen < len)
            {
                if ((unsigned)c[fragLen] < 32)
                    break;
                fragLen++;
            }
            
            GetTextExtentPoint32(g_windowManager->m_memDC, c, fragLen, &size);
            width += size.cx;
            len -= fragLen;
            c += fragLen;
            
            while (*c < 32 && len > 0)
            {
                width += m_avgCharWidth;
                c++;
                len--;
            }
        }

		if (!m_inFastSection) ClearDrawingState();

		return width;
	}
}


void SetTextColour(RGBAColour const &c)
{
    ::SetTextColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
}
