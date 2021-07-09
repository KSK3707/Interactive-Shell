#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#define KDEBUG
#include "debug_stat.h"
#define NUMOFCDFLAGS sizeof(cd_flags)/sizeof(char*)

char* cd_flags[] = {
  "~",
  ".",
  ".."
};

void bi_cd(char** arguments, char* home_directory){
  myprintf(("Entered function\n"));
  char* current_directory = calloc(1024, sizeof(char));
	getcwd(current_directory, 1024);
  if(arguments[1] == NULL){
    //Current directory
    arguments[1] = current_directory;
    return;
  }
  for(int i = 0; i < NUMOFCDFLAGS; i++){
    if(strcmp(arguments[1], cd_flags[i]) == 0){
      switch(i){
        case 0:
          //Home directory
          arguments[1] = calloc((strlen(home_directory)+1), sizeof(char));
          strcpy(arguments[1], home_directory);
          break;
        case 1:
          //Current directory - Automatically defined by chdir
          break;
        case 2:
          //Parent directory - Automatically defined by chdir
          break;
      }
    }
  }
  myprintf(("Checking if directory is valid\n"));
  struct stat temp;
  if(stat(arguments[1], &temp)){
    perror("Error");
    return;
  }
  myprintf(("cd-ing\n"));
  chdir(arguments[1]);

  free(current_directory);
}
