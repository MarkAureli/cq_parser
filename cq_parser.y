%{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "pars_utils.h"
#include "rules.h"
#include "symbol_table.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
extern FILE *yyout;

int yyerror(const char *message);
static node_t *root;
static char error_msg[ERROR_MSG_LENGTH];
static unsigned stmt_list_counter;
static stmt_list_t stmt_list_array[MAX_NUM_OF_STMT_LISTS];
static unsigned type_info_counter;
static type_info_t type_info_array[MAX_NUM_OF_TYPE_INFOS];
static func_info_t func_info;
static init_info_t init_info;
static unsigned access_info_counter;
static access_info_t access_info_array[MAX_NUM_OF_ARRAY_INFOS];
static unsigned arg_list_counter;
static arg_list_t arg_list_array[MAX_NUM_OF_ARG_LISTS];
static unsigned else_if_list_counter;
static else_if_list_t else_if_list_array[MAX_NUM_OF_ELSE_IF_LISTS];
static unsigned case_list_counter;
static case_list_t case_list_array[MAX_NUM_OF_CASE_LISTS];

%}

/* Union to define yylval's types */
%union {
    char *name;
    value_t value;
    node_t *node;
    stmt_list_t *stmt_list;
    type_info_t *type_info;
    entry_t *entry;
    func_info_t *func_info;
    init_info_t *init_info;
    access_info_t *access_info;
    arg_list_t *arg_list;
    else_if_list_t *else_if_list;
    case_list_t *case_list;
    logical_op_t logical_op;
    integer_op_t integer_op;
    comparison_op_t comparison_op;
    equality_op_t equality_op;
    assign_op_t assign_op;
}

%token <name> ID
%token <value> BCONST
%token <value> ICONST
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
%token <value> LAND LOR LXOR
%token <value> NOT
%token <value> GE GEQ LE LEQ
%token <value> EQ NEQ
%token <value> ASSIGN ASSIGN_ADD ASSIGN_AND ASSIGN_DIV ASSIGN_MOD ASSIGN_MUL ASSIGN_OR ASSIGN_SUB ASSIGN_XOR
%token <value> COLON COMMA LBRACE LBRACKET LPAREN RBRACE RBRACKET RPAREN SEMICOLON
%left LPAREN RPAREN LBRACKET RBRACKET
%left OR
%left XOR
%left AND
%left ADD SUB
%left MUL DIV MOD
%left LOR
%left LXOR
%left LAND
%right INCR DECR
%right INV
%right NOT

%type <node> program func_tail sub_program res_sub_program
%type <node> decl stmt decl_stmt res_stmt
%type <node> variable_decl variable_def func_def const primary_expr postfix_expr unary_expr mul_expr add_expr
%type <node> logical_or_expr logical_xor_expr logical_and_expr comparison_expr equality_expr or_expr xor_expr and_expr
%type <node> array_access_expr func_call assign_expr
%type <node> assign_stmt phase_stmt measure_stmt func_call_stmt
%type <node> if_stmt switch_stmt do_stmt while_stmt for_stmt for_first
%type <node> break_stmt continue_stmt return_stmt
%type <node> optional_else case_stmt
%type <stmt_list> decl_l stmt_l res_stmt_l
%type <type_info> type_specifier par
%type <entry> declarator
%type <func_info> par_l func_head
%type <init_info> init init_elem_l
%type <access_info> array_access
%type <arg_list> argument_expr_l
%type <else_if_list> else_if
%type <case_list> case_stmt_l

%define parse.error verbose
%start program


%%

program:
	decl_l {
	    $$ = new_stmt_list_node($1->is_quantizable, $1->is_unitary, $1->stmt_nodes, $1->num_of_stmts, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    root = $$;
        --stmt_list_counter;
	}
	;

decl_l:
    decl {
        $$ = stmt_list_array + stmt_list_counter;
        if (!setup_stmt_list($$, $1, error_msg)) {
            yyerror(error_msg);
        }

        ++stmt_list_counter;
    }
	| decl_l decl {
	    $$ = $1;
	    if (!append_to_stmt_list($$, $2, error_msg)) {
	        yyerror(error_msg);
	    }
	}
	;

decl:
    variable_decl {
        $$ = $1;
    }
	| variable_def {
	    $$ = $1;
	}
	| func_def {
	    $$ = $1;
	}
	;

func_def:
	QUANTUM type_specifier declarator {
	        incr_scope();
	    } func_head func_tail {
	    hide_scope();
	    if (!set_type_info($3, QUANTUM_T, $2->type, $2->sizes, $2->depth, error_msg)) {
	        yyerror(error_msg);
	    }

	    if (!set_func_info($3, $5->is_unitary && is_unitary($6), false, $5->pars_type_info, $5->num_of_pars,
	        error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = new_func_def_node($3, $6, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	| type_specifier declarator {
	        incr_scope();
	    } func_head func_tail {
	    hide_scope();
	    if (!set_type_info($2, NONE_T, $1->type, $1->sizes, $1->depth, error_msg)) {
	        yyerror(error_msg);
	    }

	    if (!set_func_info($2, false, $4->is_quantizable && is_quantizable($5), $4->pars_type_info, $4->num_of_pars,
	        error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = new_func_def_node($2, $5, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| VOID declarator {
	        incr_scope();
	    } func_head func_tail {
	    hide_scope();
	    if (!set_type_info($2, NONE_T, VOID_T, NULL, 0, error_msg)) {
	        yyerror(error_msg);
	    }

	    if (!set_func_info($2, $4->is_unitary && is_unitary($5), $4->is_quantizable && is_quantizable($5),
	                  $4->pars_type_info, $4->num_of_pars, error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = new_func_def_node($2, $5, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

func_head:
    LPAREN par_l RPAREN {
        $$ = $2;
    }
    | LPAREN RPAREN {
        $$ = &func_info;
        if (!setup_empty_func_info($$, error_msg)) {
            yyerror(error_msg);
        }
    }
    ;

par_l:
	par {
	    $$ = &func_info;
        if (!setup_func_info($$, *$1, error_msg)) {
            yyerror(error_msg);
        }

        --type_info_counter;
	}
	| par_l COMMA par {
	    $$ = $1;
	    if (!append_to_func_info($$, *$3, error_msg)) {
	        yyerror(error_msg);
	    }

	    --type_info_counter;
	}
	;

par:
	QUANTUM type_specifier declarator {
	    if (!set_type_info($3, QUANTUM_T, $2->type, $2->sizes, $2->depth, error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = $2;
	    $$->qualifier = QUANTUM_T;
	}
	| type_specifier declarator {
        if (!set_type_info($2, NONE_T, $1->type, $1->sizes, $1->depth, error_msg)) {
            yyerror(error_msg);
        }

	    $$ = $1;
	    $$->qualifier = NONE_T;
    }
	;

func_tail:
    LBRACE sub_program RBRACE {
        $$ = $2;
    }
    ;

variable_decl:
    QUANTUM type_specifier declarator SEMICOLON {
        if (!set_type_info($3, QUANTUM_T, $2->type, $2->sizes, $2->depth, error_msg)) {
            yyerror(error_msg);
        }

        $$ = new_var_decl_node($3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        --type_info_counter;
    }
    | type_specifier declarator SEMICOLON {
        if (!set_type_info($2, NONE_T, $1->type, $1->sizes, $1->depth, error_msg)) {
            yyerror(error_msg);
        }

        $$ = new_var_decl_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        --type_info_counter;
    }
    ;

variable_def:
    QUANTUM type_specifier declarator ASSIGN init SEMICOLON {
	    if (!set_type_info($3, QUANTUM_T, $2->type, $2->sizes, $2->depth, error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = new_var_def_node($3, $5->is_init_list, $5->node, $5->qualified_types, $5->values, $5->length, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --type_info_counter;
	}
	| CONST type_specifier declarator ASSIGN init SEMICOLON {
	    if (!set_type_info($3, CONST_T, $2->type, $2->sizes, $2->depth, error_msg)) {
	        yyerror(error_msg);
	    }

	    $$ = new_var_def_node($3, $5->is_init_list, $5->node, $5->qualified_types, $5->values, $5->length, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        --type_info_counter;
    }
	| type_specifier declarator ASSIGN init SEMICOLON {
	    printf("Here1\n");
	    if (!set_type_info($2, NONE_T, $1->type, $1->sizes, $1->depth, error_msg)) {
	        yyerror(error_msg);
	    }
        printf("Here2\n");
        $$ = new_var_def_node($2, $4->is_init_list, $4->node, $4->qualified_types, $4->values, $4->length, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        --type_info_counter;
    }
	;

declarator:
	ID {
	    $$ = insert($1, strlen($1), yylineno, true, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	;

init:
    logical_or_expr {
        $$ = &init_info;
        if (!setup_init_info($$, false, $1, error_msg)) {
            yyerror(error_msg);
        }
    }
    | LBRACKET ID RBRACKET {
        entry_t *entry = insert($2, strlen($2), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }
        node_t *func_sp_node = new_func_sp_node(entry, error_msg);
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }

        $$ = &init_info;
        if (!setup_init_info($$, false, func_sp_node, error_msg)) {
            yyerror(error_msg);
        }
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
    }
    ;

init_elem_l:
    logical_or_expr {
        $$ = &init_info;
        if (!setup_init_info($$, true, $1, error_msg)) {
            yyerror(error_msg);
        }
    }
    | LBRACKET ID RBRACKET {
        entry_t *entry = insert($2, strlen($2), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }
        node_t *func_sp_node = new_func_sp_node(entry, error_msg);
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }

        $$ = &init_info;
        if (!setup_init_info($$, true, func_sp_node, error_msg)) {
            yyerror(error_msg);
        }
    }
    | init_elem_l COMMA LBRACKET ID RBRACKET {
        entry_t *entry = insert($4, strlen($4), yylineno, false, error_msg);
        node_t *func_sp_node = new_func_sp_node(entry, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }
        $$ = $1;
        if (!append_to_init_info($$, func_sp_node, error_msg)) {
            yyerror(error_msg);
        }
    }
    | init_elem_l COMMA logical_or_expr {
        $$ = $1;
        if (!append_to_init_info($$, $3, error_msg)) {
            yyerror(error_msg);
        }
    }
    ;

type_specifier:
	BOOL {
	    $$ = type_info_array + type_info_counter;
	    if (!setup_type_info($$, BOOL_T, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++type_info_counter;
	}
	| INT {
	    $$ = type_info_array + type_info_counter;
	    if (!setup_type_info($$, INT_T, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++type_info_counter;
	}
	| UNSIGNED {
	    $$ = type_info_array + type_info_counter;
	    if (!setup_type_info($$, UNSIGNED_T, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++type_info_counter;
	}
	| type_specifier LBRACKET or_expr RBRACKET {
	    $$ = $1;
	    if (!append_to_type_info($$, $3, error_msg)) {
	        yyerror(error_msg);
	    }
	}
	;

sub_program:
    stmt_l {
	    $$ = new_stmt_list_node($1->is_quantizable, $1->is_unitary, $1->stmt_nodes, $1->num_of_stmts, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --stmt_list_counter;
    }
    ;

stmt_l:
	stmt {
	    $$ = stmt_list_array + stmt_list_counter;
	    if (!setup_stmt_list($$, $1, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++stmt_list_counter;
	}
	| stmt_l stmt {
	    $$ = $1;
	    if (!append_to_stmt_list($$, $2, error_msg)) {
	        yyerror(error_msg);
	    }
	}
	;

stmt:
    decl_stmt {
        $$ = $1;
    }
	| res_stmt {
	    $$ = $1;
	}
	;

decl_stmt:
    variable_decl {
        $$ = $1;
    }
    | variable_def {
        $$ = $1;
    }
    ;

res_sub_program:
    res_stmt_l {
	    $$ = new_stmt_list_node($1->is_quantizable, $1->is_unitary, $1->stmt_nodes, $1->num_of_stmts, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --stmt_list_counter;
    }
    ;

res_stmt_l:
	res_stmt {
	    $$ = stmt_list_array + stmt_list_counter;
	    if (!setup_stmt_list($$, $1, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++stmt_list_counter;
	}
	| res_stmt_l res_stmt {
	    $$ = $1;
	    if (!append_to_stmt_list($$, $2, error_msg)) {
	        yyerror(error_msg);
	    }
	}
	;

res_stmt:
	assign_stmt {
	    $$ = $1;
	}
	| phase_stmt {
	    $$ = $1;
	}
	| measure_stmt {
	    $$ = $1;
	}
	| func_call_stmt {
	    $$ = $1;
	}
	| if_stmt {
	    $$ = $1;
	}
	| switch_stmt {
	    $$ = $1;
	}
	| do_stmt {
	    $$ = $1;
	}
	| while_stmt {
	    $$ = $1;
	}
	| for_stmt {
	    $$ = $1;
	}
	| break_stmt {
	    $$ = $1;
	}
	| continue_stmt {
	    $$ = $1;
	}
	| return_stmt {
	    $$ = $1;
	}
	;

assign_stmt:
	assign_expr SEMICOLON {
	    $$ = $1;
	}
	;

phase_stmt:
    PHASE LPAREN array_access_expr RPAREN ASSIGN_ADD logical_or_expr SEMICOLON {
	    $$ = new_phase_node($3, true, $6, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    | PHASE LPAREN array_access_expr RPAREN ASSIGN_SUB logical_or_expr SEMICOLON {
	    $$ = new_phase_node($3, false, $6, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

measure_stmt:
    MEASURE LPAREN logical_or_expr RPAREN SEMICOLON {
        $$ = new_measure_node($3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

func_call_stmt:
    INV func_call SEMICOLON {
        $$ = $2;
        func_call_node_t *func_call_node_view = (func_call_node_t *) $$;
        if (!is_unitary($2)) {
            snprintf(error_msg, sizeof (error_msg), "Trying to invert non-unitary function %s",
                     func_call_node_view->entry->name);
            yyerror(error_msg);
        } else if (!func_call_node_view->sp && func_call_node_view->entry->type != VOID_T) {
            snprintf(error_msg, sizeof (error_msg), "Trying to invert function %s with non-void return",
                     func_call_node_view->entry->name);
            yyerror(error_msg);
        }
        func_call_node_view->inverse = true;
    }
    | func_call SEMICOLON {
        $$ = $1;
    }
    ;

if_stmt:
	IF LPAREN logical_or_expr RPAREN LBRACE res_sub_program RBRACE optional_else {
	    $$ = new_if_node($3, $6, NULL, 0, $8, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	| IF LPAREN logical_or_expr RPAREN LBRACE res_sub_program RBRACE else_if optional_else {
	    $$ = new_if_node($3, $6, $8->else_if_nodes, $8->num_of_else_ifs, $9, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --else_if_list_counter;
	}
	;

else_if:
    ELSE IF LPAREN logical_or_expr RPAREN LBRACE res_sub_program RBRACE {
        node_t *else_if_node = new_else_if_node($4, $7, error_msg);
        if (else_if_node == NULL) {
            yyerror(error_msg);
        }

        $$ = else_if_list_array + else_if_list_counter;
        if (!setup_else_if_list($$, else_if_node, error_msg)) {
            yyerror(error_msg);
        }

        ++else_if_list_counter;
    }
    | else_if ELSE IF LPAREN logical_or_expr RPAREN LBRACE res_sub_program RBRACE {
        node_t *else_if_node = new_else_if_node($5, $8, error_msg);
        if (else_if_node == NULL) {
            yyerror(error_msg);
        }

        $$ = $1;
        if (!append_to_else_if_list($$, else_if_node, error_msg)) {
            yyerror(error_msg);
        }
    }
    ;

optional_else:
    ELSE LBRACE res_sub_program RBRACE {
        $$ = $3;
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

switch_stmt:
	SWITCH LPAREN logical_or_expr RPAREN LBRACE case_stmt_l RBRACE {
	    $$ = new_switch_node($3, $6->case_nodes, $6->num_of_cases, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --case_list_counter;
	}
	;

case_stmt_l:
    case_stmt {
        $$ = case_list_array + case_list_counter;
        if (!setup_case_list($$, $1, error_msg)) {
            yyerror(error_msg);
        }

        ++case_list_counter;
    }
    | case_stmt_l case_stmt {
        $$ = $1;
        if (!append_to_case_list($$, $2, error_msg)) {
            yyerror(error_msg);
        }
    }
    ;

case_stmt:
	CASE const COLON res_sub_program {
	    $$ = new_case_node($2, $4, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	| DEFAULT COLON res_sub_program {
	    $$ = new_case_node(NULL, $3, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	;

do_stmt:
	DO {
	    incr_scope();
	    incr_nested_loop_counter();
	} LBRACE sub_program RBRACE {
	    decr_nested_loop_counter();
	    hide_scope();
	} WHILE LPAREN logical_or_expr RPAREN SEMICOLON {
	    $$ = new_do_node($4, $9, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
    ;

while_stmt:
    WHILE LPAREN logical_or_expr RPAREN {
        incr_scope();
        incr_nested_loop_counter();
    } LBRACE sub_program RBRACE {
        decr_nested_loop_counter();
        hide_scope();
        $$ = new_while_node($3, $7, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

for_stmt:
    FOR {
        incr_scope();
        incr_nested_loop_counter();
    } LPAREN for_first logical_or_expr SEMICOLON assign_expr RPAREN LBRACE sub_program RBRACE {
        $$ = new_for_node($4, $5, $7, $10, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        decr_nested_loop_counter();
        hide_scope();
    }
    ;

for_first:
    variable_def {
        $$ = $1;
    }
    | assign_stmt {
        $$ = $1;
    }
    ;

break_stmt:
    BREAK SEMICOLON {
        $$ = new_break_node(error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

continue_stmt:
    CONTINUE SEMICOLON {
        $$ = new_continue_node(error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

return_stmt:
    RETURN SEMICOLON {
        $$ = new_return_node(NULL, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    | RETURN logical_or_expr SEMICOLON {
        $$ = new_return_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

assign_expr:
	array_access_expr ASSIGN logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_OR logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_OR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_XOR logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_XOR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_AND logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_AND_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_ADD logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_ADD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_SUB logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_SUB_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_MUL logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_MUL_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_DIV logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_DIV_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_MOD logical_or_expr {
	    $$ = new_assign_node($1, ASSIGN_MOD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

logical_or_expr:
	logical_xor_expr {
	    $$ = $1;
	}
	| logical_or_expr LOR logical_xor_expr {
	    $$ = new_logical_op_node($1, LOR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

logical_xor_expr:
	logical_and_expr {
	    $$ = $1;
	}
	| logical_xor_expr LXOR logical_and_expr {
	    $$ = new_logical_op_node($1, LXOR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

logical_and_expr:
	comparison_expr {
	    $$ = $1;
	}
	| logical_and_expr LAND comparison_expr {
	    $$ = new_logical_op_node($1, LAND_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

comparison_expr:
	equality_expr {
	    $$ = $1;
	}
	| comparison_expr GE equality_expr {
	    $$ = new_comparison_op_node($1, GE_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr GEQ equality_expr {
	    $$ = new_comparison_op_node($1, GEQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LE equality_expr {
	    $$ = new_comparison_op_node($1, LE_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LEQ equality_expr {
	    $$ = new_comparison_op_node($1, LEQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

equality_expr:
	or_expr {
	    $$ = $1;
	}
	| equality_expr EQ or_expr {
	    $$ = new_equality_op_node($1, EQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| equality_expr NEQ or_expr {
	    $$ = new_equality_op_node($1, NEQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

or_expr:
    xor_expr {
        $$ = $1;
    }
	| or_expr OR xor_expr {
        $$ = new_integer_op_node($1, OR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

xor_expr:
	and_expr {
	    $$ = $1;
	}
	| xor_expr XOR and_expr {
        $$ = new_integer_op_node($1, XOR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

and_expr:
	add_expr {
        $$ = $1;
    }
	| and_expr AND add_expr {
        $$ = new_integer_op_node($1, AND_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

add_expr:
	mul_expr {
	    $$ = $1;
	}
	| add_expr ADD mul_expr {
        $$ = new_integer_op_node($1, ADD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| add_expr SUB mul_expr {
        $$ = new_integer_op_node($1, SUB_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

mul_expr:
	unary_expr {
	    $$ = $1;
	}
	| mul_expr MUL unary_expr {
        $$ = new_integer_op_node($1, MUL_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr DIV unary_expr {
        $$ = new_integer_op_node($1, DIV_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr MOD unary_expr {
        $$ = new_integer_op_node($1, MOD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	;

unary_expr:
	postfix_expr {
	    $$ = $1;
	}
	| INV unary_expr {
        $$ = new_invert_op_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| NOT unary_expr {
	    $$ = new_not_op_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| MEASURE LPAREN unary_expr RPAREN {
	    $$ = new_measure_node($3, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	;

postfix_expr:
    primary_expr {
        $$ = $1;
    }
    | func_call {
        $$ = $1;
    }
    | array_access_expr {
        $$ = $1;
    }
    ;

array_access_expr:
	array_access {
        $$ = new_reference_node($1->entry, $1->index_is_const, $1->indices, $1->index_depth, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        --access_info_counter;
	}
	;

array_access:
    ID {
        entry_t *entry = insert($1, strlen($1), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }

        $$ = access_info_array + access_info_counter;
        if (!setup_access_info($$, entry, error_msg)) {
            yyerror(error_msg);
        }

        ++access_info_counter;
    }
    | array_access LBRACKET or_expr RBRACKET {
        $$ = $1;
        if (!append_to_access_info($$, $3, error_msg)) {
            yyerror(error_msg);
        }
    }
    ;

primary_expr:
	const {
	    $$ = $1;
	}
	| LPAREN logical_or_expr RPAREN {
	    $$ = $2;
	}
	;

const:
    BCONST {
        $$ = new_const_node(BOOL_T, $1, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    | ICONST {
        $$ = new_const_node(INT_T, $1, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
	;

func_call:
	ID LPAREN argument_expr_l RPAREN {
        entry_t *entry = insert($1, strlen($1), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }

        $$ = new_func_call_node(false, entry, $3->args, $3->num_of_args, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        --arg_list_counter;
	}
	| ID LPAREN RPAREN {
        entry_t *entry = insert($1, strlen($1), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }

	    $$ = new_func_call_node(false, entry, NULL, 0, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| LBRACKET ID RBRACKET LPAREN argument_expr_l RPAREN {
        entry_t *entry = insert($2, strlen($2), yylineno, false, error_msg);
        if (entry == NULL) {
            yyerror(error_msg);
        }

	    $$ = new_func_call_node(true, entry, $5->args, $5->num_of_args, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }

	    --arg_list_counter;
	}
	;

argument_expr_l:
	logical_or_expr {
	    $$ = arg_list_array + arg_list_counter;
	    if (!setup_arg_list($$, $1, error_msg)) {
	        yyerror(error_msg);
	    }

	    ++arg_list_counter;
	}
	| argument_expr_l COMMA logical_or_expr {
	    $$ = $1;
	    if (!append_to_arg_list($$, $3, error_msg)) {
	        yyerror(error_msg);
	    }
	}
	;

%%

int yyerror(const char *message) {
    fprintf(stderr, "Parsing failed in line %d: %s\n", yylineno, message);
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

    bool dump = (argc == 2 && strncmp(argv[1], "--dump", 7) == 0) || (argc == 3 && strncmp(argv[2], "--dump", 7) == 0);
    init_symbol_table();
    root = NULL;
    stmt_list_counter = 0;
    type_info_counter = 0;
    arg_list_counter = 0;
    access_info_counter = 0;
    else_if_list_counter = 0;
    case_list_counter = 0;

    yyparse();


    if (argc > 1) {
        fclose(yyin);
    }

    if (dump) {
        char symbol_table_dump_file[] = "symbol_table_dump.out";
        yyout = fopen(symbol_table_dump_file, "w");
        if (!yyout) {
            fprintf(stderr, "%s\n", symbol_table_dump_file);
            return 1;
        }
        fprint_symbol_table(yyout);
        fclose(yyout);

        char tree_dump_file[] = "tree_dump.out";
        yyout = fopen(tree_dump_file, "w");
        if (!yyout) {
            fprintf(stderr, "%s\n", tree_dump_file);
            return 1;
        }
        fprint_tree(yyout, root);
        fclose(yyout);
    }

    free_tree(root);
    free_symbol_table();
    return 0;
}
