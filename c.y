%{
	#include <stddef.h>
	#include "ast.h"
	#include "sym.h"
	

	NodeSyntax * declaration_specifiers_current;
%}
%union {
	int i;
	char * s;
	double d;
}
/*%locations*/
%token <s> IDENTIFIER STRING_LITERAL
%token <i> CONSTANT_CHAR CONSTANT_INT
%token <d> CONSTANT_DOUBLE
%token SIZEOF
/*%token IDENTIFIER CONSTANT_CHAR CONSTANT_INT CONSTANT_DOUBLE STRING_LITERAL SIZEOF*/

%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%start translation_unit
%%

/* начало expression */
primary_expression
	: IDENTIFIER	{ $$ = createNodeSym(IDENTIFIER, $1); }
	| CONSTANT_CHAR	{ $$ = createNodeVar(CONSTANT_CHAR, $1); }
	| CONSTANT_INT	{ $$ = createNodeVar(CONSTANT_INT, $1); }
	| CONSTANT_DOUBLE	{ $$ = createNodeVar(CONSTANT_DOUBLE, $1); }
	| STRING_LITERAL	{ $$ = createNodeVar(STRING_LITERAL, $1); }
	| '(' expression ')'	{ $$ = $2; }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'	{ $$ = createNode2(ARR_OP, $1, $3); }
	| postfix_expression '(' ')'	{ $$ = createNode2(CALL_OP, $1, createNodeN(NONE)); }
	| postfix_expression '(' argument_expression_list ')'	{ $$ = createNode2(CALL_OP, $1, $3); }
	| postfix_expression '.' IDENTIFIER	{ $$ = createNode2(FIELD_OP, $1, createNodeSym($3)); }
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = createNode2($2, $1, createNodeSym($3)); }
	| postfix_expression INC_OP	{ $$ = createNode1(INC_SUFF, $1); }
	| postfix_expression DEC_OP	{ $$ = createNode1(DEC_SUFF, $1); }
	;

argument_expression_list
	: assignment_expression	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| argument_expression_list ',' assignment_expression	{ $$ = appendNodeN($1, $3); }
	;

unary_expression /* l-value! */
	: postfix_expression
	| INC_OP unary_expression	{ $$ = createNode1(INC_PREF, $2); }
	| DEC_OP unary_expression	{ $$ = createNode1(DEC_PREF, $2); }
	| unary_operator cast_expression	{ $$ = createNode1($1, $2); }
	| SIZEOF unary_expression	{ $$ = createNode1($1, $2); }
	| SIZEOF '(' type_name ')'	{ $$ = createNode1($1, $2); }
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression	{ $$ = createNode2(CAST_OP, $2, $4); }
	;

/* начало бинарных операторов */
multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression	{ $$ = createNode2($2, $1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = createNode2($2, $1, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = createNode2($2, $1, $3); }
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression	{ $$ = createNode2($2, $1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = createNode2($2, $1, $3); }
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression	{ $$ = createNode2($2, $1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = createNode2($2, $1, $3); }
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression	{ $$ = createNode2($2, $1, $3); }
	| relational_expression '>' shift_expression	{ $$ = createNode2($2, $1, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = createNode2($2, $1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = createNode2($2, $1, $3); }
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression	{ $$ = createNode2($2, $1, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = createNode2($2, $1, $3); }
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression	{ $$ = createNode2($2, $1, $3); }
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression	{ $$ = createNode2($2, $1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = createNode2($2, $1, $3); }
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = createNode2($2, $1, $3); }
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression	{ $$ = createNode2($2, $1, $3); }
	;
/* конец бинарных операторов */

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = createNode3(TERNAL_OP, $1, $2, $3); }
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression	{ $$ = createNode2($2, $1, $3); }
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression	{ $$ = createNode2($2, $1, $3); }
	;

constant_expression
	: conditional_expression	{ $$ = createNode1(CONST_CK, $1); } /* ??? */
	;
/* конец expression */

/* начало declarations */
declaration /* добавить элементы в SymTable, создать новый compaund */
	: declaration_specifiers ';'	{ } /* а не поместится-ли суда typedef? */
	| declaration_specifiers init_declarator_list ';'	{ appendDeclaration($1, $2); }
	;

declaration_specifiers /* сразу заполнять sym table надо? */
	: storage_class_specifier
	| storage_class_specifier declaration_specifiers	{ $$ = createNode2(DECL_OP, $1, $2); } /* ??? */
	| type_specifier
	| type_specifier declaration_specifiers	{ $$ = createNode2(DECL_OP, $1, $2); } /* ??? */
	| type_qualifier
	| type_qualifier declaration_specifiers	{ $$ = createNode2(DECL_OP, $1, $2); } /* ??? */
	/*| function_specifier
	| function_specifier declaration_specifiers*/
	;

init_declarator_list
	: init_declarator	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| init_declarator_list ',' init_declarator	{ $$ = appendNodeN($1, $2); }
	;

init_declarator
	: declarator	{ $$ = createNode2(NONE, $1, NULL); }
	| declarator '=' initializer	{ $$ = createNode2(NONE, $1, $3); }
	;

storage_class_specifier
	: TYPEDEF
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' { $$ = createNode2($1, $2, $3); }
	| struct_or_union '{' struct_declaration_list '}' { $$ = createNode2($1, NULL, $3); }
	| struct_or_union IDENTIFIER { $$ = createNode2($1, $2, NULL); }
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration { $$ = appendNodeN(createNodeN(NONE), $1); }
	| struct_declaration_list struct_declaration { $$ = appendNodeN($1, $2); }
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression /* битовые поля */
	| declarator ':' constant_expression /* битовые поля */
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { $$ = createNode2($1, NULL, $3); }
	| ENUM IDENTIFIER '{' enumerator_list '}' { $$ = createNode2($1, $2, $3); }
	| ENUM IDENTIFIER { $$ = createNode2($1, $2, NULL); }
	;

enumerator_list
	: enumerator { $$ = appendNodeN(createNodeN(NONE), $1); }
	| enumerator_list ',' enumerator { $$ = appendNodeN($1, $2); }
	;

enumerator
	: IDENTIFIER { $$ = createNode2(ENUMERATOR, $1, NULL); }
	| IDENTIFIER '=' constant_expression { $$ = createNode2(ENUMERATOR, $1, $3); }
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator	{ $$ = createNode2(NONE, $1, $2); }
	| direct_declarator		{ $$ = createNode2(NONE, NULL, $1); }
	;

direct_declarator
	: IDENTIFIER	{ $$ = createNode1($1, NULL); }
	| '(' declarator ')'	{ $$ = createNode1(NONE, $2); }
	| direct_declarator '[' constant_expression ']'	{ $$ = createNode2(ARRAY, $1, $3); }
	| direct_declarator '[' ']'	{ $$ = createNode2(ARRAY, $1, NULL); }
	| direct_declarator '(' parameter_type_list ')'	{ $$ = createNode2(FUNC_TYPED, $1, $3); }
	| direct_declarator '(' identifier_list ')'	{ $$ = createNode2(FUNC, $1, $3); }
	| direct_declarator '(' ')'	{ $$ = createNode2(FUNC, $1, NONE); }
	;

pointer
	: '*'	{ $$ = appendNodeN(createNodeN(NONE), createNode1(POINTER, NULL)); }
	| '*' type_qualifier_list	{ $$ = appendNodeN(createNodeN(NONE), createNode1(POINTER, $2)); }
	| '*' pointer	{ $$ = appendNodeN($2, createNode1(POINTER, NULL)); }
	| '*' type_qualifier_list pointer	{ $$ = appendNodeN($3, createNode1(POINTER, $2)); }
	;

type_qualifier_list
	: type_qualifier	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| type_qualifier_list type_qualifier	{ $$ = appendNodeN($1, $2); }
	;


parameter_type_list
	: parameter_list
/* 	| parameter_list ',' ELLIPSIS поддержка функций с неограниченным количеством аргументов */
	;

parameter_list
	: parameter_declaration	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| parameter_list ',' parameter_declaration { $$ = appendNodeN($1, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER	{ $$ = appendNodeN(createNodeN(NONE), createNodeSym($1)); }
	| identifier_list ',' IDENTIFIER { $$ = appendNodeN($1, createNodeSym($3)); }
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}' { $$ = $2; }
	| '{' initializer_list ',' '}' { $$ = $2; }
	;

initializer_list
	: initializer { $$ = appendNodeN(createNodeN(INITIALIZER_LIST), $1); }
	| initializer_list ',' initializer { $$ = appendNodeN($1, $3); }
	;
/* конец declarations */

/* начало statement */
statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement /* игнорирование меток */
	: IDENTIFIER ':' statement	{ $$ = $3; }
	| CASE constant_expression ':' statement	{ $$ = $4; }
	| DEFAULT ':' statement	{ $$ = $3; }
	;

compound_statement /* добавить таблицу в symTableStack */
	: '{' '}'	{ $$ = createNode1(COMPOUND_STATEMENT, NULL); }
	| '{' compound_statement_nested '}'	{ $$ = $2; }

compound_statement_nested //объявления в середине функции
	: statement_list	{ $$ = createNode1(COMPOUND_STATEMENT, $1); }
	| declaration_list statement_list { $$ = createNode1(COMPOUND_STATEMENT, $2); }
	;

declaration_list
	: declaration	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| declaration_list declaration	{ $$ = appendNodeN($1, $2); }
	;

statement_list
	: statement	{ $$ = appendNodeN(createNodeN(STATEMENT_LIST), $1); }
	| statement_list statement	{ $$ = appendNodeN($1, $2); }
	;

expression_statement
	: ';' { $$ = NULL; }
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement	{ $$ = createNode2(IF, $3, $5); }
	| IF '(' expression ')' statement ELSE statement	{ $$ = createNode3(IF, $3, $5, $7); }
	| SWITCH '(' expression ')' statement	{ $$ = createNode2(SWITCH, $3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement	{ $$ = createNode2($1, $3, $5); }
	| DO statement WHILE '(' expression ')' ';'	{ $$ = createNode2(DO_WHILE, $5, $2); }
	| FOR '(' expression_statement expression_statement ')' statement	{ $$ = createNode4(FOR, $3, $4, NULL, $6); }
	| FOR '(' expression_statement expression_statement expression ')' statement	{ $$ = createNode4(FOR, $3, $4, $5, $7); }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{ $$ = createNode1($1, createNodeSym($2)); }
	| CONTINUE ';'	{ $$ = createNode0($1); }
	| BREAK ';'	{ $$ = createNode0($1); }
	| RETURN ';'	{ $$ = createNode0($1); }
	| RETURN expression ';'	{ $$ = createNode1($1, $2); }
	;
/* конец statement */

translation_unit
	: external_declaration	{  }
	| translation_unit external_declaration	{  }
	;

external_declaration
	: function_definition	{  }
	| declaration	{  }
	;

function_definition
	: declaration_specifiers declarator compound_statement	{ /* создаать символ */ }
	| declarator compound_statement	{ /* создаать символ */ }
	;

%%
#include <stdio.h>

extern char yytext[];
extern int column;

yyerror(s)
char *s;
{
	fflush(stdout);
	fprintf(stderr, "\n%*s\n%*s\n", column, "^", column, s);
}
