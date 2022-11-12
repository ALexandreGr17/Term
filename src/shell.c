#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#define ASH_RL_BUFFERSIZE  1024
#define ASH_TOK_BUFFERSIZE  1024
#define ASH_TOK_DELIM  " \t\r\n\a"

char* pathAbs;

int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);



char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&ash_cd,
	&ash_help,
	&ash_exit
};




char* ash_read_line(void){
	int buffsize = ASH_RL_BUFFERSIZE;
	int pos = 0;
	char* buffer = malloc(sizeof(char) * buffsize);
	int c;

	if(!buffer){
		fprintf(stderr, "ash readline: allocation error\n");
		exit(EXIT_FAILURE);
	}


	while(1){
		c = getchar();

		if(c == EOF || c == '\n'){
			buffer[pos] = '\0';
			return buffer;
		}
		else{
			buffer[pos] = c;
		}

		pos++;

		if(pos >= buffsize){
			buffsize += ASH_RL_BUFFERSIZE;
			buffer = realloc(buffer, buffsize);
			if(!buffer){
				fprintf(stderr, "ash readline: realocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


char** ash_split_line(char *line){
	int buffsize = ASH_TOK_BUFFERSIZE;
	char** tokens = malloc(sizeof(char*) * buffsize);
	char* token;
	int pos = 0;

	if(!tokens){
		fprintf(stderr, "ash split line: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, ASH_TOK_DELIM);
	while(token != NULL){
		tokens[pos] = token;
		pos++;

		if(pos >= buffsize){
			buffsize += ASH_TOK_BUFFERSIZE;
			tokens = realloc(tokens, buffsize * sizeof(char *));
			if(!tokens){
				fprintf(stderr, "ash split line: reallocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, ASH_TOK_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}


int ash_launch(char** args){
	pid_t pid, wpid;
	int status;
	pid = fork();
	if(pid == 0){
		if(execvp(args[0], args) == -1){
			perror("ash");
		}
		exit(EXIT_FAILURE);
	}

	else if(pid < 0){
		perror("ash");
	}
	else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}




int ash_num_builtin(){
	return sizeof(builtin_str) / sizeof(char *);
}


int ash_cd(char** args){
	if(args[1] == NULL){
		fprintf(stderr, "ash: expected argument to \"cd\"\n");
	}else{
		if(chdir(args[1]) != 0){
			perror("ash");
		}
	}
	getcwd(pathAbs, 1024);
	return 1;
}


int ash_help(char** args){
	printf("Alexandre GRANDON's ash\n");
	printf("Type name and arguments, and hit enter .\n");
	printf("the following are builtin\n");

	for(int i = 0; i < ash_num_builtin(); i++){
		printf("   %s\n", builtin_str[i]);
	}
	printf("use man for infomation on other program\n");
	return 1;
}

int ash_exit(char** args){ return 1;}







int ash_execute(char** args){
	if(args[0] == NULL){
		return 1;
	}
	for(int i = 0; i < ash_num_builtin(); i++){
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
		}
	}
	return ash_launch(args);
}



void ash_loop(void){
	char *line;
	char** args;
	int status;

	do{
		printf("%s\n", pathAbs);
		printf("	->");
		line = ash_read_line();
		args = ash_split_line(line);
		status = ash_execute(args);

		free(line);
		free(args);
	}while(status);
}


int main(void)
{
	pathAbs = malloc(1024 * sizeof(char));
	getcwd(pathAbs, 1024);
	ash_loop();
    return EXIT_SUCCESS;
}
