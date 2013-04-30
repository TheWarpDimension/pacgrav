#include "lib/universal_include.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "lib/binary_stream_readers.h"
#include "lib/png.h"
#include "lib/debug_utils.h"
#include "lib/filesys_utils.h"

#include "app.h"
#include "text_renderer.h"


TextRenderer g_fixedFont;
TextRenderer g_propFont;


// Horizontal size as a proportion of vertical size
#define HORIZONTAL_SIZE		0.6f

#define TEX_MARGIN			0.003f
#define TEX_STRETCH			(1.0f - (26.0f * TEX_MARGIN))

#define TEX_WIDTH			((1.0f / 16.0f) * TEX_STRETCH * 0.9f)
#define TEX_HEIGHT			((1.0f / 14.0f) * TEX_STRETCH)


// ****************************************************************************
//  Class TextRenderer
// ****************************************************************************

void TextRenderer::Initialise(char const *_filename)
{
	m_filename = strdup(_filename);
	BuildOpenGlState();
    m_renderShadow = false;
    m_renderOutline = false;
}


void TextRenderer::BuildOpenGlState()
{
	PngImage img;
	if (img.Load(m_filename) < 0)
		ReleaseAssert(false, "Couldn't load font file '%s'", m_filename);
	
	m_bitmapWidth = img.m_width * 2;
	m_bitmapHeight = img.m_height * 2;

//	BitmapRGBA scaledUp(m_bitmapWidth, m_bitmapHeight);
//	scaledUp.Blit(0, 0, img.m_width, img.m_height, &img, 0, 0, scaledUp.m_width, scaledUp.m_height, false);
	m_textureId = img.ConvertToTexture();
}


void TextRenderer::BeginText2D()
{
	GLint matrixMode;
	GLint v[4];
	
	/* Setup OpenGL matrices */
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glGetIntegerv(GL_VIEWPORT, &v[0]);
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelviewMatrix);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glGetDoublev(GL_PROJECTION_MATRIX, m_projectionMatrix);
	glLoadIdentity();
	gluOrtho2D(v[0] - 0.325, v[0] + v[2] - 0.325, v[1] + v[3] - 0.325, v[1] - 0.325);

    glDisable( GL_LIGHTING );
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable    ( GL_BLEND );
    glDisable   ( GL_CULL_FACE );
    glDisable   ( GL_DEPTH_TEST );
    glDepthMask ( false );

	glMatrixMode(matrixMode);
}


void TextRenderer::EndText2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(m_projectionMatrix);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m_modelviewMatrix);

    glDepthMask ( true );
    glEnable    ( GL_DEPTH_TEST );
    glEnable    ( GL_CULL_FACE );
    glDisable   ( GL_BLEND );
}


float TextRenderer::GetTexCoordX( unsigned char theChar )
{
	float const charWidth = 1.0f / 16.0f; // In OpenGL texture UV space
    float xPos = (theChar % 16);
    float texX = xPos * charWidth + TEX_MARGIN + 0.002f;
    return texX;
}


float TextRenderer::GetTexCoordY( unsigned char theChar )
{
	float const charHeight = 1.0f / 16.0f;
    float yPos = (theChar >> 4) - 2;
    float texY = yPos * charHeight; // In OpenGL texture UV space
    return texY;
}


void TextRenderer::DrawText2DUp( float _x, float _y, float _size, char const *_text, ... )
{
	float horiSize = _size * HORIZONTAL_SIZE;

	if( m_renderShadow )    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );    
    else                    glBlendFunc( GL_SRC_ALPHA, GL_ONE );    

    glEnable        ( GL_TEXTURE_2D );
    glEnable        ( GL_BLEND );
    glBindTexture   ( GL_TEXTURE_2D, m_textureId );
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
	unsigned numChars = strlen(_text);
    for( unsigned int i = 0; i < numChars; ++i )
    {       
        unsigned char thisChar = _text[i];

		if (thisChar > 32)
		{
			float texX = GetTexCoordX( thisChar );
			float texY = GetTexCoordY( thisChar );

			glBegin( GL_QUADS );
				glTexCoord2f(texX, texY + TEX_HEIGHT);		        glVertex2f( _x,				_y + horiSize);
				glTexCoord2f(texX + TEX_WIDTH, texY + TEX_HEIGHT);	glVertex2f( _x,	            _y );
				glTexCoord2f(texX + TEX_WIDTH, texY);				glVertex2f( _x + _size,	    _y );
				glTexCoord2f(texX, texY);							glVertex2f( _x + _size,     _y + horiSize);
			glEnd();
		}

        _y -= horiSize;    
    }

    glBlendFunc     ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDisable       ( GL_BLEND );                             // Not here, Blending is enabled during Eclipse render
    glDisable       ( GL_TEXTURE_2D );
}


void TextRenderer::DrawText2DDown( float _x, float _y, float _size, char const *_text, ... )
{
	float horiSize = _size * HORIZONTAL_SIZE;

	if( m_renderShadow )    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );    
    else                    glBlendFunc( GL_SRC_ALPHA, GL_ONE );    

    glEnable        ( GL_TEXTURE_2D );
    glEnable        ( GL_BLEND );
    glBindTexture   ( GL_TEXTURE_2D, m_textureId );
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
	unsigned numChars = strlen(_text);
    for( unsigned int i = 0; i < numChars; ++i )
    {       
        unsigned char thisChar = _text[i];

		if (thisChar > 32)
		{
			float texX = GetTexCoordX( thisChar );
			float texY = GetTexCoordY( thisChar );

			glBegin( GL_QUADS );
				glTexCoord2f(texX, texY + TEX_HEIGHT);		        glVertex2f( _x + _size,     _y);
				glTexCoord2f(texX + TEX_WIDTH, texY + TEX_HEIGHT);	glVertex2f( _x + _size,	    _y+horiSize );
				glTexCoord2f(texX + TEX_WIDTH, texY);				glVertex2f( _x,	            _y+horiSize );
				glTexCoord2f(texX, texY);							glVertex2f( _x,             _y);
			glEnd();
		}

        _y += horiSize;    
    }

    glBlendFunc     ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    //glDisable       ( GL_BLEND );                             // Not here, Blending is enabled during Eclipse render
    glDisable       ( GL_TEXTURE_2D );
}


void TextRenderer::SetRenderShadow ( bool _renderShadow )
{
    m_renderShadow = _renderShadow;
}


void TextRenderer::SetRenderOutline( bool _renderOutline )
{
    m_renderOutline = _renderOutline;
}


void TextRenderer::DrawText2DSimple(float _x, float _y, float _size, char const *_text )
{
 	glColor3ub(255,255,255);

	float horiSize = _size * HORIZONTAL_SIZE;

	glBlendFunc		( GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA );    

    glEnable        ( GL_TEXTURE_2D );
    glEnable        ( GL_BLEND );
    glBindTexture   ( GL_TEXTURE_2D, m_textureId );
    
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
	unsigned numChars = strlen(_text);
    for( unsigned int i = 0; i < numChars; ++i )
    {       
        unsigned char thisChar = _text[i];

		if (thisChar > 32)
		{
			float texX = GetTexCoordX( thisChar );
			float texY = GetTexCoordY( thisChar );

			glBegin( GL_QUADS );
				glTexCoord2f(texX, texY);		        glVertex2f( _x,				_y );
				glTexCoord2f(texX + TEX_WIDTH, texY);	glVertex2f( _x + horiSize,	_y );
				glTexCoord2f(texX + TEX_WIDTH, texY + TEX_HEIGHT);				glVertex2f( _x + horiSize,	_y + _size );
				glTexCoord2f(texX, texY + TEX_HEIGHT);							glVertex2f( _x,				_y + _size );
			glEnd();
		}

		_x += horiSize;    
    }

    glBlendFunc     ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable       ( GL_BLEND );                             // Not here, Blending is enabled during Eclipse render
    glDisable       ( GL_TEXTURE_2D );
}


void TextRenderer::DrawText( float _x, float _y, float _size, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);
    DrawText2DSimple( _x, _y, _size, buf );
}


void TextRenderer::DrawText2DRight( float _x, float _y, float _size, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);

    float width = GetTextWidth( strlen(buf), _size );
    DrawText2DSimple( _x - width, _y, _size, buf );
}


void TextRenderer::DrawText2DCentre( float _x, float _y, float _size, char const *_text, ... )
{
    char buf[512];
    va_list ap;
    va_start (ap, _text);
    vsprintf(buf, _text, ap);

    float width = GetTextWidth( strlen(buf), _size );
    DrawText2DSimple( _x - width/2, _y, _size, buf );
}


float TextRenderer::GetTextWidth( unsigned int _numChars, float _size )
{
    return _numChars * _size * HORIZONTAL_SIZE;
}
