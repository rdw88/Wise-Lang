#ifndef PARSER_H__
#define PARSER_H__

#include <stddef.h>

typedef enum {
	INT,
	ADD,
	SUB,
	MULT,
	DIV,
	VAR,
	ASN,
	PRINT
} exp_type;


typedef struct Type {
	exp_type type;
	void *type1;
	void *type2;
} Type;


char** tokenize(char *expression);

Type* parse(char **expression, size_t length);

#endif