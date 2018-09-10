#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>

struct MemoryRegion
{
  void *startAddr;
  size_t size;
  int isReadable;
  int isWriteable;
  int isExecutable;

};

struct MemoryRegion calculate_memory_size(char line[150], int index)
{

  struct MemoryRegion reg; // = malloc(sizeof *reg);
  char* pch_mem;
  pch_mem = strtok(line,"-");
  reg.startAddr = pch_mem;
  int mem_start = atoi(pch_mem);
  int mem_end = atoi(strtok(NULL,"-"));
  int mem_size = mem_end - mem_start;
  reg.size = mem_size;
  if(index == 0)
  {
     reg.isReadable = 1;
     reg.isWriteable = 0;
     reg.isExecutable = 1;
  }
  else if (index == 1){
     reg.isReadable = 1;
     reg.isWriteable - 0;
     reg.isExecutable = 0;
  }
  else{
     reg.isReadable = 1;
     reg.isWriteable = 1;
     reg.isExecutable = 0;
  }
  return reg; 
}

struct MemoryRegion parse_line(char line[150], int index)
{
  struct MemoryRegion reg; // = malloc(sizeof* reg);
  char *pch_line;
  int mem_size;
  pch_line = strtok(line," ");
  while (pch_line != NULL)
  {
	reg = calculate_memory_size(pch_line, index);
	pch_line = strtok(NULL, " ");
  }
 
  return reg;
}

void get_memory_size(int pid)
{
  int mem_size;
  char path[40], line[150];
  FILE* mapf;
  FILE* memInfo = fopen("processmaps","w");
  snprintf(path, 40,"/proc/%d/maps",pid);
  int index = 0;
  if(access(path, F_OK) == -1){
        snprintf(path,40,"/proc/%s/maps","self");
	mapf = fopen(path, "r");
  }
  else{ 	
	mapf = fopen(path, "r");
  }

   while(fgets(line, sizeof(line), mapf)){
     if (index == 3){
	      break;
	}
	//printf("%s",line);
	struct MemoryRegion region; // =  malloc(sizeof *region);
	region = parse_line(line, index);
	if( memInfo != NULL){
	  fprintf(memInfo,"Start Addr = %s Size =  %ld Read = %d Write = %d Executable = %d \n",\
		 (uint8_t*)region.startAddr, region.size, \
		 region.isReadable, region.isWriteable, region.isExecutable );
        } 
	printf("index = %d Memory Size = %ld\n",index,  region.size);	
	++index;
	}	
     
   fclose(memInfo);
   fclose(mapf);
}


int main(int argc, char *argv[])
{
   int pid;
   pid = atoi(argv[1]);
   get_memory_size(pid);

};
