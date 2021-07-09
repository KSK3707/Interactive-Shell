#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
//#define KDEBUG
#include "debug_stat.h"


char* get_state(char* pid_string){
  int length = strlen(pid_string) + strlen("/proc/") * strlen("/status") + 1;
  char* path_name = calloc(length,sizeof(char));
  strcat(path_name, "/proc/");
  strcat(path_name, pid_string);
  strcat(path_name, "/status");
  FILE* current_file;
  current_file = fopen(path_name, "r");
  char* state_string = calloc(100, sizeof(char));
  int counter = 0;
  while(fgets(state_string, 100, current_file) != NULL){
    if(counter == 2){
      break;
   }
   counter++;
  }
  fclose(current_file);

  free(path_name);

  return state_string;
}

char* get_memory(char* pid_string){
  int length = strlen(pid_string) + strlen("/proc/") * strlen("/statm") + 1;
  char* path_name = calloc(length,sizeof(char));
  strcat(path_name, "/proc/");
  strcat(path_name, pid_string);
  strcat(path_name, "/statm");
  FILE* current_file;
  current_file = fopen(path_name, "r");
  char* why = calloc(50, sizeof(char));
  fscanf(current_file, "%s", why);

  free(path_name);

  return why;
}

char* get_executable(char* pid_string){
  int length = strlen(pid_string) + strlen("/proc/") + strlen("/exe") + 1;
  char* current_path = calloc(length, sizeof(char));
  strcat(current_path, "/proc/");
  strcat(current_path, pid_string);
  strcat(current_path, "/exe");
  char* state_string = calloc(100, sizeof(char));
  readlink(current_path, state_string, 100);

  free(current_path);

  return state_string;
}

void bi_pinfo(char** arguments){
  pid_t pid_current;
  if(arguments[1] == NULL)
    pid_current = getpid();
  else
    pid_current = atoi(arguments[1]);

  if(kill(pid_current, 0) == -1){
    perror("Error");
    return;
  }

  myprintf(("%s   \n", "Value"));

  char* pid_string = calloc(1024,sizeof(char));
  sprintf(pid_string, "%d", pid_current);
  myprintf(("pid = %s   \n", pid_string));

  char* state_string = get_state(pid_string);
  myprintf(("%s   \n", state_string));

  char* memory_string = get_memory(pid_string);
  myprintf(("MemoryString = %s   \n", memory_string));

  char* executable_string = get_executable(pid_string);
  myprintf(("ExecutableString = %s\n", executable_string));

   //Final Print Statement
   printf("pid: %s\n%sMemory: %s\nExecutable Path: %s\n", pid_string, state_string, memory_string, executable_string);

   free(pid_string);
   free(state_string);
   free(memory_string);
   free(executable_string);
}
