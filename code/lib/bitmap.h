#ifndef INCLUDED_BITMAP_H
#define INCLUDED_BITMAP_H


class RGBAColour;


class BitmapRGBA
{
public:
	int m_width;
	int m_height;
	RGBAColour *m_pixels;
	RGBAColour **m_lines;

	BitmapRGBA();
	BitmapRGBA(BitmapRGBA const &_other);
	BitmapRGBA(int width, int height);
	virtual ~BitmapRGBA();

	void Initialise(int width, int height);
	
	enum ErrorCode
	{
		OK,
		ERROR_FILE_OPEN,
		ERROR_READING_FILE,
		ERROR_UNSUPPORTED_FORMAT,
		ERROR_OUT_OF_MEMORY,
	};
	
	int Load(char const *filename) {}

	void Clear( RGBAColour const &colour );

	void PutPixel(int x, int y, RGBAColour const &colour);
	RGBAColour const &GetPixel(int x, int y) const;

	void PutPixelClipped(int x, int y, RGBAColour const &colour);
	RGBAColour const &GetPixelClipped(int x, int y) const;

    void DrawLine(int x1, int y1, int x2, int y2, RGBAColour const &colour);

	RGBAColour GetInterpolatedPixel(float x, float y) const;

	void Blit(int srcX,  int srcY,  int srcW,  int srcH, BitmapRGBA *_srcBmp, 
			  int destX, int destY, int destW, int destH);
    
	void ConvertPinkToTransparent();
	int ConvertToTexture(bool _mipmapping = true) const;

	void FlipH();
	void FlipV();
};


#endif
