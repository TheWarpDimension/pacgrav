#include "bitmap.h"


RGBAColour Bitmap::GetPixel(int x, int y)
{
	return m_imageData[y * m_width + x];
}


void Bitmap::Rotate(int degrees)
{
	if (degrees == 90)
	{
		RGBAColour *all = new RGBAColour[m_width * m_height];
		RGBAColour *c = all;
		int bHeight = m_width;
		int bWidth = m_height;

		for (int x = 0; x < m_width; x++)
		{
			for (int y = m_height - 1; y >= 0; y--)
			{
				*c = GetPixel(x, y);
				c++;
			}
		}

		delete [] m_imageData;
		m_imageData = all;
		m_height = bHeight;
		m_width = bWidth;
	}
}


#define FRAC_PART(x) (x - abs(x))

void Bitmap::Resample(int newWidth, int newHeight)
{
	double fx = (double)m_width / newWidth;
	double fy = (double)m_height / newHeight;
	double invFx = 1.0 / fx;
	double invFy = 1.0 / fy;

	if (newWidth < m_width)
	{
		RGBAColour *newData = new RGBAColour [newWidth * newHeight];      
		RGBAColour *c = newData;

		for (int y = 0; y < m_height; ++y)
		{
			RGBAColour *line = &(m_imageData[y * m_width]); 

			for (int x = 0; x < newWidth; x++)
			{
				double x1 = x * fx;
				double x2 = (x+1) * fx;

				const int floorX1 = (int)floor(x1);
				int ceilX1 = (int)ceil(x1);
				const int floorX2 = (int)floor(x2);

				*c = *(line + floorX1) * (1.0 - FRAC_PART(x1)) * invFx;
				if (ceilX1 == floorX1)
					ceilX1++;
				for (int i = ceilX1; i < floorX2; i++)
					*c += *(line + i) * invFx;
				*c += *(line + (int)ceil(x2)) * FRAC_PART(x2) * invFx;

				c++;
			}
		}

		delete m_imageData;
		m_imageData = newData;
		m_width = newWidth;
	}

	//     if (newHeight < m_height)
	//     {
	//         RGBAColour *c = newData;
	//         for (int y = 0; y < m_height; ++y)
	//         {
	//             RGBAColour *line = &(m_imageData[y * m_width]); 
	//             
	//             for (int x = 0; x < newWidth; x++)
	//             {
	//                 double x1 = x * fx;
	//                 double x2 = (x+1) * fx;
	//                 
	//                 const int floorX1 = (int)floor(x1);
	//                 int ceilX1 = (int)ceil(x1);
	//                 const int floorX2 = (int)floor(x2);
	//                 
	//                 *c = *(line + floorX1) * (1.0 - FRAC_PART(x1)) * invFx;
	//                 if (ceilX1 == floorX1)
	//                     ceilX1++;
	//                 for (int i = ceilX1; i < floorX2; i++)
	//                     *c += *(line + i) * invFx;
	//                 *c += *(line + (int)ceil(x2)) * FRAC_PART(x2) * invFx;
	//                 
	//                 c++;
	//             }
	//         }
	//         
	//         delete m_imageData;
	//         m_imageData = newData;
	//         m_width = newWidth;
	//     }
}
