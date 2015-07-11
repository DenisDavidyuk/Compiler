#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "enums.h"
#include "ast.h"
#include "sym.h"
#include "asm.h"

const char * help =
"compiler\n\
Denis Davidyuk B8303a 2015\n\
Usage: compiler [KEY] [FILE]\n\
 -h\tshow this information\n\
 -l\tstart lexical analyzer\n\
 -s\tstart syntax analyzer\n";

extern FILE * yyin;
extern char * yytext;
extern int line, column, yyleng;
extern int yylex();
extern int yyparse();

//int argc = 3;
//char * argv[] = {"compiler", "-c", "test.c"};

//NodeSyntax * astRoot;
SymTable * symTable;

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
                printf("%-15s|", enum_tostring(t));
                printf("%-4d|%-4d|%s", line, column - yyleng, yytext);
                /*switch(t) {
                //case CONSTANT: printf("%d", yylval); break;
                //case STRING_LITERAL: printf("%s", yylval); free(yylval); break;
                    default:
                }*/
            } while (t);

        }
        else if (strcmp(argv[1], "-s") == 0) {
            yyin = fopen(argv[2], "r");
            int i;
            if ((i = yyparse())) {
                printf("Something wrong");
                return i;
            }
            //printNodeSyntax(astRoot, 0);
            //printSymTable(yylval.symTable, 0);
            printSymTable(symTable, 0);
        }
        else if (strcmp(argv[1], "-c") == 0) {
            yyin = fopen(argv[2], "r");
            int i;
            if ((i = yyparse())) {
                printf("Something wrong");
                return i;
            }
            printSymTable(symTable, 0);
            AsmCode * code = newAsmCode();
            asmSymTable(code, symTable);
            writeAsmCode(code);
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

void printfl(int level) {
    printf("\n");
    while(level--)
        printf("  ");
}

void error(const char * message) {
    fprintf(stderr, message);
    exit(EXIT_FAILURE);
}

char * randomString() {
    char * res = malloc(sizeof(char) * 20);
    for (int i = 0; i < 19; i++)
        res[i] = rand() % 26 + 'a';
    res[19] = 0;
    return res;
}
