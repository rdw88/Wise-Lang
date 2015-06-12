#ifndef PARSER_H__
#define PARSER_H__

#include <stddef.h>

typedef enum {
	ERR,
	INT,
	ADD,
	SUB,
	MULT,
	DIV,
	VAR,
	ASN,
	REASN,
	PRINT
} exp_type;


typedef struct Type {
	exp_type type;
	void *type1;
	void *type2;
} Type;


char** tokenize(char *expression);

Type* parseIntExpression(char **expression, size_t length, int *err);

Type* parseVarExpression(char **expression);

Type* parse(char **expression, size_t length);

#endif