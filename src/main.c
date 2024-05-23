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
	Token* tokens = NULL;
	size_t nb_tokens = 0;//parse_line("test", &tokens);
	while(1){
		printf("> ");
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
			char** args = calloc(sizeof(char*), nb_tokens+1);
			for(int i = 0; i <= nb_tokens; i++){
				args[i] = tokens[i].val;
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
	char green[20] = {0};
	unsigned char rgb[3] = {0, 255, 0};
	create_color(rgb, green);
	printf("%s", green);
	pwd("");
	printf("%s", COLOR_RESET);
	ash_loop();
	return EXIT_SUCCESS;
}
