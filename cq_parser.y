%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "rules.h"
#include "symtab.h"

extern int yylex(void);
extern int yylineno;
extern bool no_hide;
int yyerror(const char *s);
extern FILE *yyin;
extern FILE *yyout;

char error_msg[256];

%}

/* Union to define yylval's types */
%union {
    char *name;
    value_t value;
    list_t *symtab_item;
    node_t *node;
    type_info_t type_info;
    array_values_t array_values;
    arithmetic_op_t arithmetic_op;
    increment_op_t increment_op;
    bitwise_op_t bitwise_op;
    shift_op_t shift_op;
    logical_op_t logical_op;
    relation_op_t relation_op;
    equality_op_t equality_op;
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
%token <value> INCR DECR
%token <value> INV
%token <value> AND OR XOR
%token <value> LSHIFT RSHIFT
%token <value> LAND LOR LXOR
%token <value> NOT
%token <value> GE GEQ LE LEQ
%token <value> EQ NEQ
%token <value> ASSIGN ASSIGN_ADD ASSIGN_AND ASSIGN_DIV ASSIGN_LSHIFT ASSIGN_MOD ASSIGN_MUL ASSIGN_OR ASSIGN_RSHIFT ASSIGN_SUB ASSIGN_XOR
%token <value> COLON COMMA LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON
%left LPAREN RPAREN LBRACKET RBRACKET
%left LSHIFT RSHIFT
%left ADD SUB
%left MUL DIV MOD
%left XOR
%left OR
%left AND
%left LXOR
%left LOR
%left LAND
%right INCR DECR
%right INV
%right NOT
%nonassoc "then"
%nonassoc ELSE

%type <symtab_item> declarator
%type <type_info> type_specifier
%type <node> variable_decl variable_def function_def const
%type <array_values> init init_elem_l
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
	    $$ = new_func_decl_node($3);
	    set_type_of_elem($3, QUANTUM_T, $2.type, true, $2.depth, $2.sizes);
	}
	| type_specifier declarator { incr_scope(); } function_head function_tail {
	    hide_scope();
	    $$ = new_func_decl_node($2);
	    set_type_of_elem($2, NONE_T, $1.type, true, $1.depth, $1.sizes);
	}
	| VOID declarator { incr_scope(); } function_head function_tail {
	    hide_scope();
	    $$ = new_func_decl_node($2);
	    set_type_of_elem($2, NONE_T, VOID_T, true, 0, NULL);
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
	QUANTUM type_specifier declarator {
	    set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
	}
	| type_specifier declarator {
        set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
    }
	;

variable_decl:
    QUANTUM type_specifier declarator SEMICOLON {
        $$ = new_decl_node($3);
        set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
    }
    | type_specifier declarator SEMICOLON {
        $$ = new_decl_node($2);
        set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
    }
    ;

variable_def:
	QUANTUM type_specifier declarator ASSIGN init SEMICOLON {
	    $$ = new_decl_node($3);
	    set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
	    if ($5.length > $3->length) {
	        if ($3->depth == 0) {
	            if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $3->name) > 0) {
	                yyerror(error_msg);
	            } else {
	                yyerror("Attempt to initialize scalar as array");
	            }
	        } else {
	            if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $5.length, $3->name, $3->length) > 0) {
	                yyerror(error_msg);
	            } else {
	                yyerror("Too many elements initialized for array");
	            }
	        }
	    }
	    set_values_of_elem($3, $5.values, $5.length);
	}
	| CONST type_specifier declarator ASSIGN init SEMICOLON {
        $$ = new_decl_node($3);
        set_type_of_elem($3, CONST_T, $2.type, false, $2.depth, $2.sizes);
        if ($5.length > $3->length) {
            if ($3->depth == 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            } else {
                if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $5.length, $3->name, $3->length) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Too many elements initialized for array");
                }
            }
        }
        set_values_of_elem($3, $5.values, $5.length);
    }
	| type_specifier declarator ASSIGN init SEMICOLON {
        $$ = new_decl_node($2);
        set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
        if ($4.length > $2->length) {
            if ($2->depth == 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $2->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            } else {
                if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $4.length, $2->name, $2->length) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Too many elements initialized for array");
                }
            }
        }
        set_values_of_elem($2, $4.values, $4.length);
    }
	;

declarator:
	ID {
	    $$ = insert($1, strlen($1), UNDEFINED_T, yylineno, true);
	}
	;

init:
    const {
        $$ = array_values_init(NULL, 0, 1);
        $$.values[0] = ((const_node_t *) $1)->value;
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
    }
    ;

init_elem_l:
    const {
        $$ = array_values_init(NULL, 0, 1);
        $$.values[0] = ((const_node_t *) $1)->value;
    }
    | init_elem_l COMMA const {
        $$ = array_values_init($1.values, $1.length, $1.length + 1);
        $$.values[$1.length] = ((const_node_t *) $3)->value;
        free($1.values);
    }
    ;

type_specifier:
	BOOL { $$ = type_info_init(BOOL_T, 0); }
	| INT { $$ = type_info_init(INT_T, 0); }
	| UNSIGNED { $$ = type_info_init(UNSIGNED_T, 0); }
	| type_specifier LBRACKET UCONST RBRACKET {
	    if ($1.depth == MAXARRAYDEPTH) {
	        if (snprintf(error_msg, sizeof (error_msg), "Exceeding maximal array length of %i", MAXARRAYDEPTH) > 0) {
	            yyerror(error_msg);
	        } else {
	            yyerror("Exceeding maximal array length");
	        }
	    }
	    $$ = $1;
	    $$.sizes[($$.depth)++] = $3.uval;
	}
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
	| logical_or_expr LOR logical_xor_expr
	;

logical_xor_expr:
	logical_and_expr
	| logical_xor_expr LXOR logical_and_expr
	;

logical_and_expr:
	bit_or_expr
	| logical_and_expr LAND bit_or_expr
	;

bit_or_expr:
	bit_xor_expr
	| bit_or_expr OR bit_xor_expr
	;

bit_xor_expr:
	bit_and_expr
	| bit_xor_expr XOR bit_and_expr
	;

bit_and_expr:
	equality_expr
	| bit_and_expr AND equality_expr
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
	| shift_expr LSHIFT add_expr
	| shift_expr RSHIFT add_expr
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
	INV
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
	| const
	| LPAREN logical_or_expr RPAREN
	;

argument_expr_l:
	logical_or_expr
	| argument_expr_l COMMA logical_or_expr
	;

const:
    BCONST { $$ = new_const_node(BOOL_T, $1); }
    | ICONST { $$ = new_const_node(INT_T, $1); }
    | UCONST { $$ = new_const_node(UNSIGNED_T, $1); }
	;

%%

int yyerror(const char *msg) {
    fprintf(stderr, "Parsing failed in line %d: %s\n", yylineno, msg);
    exit(1);
}

int main(int argc, char **argv) {
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

    return 0;
}
