#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>

#include "parser.h"
#include "helper.h"


#define TOKEN_ASSIGNMENT "is"
#define TOKEN_INTEGER "integer"
#define TOKEN_ADD "+"
#define TOKEN_SUBTRACT "-"
#define TOKEN_MULT "*"
#define TOKEN_DIVIDE "/"
#define TOKEN_FUNCTION "fun"
#define TOKEN_BOOL_TRUE "true"
#define TOKEN_BOOL_FALSE "false"
#define TOKEN_STDOUT "show"


#define NO_ERROR -1
#define SYNTAX_ERROR 0
#define TYPE_ERROR 1


char** tokenize(char *expression) {
	char **tokens = NULL;
	char *token;
	token = strtok(expression, " ");
	int count = 0;

	while (token != NULL) {
		tokens = realloc(tokens, sizeof(char*) * ++count);

		if (tokens == NULL) {
			printf("FATAL: Memory allocation error\n");
			exit(-1);
		}

		tokens[count - 1] = token;
		token = strtok(NULL, " ");
	}

	tokens = realloc(tokens, sizeof(char*) * (count + 1));
	tokens[count] = '\0';

	return tokens;
}


Type* error(Type *t, int *err, int errno) {
	t->type = ERR;
	*err = errno;
	return t;
}


char*** splitExpression(char **expression, size_t length, int split) {
	char **subset1 = malloc(sizeof(char *) * (split + 1));
	char **subset2 = malloc(sizeof(char *) * (length - split));
	char ***result = (char ***) malloc(sizeof(char **) * 2);

	for (int i = 0; i < split; i++) {
		subset1[i] = expression[i];
	}
	subset1[split] = '\0';

	for (int i = split + 1; i < length; i++) {
		subset2[i - split - 1] = expression[i];
	}
	subset2[length - split - 1] = '\0';

	result[0] = subset1;
	result[1] = subset2;

	return result;
}


Type* varDeclare(char **expression, size_t length, int *err) {
	Type *rootType = (Type *) malloc(sizeof(Type));

	if (length < 4 || strcmp(expression[2], TOKEN_ASSIGNMENT) != 0) {
		return error(rootType, err, SYNTAX_ERROR);
	}

	Type *varType = (Type *) malloc(sizeof(Type));
	rootType->type = ASN;
	varType->type = VAR;

	size_t len = strlen(expression[1]);
	char *varName = (char *) malloc(sizeof(char) * len + 1);
	memcpy(varName, expression[1], sizeof(char) * len);
	varName[len] = '\0';

	varType->type1 = (void *) varName;

	if (strcmp(expression[0], TOKEN_INTEGER) == 0) {
		varType->type2 = (void *) INT;
	}

	size_t rValueLength = length - 3;
	char **rValue = malloc(sizeof(char *) * (rValueLength + 1));
	for (int i = 0; i < rValueLength; i++) {
		rValue[i] = expression[i + 3];
	}

	rValue[rValueLength] = '\0';

	rootType->type1 = (void *) varType;

	Type *p = parseIntExpression(rValue, rValueLength, err);
	if (p->type == ERR)
		rootType->type = ERR;

	rootType->type2 = (void *) p;

	return rootType;
}


exp_type getType(char **expression, size_t length) {
	if (strcmp(expression[0], TOKEN_INTEGER) == 0) {
		return ASN;
	} else if (strcmp(expression[0], TOKEN_STDOUT) == 0) {
		return PRINT;
	}

	if (length == 1) {
		int n = atoi(expression[0]);
		if (n == 0 && expression[0][0] != '0')
			return VAR;

		return INT;
	}

	if (strcmp(expression[1], TOKEN_ASSIGNMENT) == 0) {
		return REASN;
	} else {
		return INT;
	}
}


Type* varReassign(char **expression, size_t length, int *err) {
	Type *type = (Type *) malloc(sizeof(Type));

	if (length < 3) {
		return error(type, err, SYNTAX_ERROR);
	}

	char ***split = splitExpression(expression, length, 1);

	Type* varType = (Type *) malloc(sizeof(Type));
	type->type = REASN;
	varType->type = VAR;

	size_t len = strlen(expression[0]);
	char *varName = (char *) malloc(sizeof(char) * len + 1);
	memcpy(varName, expression[0], sizeof(char) * len);
	varName[len] = '\0';

	exp_type reasnType = getType(split[1], length);
	varType->type1 = (void *) varName;
	varType->type2 = (void *) reasnType;

	type->type1 = (void *) varType;

	if (reasnType == INT)
		type->type2 = (void *) parseIntExpression(split[1], length, err);

	return type;
}


Type* parseStdout(char **expression, size_t length, int *err) {
	Type *type = (Type *) malloc(sizeof(Type));
	type->type = PRINT;

	if (length == 1) {
		type->type1 = 0;
		type->type2 = 0;
		return type;
	}

	char **printExp = malloc(sizeof(char *) * (length - 1));
	for (int i = 1; i < length; i++) {
		printExp[i - 1] = expression[i];
	}

	Type *p = parse(printExp, length - 1);
	if (p->type == ERR)
		type->type = ERR;

	type->type1 = (void *) p;
	type->type2 = 0;

	free(printExp);
	return type;
}


Type* parseVarExpression(char **expression) {
	Type *type = (Type *) malloc(sizeof(Type));
	size_t len = strlen(expression[0]);
	char *varName = (char *) malloc(sizeof(char) * len + 1);
	memcpy(varName, expression[0], sizeof(char) * len);
	varName[len] = '\0';

	type->type = VAR;
	type->type1 = (void *) varName;
	type->type2 = (void *) INT;

	if (DEBUG)
		printf("Got to VAR LOOKUP with %s\n", expression[0]);

	return type;
}


Type* parseIntExpression(char **expression, size_t length, int *err) {
	Type *type = (Type *) malloc(sizeof(Type));

	if (length == 1) {
		if (isInt(expression[0])) {
			type->type = INT;
			int *n = (int *) malloc(sizeof(int));
			*n = atoi(expression[0]);
			type->type1 = (void *) n;
			type->type2 = 0;	
		} else {
			type = parseVarExpression(expression);
		}
		
		return type;
	}

	char *order[4] = {TOKEN_DIVIDE, TOKEN_MULT, TOKEN_SUBTRACT, TOKEN_ADD};
	exp_type types[4] = {DIV, MULT, SUB, ADD};

	for (int k = 0; k < 4; k++) {
		for (int i = 0; i < length; i++) {
			if (strcmp(expression[i], order[k]) == 0) {
				if (i == 0) {
					return error(type, err, SYNTAX_ERROR);
				}

				char ***split = splitExpression(expression, length, i);

				type->type = types[k];
				type->type1 = (void *) parseIntExpression(split[0], i, err);
				type->type2 = (void *) parseIntExpression(split[1], length - i - 1, err);

				free(split[0]);
				free(split[1]);
				free(split);
			}
		}
	}

	return type;
}


Type* parse(char **expression, size_t length) {
	exp_type expressionType = getType(expression, length);
	int *err = (int *) malloc(sizeof(int));
	*err = NO_ERROR;
	Type *type;

	if (expressionType == ASN) {
		type = varDeclare(expression, length, err);
	} else if (expressionType == PRINT) {
		type = parseStdout(expression, length, err);
	} else if (expressionType == INT) {
		type = parseIntExpression(expression, length, err);
	} else if (expressionType == VAR) {
		type = parseVarExpression(expression);
	} else {
		type = varReassign(expression, length, err);
	}

	if (*err == SYNTAX_ERROR) {
		printf("ERROR: Syntax error\n");
	}

	if (*err == TYPE_ERROR) {
		printf("ERROR: Type error\n");
	}

	free(err);

	return type;
}