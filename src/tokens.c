#include <stdlib.h>
#include <stdio.h>
#include "tokens.h"

typedef struct Token Token;

struct Token{
	char* type;/*type of the token*/
	char* val;/*value of the token*/
};
/*split line with th separation char ": ;"*/
char** conf_read_line(char* a, int size){
	int bufsize = 10;
	char* buffer = malloc(bufsize * sizeof(char));
	char** block =  malloc(2 * sizeof(char *));
	int bpos = 0;
	int pos = 0;

	if(!buffer || !block){
		fprintf(stderr, "ash lexer: allocation probleme");
		exit(EXIT_FAILURE);
	}

	if(a[size-1] != ';'){
		fprintf(stderr, "ash config: statement must end with ;");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < size; i++){
		if(a[i] == ':' || a[i] == ';'){
			block[bpos] = malloc(10 * sizeof(char));
			for(int j = 0; j < 10; j++){
				block[bpos][j] = buffer[j];
				buffer[j] = '\0';
			}
			pos = 0;
			bpos++;
		}
		else{
			buffer[pos] = a[i];
			pos++;
		}
	}
	return block;

}


Token** lexer(char** line, int size){
	Token** tokens = malloc(sizeof(Token*) * 2);	
	if(size == 2){
		Token* top = malloc(sizeof(Token));
		top->type="Option";
		top->val=line[0];

		Token* tval = malloc(sizeof(Token));
		tval->type="Valeur";
		tval->val=line[1];

		tokens[0] = top;
		tokens[1] = tval;
	}
	else{
		fprintf(stderr, "ash lexer: syntax error");
		exit(EXIT_FAILURE);
	}
	return tokens;
}

/*
int main(void){
	char t[] = "TextColor : Blue;";
	char** line = conf_read_line(t, sizeof(t)/sizeof(t[0])-1);
	Token** tokens = lexer(line, 2);
	printf("%s : %s\n", tokens[0]->type, tokens[0]->val);
	printf("%s : %s\n", tokens[1]->type, tokens[1]->val);
	return 0;
}
*/
