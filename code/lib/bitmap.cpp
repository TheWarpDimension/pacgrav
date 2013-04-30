#include "lib/universal_include.h"

#include <gl/gl.h>
#include <gl/glu.h>

#include "lib/bitmap.h"
#include "lib/debug_utils.h"
#include "lib/filesys_utils.h"
#include "lib/math_utils.h"
#include "lib/rgb_colour.h"


// ****************************************************************************
// Public Functions
// ****************************************************************************

BitmapRGBA::BitmapRGBA()
:	m_width(-1),
	m_height(-1),
	m_pixels(NULL),
	m_lines(NULL)
{
}


BitmapRGBA::BitmapRGBA(BitmapRGBA const &_other)
{
	Initialise(_other.m_width, _other.m_height);
	memcpy(m_pixels, _other.m_pixels, m_width * m_height * sizeof(RGBAColour));
}


BitmapRGBA::BitmapRGBA(int width, int height)
:	m_width(-1),
	m_height(-1),
	m_pixels(NULL),
	m_lines(NULL)
{
	Initialise(width, height);
}


BitmapRGBA::~BitmapRGBA()
{
	m_width = -1;
	m_height = -1;
	delete [] m_pixels;
	delete [] m_lines;
	m_pixels = NULL;
	m_lines = NULL;
}


void BitmapRGBA::Initialise(int width, int height)
{
	ReleaseAssert(width % 16 == 0, "Bitmap width is not a multiple of 16");
	m_width = width;
	m_height = height;
	m_pixels = new RGBAColour[width * height];
	m_lines = new RGBAColour *[height];

	DebugAssert(m_pixels && m_lines);

	for (int y = 0; y < height; ++y)
	{
		m_lines[y] = &m_pixels[y * width];
	}
}
	

void BitmapRGBA::PutPixel(int x, int y, RGBAColour const &colour)
{
	m_lines[y][x] = colour;
}


RGBAColour const &BitmapRGBA::GetPixel(int x, int y) const
{
	return m_lines[y][x];
}


void BitmapRGBA::PutPixelClipped(int x, int y, RGBAColour const &colour)
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height)
	{
		return;
	}

	m_lines[y][x] = colour;
}


RGBAColour const &BitmapRGBA::GetPixelClipped(int x, int y) const
{
	if (x < 0 || x >= m_width ||
		y < 0 || y >= m_height)
	{
		return g_colourBlack;
	}

	return m_lines[y][x]; 
}


void BitmapRGBA::DrawLine(int x1, int y1, int x2, int y2, RGBAColour const &colour)
{
    int numSteps = sqrtf( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    float dx = float(x2-x1) / (float) numSteps;
    float dy = float(y2-y1) / (float) numSteps;

    float currentX = x1;
    float currentY = y1;

    for( int i = 0; i < numSteps; ++i )
    {
        PutPixelClipped( currentX, currentY, colour );

        currentX += dx;
        currentY += dy;
    }
}


RGBAColour BitmapRGBA::GetInterpolatedPixel(float _x, float _y) const
{
	int x1 = floorf(_x);
	int y1 = floorf(_y);
	int x2 = ceilf(_x);
	int y2 = ceilf(_y);

	float fractionalX = _x - x1;
	float fractionalY = _y - y1;

	float weight11 = (1.0f - fractionalX) * (1.0f - fractionalY);
	float weight12 = (1.0f - fractionalX) * (fractionalY);
	float weight21 = (fractionalX) * (1.0f - fractionalY);
	float weight22 = (fractionalX) * (fractionalY);

    RGBAColour const &c11 = GetPixelClipped(x1, y1); 
    RGBAColour const &c12 = GetPixelClipped(x1, y2); 
    RGBAColour const &c21 = GetPixelClipped(x2, y1); 
    RGBAColour const &c22 = GetPixelClipped(x2, y2); 

	RGBAColour returnVal((float)c11.r * weight11 + (float)c12.r * weight12 + (float)c21.r * weight21 + (float)c22.r * weight22,
						 (float)c11.g * weight11 + (float)c12.g * weight12 + (float)c21.g * weight21 + (float)c22.g * weight22,
						 (float)c11.b * weight11 + (float)c12.b * weight12 + (float)c21.b * weight21 + (float)c22.b * weight22,
						 (float)c11.a * weight11 + (float)c12.a * weight12 + (float)c21.a * weight21 + (float)c22.a * weight22);

	return returnVal;
}


void BitmapRGBA::Blit(int _srcX, int _srcY, int _srcW, int _srcH, BitmapRGBA *_srcBmp, 
					  int _destX, int _destY, int _destW, int _destH)
{
	DebugAssert(_srcX + _srcW <= _srcBmp->m_width);
	DebugAssert(_srcY + _srcH <= _srcBmp->m_height);
	DebugAssert(_destX + _destW <= m_width);
	DebugAssert(_destY + _destH <= m_height);

	float sxPitch = (float)_srcW / (float)_destW;
	float syPitch = (float)_srcH / (float)_destH;

	float sy = _srcY;
	for (int dy = _destY; dy < _destY + _destH; ++dy)
	{
		float sx = _srcX;
		for (int dx = _destX; dx < _destX + _destW; ++dx)
		{
			RGBAColour const &c = _srcBmp->GetInterpolatedPixel(sx, sy);
//			RGBAColour const &c = _srcBmp->GetPixel(sx, sy);
			PutPixel(dx, dy, c);
			sx += sxPitch;
		}
		sy += syPitch;
	}
}


void BitmapRGBA::ConvertPinkToTransparent() 
{
	for (int y = 0; y < m_height; ++y) 
	{
		for (int x = 0; x < m_width; ++x) 
		{
			RGBAColour const c(GetPixel(x, y));
			if (c == RGBAColour(255,0,255))
			{
				PutPixel(x, y, RGBAColour(128,128,128,0));
			}
		}
	}
}


void BitmapRGBA::Clear( RGBAColour const &colour )
{
	for (int x = 0; x < m_width; ++x) 
	{
        PutPixel( x, 0, colour );
    }

	int const size = sizeof(RGBAColour) * m_width;
 	for (int y = 0; y < m_height; ++y) 
	{
		memcpy(m_lines[y], m_lines[0], size);
    }
}


int BitmapRGBA::ConvertToTexture(bool _mipmapping) const
{
	unsigned int texId;

	glEnable        (GL_TEXTURE_2D);

	glGenTextures	(1, &texId);
	glBindTexture	(GL_TEXTURE_2D, texId);
    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	
//    bool texturingWasEnabled = glIsEnabled(GL_TEXTURE_2D);

	glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri	(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	
    if (_mipmapping)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 
						  m_width, m_height, 
						  GL_RGBA, GL_UNSIGNED_BYTE, 
						  m_pixels);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels);
	}

//    if (!texturingWasEnabled)
//    {
//        glDisable(GL_TEXTURE_2D);
//    }

	return (int)texId;
}


void BitmapRGBA::FlipH()
{
	for (int y = 0; y < m_height; y++)
	{
		RGBAColour *line = m_lines[y];
		int halfWidth = m_width / 2;
		for (int x = 0; x < m_width / 2; x++)
		{
			RGBAColour c = line[x];
			line[x] = line[m_width - x - 1];
			line[m_width - x - 1] = c;
		}
	}
}


void BitmapRGBA::FlipV()
{
	int halfHeight = m_height / 2;

	for (int y = 0; y < halfHeight; y++)
	{
		RGBAColour *line1 = m_lines[y];
		RGBAColour *line2 = m_lines[m_height - y - 1];
		for (int x = 0; x < m_width; x++)
		{
			RGBAColour c = line1[x];
			line1[x] = line2[x];
			line2[x] = c;
		}
	}
}
