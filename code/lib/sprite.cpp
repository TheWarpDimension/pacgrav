#include "lib/universal_include.h"

#include <gl/gl.h>
#include <gl/GLU.h>
#include "lib/png.h"
#include "lib/sprite.h"
#include "lib/tga.h"
#include "lib/window_manager.h"



Sprite::Sprite()
{
	m_width = 0;
	m_height = 0;
	m_textureId = 456456;
}


bool Sprite::Load(char const *filename, int rotateAmount)
{
	char const *ext = strrchr(filename, '.');
	if (!ext)
		return false;

	if (stricmp(ext, ".tga") == 0)
	{
		TgaImage tga;
		if (tga.Load(filename) != TgaImage::OK)
		{
			ReleaseAssert(false, "Couldn't open image file '%s'", filename);
			return false;
		}

		m_width = tga.m_width;
		m_height = tga.m_height;
		m_textureId = tga.ConvertToTexture();
	}
	else if (stricmp(ext, ".png") == 0)
	{
		PngImage png;
		if (png.Load(filename) != TgaImage::OK)
		{
			ReleaseAssert(false, "Couldn't open image file '%s'", filename);
			return false;
		}

		m_width = png.m_width;
		m_height = png.m_height;
		if (rotateAmount == 90)
		{
			png.FlipH();
		}
		else if (rotateAmount == 180)
		{
			png.FlipH();
			png.FlipV();
		}
		else if (rotateAmount == 270)
		{
			png.FlipV();
		}

		m_textureId = png.ConvertToTexture();
	}
	
	return true;
}


void Sprite::Render(int x, int y, Mode mode, int w, int h)
{
	if (m_textureId == 456456)
		return;

	int u1 = 0;
	int u2 = 1;
	int v1 = 0;
	int v2 = 1;

	switch (mode)
	{
	case ModeNormal:
		break;
	case ModeFlipH:
		u1 = 1;
		u2 = 0;
		break;
	case ModeRotate90:
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(0, 1, 0);
		glRotatef(-90, 0, 0, 1);
		break;
	case ModeRotate270:
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(1, 0, 0);
		glRotatef(90, 0, 0, 1);
		break;
	}

	glColor3ub(255,255,255);
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
 	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glBegin(GL_QUADS);
    // (u1,v1)
	//    +-----+
	//    |     |
	//    |     |
	//    +-----+
	// (u1,v2) (u2,v2)
	glTexCoord2i(u1, v1);		glVertex2i(x, y);
	glTexCoord2i(u1, v2);		glVertex2i(x, y + h);
	glTexCoord2i(u2, v2);		glVertex2i(x + w, y + h);
	glTexCoord2i(u2, v1);		glVertex2i(x + w, y);
	glEnd();
 	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	switch (mode)
	{
	case ModeRotate90:
	case ModeRotate270:
		glLoadIdentity();
		break;
	}
}
