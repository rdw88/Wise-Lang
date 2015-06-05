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


Type* parse(char **expression, size_t length) {
	Type *rootType = malloc(sizeof(Type));

	if (length == 1) {
		int *n = (int*) malloc(sizeof(int));
		*n = atoi(expression[0]);

		if (*n == 0 && expression[0][0] != '0') { // treated as a variable otherwise
			size_t length = strlen(expression[0]);
			char *varName = (char *) malloc(sizeof(char) * length + 1);
			memcpy(varName, expression[0], sizeof(char) * length);
			varName[length] = '\0';

			rootType->type = VAR;
			rootType->type1 = (void *) varName;
			rootType->type2 = INT;

			if (DEBUG)
				printf("Got to VAR LOOKUP with %s", expression[0]);

			free(n);

			return rootType;
		} else {
			rootType->type = INT;
			rootType->type1 = (void *) n;
			rootType->type2 = 0;

			if (DEBUG)
				printf("Got to INT with num: %d\n", *(int*)rootType->type1);

			return rootType;
		}
	}


	if (strcmp(expression[0], TOKEN_STDOUT) == 0) {
		rootType->type = PRINT;

		char **printExp = malloc(sizeof(char*) * (length - 1));
		for (int i = 1; i < length; i++) {
			printExp[i - 1] = expression[i];
		}

		rootType->type1 = parse(printExp, length - 1);
		rootType->type2 = 0;
		free(printExp);
		return rootType;
	}


	int i;
	for (i = 0; i < length; i++) {
		if (strcmp(expression[i], TOKEN_ASSIGNMENT) == 0) {
			char **subset = malloc(sizeof(char*) * i);
			char **subset2 = malloc(sizeof(char*) * (length - i - 1));

			int k;
			for (k = 0; k < i; k++) {
				subset[k] = expression[k];
			}

			for (k = i + 1; k < length; k++) {
				subset2[k - (i + 1)] = expression[k];
			}

			if (DEBUG)
				printf("Got to ASN with subset: %s %s and subset2: %s\n", subset[0], subset[1], *subset2);

			rootType->type = ASN;
			rootType->type1 = (void *) parse(subset, i);
			rootType->type2 = (void *) parse(subset2, length - i - 1);
			
			free(subset);
			free(subset2);

			return rootType;
		}
	}

	for (i = 0; i < length; i++) {
		if (strcmp(expression[i], TOKEN_INTEGER) == 0) {
			if (i == 0) { // var declaration
				size_t length = strlen(expression[i + 1]);
				char *varName = (char *) malloc(sizeof(char) * length + 1);
				memcpy(varName, expression[i + 1], sizeof(char) * length);
				varName[length] = '\0';

				rootType->type = VAR;
				rootType->type1 = (void *) varName;
				rootType->type2 = (void *) INT;

				if (DEBUG)
					printf("Got to VAR with type1: %s and type2: %s\n", (char*)rootType->type1, (char*)rootType->type2);

				return rootType;
			}
		}
	}

	for (i = 0; i < length; i++) {
		if (strcmp(expression[i], TOKEN_ADD) == 0 || strcmp(expression[i], TOKEN_SUBTRACT) == 0 || strcmp(expression[i], TOKEN_MULT) == 0 || strcmp(expression[i], TOKEN_DIVIDE) == 0) {
			if (strcmp(expression[i], TOKEN_ADD) == 0)
				rootType->type = ADD;
			else if (strcmp(expression[i], TOKEN_SUBTRACT) == 0)
				rootType->type = SUB;
			else if (strcmp(expression[i], TOKEN_MULT) == 0)
				rootType->type = MULT;
			else if (strcmp(expression[i], TOKEN_DIVIDE) == 0)
				rootType->type = DIV;
			
			char **subset1 = malloc(sizeof(char*) * i);
			char **subset2 = malloc(sizeof(char*) * (length - i - 1));

			int k;
			for (k = 0; k < i; k++) {
				subset1[k] = expression[k];
			}

			for (k = i + 1; k < length; k++) {
				subset2[k - (i + 1)] = expression[k];
			}
			
			rootType->type1 = parse(subset1, i);
			rootType->type2 = parse(subset2, length - i - 1);

			free(subset1);
			free(subset2);

			return rootType;
		}
	}

	printf("ERROR: Error in expression\n");
	exit(-1);

	return rootType;
}