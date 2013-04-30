#ifndef INCLUDED_PNG_H
#define INCLUDED_PNG_H


#include "bitmap.h"


class PngImage: public BitmapRGBA
{
public:
	int Load(char const *filename);
};


#endif
