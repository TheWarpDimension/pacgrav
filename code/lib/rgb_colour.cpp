#include "lib/universal_include.h"

#include "rgb_colour.h"


RGBAColour g_colourBlack(0,0,0);
RGBAColour g_colourWhite(255,255,255);


RGBAColour::RGBAColour()
{
}

RGBAColour::RGBAColour(unsigned col)
{
	r = (col & 0xff0000) >> 16;
	g = (col & 0xff00) >> 8;
	b = (col & 0xff);
}

RGBAColour::RGBAColour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
    Set( _r, _g, _b, _a );
}


RGBAColour::RGBAColour(int _col)
{
    Set( _col);
}


void RGBAColour::Set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


void RGBAColour::Set(int _col)
{
    r = (_col & 0xff000000) >> 24;
    g = (_col & 0x00ff0000) >> 16;
    b = (_col & 0x0000ff00) >> 8;
    a = (_col & 0x000000ff) >> 0;
}


void RGBAColour::ConvertToHsv()
{
    if (((int)r + (int)g + (int)b) == 0) return;

    unsigned char v;
    float h = 0.0f;
    unsigned char delta;

    // Is blue the dominant colour?
    if (b > g && b > r)
    {
        v = b;

        if (r > g) delta = v - g;
        else       delta = v - r;

        if (delta > 0)
        {
            h = 4.0f + (r - g) / (float)delta;
        }
    }
    else if (g > r)         // Is green the dominant colour?
    {
        v = g;

        if (r > b) delta = v - b;
        else       delta = v - r;
        
        if (delta > 0)
        {
            h = 2.0f + (r - b) / (float)delta;
        }
    }
    else                    // Red is dominant
    {
        v = r;

        if (g > b) delta = v - b;
        else       delta = v - g;
        
        if (delta > 0)
        {
            h = g - b / (float)delta;
        }
    }

    r = h * 42.667f;
    g = (float)((int)delta << 8) / (float)v;
    b = v;
}


void RGBAColour::ConvertToRgb()
{
    float h = r;
    float s = g / 255.0f;
    float v = b;
  
    if( v == 0 ) 
    {
        r = 0;
        b = 0;
        return;
    }
    
    h /= 42.667f;		// sector 0 to 5
    int i = floor( h );
    float f = h - i;			// fractional part of h
    float p = v * ( 1 - s );
    float q = v * ( 1 - s * f );
    float t = v * ( 1 - s * ( 1 - f ) );
    
    switch( i ) 
    {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    default:		// case 5:
        r = v;
        g = p;
        b = q;
        break;
	}
}


// *** Operator +
RGBAColour RGBAColour::operator + (RGBAColour const &_b) const
{
	int x = r + _b.r;
	int y = g + _b.g;
	int z = b + _b.b;
	int w = a + _b.a;

	if (x > 255) x = 255;
	if (y > 255) y = 255;
	if (z > 255) z = 255;

	return RGBAColour(x, y, z, a);
}


// *** Operator -
RGBAColour RGBAColour::operator - (RGBAColour const &_b) const
{
	int x = r - _b.r;
	int y = g - _b.g;
	int z = b - _b.b;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;

	return RGBAColour(x, y, z, a);
}


// *** Operator *
RGBAColour RGBAColour::operator * (float const _b) const
{
	if (_b < 0.0f) return *this;

	float newR = (float)r * _b;		if (newR > 255.0f) newR = 255.0f;
	float newG = (float)g * _b;		if (newG > 255.0f) newG = 255.0f;
	float newB = (float)b * _b;		if (newB > 255.0f) newB = 255.0f;

	return RGBAColour((unsigned char)newR, (unsigned char)newG, (unsigned char)newB, a);
}


// *** Operator /
RGBAColour RGBAColour::operator / (float const _b) const
{
	float multiplier = 1.0f / _b;
	return RGBAColour((unsigned char) ((float)r * multiplier),
					  (unsigned char) ((float)g * multiplier),
					  (unsigned char) ((float)b * multiplier),
					  a);
}


// *** Operator =
RGBAColour const &RGBAColour::operator = (RGBAColour const &_b)
{
	r = _b.r;
	g = _b.g;
	b = _b.b;
	a = _b.a;
	return *this;
}


// *** Operator *=
RGBAColour const &RGBAColour::operator *= (float const _b)
{
	if (_b > 0.0f)
	{
		float newR = (float)r * _b;		if (newR > 255.0f) newR = 255.0f;
		float newG = (float)g * _b;		if (newG > 255.0f) newG = 255.0f;
		float newB = (float)b * _b;		if (newB > 255.0f) newB = 255.0f;
		
		r = (unsigned char)newR;
		g = (unsigned char)newG;
		b = (unsigned char)newB;
	}

	return *this;
}


// *** Operator /=
RGBAColour const &RGBAColour::operator /= (float const _b)
{
	float multiplier = 1.0f / _b;

	if (multiplier > 0.0f)
	{
		float newR = (float)r * _b;		if (newR > 255.0f) newR = 255.0f;
		float newG = (float)g * _b;		if (newG > 255.0f) newG = 255.0f;
		float newB = (float)b * _b;		if (newB > 255.0f) newB = 255.0f;
		
		r = (unsigned char)newR;
		g = (unsigned char)newG;
		b = (unsigned char)newB;
	}

	return *this;
}


// *** Operator +=
RGBAColour const &RGBAColour::operator += (RGBAColour const &_b)
{
	int x = r + _b.r;
	int y = g + _b.g;
	int z = b + _b.b;

	if (x > 255) x = 255;
	if (y > 255) y = 255;
	if (z > 255) z = 255;
	
	r = x;
	g = y;
	b = z;

	return *this;
}


// *** Operator -=
RGBAColour const &RGBAColour::operator -= (RGBAColour const &_b)
{
	int x = r - _b.r;
	int y = g - _b.g;
	int z = b - _b.b;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;

	r = x;
	g = y;
	b = z;

	return *this;
}


// *** Operator ==
bool RGBAColour::operator == (RGBAColour const &_b) const
{
	return (r == _b.r && g == _b.g && b == _b.b && a == _b.a);
}


// *** Operator !=
bool RGBAColour::operator != (RGBAColour const &_b) const
{
	return (r != _b.r || g != _b.g || b != _b.b || a != _b.a);
}


unsigned char const *RGBAColour::GetData() const
{
	return &r;
}


unsigned char RGBAColour::GetLuminance() const
{
	float lum = 0.3f * r + 0.59f * g + 0.11f * b;
	return lum;
}


void RGBAColour::SignedAddWithSaturate(int _r, int _g, int _b)
{
	int x = r + _r;
	int y = g + _g;
	int z = b + _b;
	
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (z < 0) z = 0;
	
	if (x > 255) x = 255;
	if (y > 255) y = 255;
	if (z > 255) z = 255;

	r = x;
	g = y;
	b = z;
}


void RGBAColour::BlendTowards(RGBAColour const &other, float fraction)
{
	float frac2 = 1.0f - fraction;
	r = (float)other.r * fraction + (float)r * frac2; 
	g = (float)other.g * fraction + (float)g * frac2; 
	b = (float)other.b * fraction + (float)b * frac2; 
	a = (float)other.a * fraction + (float)a * frac2; 
}
