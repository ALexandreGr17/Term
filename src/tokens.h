#ifndef TOKENS_H
#define TOKENS_H

typedef struct Token Token;
char** conf_read_line(char* a, int size);
Token** lexer(char** line, int size);

#endif
