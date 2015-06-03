#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"


int get_array_length(char** array) {
	int len = 0;
	int i = 0;

	for(i = 0;; i++) {
		if (array[i] == 0)
			break;

		len++;
	}

	return len;
}


char* parseTreeToString(Type *t) {
	int len = 80;
	char *str = malloc(sizeof(char) * len);

	switch (t->type) {
		case VAR:;

		char *var2 = "NOP";
		exp_type *typ = (exp_type*) t->type2;

		if (typ == INT) {
			var2 = "INT";
		}

		sprintf(str, "VAR(%s, %s)", (char*) t->type1, var2);
		break;

		case ASN:
		sprintf(str, "ASN(%s, %s)", parseTreeToString((Type*) t->type1), parseTreeToString((Type*) t->type2));
		break;

		case ADD:
		sprintf(str, "ADD(%s, %s)", parseTreeToString((Type*) t->type1), parseTreeToString((Type*) t->type2));
		break;

		case SUB:
		sprintf(str, "SUB(%s, %s)", parseTreeToString((Type*) t->type1), parseTreeToString((Type*) t->type2));
		break;

		case MULT:
		sprintf(str, "MULT(%s, %s)", parseTreeToString((Type*) t->type1), parseTreeToString((Type*) t->type2));
		break;

		case DIV:
		sprintf(str, "DIV(%s, %s)", parseTreeToString((Type*) t->type1), parseTreeToString((Type*) t->type2));
		break;

		case INT:
		sprintf(str, "INT(%d)", *(int*) t->type1);
		break;

		case PRINT:
		sprintf(str, "PRINT(%s)", parseTreeToString((Type*) t->type1));
		break;
	}

	return str;
}


void free_env(Environment *e) {
	if (e->next != 0) {
		free_env(e->next);
	}

	free_type(e->t);
	free(e);
}


void free_type(Type *t) {
	if (t->type == INT) {
		free(t->type1);
	} else if (t->type != VAR) {
		free_type(t->type1);
		free_type(t->type2);
	} else {
		free(t->type1);
	}

	free(t);
}


int power(int base, unsigned int e) {
	if (e == 1)
		return base;

	return base * power(base, e - 1); 
}


int strToNum(char *str) {
	int length = 0;
	int i = 0;
	int result = 0;

	for (i = 0;; i++) {
		if (str[i] == 0)
			break;

		length ++;
	}

	for (i = 0; i < length; i++) {
		int c = str[i] - '0';

		if (i == length - 1)
			result += c;
		else
			result += c * (power(10, (length - i - 1)));
	}

	return result;
}


int contains(char **array, char *val, unsigned int len) {
	for (unsigned int i = 0; i < len; i++) {
		if (strcmp(array[i], val) == 0) {
			return 1;
		}
	}

	return 0;
}