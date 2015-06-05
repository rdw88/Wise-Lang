#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "helper.h"


Type* interp_add(Type *addNode, Environment *env) {
	Type *num1 = interp(addNode->type1, env);
	Type *num2 = interp(addNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;

	int *sum = (int *) malloc(sizeof(int));
	*sum = val1 + val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) sum;

	return newInt;
}


Type* interp_sub(Type *subNode, Environment *env) {
	Type *num1 = interp(subNode->type1, env);
	Type *num2 = interp(subNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;

	int *diff = (int *) malloc(sizeof(int));
	*diff = val1 - val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) diff;

	return newInt;
}


Type* interp_mult(Type *multNode, Environment *env) {
	Type *num1 = interp(multNode->type1, env);
	Type *num2 = interp(multNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;

	int *prod = (int *) malloc(sizeof(int));
	*prod = val1 * val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) prod;

	return newInt;
}


Type* interp_div(Type *divNode, Environment *env) {
	Type *num1 = interp(divNode->type1, env);
	Type *num2 = interp(divNode->type2, env);
	int val1 = *(int *) num1->type1;
	int val2 = *(int *) num2->type1;

	int *quo = (int *) malloc(sizeof(int));
	*quo = val1 / val2;

	Type *newInt = (Type *) malloc(sizeof(Type));
	newInt->type = INT;
	newInt->type1 = (void *) quo;

	return newInt;
}


Type* interp(Type *parseTree, Environment *env) {
	exp_type node = parseTree->type;

	switch (node) {
		case INT:
		return parseTree;
		break;

		case ASN:;
		Type* var = (Type *) parseTree->type1;
		char* name = (char *) var->type1;
		extend_env(env, name, interp(parseTree->type2, env));
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
		return interp_mult(parseTree, env);
		break;

		case DIV:
		return interp_div(parseTree, env);
		break;

		case PRINT:;
		Type *printable = interp(parseTree->type1, env);
		if (printable->type == INT) {
			printf("- %d\n", *(int *) printable->type1);
		}

		free_type(printable);

		break;
	}
}