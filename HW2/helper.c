#include <sys/mman.h>
#include "helper.h"

      

void parse_line(char line[150], struct MemoryRegion* layout)
{
  char *pch = NULL;
  char *pchadd = NULL;
  char *permp = NULL;
  char *offset = NULL;
  layout->isReadable = PROT_NONE;
  layout->isWriteable = PROT_NONE;
  layout->isExecutable = PROT_NONE;
  layout->startAddr = 0;
  layout->endAddr = 0;
  layout->size = 0;
  layout->offset = 0;

  pch = strtok(line," ");
  pchadd = pch;
  pch = strtok(NULL," ");
  permp = pch;
  int i;
  for(i = 0; i < 3; i++)
  {
    if(*permp == 'r')
       layout->isReadable = PROT_READ;
    else if(*permp == 'w')
       layout->isWriteable = PROT_WRITE;
    else if(*permp == 'x')
       layout->isExecutable = PROT_EXEC;
    permp++;
  }
  pch = strtok(NULL," ");
  offset = pch; 
  layout->offset = (off_t)strtol(offset,NULL, 16);
  pchadd = strtok(pchadd," ");
  pchadd = strtok(pchadd,"-");
  layout->startAddr = strtol(pchadd, NULL, 16);
  pchadd = strtok(NULL, "-");
  layout->endAddr = strtol(pchadd, NULL, 16);
  layout->size = layout->endAddr - layout->startAddr;
}


long unsigned int mtcp_strtol (char *str_ptr)
{
  long unsigned int v = 0;
  int base = 16;
  char* str = str_ptr;
  while (*str != '\0') {
    long unsigned int c;
    if ((*str >= '0') && (*str <= '9')) c = *str - '0';
    else if ((*str >= 'a') && (*str <= 'f')) c = *str + 10 - 'a';
    else if ((*str >= 'A') && (*str <= 'F')) c = *str + 10 - 'A';
    v = v * base + c;
    str++;
  }
  return v;
}


