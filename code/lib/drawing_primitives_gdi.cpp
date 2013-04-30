#include "lib/universal_include.h"

#ifndef ENABLE_OPENGL

#include "lib/bitmap.h"
#include "lib/drawing_primitives.h"
#include "lib/rgb_colour.h"
#include "lib/window_manager.h"


void PutPixel(int x, int y, RGBAColour const &c)
{
	SetPixel(g_windowManager->m_memDC, x, y, RGB(c.r, c.g, c.b));
}


void DrawLine(int x1, int y1, int x2, int y2, RGBAColour const &c)
{
    SetDCPenColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
    SelectObject(g_windowManager->m_memDC, GetStockObject(DC_PEN));
    MoveToEx(g_windowManager->m_memDC, x1, y1, NULL);
    LineTo(g_windowManager->m_memDC, x2, y2);
}


void DrawHLine(int x1, int y1, int w, RGBAColour const &c)
{
	// If some of this fails to compile, just delete the #if surrounding
	// the necessary definitions in wingdi.h
	SetDCPenColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
	SelectObject(g_windowManager->m_memDC, GetStockObject(DC_PEN));
	MoveToEx(g_windowManager->m_memDC, x1, y1, NULL);
	LineTo(g_windowManager->m_memDC, x1 + w, y1);
}


void DrawVLine(int x1, int y1, int h, RGBAColour const &c)
{
	SetDCPenColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
	SelectObject(g_windowManager->m_memDC, GetStockObject(DC_PEN));
	MoveToEx(g_windowManager->m_memDC, x1, y1, NULL);
	LineTo(g_windowManager->m_memDC, x1, y1 + h);
}


void DrawOutlineBox(int x1, int y1, int w, int h, RGBAColour const &c)
{
	SetDCPenColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
	SelectObject(g_windowManager->m_memDC, GetStockObject(DC_PEN));
	MoveToEx(g_windowManager->m_memDC, x1, y1, NULL);

	LineTo(g_windowManager->m_memDC, x1+w-1, y1);
	LineTo(g_windowManager->m_memDC, x1+w-1, y1+h-1);
	LineTo(g_windowManager->m_memDC, x1, y1+h-1);
	LineTo(g_windowManager->m_memDC, x1, y1);
}


void DrawHatchedBox(int x1, int y1, int w, int h, RGBAColour const &c)
{
    COLORREF col = RGB(c.r, c.g, c.b);

    for (int y = 0; y < h; ++y)
    {
        for (int x = y & 1; x < w; x += 2)
        {
            SetPixel(g_windowManager->m_memDC, x1 + x, y1 + y, col);
        }
    }
}


void DrawFilledBox(int x1, int y1, int w, int h, RGBAColour const &c)
{
//	SetDCBrushColor(g_windowManager->m_memDC, RGB(c.r, c.g, c.b));
//	HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);
	RECT rect;
	rect.left = x1;
	rect.right = x1 + w;
	rect.top = y1;
	rect.bottom = y1 + h;
//	FillRect(g_windowManager->m_memDC, &rect, brush);
//	COLORREF c2 = RGB(c.r, c.g, c.b);
	COLORREF c2 = *((COLORREF*)&c) & 0xffffff;
	SetBkColor(g_windowManager->m_memDC, c2);
    ExtTextOut(g_windowManager->m_memDC, 0, 0, ETO_OPAQUE, &rect, "", 0, 0);
//    SetBkColor(g_windowManager->m_memDC, oldcr);
}


void DrawTexturedRect(int x1, int y1, int w, int h, int textureId)
{
	BitmapRGBA *bmp = (BitmapRGBA*)textureId;
	for (int y = 0; y < bmp->m_height; ++y)
	{
		RGBAColour *line = bmp->m_lines[y];
		for (int x = 0; x < bmp->m_width; ++x)
		{
			RGBAColour *pixel = line + x;
			SetPixel(g_windowManager->m_memDC, x1 + x, y1, RGB(pixel->r, pixel->g, pixel->b));
		}
		y1++;
	}
}

#endif // ENABLE_OPENGL
