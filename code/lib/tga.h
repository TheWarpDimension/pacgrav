#ifndef INCLUDED_TGA_H
#define INCLUDED_TGA_H


#include "bitmap.h"


class TgaImage: public BitmapRGBA
{
private:
	void LoadHeader(FILE *file, int *type, int *pixelDepth);
	void LoadUncompressedData(FILE *file, int bytesPerPixel, int totalPixels);
	void LoadRleData(FILE *file, int bytesPerPixel, int totalPixels);

public:
	int Load(char const *filename);
};


#endif
