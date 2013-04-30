#ifndef INCLUDED_DRAWING_PRIMITIVES
#define INCLUDED_DRAWING_PRIMITIVES

class RGBAColour;

void SetTextColour(RGBAColour const &);

//void PutPixel(int x, int y, RGBAColour const &c);

void DrawLine(int x1, int y1, int x2, int y2, RGBAColour const &c);
void DrawHLine(int x1, int y1, int w, RGBAColour const &c);
void DrawVLine(int x1, int y1, int h, RGBAColour const &c);

void DrawOutlineBox(int x1, int y1, int w, int h, RGBAColour const &c);
void DrawHatchedBox(int x1, int y1, int w, int h, RGBAColour const &c);
void DrawFilledBox(int x1, int y1, int w, int h, 
				   unsigned char r, unsigned char g, unsigned char b);

void DrawTexturedRect(int x1, int y1, int w, int h, int textureId);

#endif
