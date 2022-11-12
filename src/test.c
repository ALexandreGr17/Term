#include <stdio.h>
#include <stdlib.h>


typedef struct token token;

struct token{
	char* type;
	char* val;
};

int main(void)
{
	token** tokens = malloc(sizeof(token) * 2);
	token *t = malloc(sizeof(*t));
	t->val = "test";
	tokens[0] = t;
	printf("%s", tokens[0]->val);
	return 0;
}
