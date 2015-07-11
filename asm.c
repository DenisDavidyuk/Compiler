#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"

String * createString() {
    String * s = malloc(sizeof(String));
    s->length = 0;
    s->data = 0;
    return s;
}

String * appendToString(String * s, char * c) {
    int newLength = s->length + strlen(c) + 1;
    s->data = realloc(s->data, newLength * sizeof(char));
    if (s->length) s->data[s->length - 1] = '\n';
    strcpy(s->data + s->length, c);
    s->length = newLength;
    s->data[s->length - 1] = 0;
    return s;
}

AsmCode * newAsmCode() {
    AsmCode * code = malloc(sizeof(AsmCode));
    code->name = malloc(20 * sizeof(char));
    strcpy(code->name, "out.asm");
    code->code = createString();
    code->data = createString();
    return code;
}

extern const char * tpl, * prolog, * epilog;

void writeAsmCode(AsmCode * code) {
    FILE * f = fopen(code->name, "w");
    fprintf(f, tpl);
    fprintf(f, ".data\n");
    fprintf(f, "%s", code->data->data);
    fprintf(f, "\n.code\n\n");
    fprintf(f, "%s", code->code->data);
    fprintf(f, "\n\nend");
    fclose(f);
}

char buf[1000];
static int string_literal_count;

void asmNodeVal(AsmCode * code, NodeVal * node) {
    switch (node->action) {
    case CONSTANT_INT:
        sprintf(buf, "push %d", node->value.i);
        appendToString(code->code, buf);
        break;
    case STRING_LITERAL:
        sprintf(buf, "fmt%d db '%s', 0", ++string_literal_count, node->value.s);
        appendToString(code->data, buf);
        sprintf(buf, "push offset fmt%d", string_literal_count);
        appendToString(code->code, buf);
        break;
    }
}

void asmNodeN(AsmCode * code, NodeN * node) {
    switch (node->action) {
    case STATEMENT_LIST:
        for (int i = 0; i < node->length; i++)
            asmNodeSyntax(code, node->nodes[i]);
        break;
    case ARGUMENT_EXPRESSION_LIST:
        for (int i = node->length-1; i >= 0; i--)
            asmNodeSyntax(code, node->nodes[i]);
        break;
    case FUNC_CALL:
        asmNodeN(code, node->nodes[1]);
        sprintf(buf, "call %s", ((NodeSym *)node->nodes[0])->name);
        appendToString(code->code, buf);
        sprintf(buf, "add esp, %d", ((NodeN *)node->nodes[1])->length * 4);
        appendToString(code->code, buf);
        break;
    case '+':
        asmNodeSyntax(code, node->nodes[0]);
        asmNodeSyntax(code, node->nodes[1]);
        appendToString(code->code, "pop eax");
        appendToString(code->code, "pop ebx");
        appendToString(code->code, "add eax, ebx");
        appendToString(code->code, "push eax");
        break;
    case '=':
        asmNodeSyntax(code, node->nodes[1]);
        appendToString(code->code, "pop eax");
        sprintf(buf, "mov %s, eax", ((NodeSym *)node->nodes[0])->name);
        appendToString(code->code, buf);
        break;
    default:
        fprintf(stderr, "\nInvalid node action: %s", enum_tostring(node->action));
        exit(EXIT_FAILURE);
    }
}

void asmNodeSyntax(AsmCode * code, NodeSyntax * node) {
    switch (node->type) {
    case NODE_VAL: asmNodeVal(code, node); break;
    case NODE_N: asmNodeN(code, node); break;
    }
}

void asmSymFunc(AsmCode * code, SymFunc * sym) {
    char c = ' ';
    if (!strcmp(sym->name, "main")) c = '_';

    sprintf(buf, "%c%s proc", c, sym->name);
    appendToString(code->code, buf);
    appendToString(code->code, prolog);

    asmNodeN(code, sym->compound);

    appendToString(code->code, epilog);
    sprintf(buf, "%c%s endp", c, sym->name);
    appendToString(code->code, buf);
}

void asmSymVar(AsmCode * code, SymVar * sym) {
    sprintf(buf, "%s dd 0", sym->name);
    appendToString(code->data, buf);
}

void asmSymTable(AsmCode * code, SymTable * table) {
    for (int i = 0; i < table->length; i++)
        switch (table->syms[i]->kind) {
        case SYM_FUNC: asmSymFunc(code, table->syms[i]); break;
        case SYM_VAR: asmSymVar(code, table->syms[i]); break;
        }
}
