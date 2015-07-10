#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED
//#include "c.parser.h"

#define def(type) type = (1000 + __COUNTER__),
enum {
    def(NONE)
    def(INC_PREF)
    def(INC_SUFF)
    def(DEC_PREF)
    def(DEC_SUFF)
    def(ARR_OP)
    def(CALL_OP)
    def(FIELD_OP)
    def(CAST_OP)
    def(TERNAL_OP)
    def(CONST_CK)
    def(DECL_OP)
    def(DO_WHILE)
    def(COMPOUND_STATEMENT)
    def(STATEMENT_LIST)
    def(INITIALIZER_LIST)
    def(ENUMERATOR)
    def(ARRAY)
    def(FUNC_TYPED)
    def(FUNC)
    def(POINTER)

};
#undef def

const char * enum_tostring(int v);

#endif // ENUMS_H_INCLUDED
