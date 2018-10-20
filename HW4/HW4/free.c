#include<pthread.h>
#include "malloc.h"


void free(void *ptr)
{
  pthread_mutex_lock(&mutex);
  total_free_requests += 1;
  t_block b;
  if(valid_addr(ptr))
  {
        b = get_block(ptr);
        b->free = 1;
        if(b->prev && b->prev->free)
                b = fusion(b->prev);
        if(b->next)
           fusion(b);
        else{

                if(b->prev)
                        b->prev->next = NULL;
                else    
                        base = NULL;
		brkp-=b->size;
		free_blocks += b->size;
                //brk(b);

        }


  }
  pthread_mutex_unlock(&mutex);

  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  //char buf[1024];
  //snprintf(buf, 1024, "%s:%d free(%p): Freeing %zu bytes from",
  //         __FILE__, __LINE__,ptr, b->size);
  //write(STDOUT_FILENO, buf, strlen(buf) + 1);
}

