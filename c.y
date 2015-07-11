%{
	#include <stddef.h>
	#include <assert.h>
	#include "ast.h"
	#include "sym.h"
	#define YYDEBUG 1
	//extern NodeSyntax * astRoot;
	extern SymTable * symTable;
%}
%union {
	float d;
	int i;
	char * s;
	//NodeSyntax * nodeSyntax;
	//SymTable * symTable;
	void * nodeSyntax;
	void * symTable;
}
/*%locations*/
%token <s> IDENTIFIER STRING_LITERAL CONSTANT_CHAR
%token <i> CONSTANT_INT
%token <d> CONSTANT_DOUBLE
%token SIZEOF

%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%type <nodeSyntax> primary_expression postfix_expression argument_expression_list
%type <nodeSyntax> unary_expression cast_expression multiplicative_expression additive_expression
%type <nodeSyntax> shift_expression relational_expression equality_expression and_expression
%type <nodeSyntax> exclusive_or_expression inclusive_or_expression logical_and_expression
%type <nodeSyntax> logical_or_expression conditional_expression assignment_expression
%type <nodeSyntax> expression constant_expression
%type <nodeSyntax> declaration_specifiers init_declarator_list init_declarator type_specifier struct_or_union_specifier
%type <nodeSyntax> struct_declaration_list struct_declaration specifier_qualifier_list
%type <nodeSyntax> struct_declarator_list struct_declarator enum_specifier enumerator_list
%type <nodeSyntax> enumerator declarator direct_declarator pointer type_qualifier_list
%type <nodeSyntax> parameter_type_list parameter_list parameter_declaration identifier_list
%type <nodeSyntax> type_name abstract_declarator direct_abstract_declarator initializer
%type <nodeSyntax> initializer_list
%type <nodeSyntax> statement labeled_statement compound_statement /*compound_statement_nested*/
%type <nodeSyntax> /*declaration_list*/ statement_list expression_statement selection_statement
%type <nodeSyntax> iteration_statement jump_statement

%type <i> unary_operator assignment_operator storage_class_specifier
%type <i> struct_or_union type_qualifier

%type <symTable> declaration translation_unit external_declaration function_definition declaration_list

%start translation_unit
%%

/* начало expression */
primary_expression
	: IDENTIFIER	{ $$ = createNodeSym($1); }
	| CONSTANT_CHAR	{ $$ = createNodeValChar($1); }
	| CONSTANT_INT	{ $$ = createNodeValInt($1); }
	| CONSTANT_DOUBLE	{ $$ = createNodeValDouble($1); }
	| STRING_LITERAL	{ $$ = createNodeValString($1); }
	| '(' expression ')'	{ $$ = $2; }
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'	{ $$ = createNode2(ARR_OP, $1, $3); }
	| postfix_expression '(' ')'	{ $$ = createNode2(FUNC_CALL, $1, createNodeN(ARGUMENT_EXPRESSION_LIST)); }
	| postfix_expression '(' argument_expression_list ')'	{ $$ = createNode2(FUNC_CALL, $1, $3); }
	| postfix_expression '.' IDENTIFIER	{ $$ = createNode2(FIELD_OP, $1, createNodeSym($3)); }
	| postfix_expression PTR_OP IDENTIFIER	{ $$ = createNode2(PTR_OP, $1, createNodeSym($3)); }
	| postfix_expression INC_OP	{ $$ = createNode1(INC_SUFF, $1); }
	| postfix_expression DEC_OP	{ $$ = createNode1(DEC_SUFF, $1); }
	;

argument_expression_list
	: assignment_expression	{ $$ = appendNodeN(createNodeN(ARGUMENT_EXPRESSION_LIST), $1); }
	| argument_expression_list ',' assignment_expression	{ $$ = appendNodeN($1, $3); }
	;

unary_expression /* l-value! */
	: postfix_expression
	| INC_OP unary_expression	{ $$ = createNode1(INC_PREF, $2); }
	| DEC_OP unary_expression	{ $$ = createNode1(DEC_PREF, $2); }
	| unary_operator cast_expression	{ $$ = createNode1($1, $2); }
	| SIZEOF unary_expression	{ $$ = createNode1(SIZEOF, $2); }
	| SIZEOF '(' type_name ')'	{ $$ = createNode1(SIZEOF, $3); }
	;

unary_operator
	: '&' { $$ = '&'; }
	| '*' { $$ = '*'; }
	| '+' { $$ = '+'; }
	| '-' { $$ = '-'; }
	| '~' { $$ = '~'; }
	| '!' { $$ = '!'; }
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression	{ $$ = createNode2(CAST_OP, $2, $4); }
	;

/* начало бинарных операторов */
multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression	{ $$ = createNode2('*', $1, $3); }
	| multiplicative_expression '/' cast_expression	{ $$ = createNode2('/', $1, $3); }
	| multiplicative_expression '%' cast_expression	{ $$ = createNode2('%', $1, $3); }
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression	{ $$ = createNode2('+', $1, $3); }
	| additive_expression '-' multiplicative_expression	{ $$ = createNode2('-', $1, $3); }
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP  additive_expression	{ $$ = createNode2(LEFT_OP , $1, $3); }
	| shift_expression RIGHT_OP additive_expression	{ $$ = createNode2(RIGHT_OP, $1, $3); }
	;

relational_expression
	: shift_expression
	| relational_expression '<' shift_expression	{ $$ = createNode2('<', $1, $3); }
	| relational_expression '>' shift_expression	{ $$ = createNode2('>', $1, $3); }
	| relational_expression LE_OP shift_expression	{ $$ = createNode2(LE_OP, $1, $3); }
	| relational_expression GE_OP shift_expression	{ $$ = createNode2(GE_OP, $1, $3); }
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression	{ $$ = createNode2(EQ_OP, $1, $3); }
	| equality_expression NE_OP relational_expression	{ $$ = createNode2(NE_OP, $1, $3); }
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression	{ $$ = createNode2('&', $1, $3); }
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression	{ $$ = createNode2('^', $1, $3); }
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression	{ $$ = createNode2('|', $1, $3); }
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression	{ $$ = createNode2(AND_OP, $1, $3); }
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression	{ $$ = createNode2(OR_OP, $1, $3); }
	;
/* конец бинарных операторов */

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression	{ $$ = createNode3(TERNAL_OP, $1, $3, $5); }
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression	{ $$ = createNode2($2, $1, $3); }
	;

assignment_operator
	: '='			{ $$ = '='; }
	| MUL_ASSIGN	{ $$ = MUL_ASSIGN; }
	| DIV_ASSIGN	{ $$ = DIV_ASSIGN; }
	| MOD_ASSIGN	{ $$ = MOD_ASSIGN; }
	| ADD_ASSIGN	{ $$ = ADD_ASSIGN; }
	| SUB_ASSIGN	{ $$ = SUB_ASSIGN; }
	| LEFT_ASSIGN	{ $$ = LEFT_ASSIGN; }
	| RIGHT_ASSIGN	{ $$ = RIGHT_ASSIGN; }
	| AND_ASSIGN	{ $$ = AND_ASSIGN; }
	| XOR_ASSIGN	{ $$ = XOR_ASSIGN; }
	| OR_ASSIGN		{ $$ = OR_ASSIGN; }
	;

expression
	: assignment_expression
	| expression ',' assignment_expression	{ $$ = createNode2(',', $1, $3); }
	;

constant_expression
	: conditional_expression	{ $$ = createNode1(CONSTANT_EVAL, $1); } /* ??? */
	;
/* конец expression */

/* начало declarations */
declaration /* добавить элементы в SymTable, создать новый compaund */
	: declaration_specifiers ';'	{ $$ = createSymTable(); }
	| declaration_specifiers init_declarator_list ';'	{ $$ = declarationToSymTable($1, $2); /*$$ = createNode2(DECLARATION, $1, $2);*/ }
	//| declaration_specifiers init_declarator_list ';'	{ $$ = createNode2(DECLARATION, $1, $2); }
	;

declaration_specifiers /* const const int float */
	: storage_class_specifier	{ $$ = appendNodeN(createNodeN(DECLARATION_SPECIFIERS), $1); }
	| storage_class_specifier declaration_specifiers	{ $$ = appendNodeN($2, $1); }
	| type_specifier	{ $$ = appendNodeN(createNodeN(DECLARATION_SPECIFIERS), $1); }
	| type_specifier declaration_specifiers	{ $$ = appendNodeN($2, $1); }
	| type_qualifier	{ $$ = appendNodeN(createNodeN(DECLARATION_SPECIFIERS), $1); }
	| type_qualifier declaration_specifiers	{ $$ = appendNodeN($2, $1); }
	/*| function_specifier
	| function_specifier declaration_specifiers*/
	;

init_declarator_list
	: init_declarator	{ $$ = appendNodeN(createNodeN(INIT_DECLARATOR_LIST), $1); }
	| init_declarator_list ',' init_declarator	{ $$ = appendNodeN($1, $3); }
	;

init_declarator
	: declarator	{ $$ = createNode2(INIT_DECLARATOR, $1, NULL); }
	| declarator '=' initializer	{ $$ = createNode2(INIT_DECLARATOR, $1, $3); }
	;

storage_class_specifier
	: TYPEDEF	{ $$ = TYPEDEF; }
	| EXTERN	{ $$ = EXTERN; }
	| STATIC	{ $$ = STATIC; }
	| AUTO		{ $$ = AUTO; }
	| REGISTER	{ $$ = REGISTER; }
	;

type_specifier
	: VOID		{ $$ = createNode0(VOID); }
	| CHAR		{ $$ = createNode0(CHAR); }
	| SHORT		{ $$ = createNode0(SHORT); }
	| INT		{ $$ = createNode0(INT); }
	| LONG		{ $$ = createNode0(LONG); }
	| FLOAT		{ $$ = createNode0(FLOAT); }
	| DOUBLE	{ $$ = createNode0(DOUBLE); }
	| SIGNED	{ $$ = createNode0(SIGNED); }
	| UNSIGNED	{ $$ = createNode0(UNSIGNED); }
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME	{ $$ = createNode0(TYPE_NAME); assert(0); }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}' { $$ = createNode2($1, $2, $4); }
	| struct_or_union '{' struct_declaration_list '}' { $$ = createNode2($1, NULL, $3); }
	| struct_or_union IDENTIFIER { $$ = createNode2($1, $2, NULL); }
	;

struct_or_union
	: STRUCT	{ $$ = STRUCT; }
	| UNION	{ $$ = UNION; }
	;

struct_declaration_list
	: struct_declaration { $$ = appendNodeN(createNodeN(NONE), $1); }
	| struct_declaration_list struct_declaration { $$ = appendNodeN($1, $2); }
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'	{ $$ = createNode2(STRUCT_DECLARATION, $1, $2); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list	{ $$ = appendNodeN($1, $2); }
	| type_specifier	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| type_qualifier specifier_qualifier_list	{ $$ = appendNodeN($1, $2); }
	| type_qualifier	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	;

struct_declarator_list
	: struct_declarator	{ $$ = appendNodeN(createNodeN(NONE), $1); }
	| struct_declarator_list ',' struct_declarator	{ $$ = appendNodeN($1, $3); }
	;

struct_declarator
	: declarator
	| ':' constant_expression /* битовые поля */ { $$ = NULL; }
	| declarator ':' constant_expression /* битовые поля */
	;

enum_specifier
	: ENUM '{' enumerator_list '}' { $$ = createNode2(ENUM, NULL, $3); }
	| ENUM IDENTIFIER '{' enumerator_list '}' { $$ = createNode2(ENUM, createNodeSym($2), $4); }
	| ENUM IDENTIFIER { $$ = createNode2(ENUM, createNodeSym($2), NULL); }
	;

enumerator_list
	: enumerator { $$ = appendNodeN(createNodeN(NONE), $1); }
	| enumerator_list ',' enumerator { $$ = appendNodeN($1, $3); }
	;

enumerator
	: IDENTIFIER { $$ = createNode2(ENUMERATOR, createNodeSym($1), NULL); }
	| IDENTIFIER '=' constant_expression { $$ = createNode2(ENUMERATOR, createNodeSym($1), $3); }
	;

type_qualifier
	: CONST		{ $$ = createNode0(CONST); }
	| VOLATILE	{ $$ = createNode0(VOLATILE); }
	;

declarator
	: pointer direct_declarator	{ $$ = appendNodeN($2, $1); }
	| direct_declarator
	;

direct_declarator
	: IDENTIFIER	{ $$ = appendNodeN(createNodeN(DECLARATION), createNodeSym($1)); }
	| '(' declarator ')'	{ $$ = $2; }
	| direct_declarator '[' constant_expression ']'	{ $$ = appendNodeN($1, createNode1(ARRAY, $3)); }
	| direct_declarator '[' ']'	{ $$ = appendNodeN($1, createNode1(ARRAY, NULL)); }
	| direct_declarator '(' parameter_type_list ')'	{ $$ = appendNodeN($1, $3); }
	//| direct_declarator '(' identifier_list ')'	{ $$ = appendNodeN($1, createNode1(FUNC_CALL, $3)); }
	| direct_declarator '(' ')'	{ $$ = appendNodeN($1, createNodeN(PARAMETER_LIST)); }
	;

pointer
	: '*'	{ $$ = appendNodeN(createNodeN(DECLARATOR_POINTER), createNode0(POINTER)); }
	| '*' type_qualifier_list	{ $$ = appendNodeN(appendNodeN(createNodeN(DECLARATOR_POINTER), $2), createNode0(POINTER)); }
	| '*' pointer	{ $$ = appendNodeN($2, createNode0(POINTER)); }
	| '*' type_qualifier_list pointer	{ $$ = appendNodeN(appendNodeN($3, $2), createNode0(POINTER)); }
	;

type_qualifier_list
	: type_qualifier	{ $$ = appendNodeN(createNodeN(TYPE_QUALIFIER_LIST), $1); }
	| type_qualifier_list type_qualifier	{ $$ = appendNodeN($1, $2); }
	;


parameter_type_list
	: parameter_list
/* 	| parameter_list ',' ELLIPSIS поддержка функций с неограниченным количеством аргументов */
	;

parameter_list
	: parameter_declaration	{ $$ = appendNodeN(createNodeN(PARAMETER_LIST), $1); }
	| parameter_list ',' parameter_declaration { $$ = appendNodeN($1, $3); }
	;

parameter_declaration
	: declaration_specifiers declarator	{ $$ = createNode2(PARAMETER_DECLARATION, $1, $2); }
	| declaration_specifiers abstract_declarator	{ $$ = createNode2(PARAMETER_DECLARATION, $1, $2); }
	| declaration_specifiers	{ $$ = createNode2(PARAMETER_DECLARATION, $1, NULL); }
	;

identifier_list
	: IDENTIFIER	{ $$ = appendNodeN(createNodeN(NONE), createNodeSym($1)); }
	| identifier_list ',' IDENTIFIER { $$ = appendNodeN($1, createNodeSym($3)); }
	;

type_name
	: specifier_qualifier_list	{ $$ = createNode2(NONE, $1, NULL); }
	| specifier_qualifier_list abstract_declarator	{ $$ = createNode2(NONE, $1, $2); }
	;

abstract_declarator
	: pointer	{ $$ = createNode2(NONE, $1, NULL); }
	| direct_abstract_declarator			{ $$ = createNode2(NONE, NULL, $1); }
	| pointer direct_abstract_declarator	{ $$ = createNode2(NONE, $1, $2); }
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'								{ $$ = appendNodeN(createNodeN(DECLARATION), $2); }
	| '[' ']'													{ $$ = appendNodeN(createNodeN(DECLARATION), createNode0(ARRAY)); }
	| '[' constant_expression ']'								{ $$ = appendNodeN(createNodeN(DECLARATION), createNode1(ARRAY, $2)); }
	| direct_abstract_declarator '[' ']'						{ $$ = appendNodeN($1, createNode0(ARRAY)); }
	| direct_abstract_declarator '[' constant_expression ']'	{ $$ = appendNodeN($1, createNode1(ARRAY, $3)); }
	| '(' ')'													{ $$ = appendNodeN(createNodeN(DECLARATION), createNode0(NONE)); }
	| '(' parameter_type_list ')'								{ $$ = appendNodeN(createNodeN(DECLARATION), $2); }
	| direct_abstract_declarator '(' ')'						{ $$ = appendNodeN($1, createNode0(FUNC_DECLARATION)); }
	| direct_abstract_declarator '(' parameter_type_list ')'	{ $$ = appendNodeN($1, createNode1(FUNC_DECLARATION, $3)); }
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'		{ $$ = $2; }
	| '{' initializer_list ',' '}'	{ $$ = $2; }
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
	//| declaration { $$ = NULL; } //не std-c89
	;

labeled_statement /* игнорирование меток */
	: IDENTIFIER ':' statement	{ $$ = $3; }
	| CASE constant_expression ':' statement	{ $$ = $4; }
	| DEFAULT ':' statement	{ $$ = $3; }
	;

compound_statement
	: '{' '}'	{ $$ = createNode1(COMPOUND_STATEMENT, NULL); }
	| '{' statement_list '}'	{ $$ = createNode2(COMPOUND_STATEMENT, NULL, $2); }
	
	| '{' declaration_list '}'	{ $$ = createNode2(COMPOUND_STATEMENT, $2, NULL); }
	| '{' declaration_list statement_list '}'	{ $$ = createNode2(COMPOUND_STATEMENT, $2, $3); }
	;

/*compound_statement
	: '{' '}'	{ $$ = createNode1(COMPOUND_STATEMENT, NULL); }
	| '{' compound_statement_nested '}'	{ $$ = $2; }

compound_statement_nested //объявления в середине функции
	: statement_list	{ $$ = createNode1(COMPOUND_STATEMENT, $1); }
	| declaration_list	{ $$ = createNode1(COMPOUND_STATEMENT, NULL); }
	| declaration_list statement_list { $$ = createNode1(COMPOUND_STATEMENT, $2); }
	;*/

declaration_list
	: declaration	{ $$ = mergeSymTables(createSymTable(), $1); }
	| declaration_list declaration	{ $$ = mergeSymTables($1, $2); }
	;

statement_list
	: statement	{ $$ = appendNodeN(createNodeN(STATEMENT_LIST), $1); }
	| statement_list statement	{ $$ = appendNodeN($1, $2); }
	;

expression_statement
	: ';' { $$ = createNode0(NONE); }
	| expression ';'
	;

selection_statement
	: IF '(' expression ')' statement	{ $$ = createNode2(IF, $3, $5); }
	| IF '(' expression ')' statement ELSE statement	{ $$ = createNode3(IF, $3, $5, $7); }
	| SWITCH '(' expression ')' statement	{ $$ = createNode2(SWITCH, $3, $5); }
	;

iteration_statement
	: WHILE '(' expression ')' statement	{ $$ = createNode2(WHILE, $3, $5); }
	| DO statement WHILE '(' expression ')' ';'	{ $$ = createNode2(DO_WHILE, $5, $2); }
	| FOR '(' expression_statement expression_statement ')' statement	{ $$ = createNode4(FOR, $3, $4, NULL, $6); }
	| FOR '(' expression_statement expression_statement expression ')' statement	{ $$ = createNode4(FOR, $3, $4, $5, $7); }
	;

jump_statement
	: GOTO IDENTIFIER ';'	{ $$ = createNode1(GOTO, createNodeSym($2)); }
	| CONTINUE ';'	{ $$ = createNode0(CONTINUE); }
	| BREAK ';'	{ $$ = createNode0(BREAK); }
	| RETURN ';'	{ $$ = createNode0(RETURN); }
	| RETURN expression ';'	{ $$ = createNode1(RETURN, $2); }
	;
/* конец statement */

translation_unit /* добавить символ external_declaration в таблицу символов */
	: external_declaration	{ symTable = $$ = mergeSymTables(createSymTable(), $1); /*astRoot = appendNodeN(createNodeN(NONE), $1);*/ }
	| translation_unit external_declaration	{ symTable = $$ = mergeSymTables($1, $2); /*astRoot = appendNodeN(astRoot, $2);*/ }
	;

external_declaration
	: function_definition	{ $$ = appendToSymTable(createSymTable(), $1); }
	| declaration
	;

function_definition
	: declaration_specifiers declarator compound_statement	{ $$ = functionDefinitionToSymTable($1, $2, $3); }
	| declarator compound_statement	{ $$ = functionDefinitionToSymTable(NULL, $1, $2); }
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
