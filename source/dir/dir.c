#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

#include "dir.h"

int createDir(const char *sPathName)  
  {  
	char dirName[256];  
    strcpy(dirName, sPathName);  

	if(mkdir(dirName, MODE)==-1)  
	{   
		  printf("mkdir   error\n");   
		  return -1;   
	}

	return 0;  
  } 
