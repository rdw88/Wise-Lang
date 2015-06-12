#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "helper.h"


#define ARITH_ADD 0
#define ARITH_SUB 1
#define ARITH_MULT 2
#define ARITH_DIV 3


Type* interp_arith(Type *type, Environment *env, int mode) {
	int num1 = 0;
	int num2 = 0;

	Type *node1 = (Type *) type->type1;
	Type *node2 = (Type *) type->type2;

	if (node1->type == VAR) {
		Type *t1 = lookup_env(env, (char *) node1->type1);

		if (t1 == NULL) {
			return NULL;
		}

		num1 = *(int *) t1->type1;
	} else if (node1->type == INT) {
		num1 = *(int *) node1->type1;
	} else {
		Type *rec = interp(node1, env);

		if (rec != NULL) {
			num1 = *(int *) rec->type1;
		}
	}

	if (node2->type == VAR) {
		Type *t2 = lookup_env(env, (char *) node2->type1);

		if (t2 == NULL) {
			return NULL;
		}

		num2 = *(int *) t2->type1;
	} else if (node2->type == INT) {
		num2 = *(int *) node2->type1;
	} else {
		Type *rec = interp(node2, env);

		if (rec != NULL) {
			num2 = *(int *) rec->type1;
		}
	}

	int *result = (int *) malloc(sizeof(int));

	if (mode == ARITH_ADD)
		*result = num1 + num2;
	else if (mode == ARITH_SUB)
		*result = num1 - num2;
	else if (mode == ARITH_MULT)
		*result = num1 * num2;
	else
		*result = num1 / num2;

	Type *final = (Type *) malloc(sizeof(Type));
	final->type = INT;
	final->type1 = (void *) result;
	final->type2 = 0;

	return final;
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

		Type *val = interp(parseTree->type2, env);
		if (val != NULL)
			extend_env(env, name, val);
		break;

		case REASN:;
		var = (Type *) parseTree->type1;
		name = (char *) var->type1;
		modify_env(env, name, parseTree->type2);
		break;

		case VAR:
		return lookup_env(env, (char *) parseTree->type1);
		break;

		case ADD:
		return interp_arith(parseTree, env, ARITH_ADD);
		break;

		case SUB:
		return interp_arith(parseTree, env, ARITH_SUB);
		break;

		case MULT:
		return interp_arith(parseTree, env, ARITH_MULT);
		break;

		case DIV:
		return interp_arith(parseTree, env, ARITH_DIV);
		break;

		case PRINT:;
		if (parseTree->type1 == 0) {
			printf("- \n");
			break;
		}

		Type *printable = interp(parseTree->type1, env);

		if (printable == NULL)
			break;

		if (printable->type == INT) {
			printf("- %d\n", *(int *) printable->type1);
		}

		free_type(printable);

		break;
	}
}