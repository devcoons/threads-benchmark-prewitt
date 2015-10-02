#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <libeasy.h>
#include "bitmap.h"
#include "thread.h"

	void Print_Usage()
	{
		printf("Usage : -i(image) $ -t(threads) $ [-o(output) $] [-j(json)]\n");
	}

	int main(int argc, char ** argv)
	{
		int long_index = 0, json_format = 0, opt = 0, threads = 0, saved =0,i;
		unsigned long timelapse = 0;
		struct bitmap_t * bitmap;
		char * in_img = NULL, * out_img = NULL;
		
		static struct option long_options[] = {
			{"image",   required_argument,  0,  'i' },
			{"threads", required_argument,  0,  't' },
			{"output",  required_argument,	0,  'o' },
			{"json",	no_argument,		0,	'j' },
			{0,			0,					0,	0   }
		};
		
		while ( (opt = getopt_long(argc, argv, "i:t:o:j", long_options, &long_index )) != -1 ) 
			switch (opt) 
			{
				case 'i' :
					in_img = str_merge(in_img,optarg,NULL);
					bitmap = Load_Image(in_img);
					break;
				case 't' : 
					threads=atoi(optarg);
					break;
				case 'o' : 
					out_img = str_merge(out_img,optarg,NULL);
					break;
				case 'j' : 
					json_format=1;
					break;
				default:
					Print_Usage();
					exit(EXIT_FAILURE);
			}
				
		if(bitmap == NULL)
			{Print_Usage(); exit(EXIT_FAILURE);}

		if(threads < 1)
			{Print_Usage();	exit(EXIT_FAILURE);}
		for(i=0;i<10;i++)
		{
			timelapse += Thread_Pool_Manager(bitmap, threads);
			if(out_img && i==0)
				saved = Save_Image(out_img,bitmap);
		}
		timelapse = timelapse/10;
		
		if(json_format == 0)
			if(out_img == NULL)
				printf("Threads : %d\nTimelapse : %lu\n",threads, timelapse);			
			else		
				printf("Threads : %d\nTimelapse : %lu\nOutput : %s[%d]\n",threads, timelapse, out_img, saved);	
		else
			if(out_img == NULL)				
				printf("{\"threads\":\"%d\",\"timelapse\":\"%lu\"}",threads, timelapse);
			else			
				printf("{\"threads\":\"%d\",\"timelapse\":\"%lu\",\"output\":\"%s[%d]\"}",threads, timelapse, out_img, saved);

		exit(0);
	}