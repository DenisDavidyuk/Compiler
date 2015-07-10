%{
	#include <stddef.h>
	#include "ast.h"
	extern NodeSyntax * astRoot;
	/*#define YYSTYPE yys
	typedef union {
		NodeSyntax *node;
	} yys;*/
%}
%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
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

primary_expression
	: IDENTIFIER	{ $$ = createNodeSym($1); }
	| CONSTANT	{ $$ = createNodeSym($1); }
	| STRING_LITERAL	{ $$ = createNodeSym($1); }
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

/* начало declarations */
declaration
	: declaration_specifiers ';'	{ $$ = createNode2(DECL_OP, $1, NULL); }
	| declaration_specifiers init_declarator_list ';'	{ $$ = createNode2(DECL_OP, $1, $3); }
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
	: declarator
	| declarator '=' initializer /* обращение к symtable */
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
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
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
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;

type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
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
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement /* добавить таблицу в symTableStack */
	: '{' '}'	{ $$ = createNode2(COMPAUND, NULL, NULL); }
	| '{' statement_list '}'	{ $$ = createNode2(COMPAUND, NULL, $3); }
	| '{' declaration_list '}'	{ $$ = createNode2(COMPAUND, $2, NULL); }
	| '{' declaration_list statement_list '}'	{ $$ = createNode2(COMPAUND, $2, $3); }
	;

declaration_list
	: declaration	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| declaration_list declaration	{ $$ = appendNodeN($1, $2); }
	;

statement_list
	: statement	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| statement_list statement	{ $$ = appendNodeN($1, $2); }
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement	{ $$ = createNode2(IF, $3, $5); }
	| IF '(' expression ')' statement ELSE statement	{ $$ = createNode3(IF, $3, $5, $7); }
	| SWITCH '(' expression ')' statement	{ $$ = createNode2(SWITCH, $3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement	{ $$ = createNode2(WHILE_PRE, $3, $5); }
	| DO statement WHILE '(' expression ')' ';'	{ $$ = createNode2(WHILE_POST, $5, $2); }
	| FOR '(' expression_statement expression_statement ')' statement	{ $$ = createNode4(FOR, $3, $4, NULL, $6); }
	| FOR '(' expression_statement expression_statement expression ')' statement	{ $$ = createNode4(FOR, $3, $4, $5, $7); }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{ $$ = createNode1(GOTO, createNodeSym($2)); }
	| CONTINUE ';'	{ $$ = createNode1(GOTO, NULL); }
	| BREAK ';'	{ $$ = createNode1(BREAK, NULL); }
	| RETURN ';'	{ $$ = createNode1(RETURN, NULL); }
	| RETURN expression ';'	{ $$ = createNode1(RETURN, $2); }
	;

translation_unit
	: external_declaration	{ astRoot = appendNodeN(createNodeN(NONE), $1); }
	| translation_unit external_declaration	{ astRoot = appendNodeN(astRoot, $2); }
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement { $$ = createNode4(DECL_OP, $1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement { $$ = createNode4(DECL_OP, $1, $2, NULL, $3); }
	| declarator declaration_list compound_statement { $$ = createNode4(DECL_OP, NULL, $1, $2, $3); }
	| declarator compound_statement { $$ = createNode4(DECL_OP, NULL, $1, NULL, $2); }
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
