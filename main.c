#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "c.parser.h"
#include "ast.h"

const char * help =
"compiler\n\
Denis Davidyuk B8303a 2015\n\
Usage: compiler [KEY] [FILE]\n\
 -h\tshow this information\n\
 -l\tstart lexical analyzer\n\
 -s\tstart syntax analyzer\n";

//int argc = 3;
//char * argv[] = {"compiler", "-s", "test.c"};

const char * tokentype_tostring(enum yytokentype tt);

extern FILE * yyin;
extern char * yytext;
extern int line, column, yyleng;

NodeSyntax * astRoot = 0;

int main(int argc, char ** argv)
{
    if (argc == 1 || (argc == 2 && strcmp(argv[1], "-h"))) {
        fprintf(stdout, "%s", help);
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "-l") == 0) {
            printf("%-15s|", "Tokentype");
            printf("line|col |value|text");
            yyin = fopen(argv[2], "r");
            enum yytokentype t;
            do {
                printf("\n");
                t = yylex();
                if (t < 256) printf("%-15c|", t);
                else printf("%-15s|", tokentype_tostring(t));
                printf("%-4d|%-4d|%s", line, column - yyleng, yytext);
                switch(t) {
                //case CONSTANT: printf("%d", yylval); break;
                //case STRING_LITERAL: printf("%s", yylval); free(yylval); break;
                }
                printf("\n");
            } while (t);

        }
        else if (strcmp(argv[1], "-s") == 0) {
            yyin = fopen(argv[2], "r");
            if (yyparse()) {
                printf("Something wrong");
                return;
            }
            printNodeSyntax(astRoot, 0);
        }
        else {
            fprintf(stderr, help);
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, help);
        exit(EXIT_FAILURE);
    }
    return 0;
}

const char * tokentype_tostring(enum yytokentype tt) {
    switch (tt) {
    case IDENTIFIER: return "IDENTIFIER";
    case CONSTANT: return "CONSTANT";
    case STRING_LITERAL: return "STRING_LITERAL";
    case SIZEOF: return "SIZEOF";
    case PTR_OP: return "PTR_OP";
    case INC_OP: return "INC_OP";
    case DEC_OP: return "DEC_OP";
    case LEFT_OP: return "LEFT_OP";
    case RIGHT_OP: return "RIGHT_OP";
    case LE_OP: return "LE_OP";
    case GE_OP: return "GE_OP";
    case EQ_OP: return "EQ_OP";
    case NE_OP: return "NE_OP";
    case AND_OP: return "AND_OP";
    case OR_OP: return "OR_OP";
    case MUL_ASSIGN: return "MUL_ASSIGN";
    case DIV_ASSIGN: return "DIV_ASSIGN";
    case MOD_ASSIGN: return "MOD_ASSIGN";
    case ADD_ASSIGN: return "ADD_ASSIGN";
    case SUB_ASSIGN: return "SUB_ASSIGN";
    case LEFT_ASSIGN: return "LEFT_ASSIGN";
    case RIGHT_ASSIGN: return "RIGHT_ASSIGN";
    case AND_ASSIGN: return "AND_ASSIGN";
    case XOR_ASSIGN: return "XOR_ASSIGN";
    case OR_ASSIGN: return "OR_ASSIGN";
    case TYPE_NAME: return "TYPE_NAME";
    case TYPEDEF: return "TYPEDEF";
    case EXTERN: return "EXTERN";
    case STATIC: return "STATIC";
    case AUTO: return "AUTO";
    case REGISTER: return "REGISTER";
    case CHAR: return "CHAR";
    case SHORT: return "SHORT";
    case INT: return "INT";
    case LONG: return "LONG";
    case SIGNED: return "SIGNED";
    case UNSIGNED: return "UNSIGNED";
    case FLOAT: return "FLOAT";
    case DOUBLE: return "DOUBLE";
    case CONST: return "CONST";
    case VOLATILE: return "VOLATILE";
    case VOID: return "VOID";
    case STRUCT: return "STRUCT";
    case UNION: return "UNION";
    case ENUM: return "ENUM";
    case ELLIPSIS: return "ELLIPSIS";
    case CASE: return "CASE";
    case DEFAULT: return "DEFAULT";
    case IF: return "IF";
    case ELSE: return "ELSE";
    case SWITCH: return "SWITCH";
    case WHILE: return "WHILE";
    case DO: return "DO";
    case FOR: return "FOR";
    case GOTO: return "GOTO";
    case CONTINUE: return "CONTINUE";
    case BREAK: return "BREAK";
    case RETURN: return "RETURN";
    default:
        assert(0);
    }
}
