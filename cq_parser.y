%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"

extern int yylex(void);
extern int yylineno;
extern bool no_hide;
int yyerror(const char *s);
int success = 1;
extern FILE *yyin;
extern FILE *yyout;

%}

/* Union to define yylval's types */
%union {
    char *name;
    value_t value;
    list_t *symtab_item;
    node_t *node;
    array_info_t type_info;
    type_t const_type;
}

%token <name> ID
%token <value> BCONST
%token <value> ICONST
%token <value> UCONST
%token <value> BOOL INT UNSIGNED VOID
%token <value> CONST QUANTUM
%token <value> BREAK CONTINUE DO FOR RETURN WHILE
%token <value> CASE DEFAULT ELSE IF SWITCH
%token <value> MEASURE
%token <value> PHASE
%token <value> ADD DIV MOD MUL SUB
%token <value> BIT_AND BIT_LSHIFT BIT_OR BIT_RSHIFT BIT_XOR
%token <value> AND EQ GE GEQ LE LEQ NEQ NOT NOT_OP OR XOR
%token <value> ASSIGN ASSIGN_ADD ASSIGN_AND ASSIGN_DIV ASSIGN_LSHIFT ASSIGN_MOD ASSIGN_MUL ASSIGN_OR ASSIGN_RSHIFT ASSIGN_SUB ASSIGN_XOR
%token <value> COLON COMMA LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON
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

%type <symtab_item> declarator
%type <type_info> type_specifier
%type <node> variable_decl
%define parse.error verbose
%start program


%%

program:
	decl_l
	;

decl_l:
	decl decl_l
	| decl
	;

decl:
    variable_decl
	| variable_def
	| function_def
	;

function_def:
	QUANTUM type_specifier declarator { incr_scope(); } function_head function_tail {
	    hide_scope();
	    set_type_of_elem($3, QUANTUM_T, $2.type, true, $2.depth);
	}
	| type_specifier declarator { incr_scope(); } function_head function_tail {
	    hide_scope();
	    set_type_of_elem($2, NONE_T, $1.type, true, $1.depth);
	}
	| VOID declarator { incr_scope(); } function_head function_tail {
	    hide_scope();
	    set_type_of_elem($2, NONE_T, VOID_T, true, 0);
	}
	;

function_head:
    LPAREN par_l RPAREN
    ;

function_tail:
    LBRACE stmt_l RBRACE
    ;

par_l:
	par_l COMMA par
	| par
	| /* empty */
	;

par:
	QUANTUM type_specifier declarator
	| type_specifier declarator
	;

variable_decl:
    QUANTUM type_specifier declarator SEMICOLON {
        $$ = new_decl_node($2.type, $3);
        set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth);
    }
    | type_specifier declarator SEMICOLON {
        $$ = new_decl_node($1.type, $2);
        set_type_of_elem($2, NONE_T, $1.type, false, $1.depth);
    }
    ;

variable_def:
	QUANTUM type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| QUANTUM type_specifier declarator ASSIGN LBRACE initializer_l RBRACE SEMICOLON
	| QUANTUM type_specifier declarator ASSIGN LBRACKET initializer_l RBRACKET SEMICOLON
	| CONST type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| CONST type_specifier declarator ASSIGN LBRACE initializer_l RBRACE SEMICOLON
	| type_specifier declarator ASSIGN logical_or_expr SEMICOLON
	| type_specifier declarator ASSIGN LBRACE initializer_l RBRACE SEMICOLON
	;

declarator:
	ID { $$ = insert($1, strlen($1), UNDEFINED_T, yylineno, true); }
	;

initializer_l:
	initializer_l COMMA logical_or_expr
	| logical_or_expr
	;

type_specifier:
	BOOL { $$ = array_info_init(BOOL_T, 0); }
	| INT { $$ = array_info_init(INT_T, 0); }
	| UNSIGNED { $$ = array_info_init(UNSIGNED_T, 0); }
	| type_specifier LBRACKET logical_or_expr RBRACKET { $$ = $1; ++($$.depth); }
	| type_specifier LBRACKET RBRACKET { $$ = $1; ++($$.depth); }
	;

stmt_l:
	stmt
	| stmt_l stmt
	;

stmt:
    decl_stmt
	| expr_stmt
	| if_stmt
	| switch_stmt
	| do_stmt
	| while_stmt
	| for_stmt
	| jump_stmt
	;

decl_stmt:
    variable_decl
    | variable_def
    ;

res_stmt_l:
	res_stmt
	| res_stmt_l res_stmt
	;

res_stmt:
	expr_stmt
	| if_stmt
	| switch_stmt
	| do_stmt
	| while_stmt
	| for_stmt
	| jump_stmt
	;

expr_stmt:
	expr SEMICOLON
	| SEMICOLON
	;

if_stmt:
	IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE	%prec "then"
	| IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE ELSE LBRACE stmt_l RBRACE

switch_stmt:
	SWITCH LPAREN logical_or_expr RPAREN LBRACE case_stmt_l RBRACE
	;

case_stmt_l:
    case_stmt
    | case_stmt_l case_stmt
    ;

case_stmt:
	CASE logical_or_expr COLON res_stmt_l
	| DEFAULT COLON res_stmt_l
	;

do_stmt:
	DO { incr_scope(); } LBRACE stmt_l RBRACE { hide_scope(); } WHILE LPAREN expr RPAREN SEMICOLON
    ;

while_stmt:
    WHILE LPAREN expr RPAREN { incr_scope(); } LBRACE stmt_l RBRACE { hide_scope(); }
    ;

for_stmt:
    FOR { incr_scope(); } LPAREN for_first_stmt expr_stmt expr RPAREN LBRACE stmt_l RBRACE { hide_scope(); }
    ;

for_first_stmt:
    type_specifier declarator ASSIGN logical_or_expr SEMICOLON
    | expr_stmt
    ;

jump_stmt:
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
	| postfix_expr LPAREN argument_expr_l RPAREN
	| postfix_expr LPAREN RPAREN
	;

primary_expr:
	ID { insert($1, strlen($1), UNDEFINED_T, yylineno, false); }
	| consts
	| LPAREN logical_or_expr RPAREN
	;

argument_expr_l:
	logical_or_expr
	| argument_expr_l COMMA logical_or_expr
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
        no_hide = true;
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
