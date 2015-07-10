#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sym.h"

SymTable * createSymTable() {
    SymTable * table = malloc(sizeof(SymTable));
    table->length = 0;
    table->syms = NULL;
    return table;
}

SymTable * appendSymTable(SymTable * table, Sym * sym) {
    table->length++;
    table->syms = realloc(table->syms, sizeof(Sym *) * table->length);
    table->syms[table->length - 1] = sym;
    return table;
}


void appendDeclaration(NodeSyntax * type, NodeN * ident) {
    for (int i = 0; i < ident->length; i++) {
        ident->nodes[i];
    }
}


SymFunc * createSymFunc(char * name, NodeSyntax * type, NodeN * args, NodeSyntax * compound) {
    SymFunc * sym = malloc(sizeof(SymFunc));
    sym->kind = SYM_FUNC;
    sym->name = name;
    sym->type = type;
    sym->args = args;
    sym->compound = compound;
    return sym;
}

SymType * createSymType(char * name) {
    SymType * sym = malloc(sizeof(SymType));
    sym->kind = SYM_TYPE;
    sym->name = name;
    /* ??? */
    return sym;
}

SymVar * createSymVar(char * name, NodeSyntax * type, NodeSyntax * init) {
    SymVar * sym = malloc(sizeof(SymVar));
    sym->kind = SYM_VAR;
    sym->name = name;
    sym->type = type;
    sym->init = init;
    return sym;
}

extern void printfl(int level);

void printSymFunc(SymFunc * sym, int level) {
    printfl(level);
    printf("\n%s", sym->name);
    printfl(level);
    printf("Type:");
    printNodeSyntax(sym->type, level + 1);
    printfl(level);
    printf("Args conunt: %d", sym->args->length);
    printfl(level);
    printf("Args:");
    printNodeN(sym->args, level + 1);
    printf("Compound:");
    printNodeSyntax(sym->compound, level + 1);
}

void printSymType(SymType * sym, int level) {
    printfl(level);
    printf("\n%s", sym->name);
}

void printSymVar(SymVar * sym, int level) {
    printfl(level);
    printf("\n%s", sym->name);
    printfl(level);
    printf("Type:");
    printNodeSyntax(sym->type, level + 1);
    printfl(level);
    printf("Initializer:");
    printNodeSyntax(sym->init, level + 1);
}

void printSym(Sym * sym, int level) {
    if (!sym) {
        printfl(level);
        printf("NULL SYM");
        return;
    }
    switch (sym->kind) {
        case SYM_FUNC: printSymFunc((SymFunc *)sym, level); break;
        case SYM_TYPE: printSymType((SymType *)sym, level); break;
        case SYM_VAR: printSymVar((SymVar *)sym, level); break;
        default: assert(0);
    }
}
