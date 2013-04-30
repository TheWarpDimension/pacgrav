#ifndef INCLUDED_RGB_COLOUR_H
#define INCLUDED_RGB_COLOUR_H


class RGBAColour
{
public:
	unsigned char r, g, b, a;

	RGBAColour();
	RGBAColour(unsigned col);
	RGBAColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    RGBAColour(int _col);

	void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    void Set(int _col);

    void ConvertToHsv();
    void ConvertToRgb();

	void SignedAddWithSaturate(int r, int g, int b);
	void BlendTowards(RGBAColour const &other, float fraction);

	RGBAColour operator + (RGBAColour const &b) const;
	RGBAColour operator - (RGBAColour const &b) const;
	RGBAColour operator * (float const b) const;
	RGBAColour operator / (float const b) const;

    RGBAColour const &operator = (RGBAColour const &b);
	RGBAColour const &operator *= (float const b);
	RGBAColour const &operator /= (float const b);
	RGBAColour const &operator += (RGBAColour const &b);
	RGBAColour const &operator -= (RGBAColour const &b);

	bool operator == (RGBAColour const &b) const;
	bool operator != (RGBAColour const &b) const;

	unsigned char const *GetData() const;

	unsigned char GetLuminance() const; // Returns a value between 0 and 255
};

// Operator * between float and RGBAColour
inline RGBAColour operator * ( float _scale, RGBAColour const &_v )
{
	return _v * _scale;
}


extern RGBAColour g_colourBlack;
extern RGBAColour g_colourWhite;


#endif
