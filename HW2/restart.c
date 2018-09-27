#include<stdio.h>
#include <sys/mman.h>
#include<string.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "helper.h"

char ckpt_image[1000];
static ucontext_t mycontext;
void readImageFile(int fd)
{
 
  struct MemoryRegion layout;
  read(fd, &mycontext, sizeof(ucontext_t));
  ssize_t read_val = 0;
  do
  {
  read_val = read(fd, &layout, sizeof(struct MemoryRegion));
  unsigned long int* pu;
  pu = mmap((void *)layout.startAddr, layout.size,\
		 layout.isReadable |PROT_WRITE |layout.isExecutable,
		MAP_PRIVATE|MAP_ANONYMOUS,-1, 0);
   if(pu == MAP_FAILED)
    {
     printf("failed\n");
    }
    char buffer;
    unsigned long int size = 0;
    unsigned long int hex = 0; 	  
    while(size < layout.size)
    {
    	read(fd,&buffer,sizeof(char));
	hex = strtol(buffer, NULL,16);
        *pu = hex;
    	size++;
    }
  }while( read_val != 0);
  setcontext(&mycontext);
  exit(0);
}


void FindStack(char *word , char *file){
   char line[150] ;
   struct MemoryRegion stack;
   FILE* fp = fopen(file, "r") ;
   while (fgets(line , sizeof(line) , fp )!= NULL)
   {
      if (strstr(line , word)!= NULL)
      {
        parse_line(line, &stack);
        int mu =  munmap((void *)stack.startAddr, stack.size);
         if( mu == -1)
             printf("mum map failed\n");
        fclose(fp);
        break;
     }
   }
   
}

void restore_memory()
{
    char path[40];
    char word[50] = "stack";
    snprintf(path, 40,"/proc/%s/maps","self");
    FindStack(word, path);
    int filedesc = open(ckpt_image, O_RDONLY);
    readImageFile(filedesc);
    close(filedesc);
}

int main(int argc, char **argv)
{

    for(int i = 0; i < 6; i++)
        ckpt_image[i] = argv[1][i];

   void  *new_stack_loc = (void *) 0x5300000;
   size_t new_stack_size = 0x1000;
   void    *pu;
   pu = mmap(new_stack_loc, new_stack_size, PROT_READ|PROT_WRITE|PROT_EXEC, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
   if (pu == MAP_FAILED)
        printf("mmap failure\n"); 
   void *stack_ptr = pu + new_stack_size;
   asm volatile ("mov %0,%%rsp" : : "g" (stack_ptr) : "memory");
   restore_memory();

}
