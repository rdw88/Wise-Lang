#ifndef INTERP_H__
#define INTERP_H__

#include "env.h"

Type * interp(Type *parseTree, Environment *e);

#endif