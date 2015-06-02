#include <stdio.h>
#include <stdlib.h>

#include "interp.h"
#include "helper.h"


int main() {
	char express1[] = "integer x is 420 + 5";
	char express2[] = "show x";
	char **terms = tokenize(express1);
	int length = get_array_length(terms);
	Type *t = parse(terms, length);
	
	char *strRep = parseTreeToString(t);
	printf("%s\n", strRep);

	Environment *e = new_env();
	interp(t, e);

	printf("%d\n", *(int *) e->t->type1);
	printf("%d\n", *(int *) e->t->type1);

	free(strRep);
	free(terms);
	free_type(t);
	free_env(e);
}