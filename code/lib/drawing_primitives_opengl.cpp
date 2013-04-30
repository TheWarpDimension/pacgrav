#include "lib/universal_include.h"

#ifdef ENABLE_OPENGL

#include <gl/gl.h>
#include "lib/drawing_primitives.h"
#include "lib/rgb_colour.h"


static void SetColour(RGBAColour const &c)
{
	if (c.a != 255)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glColor4ubv(c.GetData());
}


void DrawHLine(int x1, int y1, int w, RGBAColour const &c)
{
	SetColour(c);

	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x1 + w - 1, y1);
	glEnd();

	glDisable(GL_BLEND);
}


void DrawVLine(int x1, int y1, int h, RGBAColour const &c)
{
	SetColour(c);

	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x1, y1 + h - 1);
	glEnd();

	glDisable(GL_BLEND);
}


void DrawOutlineBox(int x1, int y1, int w, int h, RGBAColour const &c)
{
	SetColour(c);

	y1++;
	int x2 = x1 + w - 1;
	int y2 = y1 + h - 1;

	glLineWidth(1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_BLEND);
}


void DrawFilledBox(int x1, int y1, int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
 	SetColour(RGBAColour(r, g, b));

	int x2 = x1 + w;
	int y2 = y1 + h;

	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
	glEnd();

	glDisable(GL_BLEND);
}


void DrawTexturedRect(int x1, int y1, int w, int h, int textureId)
{
	glColor3ub(255,255,255);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 1);		glVertex2i(x1, y1);
		glTexCoord2i(0, 0);		glVertex2i(x1, y1 + h);
		glTexCoord2i(1, 0);		glVertex2i(x1 + w, y1 + h);
		glTexCoord2i(1, 1);		glVertex2i(x1 + w, y1);
	glEnd();
    glDisable(GL_TEXTURE_2D);
}


#endif // ENABLE_OPENGL