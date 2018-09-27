#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<ucontext.h>
#include <signal.h>



struct MemoryRegion
{
  unsigned long int startAddr;
  unsigned long int endAddr;
  unsigned long int size;
  off_t offset;
  unsigned  long int isReadable;
  unsigned long int isWriteable;
  unsigned long int isExecutable;
};



void writeContext();
void readImageFile(int fd);
void writeToImage(struct MemoryRegion layout, int);
void processMapsFile(void);
void parse_line(char line[150], struct MemoryRegion*);
long unsigned int mtcp_strtol(char*);
void handle_sigint(int sig);

