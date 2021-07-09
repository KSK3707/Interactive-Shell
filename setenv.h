#include <stdio.h>
#include <stdlib.h>
#include "tracer.h"

void bi_setenv(char **arguments){
  if(arguments[1] == NULL){
    fprintf(stderr, "Error: Not enough arguments.\n");
  }
  else if(arguments[3] != NULL){
    fprintf(stderr, "Error: Too many arguments.\n");
  }
  else{
    if(getenv(arguments[1]) != NULL){
      char *here = getenv(arguments[1]);
      char *empty = "";
      if(arguments[2] == NULL)
        strcpy(here, empty);
      else{
        if(strlen(here) >= strlen(arguments[2]))
          strcpy(here, arguments[2]);
        else{
          here = realloc(here, strlen(arguments[2]) + 1);
          strcpy(here, arguments[2]);
        }
      }
      free(here);
    }
    else{
      char* here;
      if(arguments[2] != NULL){
        here = calloc(strlen(arguments[1]) + strlen(arguments[2] + 2), sizeof(char));
        if(sprintf(here, "%s=%s", arguments[1], arguments[2]) < 0)
          perror("Error:");
        putenv(here);
      }
      else{
        here = calloc(strlen(arguments[1]) + 2, sizeof(char));
        if(sprintf(here, "%s=", arguments[1]) < 0)
          perror("Error:");
        putenv(here);
      }
      free(here);
    }
  }
}

void bi_unsetenv(char **arguments){
  if(arguments[1] == NULL)
    fprintf(stderr, "Error: Not enough arguments\n");
  else if(arguments[2] != NULL)
    fprintf(stderr, "Error: Too many arguments\n");
  else{
    char *here = getenv(arguments[1]);
    if(here == NULL)
      fprintf(stderr, "Error: Invalid variable name\n");
    here = NULL;
    here = getenv(arguments[1]);
    if(here != NULL){
      char *empty = "";
      strcpy(here, empty);
      free(here);
    }
  }
}
