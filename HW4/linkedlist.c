#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "linkedlist.h"


size_t align8(size_t s)
{
  if((s &0x7) == 0)
	return s;
   return((s>>3) + 1) <<3;


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
   b = sbrk(0);
   if(sbrk(BLOCK_SIZE + s) == (void *) 1)
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





