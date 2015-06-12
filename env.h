#ifndef ENV_H__
#define ENV_H__

#include "parser.h"

typedef struct Environment {
	char *var;
	Type *t;
	struct Environment *next;
} Environment;


Environment* new_env();

Type* copyTypeForEnv(Type *type);

void extend_env(Environment *e, char *var, Type *t);

void remove_env(Environment *e, char *var);

int modify_env(Environment *e, char *var, Type *t);

Type* lookup_env(Environment *e, char *var);


#endif