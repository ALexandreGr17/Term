#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "parse.h"
#include "tokens.h"
#include "builtin.h"

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

void ash_loop(){
	Token* tokens = NULL;
	size_t nb_tokens = parse_line("test", &tokens);
	while(1){
		free(tokens);
		printf("> ");
		char* line = NULL;
		size_t size = 0;
		getline(&line, &size, stdin);
		nb_tokens = parse_line(line, &tokens);
		
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
		free(line);
	}
}

int main(void){
	pwd("");
	ash_loop();
	return EXIT_SUCCESS;
}
