#include <stdio.h>
#include <stdlib.h>
#include "debug_stat.h"

void bi_pwd(){
  char* working_directory = (char *) malloc(1024);
  getcwd(working_directory, 1024);
  printf("%s\n", working_directory);
  free(working_directory);
}
