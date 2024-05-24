#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "parse.h"
#include "tokens.h"
#include "builtin.h"
#include "color.h"

unsigned char path_rgb[] = {0x98, 0x97, 0x1A};

/*
 * Get config
 * loop -> get input
 *		-> exec input
 *	cleanup
 */

// buitlins func
void (*buitlins[])(char*) = {
	cd,
	help,
	quit,
	pwd
};


void free_token_list(Token* tokens, size_t nb_tokens){
	for(size_t i = 0; i <= nb_tokens; i++){
		free(tokens[i].val);
	}
	free(tokens);
}

void ash_loop(){
	char path_color[20] = {0};
	create_color(path_rgb, path_color);
	Token* tokens = NULL;
	size_t nb_tokens = 0;//parse_line("test", &tokens);
	while(1){
		char* path = getcwd(NULL, 0);
		printf("%s%s %s> ",path_color, path, COLOR_RESET);
		free(path);
		char* line = NULL;
		size_t size = 0;
		getline(&line, &size, stdin);
		nb_tokens = parse_line(line, &tokens);
		free(line);
		// builtin func
		if(tokens[0].type == BUILTIN){
			char* args = NULL;
			if(nb_tokens >= 1){
				args = tokens[1].val;
			}
			buitlins[tokens[0].index](args);
		}
		else{
			int nb_arg = nb_tokens + 1;
			char* arg_sup = NULL;
			if(strcmp(tokens[0].val, "ls") == 0){
				nb_arg += 1;
				arg_sup = "--color";
			}
			char** args = calloc(sizeof(char*), nb_arg);
			for(int i = 0; i <= nb_tokens; i++){
				args[i] = tokens[i].val;
			}
			if(arg_sup){
				args[nb_tokens + 1] = arg_sup;
			}

			int pid = fork();

			if(pid < 0){
				printf("fork() failed\n");
				exit(EXIT_FAILURE);
			}
			else if(pid){
				int status;
				while(wait(&status) > 0);
			}
			else if(!pid && execvp(tokens[0].val, args) < 0){
				printf("Unknown command\n");
				exit(EXIT_SUCCESS);
			}

		}
		free_token_list(tokens, nb_tokens);
	}
}



int main(void){
	ash_loop();
	return EXIT_SUCCESS;
}
