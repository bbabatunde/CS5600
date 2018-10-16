#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "malloc.h"

typedef struct MallocHeader *t_block;
void *base = NULL;

struct MallocHeader
{
  size_t size;
  struct MallocHeader* next;
  struct MallocHeader* prev;
  int free;
  void *ptr;
  char data[1];
};

void *calloc(size_t nmemb, size_t size)
{
	size_t *new;
	size_t s4, i;
	new = malloc(nmemb * size);
	if(new)
		memset(new,0,nmemb);
	return(new);
}

t_block get_block(void *p)
{
	char* tmp;
	tmp = p;
	return (p = tmp -= BLOCK_SIZE);

}

int valid_addr(void *p)
{
	if(base)
	{	
		if(p >base && p<sbrk(0))
		{	
			return (p == (get_block(p))->ptr);
		}

	}

   return(0);
}

t_block fusion(t_block b){
	if(b->next && b->next->free){
		b->size += BLOCK_SIZE + b->next->size;
		b->next = b->next->next;
      	  if(b->next)
			b->next->prev = b;
	}
	return b;
}

void split_block(t_block b, size_t s)
{
	t_block new;
	new = (t_block)(b->data + s);
	new->size = b->size -s - BLOCK_SIZE;
	new->next = b->next;
        new->prev = b;
	new->free = 1;
	new->ptr = new->data;
	b->size = s;
	b->next = new;
	if(new->next)
	   new->next->prev = new;

}

t_block find_block(t_block *last, size_t size)
{
	t_block b=base;
	while(b && !(b->free && b->size >= size)){
	*last = b;
	 b = b->next;
	}
	return b;

}

t_block extend_heap(t_block last, size_t s){
   t_block b;
   int sb;
   b = sbrk(0);
   sb = (int)sbrk(BLOCK_SIZE + s);
   if(sb < 0)
	return NULL;

   b->size = s;
   b->next = NULL;
   b->prev = last;
   b->ptr = b->data;
   if(last)
	last->next = b;
   b->free = 0;
   return (b);

}

void *malloc(size_t size)
{

  t_block b, last;
  size_t s;
  s = align4(size);
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
		return NULL;
	
	}
   }

   else{
	   b = extend_heap(NULL, s);
	   if(!b)
		return(NULL);
	   base = b;
   }

 
   
  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  char buf[1024];
  snprintf(buf, 1024, "%s:%d malloc(%zu): Allocated %zu bytes at %p\n",
           __FILE__, __LINE__, size, b->size, b->data);
  write(STDOUT_FILENO, buf, strlen(buf) + 1);

  return b->data;
}

void free(void *ptr)
{
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
		brk(b);

	}


  }
  // We can't use printf here because printf internally calls `malloc` and thus
  // we'll get into an infinite recursion leading to a segfault.
  // Instead, we first write the message into a string and then use the `write`
  // system call to display it on the console.
  char buf[1024];
  //snprintf(buf, 1024, "%s:%d free(%p): Freeing %zu bytes from",
  //         __FILE__, __LINE__,ptr, b->size);
  //write(STDOUT_FILENO, buf, strlen(buf) + 1);
}



void *realloc(void *ptr, size_t size)
{
  // Allocate new memory (if needed) and copy the bits from old location to new.
  size_t s;
  t_block b, new;
  void *newp;
  if(!ptr)
    return (malloc(size));
  if(valid_addr(ptr))
  {

	s = align4(size);
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
			newp = malloc(s);
			if(!newp)
			return NULL;
			new = get_block(newp);
                        memcpy(new->ptr, b->ptr, b->size);
			//copy_block(b,new);
			free(ptr);
			return (newp);
		}
	}


    return (ptr);
  }
  return NULL;
}

int main()
{

  printf("%ld\n",pagealign);
  int input,n;
  int count = 0;
  int* numbers = NULL;
  int* more_numbers = NULL;

  do {
     printf ("Enter an integer value (0 to end): ");
     scanf ("%d", &input);
     count++;

     more_numbers = (int*) realloc (numbers, count * sizeof(int));

     if (more_numbers!=NULL) {
       numbers=more_numbers;
       numbers[count-1]=input;
     }
     else {
       free (numbers);
       puts ("Error (re)allocating memory");
       exit (1);
     }
  } while (input!=0);

  printf ("Numbers entered: ");
  for (n=0;n<count;n++) printf ("%d ",numbers[n]);
  free (numbers);

  return 0;
}
