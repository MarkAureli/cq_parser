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
extern bool hide;
extern FILE *yyin;
extern FILE *yyout;

int yyerror(const char *s);
char error_msg[ERRORMSGLENGTH];

%}

/* Union to define yylval's types */
%union {
    char *name;
    value_t value;
    list_t *symtab_item;
    node_t *node;
    type_info_t type_info;
    func_info_t func_info;
    init_info_t *init_info;
    access_info_t access_info;
    assign_op_t assign_op;
    integer_op_t integer_op;
    logical_op_t logical_op;
    comparison_op_t comparison_op;
    equality_op_t equality_op;
    else_if_list_t else_if_list;
    arg_list_t arg_list;
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

%type <symtab_item> declarator
%type <type_info> type_specifier par
%type <func_info> par_l func_head
%type <node> program decl_l decl
%type <node> variable_decl variable_def func_def const primary_expr postfix_expr unary_expr mul_expr add_expr
%type <node> logical_or_expr logical_xor_expr logical_and_expr comparison_expr equality_expr or_expr xor_expr and_expr
%type <node> array_access_expr func_call assign_expr
%type <node> stmt_l stmt decl_stmt res_stmt_l res_stmt
%type <node> assign_stmt func_call_stmt if_stmt switch_stmt do_stmt while_stmt for_stmt for_first jump_stmt
%type <node> optional_else
%type <else_if_list> else_if
%type <init_info> init init_elem_l
%type <access_info> array_access
%type <arg_list> argument_expr_l
%define parse.error verbose
%start program


%%

program:
	decl_l {
	    $$ = $1;
	    tree_traversal($$);
	}
	;

decl_l:
    decl {
        $$ = new_stmt_list_node($1);
    }
	| decl_l decl {
	   $$ = $1;
	   append_to_stmt_list($$, $2);
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
	QUANTUM type_specifier declarator { incr_scope(); /* global info for return check and to avoid recursion */ } func_head func_tail {
	    hide_scope();
	    $$ = new_func_decl_node($3);
	    set_type_info_of_elem($3, QUANTUM_T, $2.type, $2.depth, $2.sizes, true);
	    set_func_info_of_elem($3, $5);
	}
	| type_specifier declarator { incr_scope(); /* global info for return check and to avoid recursion */ } func_head func_tail {
	    hide_scope();
	    $$ = new_func_decl_node($2);
	    set_type_info_of_elem($2, NONE_T, $1.type, $1.depth, $1.sizes, true);
	    set_func_info_of_elem($2, $4);
	}
	| VOID declarator { incr_scope(); /* global info for return check and to avoid recursion */ } func_head func_tail {
	    hide_scope();
	    $$ = new_func_decl_node($2);
	    set_type_info_of_elem($2, NONE_T, VOID_T, 0, NULL, true);
	    set_func_info_of_elem($2, $4);
	}
	;

func_head:
    LPAREN par_l RPAREN {
        $$ = $2;
    }
    | LPAREN RPAREN {
        $$ = create_empty_func_info();
    }
    ;

par_l:
	par {
	    $$ = create_func_info($1);
	}
	| par_l COMMA par {
	    $$ = $1;
	    append_to_func_info(&$$, $3);
	}
	;

par:
	QUANTUM type_specifier declarator {
	    set_type_info_of_elem($3, QUANTUM_T, $2.type, $2.depth, $2.sizes, false);
	    $$ = $3->type_info;
	}
	| type_specifier declarator {
        set_type_info_of_elem($2, NONE_T, $1.type, $1.depth, $1.sizes, false);
        $$ = $2->type_info;
    }
	;

func_tail:
    LBRACE stmt_l RBRACE
    ;

variable_decl:
    QUANTUM type_specifier declarator SEMICOLON {
        $$ = new_var_decl_node($3);
        set_type_info_of_elem($3, QUANTUM_T, $2.type, $2.depth, $2.sizes, false);
    }
    | type_specifier declarator SEMICOLON {
        $$ = new_var_decl_node($2);
        set_type_info_of_elem($2, NONE_T, $1.type, $1.depth, $1.sizes, false);
    }
    ;

variable_def:
    QUANTUM type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_info_of_elem($3, QUANTUM_T, $2.type, $2.depth, $2.sizes, false);
	    $$ = build_var_def_node($3, $5, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	| CONST type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_info_of_elem($3, CONST_T, $2.type, $2.depth, $2.sizes, false);
	    $$ = build_var_def_node($3, $5, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }

        if ($5->is_init_list) {
            for (unsigned i = 0; i < $5->init_list.length; ++i) {
                $3->values[i] = $5->init_list.values[i].const_value;
            }
        } else {
            type_info_t *type_info = get_type_info_of_node($5->node);
            memcpy($3->values, ((const_node_t *) $5->node)->values,
                   get_length_of_array(type_info->sizes, type_info->depth) * sizeof (value_t));
        }
    }
	| type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_info_of_elem($2, NONE_T, $1.type, $1.depth, $1.sizes, false);
        $$ = build_var_def_node($2, $4, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
	;

declarator:
	ID {
	    $$ = insert($1, strlen($1), yylineno, true);
	}
	;

init:
    logical_or_expr {
        $$ = new_init_info_from_node($1);
    }
    | LBRACKET ID RBRACKET {
        node_t *func_sp_node = build_func_sp_node(insert($2, strlen($2), yylineno, false), error_msg);
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }
        $$ = new_init_info_from_node(func_sp_node);
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
    }
    ;

init_elem_l:
    logical_or_expr {
        type_info_t *info = get_type_info_of_node($1);
        if (info->depth != 0) {
            snprintf(error_msg, sizeof (error_msg), "Element of array initializer list is itself a depth-%u array",
                     info->depth);
            yyerror(error_msg);
        }
        array_value_t value;
        if (info->qualifier == CONST_T) {
            value.const_value = ((const_node_t *) $1)->values[0];
        } else {
            value.node_value = $1;
        }
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        $$ = new_init_info_from_init_list(qualified_type, value);
    }
    | LBRACKET ID RBRACKET {
        node_t *func_sp_node = build_func_sp_node(insert($2, strlen($2), yylineno, false), error_msg);
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }
        array_value_t value;
        value.node_value = func_sp_node;
        type_info_t *info = get_type_info_of_node(func_sp_node);
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        $$ = new_init_info_from_init_list(qualified_type, value);
    }
    | init_elem_l COMMA LBRACKET ID RBRACKET {
        node_t *func_sp_node = build_func_sp_node(insert($4, strlen($4), yylineno, false), error_msg);
        if (func_sp_node == NULL) {
            yyerror(error_msg);
        }
        $$ = $1;
        array_value_t value;
        value.node_value = func_sp_node;
        type_info_t *info = get_type_info_of_node(func_sp_node);
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        append_to_init_info($$, qualified_type, value);
    }
    | init_elem_l COMMA logical_or_expr {
        type_info_t *info = get_type_info_of_node($3);
        if (info->depth != 0) {
            snprintf(error_msg, sizeof (error_msg), "Element of array initializer list is itself a depth-%u array",
                     info->depth);
            yyerror(error_msg);
        }
        $$ = $1;
        array_value_t value;
        if (info->qualifier == CONST_T) {
            value.const_value = ((const_node_t *) $3)->values[0];
        } else {
            value.node_value = $3;
        }
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        append_to_init_info($$, qualified_type, value);
    }
    ;

type_specifier:
	BOOL {
	    $$ = create_type_info(BOOL_T, 0);
	}
	| INT {
	    $$ = create_type_info(INT_T, 0);
	}
	| UNSIGNED {
	    $$ = create_type_info(UNSIGNED_T, 0);
	}
	| type_specifier LBRACKET or_expr RBRACKET {
	    if ($1.depth == MAXARRAYDEPTH) {
	        snprintf(error_msg, sizeof (error_msg), "Exceeding maximal array length of %i", MAXARRAYDEPTH);
	        yyerror(error_msg);
	    }
	    type_info_t *size_info = get_type_info_of_node($3);
	    if (size_info->depth != 0) {
            snprintf(error_msg, sizeof (error_msg),
                     "Size parameter at position %u of array initialization is itself an array", $$.depth);
            yyerror(error_msg);
	    }
	    if (size_info->type == BOOL_T) {
            snprintf(error_msg, sizeof (error_msg),
                     "Size parameter at position %u of array initialization is of type bool", $$.depth);
            yyerror(error_msg);
	    } else if (size_info->qualifier != CONST_T) {
            snprintf(error_msg, sizeof (error_msg),
                     "Size parameter at position %u of array initialization is not constant", $$.depth);
            yyerror(error_msg);
	    } else {
            $$ = $1;
            $$.sizes[($$.depth)++] = ((const_node_t *) $3)->values[0].uval;
	    }
	}
	;

stmt_l:
	stmt {
	    $$ = new_stmt_list_node($1);
	}
	| stmt_l stmt {
	    $$ = $1;
	    append_to_stmt_list($$, $2);
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

res_stmt_l:
	res_stmt {
	    $$ = new_stmt_list_node($1);
	}
	| res_stmt_l res_stmt {
	    $$ = $1;
	    append_to_stmt_list($$, $2);
	}
	;

res_stmt:
	assign_stmt {
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
	| jump_stmt {
	    $$ = $1;
	}
	;

assign_stmt:
	assign_expr SEMICOLON {
	    $$ = $1;
	}
	;

func_call_stmt:
    INV func_call SEMICOLON {
        $$ = $2;
        func_call_node_t *func_call_node_view = (func_call_node_t *) $$;
        if (func_call_node_view->type_info.type != VOID_T) {
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
	IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE optional_else {
	    $$ = build_if_node($3, $6, NULL, 0, $8, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	| IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE else_if optional_else {
	    $$ = build_if_node($3, $6, $8.else_if_nodes, $8.num_of_else_ifs, $9, error_msg);
	    if ($$ == NULL) {
	        yyerror(error_msg);
	    }
	}
	;

else_if:
    ELSE IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE {
        node_t *else_if_node = build_else_if_node($4, $7, error_msg);
        if (else_if_node == NULL) {
            yyerror(error_msg);
        }
        $$ = create_else_if_list(else_if_node);
    }
    | else_if ELSE IF LPAREN logical_or_expr RPAREN LBRACE res_stmt_l RBRACE {
        node_t *else_if_node = build_else_if_node($5, $8, error_msg);
        if (else_if_node == NULL) {
            yyerror(error_msg);
        }
        $$ = $1;
        append_to_else_if_list(&$$, else_if_node);
    }
    ;

optional_else:
    ELSE LBRACE res_stmt_l RBRACE {
        $$ = $3;
    }
    | /* empty */ {
        $$ = NULL;
    }
    ;

switch_stmt:
	SWITCH LPAREN logical_or_expr RPAREN LBRACE case_stmt_l RBRACE { /* dummy */ $$ = NULL; }
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
	DO { incr_scope(); } LBRACE stmt_l RBRACE { hide_scope(); } WHILE LPAREN logical_or_expr RPAREN SEMICOLON {
	    $$ = build_do_node($4, $9, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
    ;

while_stmt:
    WHILE LPAREN logical_or_expr RPAREN { incr_scope(); } LBRACE stmt_l RBRACE {
        hide_scope();
        $$ = build_while_node($3, $7, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
    }
    ;

for_stmt:
    FOR { incr_scope(); } LPAREN for_first logical_or_expr SEMICOLON assign_expr RPAREN LBRACE stmt_l RBRACE {
        hide_scope();
        $$ = build_for_node($4, $5, $7, $10, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
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

jump_stmt:
	CONTINUE SEMICOLON { /* dummy */ $$ = NULL; }
	| BREAK SEMICOLON { /* dummy */ $$ = NULL; }
	| RETURN logical_or_expr SEMICOLON { /* dummy */ $$ = NULL; }
	| RETURN SEMICOLON { /* dummy */ $$ = NULL; }
	;

assign_expr:
	array_access_expr ASSIGN logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_OR logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_OR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_XOR logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_XOR_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_AND logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_AND_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_ADD logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_ADD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_SUB logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_SUB_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_MUL logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_MUL_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_DIV logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_DIV_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| array_access_expr ASSIGN_MOD logical_or_expr {
	    $$ = build_assign_node($1, ASSIGN_MOD_OP, $3, error_msg);
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
	    $$ = build_logical_op_node($1, LOR_OP, $3, error_msg);
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
	    $$ = build_logical_op_node($1, LXOR_OP, $3, error_msg);
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
	    $$ = build_logical_op_node($1, LAND_OP, $3, error_msg);
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
	    $$ = build_comparison_op_node($1, GE_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr GEQ equality_expr {
	    $$ = build_comparison_op_node($1, GEQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LE equality_expr {
	    $$ = build_comparison_op_node($1, LE_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LEQ equality_expr {
	    $$ = build_comparison_op_node($1, LEQ_OP, $3, error_msg);
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
	    $$ = build_equality_op_node($1, EQ_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| equality_expr NEQ or_expr {
	    $$ = build_equality_op_node($1, NEQ_OP, $3, error_msg);
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
        $$ = build_integer_op_node($1, OR_OP, $3, error_msg);
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
        $$ = build_integer_op_node($1, XOR_OP, $3, error_msg);
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
        $$ = build_integer_op_node($1, AND_OP, $3, error_msg);
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
        $$ = build_integer_op_node($1, ADD_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| add_expr SUB mul_expr {
        $$ = build_integer_op_node($1, SUB_OP, $3, error_msg);
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
        $$ = build_integer_op_node($1, MUL_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr DIV unary_expr {
        $$ = build_integer_op_node($1, DIV_OP, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr MOD unary_expr {
        $$ = build_integer_op_node($1, MOD_OP, $3, error_msg);
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
        $$ = build_invert_op_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| NOT unary_expr {
	    $$ = build_not_op_node($2, error_msg);
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
	    list_t *entry = $1.entry;
        bool all_indices_const = true;
        unsigned new_depth = entry->type_info.depth - $1.depth;
        unsigned new_sizes[entry->type_info.depth];
        memcpy(new_sizes, entry->type_info.sizes, entry->type_info.depth * sizeof (unsigned));
        for (unsigned i = 0; i < $1.depth; ++i) {
            all_indices_const &= $1.index_is_const[i];
            if ($1.index_is_const[i] && $1.indices[i].const_index >= entry->type_info.sizes[i]) {
                snprintf(error_msg, sizeof (error_msg), "%u-th index (%u) of array %s%s out of bounds (%u)",
                         i, $1.indices[i].const_index, ($1.entry->is_function) ? "returned by " : "", entry->name,
                         entry->type_info.sizes[i]);
                yyerror(error_msg);
            }
            for (unsigned j = i; j < entry->type_info.depth; ++j) {
                new_sizes[j] = new_sizes[j + 1];
            }
        }
        if (entry->type_info.qualifier == CONST_T && all_indices_const) {
            unsigned const_indices[MAXARRAYDEPTH];
            for (unsigned i = 0; i < $1.depth; ++i) {
                const_indices[i] = $1.indices[i].const_index;
            }
            value_t *new_values = get_reduced_array(entry->values, entry->type_info.sizes, entry->type_info.depth,
                                                    const_indices, $1.depth);
            $$ = new_const_node(entry->type_info.type, new_sizes, new_depth, new_values);
        } else {
            $$ = new_reference_node(new_sizes, new_depth, $1.index_is_const, $1.indices, entry);
        }
	}
	;

array_access:
    ID {
        list_t *entry = insert($1, strlen($1), yylineno, false);
        if (entry->is_function) {
            snprintf(error_msg, sizeof (error_msg), "Function %s is not called", entry->name);
            yyerror(error_msg);
        }
        $$ = create_access_info(entry);
    }
    | array_access LBRACKET or_expr RBRACKET {
        $$ = $1;
        if ($$.entry->type_info.depth == 0) {
            snprintf(error_msg, sizeof (error_msg), "Array access of scalar %s", $$.entry->name);
            yyerror(error_msg);
        } else if ($$.depth == $$.entry->type_info.depth) {
            snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s",
                     $$.depth + 1, $$.entry->type_info.depth, $$.entry->name);
            yyerror(error_msg);
        }

        type_info_t *index_info = get_type_info_of_node($3);
        if (index_info->qualifier == QUANTUM_T) {
            snprintf(error_msg, sizeof (error_msg), "Index at position %u of access of depth-%u array %s is quantum",
                     $$.depth, $$.entry->type_info.depth, $$.entry->name);
            yyerror(error_msg);
        } else if (index_info->type == BOOL_T) {
            snprintf(error_msg, sizeof (error_msg),
                     "Index at position %u of access of depth-%u array %s is of type bool",
                     $$.depth, $$.entry->type_info.depth, $$.entry->name);
            yyerror(error_msg);
        } else if (index_info->depth != 0) {
            snprintf(error_msg, sizeof (error_msg),
                     "Index at position %u of access of depth-%u array %s is array-valued",
                     $$.depth, $$.entry->type_info.depth, $$.entry->name);
            yyerror(error_msg);
        }

        if (index_info->qualifier == CONST_T) {
            $$.index_is_const[$$.depth] = true;
            $$.indices[($$.depth)++].const_index = ((const_node_t *) $3)->values[0].uval;
        } else {
            $$.index_is_const[$$.depth] = false;
            $$.indices[($$.depth)++].node_index = $3;
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
        value_t *value = calloc(1, sizeof (value_t));
        value[0] = $1;
        $$ = new_const_node(BOOL_T, NULL, 0, value);
    }
    | ICONST {
        value_t *value = calloc(1, sizeof (value_t));
        value[0] = $1;
        $$ = new_const_node(INT_T, NULL, 0, value);
    }
	;

func_call:
	ID LPAREN argument_expr_l RPAREN {
        $$ = build_func_call_node(insert($1, strlen($1), yylineno, false), $3.args, $3.num_of_args, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| ID LPAREN RPAREN {
	    $$ = build_func_call_node(insert($1, strlen($1), yylineno, false), NULL, 0, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}

argument_expr_l:
	logical_or_expr {
	    $$ = create_arg_list($1);
	}
	| argument_expr_l COMMA logical_or_expr {
	    $$ = $1;
	    append_to_arg_list(&$$, $3);
	}
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

    bool dump = (argc == 2 && strncmp(argv[1], "--dump", 7) == 0) || (argc == 3 && strncmp(argv[2], "--dump", 7) == 0);

    if (dump) {
        hide = false;
    }

    // parsing
    yyparse();

    // close input file
    if (argc > 1) {
        fclose(yyin);
    }

    // symbol table dump
    if (dump) {
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
