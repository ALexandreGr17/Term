#include "builtin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const char* buitlins_name[] = {
	"cd",
	"help",
	"quit",
	"pwd"
};


void pwd(char* args){
	printf("%s\n", getcwd(NULL, 0));
}

void help(char *args){
	printf("Display help message\n");
}

void cd(char* args){
	if(args){
		chdir(args);
	}
}

void quit(char *args){
	exit(EXIT_SUCCESS);
}

int get_builtin(char *name){
	for(int i = 0; i < sizeof(buitlins_name)/sizeof(char*); i++){
		if(strcmp(name, buitlins_name[i]) == 0){
			return i;
		}
	}
	return -1;
}
