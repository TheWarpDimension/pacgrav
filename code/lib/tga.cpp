#include "lib/universal_include.h"

#include "lib/rgb_colour.h"
#include "lib/tga.h"

#include <stdio.h>
#include <stdlib.h>


typedef enum
{
	IMG_TYPE_NO_IMAGE,
	IMG_TYPE_UNCOMPRESSED_COLOUR_MAPPED,
	IMG_TYPE_UNCOMPRESSED_TRUE_COLOUR,
	IMG_TYPE_UNCOMPRESSED_BLACK_AND_WHITE,
	IMG_TYPE_RLE_COLOUR_MAPPED = 9,
	IMG_TYPE_RLE_TRUE_COLOUR,
	IMG_TYPE_RLE_BLACK_AND_WHITE
} ImageType;


// load the image header fields. We only keep those that matter!
void TgaImage::LoadHeader(FILE *file, int *type, int *pixelDepth) 
{
	unsigned char cGarbage;
	short int iGarbage;
	
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	*type = cGarbage;

	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	fread(&iGarbage, sizeof(short int), 1, file);
	
	fread(&iGarbage, sizeof(short int), 1, file);
	m_width = iGarbage; 
	fread(&iGarbage, sizeof(short int), 1, file);
	m_height = iGarbage; 
	
	fread(&cGarbage, sizeof(unsigned char), 1, file);
	*pixelDepth = cGarbage;

	fread(&cGarbage, sizeof(unsigned char), 1, file);
}


void TgaImage::LoadUncompressedData(FILE *file, int bytesPerPixel, int totalPixels) 
{	
	fread(m_pixels, sizeof(unsigned char), totalPixels, file);
	
	// mode=3 or 4 implies that the image is RGB(A). However TGA
	// stores it as BGR(A) so we'll have to swap R and B.
// 	if (bytesPerPixel >= 3)
// 	{
// 		for (int i=0; i < totalPixels; i += bytesPerPixel)
// 		{
// 			unsigned char aux = m_pixels[i];
// 			m_pixels[i] = m_pixels[i+2];
// 			m_pixels[i+2] = aux;
// 		}
// 	}
}


void TgaImage::LoadRleData(FILE *file, int bytesPerPixel, int totalPixels) 
{	
	for (int y = 0; y < m_height; y++)
	{
		RGBAColour *row = m_pixels + y * m_width;

		for (int x = 0; x < m_width;)
		{
			int c = fgetc(file);
			int numPixels = (c & 0x7f) + 1;
			unsigned colour;

			ReleaseAssert(x + numPixels <= m_width, "TGA run length goes beyond end of line");

			if (c & 0x80)
			{
				// Run of identical pixels
				fread(&colour, bytesPerPixel, 1, file);
// 				char tmp = colour & 0xff;
// 				colour &= 0xffff00;
// 				colour |= colour >> 16;
// 				colour &= 0xffff;
// 				colour |= tmp << 16;
				for (int i = 0; i < numPixels; i++)
					row[x + i] = colour;
				colour |= 0xff;
			}
			else
			{
				// Run of non-identical pixels
				for (int i = 0; i < numPixels; i++)
				{
					fread(&colour, bytesPerPixel, 1, file);
// 					char tmp = colour & 0xff;
// 					colour &= 0xffff00;
// 					colour |= colour >> 16;
// 					colour &= 0xffff;
// 					colour |= tmp << 16;
					colour |= 0xff;
					row[x + i] = colour;
				}
			}

			x += numPixels;
		}
	}

// 	for (int y = 0; y < m_height; y++)
// 	{
// 		RGBAColour *row = m_pixels + y * m_width;
// 		
// 		for (int x = 0; x < m_width; x++)
// 		{
// 			row[x] = g_colourBlack;
// 			if (x == y)
// 				row[x] = g_colourWhite;
// 		}
// 	}
}	


// this is the function to call when we want to load an image
int TgaImage::Load(char const *filename)
{
	FILE *file;
	int bytesPerPixel, totalPixels;
	int type, pixelDepth;
	
	// open the file for reading (binary mode)
	file = fopen(filename, "rb");
	if (file == NULL) 
		return ERROR_FILE_OPEN;
	
	// load the header
	LoadHeader(file, &type, &pixelDepth);
	
	// check for errors when loading the header
	if (ferror(file)) 
	{
		fclose(file);
		return ERROR_READING_FILE;
	}
	
	// check for image types we don't support
	if (type != IMG_TYPE_RLE_TRUE_COLOUR && 
		type != IMG_TYPE_UNCOMPRESSED_BLACK_AND_WHITE &&
		type != IMG_TYPE_UNCOMPRESSED_TRUE_COLOUR) 
	{
		fclose(file);
		return ERROR_UNSUPPORTED_FORMAT;
	}
	
	bytesPerPixel = pixelDepth / 8;
	totalPixels = m_height * m_width * bytesPerPixel;
	
	// allocate memory for image pixels
	m_pixels = new RGBAColour[totalPixels];
	
	// check to make sure we have the memory required
	if (m_pixels == NULL) 
	{
		fclose(file);
		return ERROR_OUT_OF_MEMORY;
	}

	// finally load the image pixels
	if (type == IMG_TYPE_RLE_TRUE_COLOUR)
		LoadRleData(file, bytesPerPixel, totalPixels);
	else
		LoadUncompressedData(file, bytesPerPixel, totalPixels);
	
	// check for errors when reading the pixels
	if (ferror(file)) 
	{
		fclose(file);
		return ERROR_READING_FILE;
	}

	fclose(file);

	return OK;
}		


// saves an array of pixels as a TGA image
// int tgaSave(char		*filename, 
// 			short int	width, 
// 			short int	height, 
// 			unsigned char	pixelDepth,
// 			unsigned char	*imageData) 
// {	
// 	unsigned char cGarbage = 0, type,mode,aux;
// 	short int iGarbage = 0;
// 	int i;
// 	FILE *file;
// 	
// 	// open file and check for errors
// 	file = fopen(filename, "wb");
// 	if (file == NULL) {
// 		return(TGA_ERROR_FILE_OPEN);
// 	}
// 	
// 	// compute image type: 2 for RGB(A), 3 for greyscale
// 	mode = pixelDepth / 8;
// 	if (pixelDepth == 24 || pixelDepth == 32)
// 		type = 2;
// 	else
// 		type = 3;
// 	
// 	// write the header
// 	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
// 	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
// 	
// 	fwrite(&type, sizeof(unsigned char), 1, file);
// 	
// 	fwrite(&iGarbage, sizeof(short int), 1, file);
// 	fwrite(&iGarbage, sizeof(short int), 1, file);
// 	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
// 	fwrite(&iGarbage, sizeof(short int), 1, file);
// 	fwrite(&iGarbage, sizeof(short int), 1, file);
// 	
// 	fwrite(&width, sizeof(short int), 1, file);
// 	fwrite(&height, sizeof(short int), 1, file);
// 	fwrite(&pixelDepth, sizeof(unsigned char), 1, file);
// 	
// 	fwrite(&cGarbage, sizeof(unsigned char), 1, file);
// 	
// 	// convert the image data from RGB(a) to BGR(A)
// 	if (mode >= 3)
// 		for (i=0; i < width * height * mode ; i+= mode) {
// 			aux = imageData[i];
// 			imageData[i] = imageData[i+2];
// 			imageData[i+2] = aux;
// 		}
// 		
// 		// save the image data
// 		fwrite(imageData, sizeof(unsigned char), 
// 			width * height * mode, file);
// 		fclose(file);
// 		// release the memory
// 		free(imageData);
// 		
// 		return(TGA_OK);
// }
