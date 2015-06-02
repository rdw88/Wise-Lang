#include <stdio.h>
#include <stdlib.h>
#include "interp.h"


Type* interp_add(Type *addNode, Environment *env) {
	Type *num1 = interp(addNode->type1, env);
	Type *num2 = interp(addNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;
	int sum = val1 + val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) &sum;

	return newInt;
}


Type* interp_sub(Type *addNode, Environment *env) {
	Type *num1 = interp(addNode->type1, env);
	Type *num2 = interp(addNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;
	int diff = val1 - val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) &diff;

	return newInt;
}


Type* interp(Type *parseTree, Environment *env) {
	exp_type node = parseTree->type;

	switch (node) {
		case INT:
		return parseTree;
		break;

		case ASN:; // Takes a VAR("x", TYPE), VAL where VAL is of type TYPE
		Type *var = (Type *) parseTree->type1;
		extend_env(env, (char *) var->type1, interp(parseTree->type2, env));
		break;

		case VAR:
		return lookup_env(env, (char *) parseTree->type1);
		break;

		case ADD:
		return interp_add(parseTree, env);
		break;

		case SUB:
		return interp_sub(parseTree, env);
		break;

		case MULT:
		break;

		case DIV:
		break;

		case PRINT:;
		Type *printable = interp(parseTree->type1, env);
		if (printable->type == INT) {
			printf("%d\n", *(int *) printable->type1);
		}

		break;
	}
}