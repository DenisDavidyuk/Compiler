#ifndef ASM_H_INCLUDED
#define ASM_H_INCLUDED

#include "sym.h"

typedef struct {
    int length;
    char * data;
} String;

String * createString();
String * appendToString(String *, char *);

typedef struct {
    char * name;
    String * data, * code;
} AsmCode;

AsmCode * newAsmCode();

void writeAsmCode(AsmCode *);

void asmSymTable(AsmCode *, SymTable *);

#endif // ASM_H_INCLUDED
