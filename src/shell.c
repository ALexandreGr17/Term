#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tokens.h"



#define ASH_RL_BUFFERSIZE  1024 /*size of the buffer in ash_read_line() */
#define ASH_TOK_BUFFERSIZE  1024 /*size of the buffer in ash_split_line()*/
#define ASH_TOK_DELIM  " \t\r\n\a" /*demarcation for token in ash_split_line()*/
#define ASHRC_BUFFERSIZE 1024 /*size of the buffer in get_conf_file*/


typedef struct tuple2 tuple2;
typedef struct Token Token;
typedef struct tuple tuple;
struct tuple{
	char* buffer;
	int bufsize;
};
struct Token{
	char* type;
	char* val;
};

struct tuple2{
	tuple** lines;
	int size;
};



char* pathAbs; /*path of the curent file*/
char* color_path;
char* color_Answer;
char* commandChar = ">";
/*declaration of the builtin fonction*/
int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);


void change_PathColor(char* color);
void change_AnswerColor(char* color);
void change_CommandLine(char* car);
/*name of the builtin fonction*/
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

/*list of the builtin fonction*/
int (*builtin_func[]) (char **) = {
	&ash_cd,
	&ash_help,
	&ash_exit
};


char *conf_str[] = {
	"PathColor",
	"AnswerColor",
	"CommandLine"
};

void (*conf_func[]) (char*) = {
	&change_PathColor,
	&change_AnswerColor,
	&change_CommandLine
};

void Color(char* color)
{
	if(strcmp(" GREEN", color) == 0){
		printf("\033[0;32m");
	}
	else if(strcmp(" BLUE", color) == 0){
		printf("\033[0;34m");
	}
	else if(strcmp(" RED", color)== 0){
		printf("\033[0;31m");
	}
}

void change_PathColor(char* color){
	color_path = color;
}

void change_AnswerColor(char* color){
	color_Answer = color;
}

void change_CommandLine(char* car){
	commandChar = car;
}


void reset()
{
	printf("\033[0m");
}




/*read and return command line*/
char* ash_read_line(void){
	int buffsize = ASH_RL_BUFFERSIZE; /*size of the buffer*/
	int pos = 0;
	char* buffer = malloc(sizeof(char) * buffsize);
	int c;

	/*if the buffer dosen't exist*/
	if(!buffer){
		fprintf(stderr, "ash readline: allocation error\n");
		exit(EXIT_FAILURE);
	}


	/*recovery of the line*/
	while(1){
		c = getchar();

		/*stop the loop when end of the line*/
		if(c == EOF || c == '\n'){
			buffer[pos] = '\0';
			return buffer;
		}
		else{
			buffer[pos] = c; /*stock the line in the buffer*/
		}

		pos++;
		/*resize  the buffer if needed*/
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

/*parse the command line*/
char** ash_split_line(char *line){
	int buffsize = ASH_TOK_BUFFERSIZE; /*size of the buffer*/
	char** tokens = malloc(sizeof(char*) * buffsize);
	char* token;
	int pos = 0;

	/*if the buffer dosen't exist*/
	if(!tokens){
		fprintf(stderr, "ash split line: allocation error\n");
		exit(EXIT_FAILURE);
	}

	/*creating the first token with the delimitation*/
	token = strtok(line, ASH_TOK_DELIM);
	while(token != NULL){
		tokens[pos] = token;
		pos++;

		/*resizing the buffer if needed*/
		if(pos >= buffsize){
			buffsize += ASH_TOK_BUFFERSIZE;
			tokens = realloc(tokens, buffsize * sizeof(char *));
			if(!tokens){
				fprintf(stderr, "ash split line: reallocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		/*create the next token*/
		token = strtok(NULL, ASH_TOK_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}


/*launch the unix command*/
int ash_launch(char** args){
	pid_t pid, wpid;
	int status;
	pid = fork();/*get pid*/
	if(pid == 0){ /*it child the execute the command*/
		if(execvp(args[0], args) == -1){
			printf("ash: command not found\n");
		}
		exit(EXIT_FAILURE);
	}

	else if(pid < 0){/*if pid error*/
		printf("ash: command not found\n");
		perror("ash");
	}
	else{/*if parrent wait child*/
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
	}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}



/*nb of builtin function*/
int ash_num_builtin(){
	return sizeof(builtin_str) / sizeof(char *);
}

/*function cd to change directorie*/
int ash_cd(char** args){
	if(args[1] == NULL){/*if 0 orgument given*/
		fprintf(stderr, "ash: expected argument to \"cd\"\n");
	}else{
		if(chdir(args[1]) != 0){/*change file and check for eventual error*/
			perror("ash");
		}
	}
	getcwd(pathAbs, 1024);/*change the path of the current directorie*/
	return 1;
}

/*help fuction*/
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
/*exit function*/
int ash_exit(char** args){ return 1;}






/*seperate builtin function and unix function*/
int ash_execute(char** args){
	if(args[0] == NULL){
		return 1;
	}
	for(int i = 0; i < ash_num_builtin(); i++){/*check if te function is builtin*/
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
		}
	}
	return ash_launch(args);
}


/*get the command pars it and execute it*/
void ash_loop(void){
	char *line;
	char** args;
	int status;

	do{
		Color(color_path);/*change color of the curent directorie*/
		printf("%s\n", pathAbs);
		reset();
		printf("	%s",commandChar);
		Color(color_Answer);
		line = ash_read_line();
		args = ash_split_line(line);
		status = ash_execute(args);

		free(line);
		free(args);
	}while(status);
}

tuple2* split_line_config(char* config, int size){
	int bufersize = ASHRC_BUFFERSIZE;
	int linessize = 1;
	char* buffer = malloc(sizeof(char) * bufersize);
	tuple** lines = malloc(sizeof(tuple) * linessize);
	int position = 0;
	int lineNb = 0;

	for(int i = 0; i < size; i++){
		if(config[i] == '\n' || i+1 == size){
			tuple *line = malloc(sizeof(*line));
			line->buffer = malloc(sizeof(char) * position);
			for(int j = 0; j <= position; j++){
				line->buffer[j] = buffer[j];
				buffer[j] = '\0';
			}
			line->bufsize = position;
			position = 0;
			lines[lineNb] = line;
			lineNb++;
			if(lineNb >= linessize){
				linessize++;
				lines = realloc(lines, linessize * sizeof(tuple));
				if(!lines){
					fprintf(stderr, "ash split line : allocation error");
					exit(EXIT_FAILURE);
				}
			}

		}
		else{
			buffer[position] = config[i];
			position++;

			if(position >= bufersize){
				bufersize += ASHRC_BUFFERSIZE;
				buffer = realloc(buffer, bufersize * sizeof(char));

				if(!buffer){
					fprintf(stderr, "ash split line : reallocation error");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	tuple2* alLines = malloc(sizeof(*alLines));
	alLines->lines = lines;
	alLines->size = lineNb;
	return alLines;
}


/*read config file*/
tuple* get_conf_file(void){
	int bufersize =  ASHRC_BUFFERSIZE;/*size of the buffer*/
	char* buffer = malloc(bufersize * sizeof(char));
	int position = 0;
	char ch;

	if(!buffer){
		fprintf(stderr, "ash read conf: alocation error");
		exit(EXIT_FAILURE);
	}

	FILE *fp = fopen(".ashrc", "r");/*get the config file*/
	if(fp != NULL){
		while((ch = fgetc(fp)) != EOF){/*read the config file until their is an EOF char*/
			buffer[position] = ch;
			position++;

			/*resizing the buffer if needed*/
			if(position >= bufersize){
				bufersize += ASHRC_BUFFERSIZE;
				buffer = realloc(buffer, bufersize * sizeof(char));
				if(!buffer){
					fprintf(stderr, "ash read conf : allocation error");
					exit(EXIT_FAILURE);
				}
				
			}
		}/*if the file is found return the buffer*/
		tuple *tu = malloc(sizeof(*tu));
		tu->buffer = buffer;
		tu->bufsize = position-1;
		return tu;
	}/*if the file isn't found return NULL*/
	return NULL;
}




void ash_init(void){
	tuple* config = get_conf_file();
	tuple2* lines = split_line_config(config->buffer, config->bufsize+1);
	for(int i = 0; i < lines->size; i++){
		char** block_conf = conf_read_line(lines->lines[i]->buffer, lines->lines[i]->bufsize);
		Token** tokens = lexer(block_conf, 2);
		for(int j = 0;  j < sizeof(conf_str)/sizeof(conf_str[0]); j++){
			//printf("%s : %s\n", tokens[0]->val, conf_str[j]);
			if(strcmp(tokens[0]->val, conf_str[j]) == 0){
			//	printf("here %s\n", conf_str[j]);
				conf_func[j](tokens[1]->val);
			}
		}
	}
}



int main(void)
{
	ash_init();
	pathAbs = malloc(1024 * sizeof(char));
	getcwd(pathAbs, 1024);
	ash_loop();
    	return EXIT_SUCCESS;
}
