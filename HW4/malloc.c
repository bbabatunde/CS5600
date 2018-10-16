#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>

#include "malloc.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* base = NULL;
void *malloc(size_t size)
{
  pthread_mutex_lock(&mutex);
  t_block b, last;
  size_t s;
  s = align8(size);
  if(base){
	last = base;
	b = find_block(&last,s);
  	if(b){
       		if((b->size - s) >= (BLOCK_SIZE +4))
		split_block(b, s);
		b->free=0;
	
    	}
       else {

		b = extend_heap(last, s);
		if(!b)
		{
		pthread_mutex_unlock(&mutex);
		return NULL;
		}
	
	}
   }

   else{
	   b = extend_heap(NULL, s);
	   if(!b)
	   {
		pthread_mutex_unlock(&mutex);
		return(NULL);
	   }
	   base = b;
   }

 
   
  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  //char buf[1024];
  //snprintf(buf, 1024, "%s:%d malloc(%zu): Allocated %zu bytes at %p\n",
  //         __FILE__, __LINE__, size, b->size, b->data);
  //write(STDOUT_FILENO, buf, strlen(buf) + 1);
  pthread_mutex_unlock(&mutex);
  return b->data;
}

