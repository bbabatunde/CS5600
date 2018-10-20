#include<pthread.h>
#include<errno.h>
#include "malloc.h"


void *realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&mutex);
  // Allocate new memory (if needed) and copy the bits from old location to new.
  size_t s;
  t_block b, new;
  void *newp;
  if(!ptr)
   {
    pthread_mutex_unlock(&mutex);
    return (malloc(size));
    }
  if(valid_addr(ptr))
  {

        s = align8(size);
        b = get_block(ptr);
        if(b->size >= s)
        {
                if(b->size - s >= (BLOCK_SIZE + 4))
                  split_block(b,s);
        }
        else
        {
                if(b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
                {

                        fusion(b);
                        if(b->size - s >= (BLOCK_SIZE + 4))
                        split_block(b,s);
                }
                else
                {
			pthread_mutex_unlock(&mutex);
                        newp = malloc(s);
			pthread_mutex_lock(&mutex);
                        if(!newp)
			{
 			    pthread_mutex_unlock(&mutex);
			    errno = ENOENT;
                            return NULL;
			}
                        new = get_block(newp);
                        memcpy(new->ptr, b->ptr, b->size);
			pthread_mutex_unlock(&mutex);
                        free(ptr);
			pthread_mutex_lock(&mutex);
			pthread_mutex_unlock(&mutex);
                        return (newp);
                }
        }
    pthread_mutex_unlock(&mutex);
    return (ptr);
  }

  pthread_mutex_unlock(&mutex);
  errno = ENOENT;
  return NULL;
}

