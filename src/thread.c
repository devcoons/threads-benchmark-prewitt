#include "thread.h"
#include "prewitt.h"

	void * Thread(void * arg)
	{
		int i,j;
		thread_arg_t * args = (thread_arg_t*)arg;
		
		pthread_barrier_wait (&BEGINBRR);	
			PrewittFilter(args->bitmap, args->start_width, args->end_width);
		pthread_barrier_wait (&ENDBRR);
	}
	
	unsigned long Thread_Pool_Manager(struct bitmap_t * bitmap, int threads)
	{
		int i=0;
		thread_arg_t args[threads];
		struct timespec time_begin, time_end;
		
		pthread_barrier_init (&BEGINBRR, NULL, threads+1);
		pthread_barrier_init (&ENDBRR,   NULL, threads+1);
		
		for(i=0;i<threads;i++)
		{
			args[i].start_width = (i == 0 ? 0 : args[i-1].end_width + 1);
			args[i].end_width = (i+1) * (bitmap->dheader.width/threads);
			args[i].bitmap = bitmap;
		}
		
		if( (TID = (pthread_t *)malloc(threads * sizeof(pthread_t))) == NULL )
			return -1;
		
		for(i=0;i<threads;i++)
			pthread_create(&TID[i], NULL, Thread,(void*)&args[i]);
		
			pthread_barrier_wait (&BEGINBRR);
				clock_gettime(CLOCK_MONOTONIC, &time_begin);
			pthread_barrier_wait (&ENDBRR);
				clock_gettime(CLOCK_MONOTONIC, &time_end);
			
		for(i=0;i<threads;i++)
			pthread_join(TID[i], NULL);
		
		pthread_barrier_destroy(&BEGINBRR);
		pthread_barrier_destroy(&ENDBRR);
		free(TID);
		
		return ( (time_end.tv_sec - time_begin.tv_sec) ) * 1000000 + ( time_end.tv_nsec/1000 - time_begin.tv_nsec/1000 );
	}