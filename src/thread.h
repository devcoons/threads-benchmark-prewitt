#ifndef _THREAD_H
	#define _THREAD_H
		#include <time.h>
		#include <float.h>
		#include "bitmap.h"
		
		pthread_t * TID;
		
		pthread_mutex_t LOCKTIME;
		pthread_mutex_t LOCKSTATE;	
		
		pthread_barrier_t BEGINBRR;
		pthread_barrier_t ENDBRR;
		
		typedef struct thread_arg_t
		{
			int thread_id;
			int start_width;
			int end_width;
			struct bitmap_t * bitmap;
		}thread_arg_t;
		
		void * Thread(void *);
		
		unsigned long Thread_Pool_Manager(struct bitmap_t *, int);
		
#endif