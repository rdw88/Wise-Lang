#ifndef HELPER_H__
#define HELPER_H__

#define DEBUG 0

#include "parser.h"
#include "env.h"

int get_array_length(char** array);

char* parseTreeToString(Type *t);

void free_env(Environment *e);

void free_type(Type *t);

int power(int base, unsigned int e);

int strToNum(char *str);

#endif