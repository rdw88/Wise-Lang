#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "interp.h"
#include "helper.h"

#define MAX_SHELL_EXPRESSION_LENGTH 256

#define FLAG_SHOW_PARSE_TREE "--tree"
#define FLAG_SHELL "--shell"


static volatile int shellRunning = 1;


void intHandler(int i) {
	shellRunning = 0;
}


void execute(char *expression, Environment *env, char **flags, int flag_len) {
	char **terms = tokenize(expression);
	int length = get_array_length(terms);
	Type *t = parse(terms, length);
	
	if (contains(flags, FLAG_SHOW_PARSE_TREE, flag_len)) {
		char *strRep = parseTreeToString(t);
		printf("%s\n", strRep);
		free(strRep);
	}

	interp(t, env);

	free(terms);
}


void shell(char **flags, int flag_len) {
	printf("Wise 0.0.1 - Simple interpreted programming language created by Ryan Wise.\nLast Modified June 2, 2015\n");
	Environment *e = new_env();
	char input[MAX_SHELL_EXPRESSION_LENGTH];

	while (shellRunning) {
		printf(">> ");

		fgets(input, MAX_SHELL_EXPRESSION_LENGTH, stdin);

		if (!shellRunning)
			break;

		if ((strlen(input) > 0) && (input[strlen(input) - 1] == '\n'))
        	input[strlen(input) - 1] = '\0';

		execute(input, e, flags, flag_len);
	}

	free_env(e);
}


int main(int argc, char **argv) {
	signal(SIGINT, intHandler);

	if (contains(argv, FLAG_SHELL, argc)) {
		shell(argv, argc);
		return 0;
	} else {
		char express[] = "integer x is 5";
		char express1[] = "integer y is 3";
		char express2[] = "show x + y";
		Environment *e = new_env();
		execute(express, e, argv, argc);
		execute(express1, e, argv, argc);
		execute(express2, e, argv, argc);
		free_env(e);
		return 0;
	}
}