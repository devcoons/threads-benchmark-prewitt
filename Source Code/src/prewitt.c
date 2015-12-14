#include "prewitt.h"

	void Prewitt_Filter(bitmap_t * bitmap, int swidth,int ewidth)
	{
		bmp_pixel_t ** result;
		float accum1 = 0, accum2 = 0;
		int row1 = 0, col1 = 0, row, col, i, j;
	
		float kGx[3][3] = { { -1.0f, 0, 1.0f },{ -1.0f, 0, 1.0f },{ -1.0f, 0, 1.0f } };
		float kGy[3][3] = { { -1.0f, -1.0f, -1.0f },{ 0, 0, 0 },{ 1.0f, 1.0f, 1.0f } };
		
		if( (result = (bmp_pixel_t **) malloc((bitmap->dheader.height)*sizeof(bmp_pixel_t *))) == NULL)
			return;
		
		for (i = 0; i < (bitmap->dheader.height); i++)
			if( (result[i] = (bmp_pixel_t*) malloc((ewidth-swidth)*sizeof(bmp_pixel_t))) == NULL)
				return;
		
		for (row = 0; row < bitmap->dheader.height; row++)
			for (col = swidth; col < ewidth; col++) 
			{
				accum1 = 0;
				accum2 = 0;
				for (i = -1; i <= 1; i++)
					for (j = -1; j <= 1; j++) 
					{
						row1 = row + i >= 0 ? row + i : 0;
						col1 = col + j >= swidth  ? col + j : 0;
						row1 = row + i >= bitmap->dheader.height ? bitmap->dheader.height - 1 : row1;
						col1 = col + j >= ewidth  ? ewidth - 1  : col1;
						accum1 += bitmap->pixels[row1][col1].value * kGy[1 + i][1 + j];
						accum2 += bitmap->pixels[row1][col1].value * kGx[1 + i][1 + j];
					}
				result[row-0][col-swidth].value = (int) sqrt(pow(accum1, 2) + pow(accum2, 2));
			}
			
		for (row = 0; row < bitmap->dheader.height; row++) 
			for (col = swidth; col < ewidth; col++) 
				bitmap->pixels[row][col].value=result[row-0][col-swidth].value;
			
		for (i = 0; i < bitmap->dheader.height; i++)
			free(result[i]);
		free(result);
	}
