#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>


#define MAX_HEAP  64 * 1024 * 1024 
#define BLOCK_SIZE 48
#define pagealign sysconf(_SC_PAGESIZE)
extern void *base;
extern long int total_no_of_blocks;
extern long int used_blocks;
extern long int free_blocks;
extern long int total_allocation_requests;
extern long int total_free_requests;

extern void *heap;
extern void *brkp;
extern void *endp;
typedef struct MallocHeader *t_block;

struct MallocHeader
{
  size_t size;
  int mapped;
  struct MallocHeader* next;
  struct MallocHeader* prev;
  int free;
  void *ptr;
  char data[1];
};



t_block get_block(void *p);
int valid_addr(void *p);
t_block fusion(t_block b);
void split_block(t_block b, size_t s);
t_block find_block(t_block *last, size_t size);
t_block extend_heap(t_block last, size_t s);
size_t align8(size_t s);
void* request_space(size_t size);

