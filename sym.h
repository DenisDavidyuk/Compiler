#ifndef SYM_H_INCLUDED
#define SYM_H_INCLUDED
#include "ast.h"

typedef enum SymKind {
    SYM_FUNC,
    SYM_TYPE,
    SYM_VAR,
} SymKind;

typedef struct Sym {
    SymKind kind;
    char * name;
} Sym;

typedef struct SymTable {
    Sym ** syms;
    int length;
} SymTable;

typedef struct SymType {
    SymKind kind;
    char * name; //alias
    NodeSyntax * type;
} SymType;

typedef struct SymFunc {
    SymKind kind;
    char * name;
    NodeSyntax * type;
    NodeN * args;
    NodeSyntax * compound;
} SymFunc;

typedef struct SymVar {
    SymKind kind;
    char * name;
    NodeSyntax * type;
    NodeSyntax * init;
} SymVar;

SymTable * createSymTable();
SymTable * appendToSymTable(SymTable *, Sym *);

void appendDeclaration(NodeSyntax * declaration_specifiers, NodeN * init_declarator_list);

SymFunc * createSymFunc(char * name, NodeSyntax * type, NodeN * args, NodeSyntax * compound);;
SymType * createSymType(char * name);
SymVar * createSymVar(char * name, NodeSyntax * type, NodeSyntax * init);

void printSymFunc(SymFunc * sym, int level);
void printSymType(SymType * sym, int level);
void printSymVar(SymVar * sym, int level);
void printSym(Sym * sym, int level);

#endif // SYM_H_INCLUDED
