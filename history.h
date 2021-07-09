#include <stdio.h>
#include <stdlib.h>

void bi_history(char** arguments, char** historical, int hist_head, int hist_count){
  int counter = 0;
  if(arguments[1] == NULL){
    if(hist_count >= 10){
      for(int i = (hist_count - 10); i < 19; i++)
        printf("%s\n", historical[i]);
    }
    else{
      for(int i = 0; counter < 10; (i = (i+1)%20), counter++)
        printf("%s\n", historical[i]);
    }
  }
  else{
    if(atoi(arguments[1]) <= 20){
      if(atoi(arguments[1]) <= hist_count){
        if(hist_count <= 20){
          for(int i = (hist_count - atoi(arguments[1])); counter < atoi(arguments[1]); (i = (i+1)%20), counter++){
            printf("%s\n", historical[i]);
          }
        }
        else{
          for(int i = ((hist_head == 19)? 0 : (hist_head+1)); counter < atoi(arguments[1]); (i = (i+1)%20), counter++){
            printf("%s\n", historical[i]);
          }
        }
      }
    }
    else
      fprintf(stderr, "Entered number larger than number of previous commands.\n");
  }
}
