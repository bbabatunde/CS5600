#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<ucontext.h>
#include <signal.h>
#include "helper.h"


void handle_sigint(int sig)
{
 
    if(sig == SIGUSR2)
    {  
    	processMapsFile();
    }
}

__attribute__ ((constructor))
void myconstructor() 
{
  signal(SIGUSR2, handle_sigint);
}

void writeToImage(struct MemoryRegion layout, int filedesc)
{ 
 
 if(layout.isReadable)
 { 
   if(layout.startAddr < 0)
         return;
  char *hex = (char *)layout.startAddr;
  unsigned long int size = 0;
  char value;
  write(filedesc,&layout, sizeof(struct MemoryRegion));
  while( size < layout.size)
   {	value = *hex;
  	write(filedesc,&value , sizeof (char));
        hex++;
        size++;

   }
  }
}


void readFile(int fd, int filedesc) 
{
    char buffer[150];
    int bytes_read;
    int k = 0;
    struct MemoryRegion layout;

    do {
        char t = 0;
        bytes_read = read(fd, &t, 1); 
        buffer[k++] = t;    
        if( t == '\n') 
	{
            parse_line(buffer, &layout);
            writeToImage(layout, filedesc);
            for(int i=0; i<150; i++) 
                buffer[i]='\0';
            k = 0;
           
        }
    }
    while (bytes_read != 0);
}


void processMapsFile(void)
{
   char path[100];
   snprintf(path, 100,"/proc/%s/maps","self");
   ucontext_t mycontext;
   int mapf = open(path,O_RDONLY);
   //int contextflag = 1;
   int filedesc = open("myckpt", O_CREAT | O_RDWR, 0666);
   if(getcontext(&mycontext) == -1)
   {
      printf("getcontext failed\n");
   }
   
   write(filedesc,&mycontext, sizeof (ucontext_t));
   readFile(mapf, filedesc);
   close(mapf);
   close(filedesc);
  

}

