#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "debug_stat.h"
#define NUMOFLSFLAGS sizeof(ls_flags)/sizeof(char*)

/*
void clr_hidden(){
  //Bold blue
  printf("\033[1;34m");
}
*/

void clr_dir(){
  //Bold magenta
  printf("\033[1;31m");
}

void clr_symlnk (){
  //Bold cyan
  printf("\033[1;36m");
}

void clr_reset(){
  printf("\033[0m");
}

char* ls_flags[] = {
  ".", //0 - ls of current directory - same as ls with no flags
  "..", //1 - ls of parent directory
  "~", //2 - ls of home directory
  "-a", //3 - Show hidden files
  "-l", //4 - Show permissions
  "-al", //5 - Hidden files + permissions
  "-la" //6 - Hidden files + permissions
};

void ls_l_add(struct stat stats, int* link_flag, int* dir_flag){
  char* month_manual[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
  };
  *link_flag = 0, *dir_flag = 0;

  if(S_ISDIR(stats.st_mode)){
    printf("d");
    *dir_flag = 1;
  }
  else if(S_ISREG(stats.st_mode))
    printf("-");
  else if(S_ISLNK(stats.st_mode)){
    printf("l");
    *link_flag = 1;
  }
  else
    printf("-");

  struct tm *dt;
  char perm_current[] = "rwxrwxrwx";
  for(int i = 0; i < 9; i++){
    if((stats.st_mode & (1 << (8-i))) == 0)
      perm_current[i] = '-';
  }
  printf("%s ", perm_current);

  struct passwd * user_name = getpwuid(stats.st_uid);
  struct group * group_name = getgrgid(stats.st_gid);

  dt = localtime(&stats.st_mtime);
  printf("%ld %s %s %ld %s %d %d:%d", stats.st_nlink, (user_name?user_name->pw_name : " "), (group_name?group_name->gr_name : " "), stats.st_size, month_manual[dt->tm_mon], dt->tm_mday, dt->tm_hour, dt->tm_min);
}

int filter_func(const struct dirent * compare_me){
   if(compare_me->d_name[0] == '.')
    return 0;
  return 1;
}

void parsed_flags_ls(char** directories, int a_flag, int l_flag, int no_of_dirs){
  char* path_name;
  int link_flag = 0, dir_flag = 0;
  for(int i = 0; i < no_of_dirs; i++){
    struct stat temp;
    if(stat(directories[i], &temp)){
      perror("Error");
      return;
    }
    int num_of_files = 0;
    char* this_directory = directories[i];
    struct dirent** files_list;
    if(a_flag == 1)
      num_of_files += scandir(this_directory, &files_list, NULL, alphasort);
    else
      num_of_files += scandir(this_directory, &files_list, filter_func, alphasort);

    printf("Total = %d\n", num_of_files);

    if(l_flag == 0){
      for(int i = 0; i < num_of_files; i++)
        printf(" %s\n", files_list[i]->d_name);
    }
    else{
      for(int i = 0; i < num_of_files; i++){
        path_name = calloc((strlen(this_directory) + strlen(files_list[i]->d_name) + 2), sizeof(char));
        strcpy(path_name, this_directory);
        path_name = strcat(path_name, "/");
        path_name = strcat(path_name, files_list[i]->d_name);
        struct stat* properties = calloc(1, sizeof(struct stat));
        lstat(path_name, properties);
        ls_l_add(*properties, &link_flag, &dir_flag);
        if(link_flag == 0){
          if(dir_flag == 1)
            clr_dir();
          printf(" %s\n", files_list[i]->d_name);
          clr_reset();
        }
        else{
          char* symb_link = calloc(1000, sizeof(char));
          char* abs_path = calloc((strlen(files_list[i]->d_name) + strlen(this_directory) + 2), sizeof(char));
          strcat(abs_path, this_directory);
          strcat(abs_path, "/");
          strcat(abs_path, files_list[i]->d_name);
          readlink(abs_path, symb_link, 1000);
          if(symb_link == NULL)
            perror("Error");
          clr_symlnk();
          printf("%s", files_list[i]->d_name);
          clr_reset();
          printf(" -> %s\n", symb_link);
          free(symb_link);
          free(abs_path);
        }
        free(path_name);
      }
    }
}

}

void bi_ls(char** arguments, char* home_directory){
  int length, no_of_dirs = 0;
  for(int i = 1; arguments[i] != NULL; i++)
    no_of_dirs += 1;
  char* current_directory = calloc(1024, sizeof(char));
  getcwd(current_directory, 1024);
  int a_flag = 0, l_flag = 0, j=1;
  for(int i = 0; i < NUMOFLSFLAGS; i++){
    j = 0;
    while(arguments[j] != NULL){
      if(strcmp(arguments[j++], ls_flags[i]) == 0){
        switch(i){
          case 3:
            no_of_dirs--;
            a_flag = 1;
            break;
          case 4:
            no_of_dirs--;
            l_flag = 1;
            break;
          case 5:
          case 6:
            no_of_dirs--;
            a_flag = l_flag = 1;
            break;
        }
      }
    }
  }
  char** directories;
  char* temp;
  int free_flag = 0;
  if(no_of_dirs == 0){
    free_flag = 1;
    directories = calloc(1, sizeof(char*));
    directories[0] = current_directory;
    no_of_dirs = 1;
  }
  else{
    directories = calloc(no_of_dirs, sizeof(char*));
    int counter = 0;
    j = 1;
    while((arguments[j] != NULL) && (counter < no_of_dirs)){
      int comp_flag = 0;
      for(int i = 0; i < NUMOFLSFLAGS; i++){
        if(strcmp(arguments[j], ls_flags[i]) == 0){
          comp_flag = 1;
          switch(i){
            case 0: //current
              directories[counter] = calloc((strlen(current_directory) + 1), sizeof(char));;
              strcpy(directories[counter], current_directory);
              break;
            case 1: //parent
              temp = current_directory;
              length = strlen(current_directory);
              for(int i = (length - 1); i >=0; i--){
                if(temp[i] == '/'){
                  temp[i] = '\0';
                  break;
                }
              }
              directories[counter] = calloc(strlen(temp) + 1, sizeof(char));
              strcpy(directories[counter++], temp);
              break;
            case 2: //home
              directories[counter] = calloc((strlen(home_directory) + 1), sizeof(char));
              strcpy(directories[counter], home_directory);
              break;
          }
        }
      }

      if(comp_flag == 0){
          char* temp = calloc((strlen(arguments[j]) + 1), sizeof(char));
          strcpy(temp, arguments[j]);
          for(int i = 0; i < length; i++){
            if(temp[i] == '~'){
              comp_flag = 1;
              directories[counter] = calloc((strlen(home_directory) + strlen(&temp[i+1]) + 1), sizeof(char));
              directories[counter] = strcat(directories[counter], home_directory);
              directories[counter] = strcat(directories[counter], &temp[i+1]);
              counter++;
              break;
            }
          }
          if(comp_flag == 0){
            for(int i = 0; i < length; i ++){
              if(temp[i] == '.'){
                if(temp[i+1] != '.'){
                  comp_flag = 1;
                  directories[counter] = calloc((strlen(current_directory) + strlen(&temp[i+1]) + 1), sizeof(char));
                  directories[counter] = strcat(directories[counter], current_directory);
                  directories[counter] = strcat(directories[counter], &temp[i+1]);
                  counter++;
                  break;
                }
                else{
                  i++;
                  length = strlen(current_directory);
                  char* temp_directory = calloc((length + 1), sizeof(char));
                  strcpy(temp_directory, current_directory);
                  for(int i = (length - 1); i >=0; i--){
                    if(temp_directory[i] == '/'){
                      temp_directory[i] = '\0';
                      break;
                    }
                  }
                  comp_flag = 1;
                  directories[counter] = calloc((strlen(temp_directory) + strlen(&temp[i+1]) + 1), sizeof(char));
                  directories[counter] = strcat(directories[counter], temp_directory);
                  directories[counter] = strcat(directories[counter], &temp[i+1]);
                  counter++;
                  free(temp_directory);
                }
              }
            }
          }
          if(comp_flag == 0)
            directories[counter] = calloc((strlen(arguments[j]) + 1), sizeof(char));
          strcpy(directories[counter], arguments[j]);
          free(temp);
        }
        j++;
      }
    }
  parsed_flags_ls(directories, a_flag, l_flag, no_of_dirs);

  myprintf(("Check 2\n"));

  free(current_directory);
  if(free_flag == 0)
    free(directories);
}
