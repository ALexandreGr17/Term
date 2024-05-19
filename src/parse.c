#include "parse.h"
#include "builtin.h"
#include <stdio.h>


size_t parse_line(const char* line, Token** tokens_out){
	Token* tokens = malloc(sizeof(Token) * 10);
	size_t nb_tokens = 10;
	size_t i = 0;
	tokens[i].type = NONE;
	tokens[i].val = calloc(100, 1);
	size_t nb_char = 100;
	size_t j = 0;
	while(*line && *line != '\n'){
		if(*line == ' '){
			if(tokens[i].type != NONE){
				tokens[i].val[j] = 0;
				
				tokens[i].index = get_builtin(tokens[i].val);
				if(tokens[i].index >= 0){
					tokens[i].type = BUILTIN;
				}
				
				i++;
				if(i >= nb_tokens){
					nb_tokens += 10;
					tokens = realloc(tokens, sizeof(Token) * nb_tokens);
				}
				tokens[i].type = ARG;
				tokens[i].val = calloc(100, 1);
				nb_char = 100;
				j = 0;
			}

		}
		else{
			if (tokens->type == NONE) {
				tokens->type = PROG;
			}
			if(j >= nb_char){
				nb_char += 100;
				tokens[i].val = realloc(tokens[i].val, nb_char);
			}
			tokens[i].val[j] = *line;
			j++;
		}
		line++;
	}
	tokens[i].index = get_builtin(tokens[i].val);
	if(tokens[i].index >= 0){
		tokens[i].type = BUILTIN;
	}
	*tokens_out = tokens;
	return i;
}
