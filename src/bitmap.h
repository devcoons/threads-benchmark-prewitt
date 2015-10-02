#ifndef _BITMAP_H
	#define _BITMAP_H
		
		#include <math.h>
		#include <float.h>
		#include <stdio.h>
		#include <stdint.h>		
		#include <stdlib.h>
		#include <string.h>
		#include <assert.h>
		#include <stdbool.h>
		
		typedef struct bmp_header_t
		{
			uint32_t filesize;
			uint16_t undefined0;
			uint16_t undefined1;
			uint32_t dataoffset;
		}bmp_header_t;

		typedef struct dib_header_t
		{
			uint32_t dipsize;
			int32_t  width;
			int32_t  height;
			uint16_t nplanes;
			uint16_t depth;
			uint32_t compress_type;
			uint32_t bmp_bytesz;
			int32_t  hres;
			int32_t  vres;
			uint32_t ncolors;
			uint32_t nimpcolors;
		}dib_header_t;

		typedef struct bmp_pixel_t
		{
			unsigned int value;

		}bmp_pixel_t;

		typedef struct bitmap_t
		{
			struct bmp_header_t  fheader;
			struct dib_header_t  dheader;
			struct bmp_pixel_t ** pixels;
		}bitmap_t;

		struct bitmap_t * Load_Image(char *);
		int Save_Image(char *, struct bitmap_t *);
#endif