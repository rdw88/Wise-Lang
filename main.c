#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "interp.h"
#include "helper.h"

#define MAX_SHELL_EXPRESSION_LENGTH 256

#define FLAG_SHOW_PARSE_TREE "--tree"


static volatile int shellRunning = 1;
static int showParseTree = 0;


void intHandler(int i) {
	shellRunning = 0;
	printf("\n");
}


void execute(char *expression, Environment *env, char **flags, int flag_len) {
	char **terms = tokenize(expression);
	int length = get_array_length(terms);
	Type *t = parse(terms, length);
	
	if (t->type != ERR) {
		if (showParseTree) {
			char *strRep = parseTreeToString(t);
			printf("%s\n", strRep);
			free(strRep);
		}

		interp(t, env);
	}

	free_type(t);
	free(terms);
}


void shell(char **flags, int flag_len) {
	printf("Wise 0.0.1 - Simple interpreted programming language created by Ryan Wise.\nLast Modified June 4, 2015\nTo show the parse tree of each expression type \"--tree\".\n");
	Environment *e = new_env();
	char input[MAX_SHELL_EXPRESSION_LENGTH];

	while (shellRunning) {
		printf(">> ");

		fgets(input, MAX_SHELL_EXPRESSION_LENGTH, stdin);

		if (!shellRunning)
			break;

		if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
        	input[strlen(input) - 1] = '\0';

        if (strcmp(input, FLAG_SHOW_PARSE_TREE) == 0) {
        	showParseTree = (showParseTree == 0) ? 1 : 0;
        } else {
        	execute(input, e, flags, flag_len);
        }
	}

	free_env(e);
}


int main(int argc, char **argv) {
	signal(SIGINT, intHandler);

	if (contains(argv, FLAG_SHOW_PARSE_TREE, argc) != -1) {
		showParseTree = (showParseTree == 0) ? 1 : 0;
	}

	shell(argv, argc);
	/*char expression[] = "show 2 + 6 / 3";
	char **terms = tokenize(expression);
	int length = get_array_length(terms);
	Type *t = parse(terms, length);

	if (t->type != ERR) {
		char *strRep = parseTreeToString(t);
		printf("%s\n", strRep);
		free(strRep);
		Environment *e = new_env();
		interp(t, e);
	}
	
	free_type(t);
	free(terms);

	return 0;*/
}