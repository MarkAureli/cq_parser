%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"

extern int yylex(void);
extern int yylineno;
int yyerror(const char *s);
int success = 1;
extern FILE *yyin;
extern FILE *yyout;

%}

/* Union to define yylval's types */
%union {
    char *str;
    bool bconst;
    int iconst;
    unsigned uconst;
    list_t *symtab_item;
    node_t *node;
}

%token <str> ID
%token <bconst> BCONST
%token <iconst> ICONST
%token <uconst> UCONST
%token <uconst> BOOL INT UNSIGNED VOID
%token <uconst> CONST QUANTUM
%token <uconst> BREAK CONTINUE DO FOR RETURN WHILE
%token <uconst> CASE DEFAULT ELSE IF SWITCH
%token <uconst> MEASURE
%token <uconst> PHASE
%token <uconst> ADD DIV MOD MUL SUB
%token <uconst> BIT_AND BIT_LSHIFT BIT_OR BIT_RSHIFT BIT_XOR
%token <uconst> AND EQ GE GEQ LE LEQ NEQ NOT NOT_OP OR XOR
%token <uconst> ASSIGN ASSIGN_ADD ASSIGN_AND ASSIGN_DIV ASSIGN_LSHIFT ASSIGN_MOD ASSIGN_MUL ASSIGN_OR ASSIGN_RSHIFT ASSIGN_SUB ASSIGN_XOR
%token <uconst> COLON COMMA LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON
%left LPAREN RPAREN LBRACKET RBRACKET
%left BIT_LSHIFT BIT_RSHIFT
%left ADD SUB
%left MUL DIV MOD
%left BIT_XOR
%left BIT_OR
%left BIT_AND
%left XOR
%left OR
%left AND
%right NOT_OP
%right NOT
%nonassoc "then"
%nonassoc ELSE
%define parse.error verbose
%start program

%%

program:
	declaration_list
	;

declaration_list:
	declaration declaration_list
	| declaration
	;

declaration:
	 variable_declaration
	| variable_definition
	| function_definition
	;

function_definition:
	QUANTUM type_specifier declarator { incr_scope(); } function_head function_tail { hide_scope(); }
	| type_specifier declarator { incr_scope(); } function_head function_tail { hide_scope(); }
	| VOID declarator { incr_scope(); } function_head function_tail { hide_scope(); }
	;

function_head:
    LPAREN parameter_list RPAREN
    ;

function_tail:
    LBRACE statement_list RBRACE
    ;

parameter_list:
	parameter_list COMMA parameter
	| parameter
	| /* empty */
	;

parameter:
	QUANTUM type_specifier declarator
	| type_specifier declarator
	;

variable_declaration:
    QUANTUM type_specifier declarator SEMICOLON
    | type_specifier declarator SEMICOLON
    ;

variable_definition:
	QUANTUM type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| QUANTUM type_specifier declarator ASSIGN LBRACE initializer_list RBRACE SEMICOLON
	| QUANTUM type_specifier declarator ASSIGN LBRACKET initializer_list RBRACKET SEMICOLON
	| CONST type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| CONST type_specifier declarator ASSIGN LBRACE initializer_list RBRACE SEMICOLON
	| type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| type_specifier declarator ASSIGN LBRACE initializer_list RBRACE SEMICOLON
	;

declarator:
	ID { insert($1, strlen($1), UNDEFINED_T, yylineno, true); }
	;

initializer_list:
	initializer_list COMMA logical_or_expr
	| logical_or_expr
	;

type_specifier:
	BOOL
	| INT
	| UNSIGNED
	| type_specifier LBRACKET logical_or_expr RBRACKET
	| type_specifier LBRACKET RBRACKET
	;

statement_list:
	statement
	| statement_list statement
	;

statement:
	expr_statement
	| if_statement
	| switch_statement
	| do_statement
	| while_statement
	| for_statement
	| jump_statement
	;

expr_statement:
	expr SEMICOLON
	| SEMICOLON
	;

if_statement:
	IF LPAREN logical_or_expr RPAREN LBRACE statement_list RBRACE	%prec "then"
	| IF LPAREN logical_or_expr RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE

switch_statement:
	SWITCH LPAREN logical_or_expr RPAREN LBRACE case_statement_list RBRACE
	;

case_statement_list:
    case_statement
    | case_statement_list case_statement
    ;

case_statement:
	CASE logical_or_expr COLON statement_list
	| DEFAULT COLON statement_list
	;

do_statement:
	DO LBRACE statement_list RBRACE WHILE LPAREN expr RPAREN SEMICOLON
    ;

while_statement:
    WHILE LPAREN expr RPAREN LBRACE statement_list RBRACE
    ;

for_statement:
    FOR LPAREN expr_statement expr_statement expr RPAREN LBRACE statement_list RBRACE
    ;

jump_statement:
	CONTINUE SEMICOLON
	| BREAK SEMICOLON
	| RETURN logical_or_expr SEMICOLON
	| RETURN SEMICOLON
	;

expr:
	logical_or_expr
	| unary_expr assignment_operator expr
	;

assignment_operator:
	ASSIGN_AND
	| ASSIGN_OR
	| ASSIGN_XOR
	| ASSIGN_LSHIFT
	| ASSIGN_RSHIFT
	| ASSIGN_MUL
	| ASSIGN_DIV
	| ASSIGN_MOD
	| ASSIGN_ADD
	| ASSIGN_SUB
	| ASSIGN
	;

logical_or_expr:
	logical_xor_expr
	| logical_or_expr OR logical_xor_expr
	;

logical_xor_expr:
	logical_and_expr
	| logical_xor_expr XOR logical_and_expr
	;

logical_and_expr:
	bit_or_expr
	| logical_and_expr AND bit_or_expr
	;

bit_or_expr:
	bit_xor_expr
	| bit_or_expr BIT_OR bit_xor_expr
	;

bit_xor_expr:
	bit_and_expr
	| bit_xor_expr BIT_XOR bit_and_expr
	;

bit_and_expr:
	equality_expr
	| bit_and_expr BIT_AND equality_expr
	;

equality_expr:
	relational_expr
	| equality_expr EQ relational_expr
	| equality_expr NEQ relational_expr
	;

relational_expr:
	shift_expr
	| relational_expr LE shift_expr
	| relational_expr GE shift_expr
	| relational_expr LEQ shift_expr
	| relational_expr GEQ shift_expr
	;

shift_expr:
	add_expr
	| shift_expr BIT_LSHIFT add_expr
	| shift_expr BIT_RSHIFT add_expr
	;

add_expr:
	mul_expr
	| add_expr ADD mul_expr
	| add_expr SUB mul_expr
	;

mul_expr:
	unary_expr
	| mul_expr MUL unary_expr
	| mul_expr DIV unary_expr
	| mul_expr MOD unary_expr
	;

unary_expr:
	postfix_expr
	| unary_op unary_expr
	| MEASURE LPAREN postfix_expr RPAREN
	| PHASE LPAREN postfix_expr RPAREN
	;

unary_op:
	BIT_AND
	| MUL
	| ADD
	| SUB
	| NOT_OP
	| NOT
	;

postfix_expr:
	primary_expr
	| postfix_expr LBRACKET logical_or_expr RBRACKET
	| postfix_expr LPAREN argument_expr_list RPAREN
	| postfix_expr LPAREN RPAREN
	;

primary_expr:
	ID
	| consts
	| LPAREN logical_or_expr RPAREN
	;

argument_expr_list:
	logical_or_expr
	| argument_expr_list COMMA logical_or_expr
	;

consts:
    BCONST
    | ICONST
    | UCONST
	;

%%

int yyerror(const char *msg) {
    fprintf(stderr, "Parsing failed in line %d: %s\n", yylineno, msg);
    success = 0;
    return 0;
}

int main(int argc, char **argv) {
//#ifdef YYDEBUG
//    yydebug = 1;
//#endif

    // determine and open input file
    if (argc > 1 && strncmp(argv[1], "--dump", 7) != 0) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
    }

    init_hash_table();

    // parsing
    yyparse();

    // close input file
    if (argc > 1) {
        fclose(yyin);
    }

    // symbol table dump
    if ((argc == 2 && strncmp(argv[1], "--dump", 7) == 0) || (argc == 3 && strncmp(argv[2], "--dump", 7) == 0)) {
        yyout = fopen("symtab_dump.out", "w");
        if (!yyout) {
            fprintf(stderr, "Could not open symtab_dump.out\n");
            return 1;
        }
        symtab_dump(yyout);
        fclose(yyout);
    }

    // set return value
    if (success == 1) {
	return 0;
    } else {
        return 2;	    
    }
}
