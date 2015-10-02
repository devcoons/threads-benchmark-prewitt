#include "bitmap.h"

	struct bitmap_t * Load_Image(char * filename)
	{
		FILE * filePtr;
		bmp_pixel_t ** pixels;
		size_t pad=0,i,j;
		bitmap_t * bitmap;
		char magic[2];
		int sum = 0,q;
		uint8_t b;
		
		if ( (filePtr = fopen(filename, "rb")) == NULL ) 
			return NULL;
		
		if ( fread(&magic, sizeof(magic), 1, filePtr) != 1 )
			{fclose(filePtr);	return NULL;}
		
		if ( *((uint16_t*) magic) != 0x4D42 )
			{fclose(filePtr);	return NULL;}

		if( (bitmap = (bitmap_t *) malloc(sizeof(bitmap_t))) == NULL )
		{return NULL;}

		if ( fread(&(bitmap->fheader), sizeof(bmp_header_t),1, filePtr) != 1 )
			{fclose(filePtr);	return NULL;}
		
		if ( fread(&(bitmap->dheader), sizeof(dib_header_t),1, filePtr) != 1 )
			{fclose(filePtr);	return NULL;}

		if ( bitmap->dheader.compress_type != 0 ) 
			{fclose(filePtr);	return NULL;}
		
		if ( fseek(filePtr, bitmap->fheader.dataoffset, SEEK_SET) )
			{fclose(filePtr);	return NULL;}
		
		if ( bitmap->dheader.depth != 24 ) 
			{fclose(filePtr);	return NULL;}
		
		if( (pixels = (bmp_pixel_t**) malloc(bitmap->dheader.height*sizeof(bmp_pixel_t*))) == NULL )
			{fclose(filePtr);	return NULL;}
			
		for (i = 0; i < bitmap->dheader.height; i++)
			if( (pixels[i] = (bmp_pixel_t*) malloc(bitmap->dheader.width*sizeof(bmp_pixel_t))) == NULL )
				{fclose(filePtr);	return NULL;}
		
		if ( ((bitmap->dheader.width*bitmap->dheader.depth / 8) % 4) != 0 )
			pad = 4 - ((bitmap->dheader.width*bitmap->dheader.depth / 8) % 4);  

		for (i = 0; i < bitmap->dheader.height; i++) 
		{
			for (j = 0; j < bitmap->dheader.width; j++) 
			{
				sum = 0;
				for (q = 0; q < 3; q++)
				{
					if ( fread(&b, sizeof(uint8_t), 1, filePtr) != 1 ) 
						{fclose(filePtr);	break;}
					sum += b;				
				}
				pixels[bitmap->dheader.height-(i+1)][bitmap->dheader.width-(j+1)].value = sum / 3;
			}
			fseek(filePtr, pad, SEEK_CUR);
		}
		
		bitmap->pixels = pixels;
		fclose(filePtr);
		return bitmap;
	}
	
	int Save_Image(char * filename, struct bitmap_t * bitmap)
	{
		char c = 0;
		size_t pad = 0,i,j,q;
		char magic[] = {0x42,0x4d};
		FILE* filePtr;
		
		if ( (filePtr = fopen(filename, "wb")) == NULL ) 
			return -1;

		if ( fwrite(&magic, sizeof(magic), 1, filePtr) != 1 ) 				
			{fclose(filePtr);	return -1;}	
		
		if ( fwrite(&bitmap->fheader, sizeof(bmp_header_t), 1, filePtr) != 1 )	
			{fclose(filePtr);	return -1;}
		
		if ( fwrite(&bitmap->dheader, sizeof(dib_header_t), 1, filePtr) != 1 )	
			{fclose(filePtr);	return -1;}
		
		if ( ((bitmap->dheader.width*bitmap->dheader.depth / 8) % 4) != 0 )
			pad = 4 - ((bitmap->dheader.width*bitmap->dheader.depth / 8) % 4);	

		for (i = 0; i < bitmap->dheader.height; i++) 
		{
			for (j = 0; j < bitmap->dheader.width; j++) 
			{
				c = (unsigned char) bitmap->pixels[bitmap->dheader.height - (i + 1)][bitmap->dheader.width - (j + 1)].value;
				for (q = 0; q < 3; q++)
					if ( fwrite(&c, sizeof(char), 1, filePtr) != 1 ) 
						{fclose(filePtr);	return 0;}				
			}
			c = 0;
			for (j = 0; j<pad; j++)
				if ( fwrite(&c, sizeof(char), 1, filePtr) != 1 ) 
					{fclose(filePtr);	return 0;}
		}
		
		fclose(filePtr);
		return 0;
	}