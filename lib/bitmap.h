#ifndef INCLUDED_BITMAP_H
#define INCLUDED_BITMAP_H


#include "lib/rgb_colour.h"


class Bitmap
{
public:
	int m_width;
	int m_height;
	RGBAColour *m_imageData;

	int Load(char const *filename) = 0;
	RGBAColour GetPixel(int x, int y);
	void Rotate(int degrees);
	void Resample(int newWidth, int newHeight);
};


#endif
