#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "malloc.h"


__attribute__ ((constructor))
void init()
{
   heap = (char *)mmap(NULL,MAX_HEAP,
                                (PROT_READ | PROT_WRITE),
                                (MAP_PRIVATE | MAP_ANONYMOUS), -1,0);

  brkp = heap;
  endp = brkp + MAX_HEAP;

}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* base = NULL;
void *heap;
void *brkp = NULL;
void *endp = NULL;

long int total_no_of_blocks = 0;
long int used_blocks = 0;
long int free_blocks = 0;
long int total_allocation_requests = 0;
long int total_free_requests = 0;

void *malloc(size_t size)
{
  pthread_mutex_lock(&mutex);
  total_allocation_requests += 1;
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
		used_blocks += b->size;	
    	}
       else {

		b = extend_heap(last, s);
		if(!b)
		{
		pthread_mutex_unlock(&mutex);
		errno = ENOENT;
		return NULL;
		}
	
	}
   }

   else{
	   b = extend_heap(NULL, s);
	   if(!b)
	   {
		pthread_mutex_unlock(&mutex);
		errno = ENOENT;
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

void malloc_stats()
{

  char buf[1024];
  snprintf(buf, 1024, "Total size of arena = %d bytes\n"
			"Total No of Blocks = %ld bytes\n"  
			"Used Blocks = %ld bytes\n"
			"Free Blocks = %ld bytes\n"
			"Total Allocation Requests = %ld\n"
			"Total Free Requests = %ld\n",
				MAX_HEAP, 
				total_no_of_blocks, 
				used_blocks,
				free_blocks,
				total_allocation_requests,
				total_free_requests);

  write(STDOUT_FILENO, buf, strlen(buf) + 1);



}
