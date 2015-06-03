#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"


Environment* new_env() {
	Environment* e = malloc(sizeof(Environment));
	memset(e, 0, sizeof(Environment));
	return e;
}


void extend_env(Environment *e, char *var, Type *t) {
	if (e->var == 0) {
		e->var = var;
		e->t = t;
		return;
	}

	if (e->next == 0) {
		Environment *next = new_env();
		next->var = var;
		next->t = t;
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