#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "sym.h"

SymTable * createSymTable() {
    SymTable * table = malloc(sizeof(SymTable));
    table->length = 0;
    table->syms = NULL;
    return table;
}

SymTable * appendToSymTable(SymTable * table, Sym * sym) {
    for (int i = 0; i < table->length; i++)
        if (!strcmp(table->syms[i]->name, sym->name)) {
            //printNodeSyntax(table->syms[i]->type, 4);
            //printNodeSyntax(sym->type, 4);
            if (!compareNodeSyntax(table->syms[i]->type, sym->type)) {
                fprintf(stderr, "\nerror: conflicting types for '%s'", table->syms[i]->name);
                exit(EXIT_FAILURE);
            }
            if (isDefined(table->syms[i]) && isDefined(sym)) {
                fprintf(stderr, "\nerror: redefinition of '%s'", table->syms[i]->name);
                exit(EXIT_FAILURE);
            }
            if (isDefined(sym)) {
                table->syms[i] = sym;
                //TODO: Освободить переменную table->syms[i]
            }
            return table;
        }

    table->length++;
    table->syms = realloc(table->syms, sizeof(Sym *) * table->length);
    table->syms[table->length - 1] = sym;
    return table;
}

SymTable * mergeSymTables(SymTable * table, const SymTable * tablenew) {
    if (!tablenew) return table;
    for (int i = 0; i < tablenew->length; i++)
        appendToSymTable(table, tablenew->syms[i]);
    return table;
}

SymVar * declarationToSymVar(NodeN * declaration_specifiers, NodeN * init_declarator) {
    NodeN * init = NULL, * decl = init_declarator; //init_declarator;
    if (decl && decl->action == INIT_DECLARATOR) {
        init = decl->nodes[1];
        decl = decl->nodes[0];
    }

    char * name;
    if(decl && decl->nodes[0]->type == NODE_SYM) {
        NodeSym * node = popFirstNodeN(decl);
        name = node->name;
    } else {
        name = randomString();
    }

    NodeN * type = createNodeN(NONE);
    mergeNodeN(type, decl);
    mergeNodeN(type, declaration_specifiers);

    SymVar * sym = createSymVar(name, type, init);
    return sym;
}

SymTable * declarationToSymTable(NodeN * declaration_specifiers, NodeN * init_declarator_list) {
    //printNodeSyntax(init_declarator_list, 0);
    SymTable * symTable = createSymTable();
    for (int i = 0; i < init_declarator_list->length; i++) {
        NodeN * init_declarator = init_declarator_list->nodes[i];
        SymVar * sym = declarationToSymVar(declaration_specifiers, init_declarator);
        appendToSymTable(symTable, sym);
    }
    return symTable;
}

SymFunc * functionDefinitionToSymTable(NodeN * declaration_specifiers, NodeN * declarator, NodeN * compound_statement) {
    NodeSym * nodeSym = popFirstNodeN(declarator);
    assert(nodeSym->type == NODE_SYM);
    NodeN * type = createNodeN(NONE);
    mergeNodeN(type, declarator);
    mergeNodeN(type, declaration_specifiers);

    SymTable * table = createSymTable();
    NodeN * node = declarator->nodes[0];
    assert(node->action == PARAMETER_LIST);
    for (int i = 0; i < node->length; i++) {
        NodeN * param = node->nodes[i];
        assert(param->action == PARAMETER_DECLARATION);
        SymVar * sym = declarationToSymVar(param->nodes[0], param->nodes[1]);
        appendToSymTable(table, sym);
    }
    mergeSymTables(table, compound_statement->nodes[0]);

    SymFunc * sym = createSymFunc(nodeSym->name, type, table, compound_statement->nodes[1]);
    return sym;
}

SymFunc * createSymFunc(char * name, NodeSyntax * type, SymTable * table, NodeSyntax * compound) {
    SymFunc * sym = malloc(sizeof(SymFunc));
    sym->kind = SYM_FUNC;
    sym->name = name;
    sym->type = type;
    sym->symTable = table;
    sym->compound = compound;
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

int isDefinedFunc(SymFunc * s) {
    return s->compound != 0;
}

int isDefinedVal(SymVar * s) {
    return s->init != 0;
}

int isDefined(Sym * s) {
    switch (s->kind) {
        case SYM_FUNC: return isDefinedFunc((SymFunc *)s);
        case SYM_TYPE: return 0;
        case SYM_VAR:  return isDefinedVal((SymVar *)s);
        default: assert(0);
    }
}

extern void printfl(int level);

void printSymTable(SymTable * table, int level) {
    printfl(level);
    if (!table) {
        printf("NULL (no symbols table)");
        return;
    }
    printf("length: %d", table->length);
    for (int i = 0; i < table->length; i++)
        printSym(table->syms[i], level + 1);
}

void printSymFunc(SymFunc * sym, int level) {
    printfl(level);
    printf("%s", sym->name);
    printfl(level + 1);
    printf("Type:");
    printNodeSyntax(sym->type, level + 2);
    printfl(level + 1);
    printf("Symbols:");
    printSymTable(sym->symTable, level + 2);
    printfl(level + 1);
    printf("Compound:");
    printNodeSyntax(sym->compound, level + 2);
}

void printSymType(SymType * sym, int level) {
    printfl(level);
    printf("%s", sym->name);
}

void printSymVar(SymVar * sym, int level) {
    printfl(level);
    printf("%s", sym->name);
    printfl(level + 1);
    printf("Type:");
    printNodeSyntax(sym->type, level + 2);
    printfl(level + 1);
    printf("Initializer:");
    printNodeSyntax(sym->init, level + 2);
}

void printSym(Sym * sym, int level) {
    if (!sym) {
        printfl(level);
        printf("NULL (no symbol)");
        return;
    }
    switch (sym->kind) {
        case SYM_FUNC: printSymFunc((SymFunc *)sym, level); break;
        case SYM_TYPE: printSymType((SymType *)sym, level); break;
        case SYM_VAR: printSymVar((SymVar *)sym, level); break;
        default: assert(0);
    }
}
