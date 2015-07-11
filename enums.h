#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED
#include "c.parser.h"

#define def(type) type = (1000 + __COUNTER__),
enum {
#include "enums_list.h"

};
#undef def

const char * enum_tostring(int v);

#endif // ENUMS_H_INCLUDED
