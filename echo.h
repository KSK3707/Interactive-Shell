#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug_stat.h"

void bi_echo(char** arguments) {
	int i = 1;
	while(arguments[i] != NULL){
		printf("%s ", arguments[i++]);
	}
	printf("\n");
}
