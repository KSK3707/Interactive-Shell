#ifndef __PROMPT_H
#define __PROMPT_H
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug_stat.h"

 void prompt(int flag){
	char* hostname = (char *) malloc(1024);
	gethostname(hostname, 1024);
	char* directoryname = (char *) malloc(1024);
	getcwd(directoryname, 1024);
  if(flag == 1)
 	  printf("<%s@%s:%s>", getlogin(), hostname, directoryname);
  else
    printf("<%s@%s:~>", getlogin(), hostname);
 	free(hostname);
 	free(directoryname);
}

#endif
