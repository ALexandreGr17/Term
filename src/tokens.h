#ifndef TOKENS_H
#define TOKENS_H

#include <stdlib.h>

typedef enum {
	NONE = 0,
	BUILTIN = 1,
	PROG = 2,
	ARG = 3
}Tokens_type;

typedef struct {
	Tokens_type type;
	char* val;
	int index;
} Token;

#endif
