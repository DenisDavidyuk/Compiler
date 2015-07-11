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

SymTable * appendToSymTable(SymTable * table, Sym * sym) {
    table->length++;
    table->syms = realloc(table->syms, sizeof(Sym *) * table->length);
    table->syms[table->length - 1] = sym;
    return table;
}


void appendDeclaration(NodeN * declaration_specifiers, NodeN * init_declarator_list) {
    for (int i = 0; i < init_declarator_list->length; i++) {
        NodeN * node = init_declarator_list->nodes[i]; //init_declarator
        NodeSym * nodeSym = popFirstNodeN(node->nodes[0]); //declarator
        assert(nodeSym->type == NODE_SYM);
        NodeN * type = createNodeN(NONE);
        mergeNodeN(type, node->nodes[0]);
        mergeNodeN(type, declaration_specifiers);
        SymVar * sym = createSymVar(nodeSym->name, type, node->nodes[1]);
        printSymVar(sym, 0);
    }
}


SymFunc * createSymFunc(char * name, NodeSyntax * type, NodeSyntax * compound) {
    SymFunc * sym = malloc(sizeof(SymFunc));
    sym->kind = SYM_FUNC;
    sym->name = name;
    sym->type = type;
    sym->args = args;
    sym->compound = compound;
    sym->symTable = createSymTable();
    return sym;
}

SymType * createSymType(char * name, NodeSyntax * type) {
    SymType * sym = malloc(sizeof(SymType));
    sym->kind = SYM_TYPE;
    sym->name = name;
    sym->type = type;
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
