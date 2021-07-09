#include <sys/wait.h>
//#define KDEBUG
#include "prompt.h"
#include "builtinheaders.h"
#define DELIMITERS " \t\r\n\a"
#define NUMOFFUNCTIONS sizeof(builtin_functnames)/sizeof(char*)


char** historical;

void hist_init(){
  historical = calloc(20, sizeof(char*));
  for(int i = 0; i < 20; i++)
    historical[i] = calloc(20, sizeof(char));
}

void hist_add(char* add_me, int* current_head, int* current_count){
  int add_here;
  if(*current_head == 0)
    add_here = 0;
  else{
    add_here = *current_count%20;
  }
  if(*current_head != 0){
    if(*current_count != 1){
      if(strcmp(historical[*current_head], add_me) != 0){
        strcpy(historical[add_here], add_me);
        *current_count += 1;
        *current_head = add_here;
      }
    }
    else{
      if(strcmp(historical[0], add_me) != 0){
        strcpy(historical[add_here], add_me);
        *current_count += 1;
        *current_head = add_here;
      }
    }
  }
  else{
    strcpy(historical[add_here], add_me);
    *current_count += 1;
    *current_head = 1;
  }
}

int cd_flag = 0;

char* builtin_functnames[] = {
  "echo",
  "quit",
  "cd",
  "pwd",
  "ls",
  "pinfo",
  "history",
  "setenv",
  "unsetenv"
};

char* reader(){
  char* input = NULL;
  ssize_t size = 0;
  if(getline(&input, &size, stdin) == -1){
    if(feof(stdin) == 1)
      exit(EXIT_SUCCESS);
    else{
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }
  return input;
}

/*
char ** divide_input(char* input){
  //printf("Enterd\n");
  int token_size = 0;
  int position = 0;
  int arg_count = 0;
  char **parsed_arguments = NULL;
  char *temp_input = calloc(strlen(input), sizeof(char));
  char *current_arg = NULL;

  strcpy(temp_input, input);
//  printf("Temp - %s \n Input - %s\n", temp_input, input);
  //printf ("I am done\n\n\n");

current_arg = strtok(temp_input, DELIMITERS);

while (current_arg)
{
  arg_count++;
  current_arg = strtok(NULL, DELIMITERS);
  //printf("arg_count = %d returned string: %s\n", arg_count, current_arg);
}

  //printf("%d - ArgCount1, temp_input %s, input: %s\n", arg_count, temp_input, input);

  parsed_arguments = calloc (arg_count, sizeof(char *));
  arg_count = 0;
  current_arg = parsed_arguments[arg_count] = strtok(input, DELIMITERS);
  while(current_arg){
    //printf("arg_count = %d returned string: %s\n", arg_count, current_arg);
    current_arg = parsed_arguments[++arg_count] = strtok(NULL, DELIMITERS);
  }
  //printf("%d - ArgCount2\n", arg_count);
  return parsed_arguments;
}
*/

char** copier(char** commands){
  int max_length = 0, arg_count;
  for(arg_count = 0; commands[arg_count] != NULL; arg_count++){
    if(strlen(commands[arg_count]) > max_length)
      max_length = strlen(commands[arg_count]);
  };
  max_length++;
  char* prototype = calloc(max_length, sizeof(char));
  strcpy(prototype, commands[0]);
  char** return_me = calloc(arg_count, sizeof(char*));
  for(int i = 0; i < arg_count; i++)
    return_me[i] = calloc(1, sizeof(prototype));
  int i;
  for(i = 0; commands[i] != NULL; i++){
    return_me[i] = strcpy(return_me[i], commands[i]);
  }
  return_me[i] = NULL;
  /*
  for(int j = 0; j <= i; j++){
    printf("Checker - %d.%s\n", j, return_me[j]);
  }
  */
  return return_me;
}

char** divide_input(char* input){
  int size = 10, position = 0;
  char** arguments = calloc(size, sizeof(char*));
  char* current_arg;

  current_arg = strtok(input, DELIMITERS);
  while(current_arg != NULL){
    arguments[position] = calloc((strlen(current_arg) + 1), sizeof(char));
    strcpy(arguments[position++], current_arg);

    if(position >= size){
      size += 10;
      arguments = realloc(arguments, size*sizeof(char*));
    }
    current_arg = strtok(NULL, DELIMITERS);
  }
  arguments[position] = NULL;
  return arguments;
}

int launcher(char** arguments){
  char** commands = copier(arguments);
  int no_of_commands = 0;
  for(; commands[no_of_commands] != NULL; no_of_commands++){};

  pid_t pid, wait_pid;
  int wait_status;
  pid = fork();

  if (pid == 0) {
    myprintf(("Child pid = %d\n", pid));
    char* exit_printer = calloc((strlen(commands[0]) + 84 + strlen(" with pid  exited normally\n") + 1), sizeof(char));
    sprintf(exit_printer, "%s with pid %d exited normally\n", commands[0], getpid());
    if(commands[no_of_commands - 1][0] == '&'){
      pid_t daemon = fork();
      if(daemon == 0){
        if (execvp(commands[0], commands) == -1) {
          perror("Error");
        }
        exit(EXIT_SUCCESS);
      }
      else if(daemon < 0)
              perror("Error");
      else{
       do{
          wait_pid = waitpid(pid, &wait_status, WUNTRACED);
        }while (!WIFEXITED(wait_status) && !WIFSIGNALED(wait_status));
        printf("%s\n", exit_printer);
        free(exit_printer);
      }
      exit(EXIT_SUCCESS);
    }
    else{
      if (execvp(commands[0], commands) == -1) {
        perror("Error");
      }
      exit(EXIT_SUCCESS);
    }
  }
  else
  {
    if (pid < 0) {
    perror("Error");
  }
  else{
    if(commands[no_of_commands - 1][0] == '&'){
      signal(SIGCHLD,SIG_IGN);
      return 1;
    }
    else{
      if(commands[no_of_commands - 1][0] == '&'){
        return getpid();
      }
      else{
        do {
          myprintf(("Parent pid = %d\n", pid));
          wait_pid = waitpid(pid, &wait_status, WUNTRACED);
        } while (!WIFEXITED(wait_status) && !WIFSIGNALED(wait_status));
      }
    }
  }
}
  free(commands);
  return 1;
}

int execute(char** commands, char* home_directory, int hist_head, int hist_count){
  if(commands[0] == 0)
    return 1;

  int flag = 0;

  for(int i = 0; i < NUMOFFUNCTIONS; i++){
    if(strcmp(commands[0], builtin_functnames[i]) == 0){
      flag = 1;
      switch(i){
        case 0:
          bi_echo(commands);
          break;
        case 1:
          bi_exit(commands);
          break;
        case 2:
          cd_flag = 1;
          bi_cd(commands, home_directory);
          break;
        case 3:
          bi_pwd(commands);
          break;
        case 4:
          bi_ls(commands, home_directory);
          break;
        case 5:
          bi_pinfo(commands);
          break;
        case 6:
          bi_history(commands, historical, hist_head, hist_count);
          break;
        case 7:
          bi_setenv(commands);
          break;
        case 8:
          bi_unsetenv(commands);
          break;
      }
    }
  }
  if(flag == 0)
    return launcher(commands);
}

int looper(){
  char* input, **commands;
  myprintf(("Previous - %s\n", commands[0]));
  int status, hist_head = 0, hist_count = 0;
  char* home_directory = calloc(1024, sizeof(char));
  getcwd(home_directory, 1024);
  hist_init();
  do{
    prompt(cd_flag);
    input = reader();
    commands = divide_input(input);
    hist_add(commands[0], &hist_head, &hist_count);
    status = execute(commands, home_directory, hist_head, hist_count);
    myprintf(("Now freeing\n"));
    free(input);
    free(commands);
  }while(status);
  return status;
  free(historical);
}
