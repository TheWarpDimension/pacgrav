#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H


class RGBAColour;


#define DEF_FONT_SIZE 20.0f
#undef DrawText


// ****************************************************************************
//  Class TextRenderer
// ****************************************************************************

class TextRenderer
{
protected:
	double			m_projectionMatrix[16];
	double			m_modelviewMatrix[16];
	char			*m_filename;
	unsigned int    m_textureId;
	unsigned int	m_bitmapWidth;
	unsigned int	m_bitmapHeight;
    bool            m_renderShadow;    
    bool            m_renderOutline;

    float GetTexCoordX  ( unsigned char theChar );
    float GetTexCoordY  ( unsigned char theChar );
    
public:
	void Initialise(char const *_filename);

	void BuildOpenGlState();

    void BeginText2D	 ();
    void EndText2D		 ();
    
    void SetRenderShadow ( bool _renderShadow );
    void SetRenderOutline( bool _renderOutline );

    void DrawText2DSimple( float _x, float _y, float _size, char const *_text );
    void DrawText      ( float _x, float _y, float _size, char const *_text, ... );	// Like simple but with variable args
    void DrawText2DRight ( float _x, float _y, float _size, char const *_text, ... );	// Like above but with right justify
    void DrawText2DCentre( float _x, float _y, float _size, char const *_text, ... );	// Like above but with centre justify
    void DrawText2DUp    ( float _x, float _y, float _size, char const *_text, ... );   // Like above but rotated 90 ccw
    void DrawText2DDown  ( float _x, float _y, float _size, char const *_text, ... );   // Like above but rotated 90 cw
	    
    float GetTextWidth   ( unsigned int _numChars, float _size=13.0f );
};


extern TextRenderer g_fixedFont;
extern TextRenderer g_propFont;


#endif
