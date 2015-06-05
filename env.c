#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "helper.h"


Environment* new_env() {
	Environment* e = malloc(sizeof(Environment));
	memset(e, 0, sizeof(Environment));
	return e;
}


Type* copyTypeForEnv(Type *type) {
	Type *newType = (Type *) malloc(sizeof(Type));
	newType->type = type->type;

	if (newType->type == INT) {
		int *t1Copy = (int *) malloc(sizeof(int));
		*t1Copy = *(int *) type->type1;
		newType->type1 = (void *) t1Copy;
	} else if (newType->type == VAR) {
		char **varCpy = (char **) malloc(sizeof(char *));
		*varCpy = (char *) type->type1;
		newType->type1 = (void *) varCpy;
		newType->type2 = INT;
	} else {
		printf("FATAL: Environment only takes leaves, Type passed to environment must be evaluated first!\n");
		exit(-1);
	}

	return newType;
}


void extend_env(Environment *e, char *var, Type *t) {
	if (e->var == 0) {
		size_t length = strlen(var);
		char* cpy = (char *) malloc(sizeof(char) * length + 1);
		memcpy(cpy, var, sizeof(char) * length);
		cpy[length] = '\0';

		Type *copied = copyTypeForEnv(t);

		e->var = cpy;
		e->t = copied;
		return;
	}

	if (e->next == 0) {
		Environment *next = new_env();

		size_t length = strlen(var);
		char* cpy = (char *) malloc(sizeof(char) * length + 1);
		memcpy(cpy, var, sizeof(char) * length);
		cpy[length] = '\0';

		Type *copied = copyTypeForEnv(t);

		next->var = cpy;
		next->t = copied;
		e->next = next;
	} else {
		extend_env(e->next, var, t);
	}
}


Type* lookup_env(Environment *e, char *var) {
	if (strcmp(e->var, var) == 0) {
		return e->t;
	}

	if (e->next == 0) {
		printf("ERROR: Variable %s is not defined!\n", var);
		return NULL;
	}

	return lookup_env(e->next, var);
}


void remove_env(Environment *e, char *var) {

}