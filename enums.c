#include <assert.h>
#include "enums.h"
#include "c.parser.h"

#define def(type) case type: return #type;

static char s[2] = {0, 0};

const char * enum_tostring(int v) {
    if (v < 256) {
        s[0] = v;
        return s;
    }
    switch (v) {
    def(IDENTIFIER)
    def(CONSTANT_CHAR)
    def(CONSTANT_INT)
    def(CONSTANT_DOUBLE)
    def(STRING_LITERAL)
    def(SIZEOF)
    def(PTR_OP)
    def(INC_OP)
    def(DEC_OP)
    def(LEFT_OP)
    def(RIGHT_OP)
    def(LE_OP)
    def(GE_OP)
    def(EQ_OP)
    def(NE_OP)
    def(AND_OP)
    def(OR_OP)
    def(MUL_ASSIGN)
    def(DIV_ASSIGN)
    def(MOD_ASSIGN)
    def(ADD_ASSIGN)
    def(SUB_ASSIGN)
    def(LEFT_ASSIGN)
    def(RIGHT_ASSIGN)
    def(AND_ASSIGN)
    def(XOR_ASSIGN)
    def(OR_ASSIGN)
    def(TYPE_NAME)
    def(TYPEDEF)
    def(EXTERN)
    def(STATIC)
    def(AUTO)
    def(REGISTER)
    def(CHAR)
    def(SHORT)
    def(INT)
    def(LONG)
    def(SIGNED)
    def(UNSIGNED)
    def(FLOAT)
    def(DOUBLE)
    def(CONST)
    def(VOLATILE)
    def(VOID)
    def(STRUCT)
    def(UNION)
    def(ENUM)
    def(ELLIPSIS)
    def(CASE)
    def(DEFAULT)
    def(IF)
    def(ELSE)
    def(SWITCH)
    def(WHILE)
    def(DO)
    def(FOR)
    def(GOTO)
    def(CONTINUE)
    def(BREAK)
    def(RETURN)

#include "enums_list.h"

    default:
        printf("Bad enum: %d\n", v);
        assert(0);
    }
}
