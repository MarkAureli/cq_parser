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

char error_msg[ERRORMSGLENGTH];

%}

/* Union to define yylval's types */
%union {
    char *name;
    value_t value;
    list_t *symtab_item;
    node_t *node;
    type_info_t type_info;
    array_var_infos_t array_var_infos;
    array_access_info_t array_access_info;
    assign_op_t assign_op;
    integer_op_t integer_op;
    logical_op_t logical_op;
    comparison_op_t comparison_op;
    equality_op_t equality_op;
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
%nonassoc "then"
%nonassoc ELSE

%type <symtab_item> declarator
%type <type_info> type_specifier
%type <node> variable_decl variable_def function_def const primary_expr postfix_expr unary_expr mul_expr add_expr
%type <node> logical_or_expr logical_xor_expr logical_and_expr comparison_expr equality_expr or_expr xor_expr and_expr
%type <node> array_access_expr function_call
%type <array_var_infos> init init_elem_l
%type <array_access_info> array_access
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
        $$ = new_var_decl_node($3);
        set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
    }
    | type_specifier declarator SEMICOLON {
        $$ = new_var_decl_node($2);
        set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
    }
    ;

variable_def:
	QUANTUM type_specifier declarator ASSIGN init SEMICOLON {
	    $$ = new_var_decl_node($3);
	    set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
	    if ($3->depth == 0 && $5.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $3->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
	    if ($3->depth != 0 && !$5.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is an array, but not is initialized as such", $3->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
	    if ($5.length > $3->length) {
            if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $5.length, $3->name, $3->length) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too many elements initialized for array");
            }
	    }
	}
	| CONST type_specifier declarator ASSIGN init SEMICOLON {
        set_type_of_elem($3, CONST_T, $2.type, false, $2.depth, $2.sizes);
	    if ($3->depth == 0 && $5.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $3->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
	    if ($3->depth != 0 && !$5.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is an array, but not is initialized as such", $3->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
        if ($5.length > $3->length) {
            if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $5.length, $3->name, $3->length) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too many elements initialized for array");
            }
        }
        value_t values[$5.length];
        for (unsigned i = 0; i < $5.length; ++i) {
            if ($5.var_infos[i].qualifier != CONST_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of constant array %s is not constant", i, $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of constant array is not constant");
                }
            } else if ($2.type == BOOL_T && $5.var_infos[i].type != BOOL_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of bool-array %s is of type %s", i, $3->name, type_to_str($5.var_infos[i].type)) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of bool-array is not of type bool");
                }
            } else if ($2.type == INT_T && $5.var_infos[i].type != INT_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of int-array %s is of type %s", i, $3->name, type_to_str($5.var_infos[i].type)) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of int-array is not of type int");
                }
            } else if ($2.type == UNSIGNED_T && $5.var_infos[i].type == BOOL_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of unsigned-array %s is of type bool", i, $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of unsigned-boolean array is of type bool");
                }
            }
            values[i] = $5.var_infos[i].value;
        }
        set_values_of_elem($3, values, $5.length);
        $$ = new_var_decl_node($3);
    }
	| type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
	    if ($2->depth == 0 && $4.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such", $2->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
	    if ($2->depth != 0 && !$4.is_array_init) {
            if (snprintf(error_msg, sizeof (error_msg), "%s is an array, but not is initialized as such", $2->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Attempt to initialize scalar as array");
            }
	    }
        if ($4.length > $2->length) {
            if (snprintf(error_msg, sizeof (error_msg), "Too many (%u) elements initialized for array %s of total length %u", $4.length, $2->name, $2->length) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too many elements initialized for array");
            }
        }
        for (unsigned i = 0; i < $4.length; ++i) {
            if ($4.var_infos[i].qualifier == QUANTUM_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of classical array %s is quantum", i, $2->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of classical array is quantum");
                }
            } else if ($1.type == BOOL_T && $4.var_infos[i].type != BOOL_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of bool-array %s is of type %s", i, $2->name, type_to_str($4.var_infos[i].type)) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of bool-array is not of type bool");
                }
            } else if ($1.type == INT_T && $4.var_infos[i].type != INT_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of int-array %s is of type %s", i, $2->name, type_to_str($4.var_infos[i].type)) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of int-array is not of type int");
                }
            } else if ($1.type == UNSIGNED_T && $4.var_infos[i].type == BOOL_T) {
                if (snprintf(error_msg, sizeof (error_msg), "Element %u in initializtion of unsigned-array %s is of type bool", i, $2->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Element in initialization of unsigned-array is of type bool");
                }
            }
        }
        $$ = new_var_decl_node($2);
    }
	;

declarator:
	ID {
	    $$ = insert($1, strlen($1), yylineno, true);
	}
	;

init:
    logical_or_expr {
        $$ = array_var_infos_init(NULL, 0, 1);
        $$.var_infos[0] = ((const_node_t *) $1)->var_info;
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
        $$.is_array_init = true;
    }
    ;

init_elem_l:
    logical_or_expr {
        $$ = array_var_infos_init(NULL, 0, 1);
        type_info_t *info = get_type_info_of_node($1);
        if (info->depth != 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Element of array initializer list is itself a depth-%u array",
                         info->depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Element of array initializer list is itself an array");
            }
        }
        $$.var_infos[0] = ((const_node_t *) $1)->var_info;
    }
    | init_elem_l COMMA logical_or_expr {
        $$ = array_var_infos_init($1.var_infos, $1.length, $1.length + 1);
        $$.var_infos[$1.length] = ((const_node_t *) $3)->var_info;
        free($1.var_infos);
    }
    ;

type_specifier:
	BOOL {
	    $$ = type_info_init(BOOL_T, 0);
	}
	| INT {
	    $$ = type_info_init(INT_T, 0);
	}
	| UNSIGNED {
	    $$ = type_info_init(UNSIGNED_T, 0);
	}
	| type_specifier LBRACKET or_expr RBRACKET {
	    if ($1.depth == MAXARRAYDEPTH) {
	        if (snprintf(error_msg, sizeof (error_msg), "Exceeding maximal array length of %i", MAXARRAYDEPTH) > 0) {
	            yyerror(error_msg);
	        } else {
	            yyerror("Exceeding maximal array length");
	        }
	    }
	    var_info_t size_info = get_var_info_of_node($3);
	    if (size_info.type == BOOL_T) {
           if (snprintf(error_msg, sizeof (error_msg), "Size parameter at position %u of array initialization is boolean", $$.depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("One size parameter of array initialization is boolean");
            }
	    } else if (size_info.qualifier != CONST_T) {
           if (snprintf(error_msg, sizeof (error_msg), "Size parameter at position %u of array initialization is not constant", $$.depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("One size parameter of array initialization is not constant");
            }
	    } else {
            $$ = $1;
            $$.sizes[($$.depth)++] = size_info.value.uval;
	    }
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
	| postfix_expr assignment_operator logical_or_expr
	;

assignment_operator:
    ASSIGN
	| ASSIGN_OR
	| ASSIGN_XOR
    | ASSIGN_AND
	| ASSIGN_ADD
	| ASSIGN_SUB
	| ASSIGN_MUL
	| ASSIGN_DIV
	| ASSIGN_MOD
	;

logical_or_expr:
	logical_xor_expr {
	    $$ = $1;
	}
	| logical_or_expr LOR logical_xor_expr {
	    $$ = build_logical_op_node(LOR_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

logical_xor_expr:
	logical_and_expr {
	    $$ = $1;
	}
	| logical_xor_expr LXOR logical_and_expr {
	    $$ = build_logical_op_node(LXOR_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

logical_and_expr:
	comparison_expr {
	    $$ = $1;
	}
	| logical_and_expr LAND comparison_expr {
	    $$ = build_logical_op_node(LAND_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

comparison_expr:
	equality_expr {
	    $$ = $1;
	}
	| comparison_expr GE equality_expr {
	    $$ = build_comparison_op_node(GE_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	| comparison_expr GEQ equality_expr {
	    $$ = build_comparison_op_node(GEQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	| comparison_expr LE equality_expr {
	    $$ = build_comparison_op_node(LE_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	| comparison_expr LEQ equality_expr {
	    $$ = build_comparison_op_node(LEQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

equality_expr:
	or_expr {
	    $$ = $1;
	}
	| equality_expr EQ or_expr {
	    $$ = build_equality_op_node(EQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	| equality_expr NEQ or_expr {
	    $$ = build_equality_op_node(NEQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

or_expr:
    xor_expr {
        $$ = $1;
    }
	| or_expr OR xor_expr {
        $$ = build_integer_op_node(OR_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	;

xor_expr:
	and_expr {
	    $$ = $1;
	}
	| xor_expr XOR and_expr {
        $$ = build_integer_op_node(XOR_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	;

and_expr:
	add_expr {
        $$ = $1;
    }
	| and_expr AND add_expr {
        $$ = build_integer_op_node(AND_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	;

add_expr:
	mul_expr {
	    $$ = $1;
	}
	| add_expr ADD mul_expr {
        $$ = build_integer_op_node(ADD_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	| add_expr SUB mul_expr {
        $$ = build_integer_op_node(SUB_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	;

mul_expr:
	unary_expr {
	    $$ = $1;
	}
	| mul_expr MUL unary_expr {
        $$ = build_integer_op_node(MUL_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
	}
	| mul_expr DIV unary_expr {
        $$ = build_integer_op_node(DIV_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	| mul_expr MOD unary_expr {
        $$ = build_integer_op_node(MOD_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	    tree_traversal($$);
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
        tree_traversal($$);
	}
	| NOT unary_expr {
	    $$ = build_not_op_node($2, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
        tree_traversal($$);
	}
	;

postfix_expr:
    primary_expr {
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
        unsigned new_depth = 0;
        unsigned new_sizes[MAXARRAYLENGTH] = {0};
        for (unsigned i = 0; i < $1.depth; ++i) {
            all_indices_const &= $1.index_is_const[i];
            if ($1.is_indexed[i]) {
                if ($1.index_is_const[i] && $1.indices[i].const_index >= entry->sizes[i]) {
                    if (snprintf(error_msg, sizeof (error_msg), "%u-th index (%u) of array %s%s out of bounds (%u)", i, $1.indices[i].const_index, ($1.entry->is_function) ? "returned by " : "", entry->name, entry->sizes[i]) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Array index out of bounds");
                    }
                }
            } else {
                new_sizes[new_depth++] = entry->sizes[i];
            }
        }
        if (entry->qualifier == CONST_T && all_indices_const && $1.depth == entry->depth) {
            value_t *new_values = get_sliced_array(entry->values, entry->sizes, $1.is_indexed, $1.indices, $1.depth);
            $$ = new_const_node(entry->type, new_sizes, new_depth, new_values);
        } else {
            $$ = new_reference_node(new_sizes, new_depth, $1.is_indexed, $1.index_is_const, $1.indices, entry);
        }
	}
	;

array_access:
    ID {
        $$ = array_access_info_init(insert($1, strlen($1), yylineno, false));
    }
    | function_call {
        $$ = array_access_info_init(((func_call_node_t *) $1)->entry);
    }
    | array_access LBRACKET or_expr RBRACKET {
        $$ = $1;
        if ($$.entry->depth == 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Array access of scalar %s", $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Array access of scalar");
            }
        } else if ($$.depth == $$.entry->depth) {
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", $$.depth + 1, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too deep acess of array");
            }
        }
        type_info_t *index_info = get_type_info_of_node($3);
        if (index_info->qualifier == QUANTUM_T) {
           if (snprintf(error_msg, sizeof (error_msg), "Index at position %u of access of depth-%u array %s is quantum", $$.depth, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Index of array access is quantum");
            }
        }
        if (index_info->type == BOOL_T) {
           if (snprintf(error_msg, sizeof (error_msg), "Index at position %u of access of depth-%u array %s is of type bool", $$.depth, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Index of array access is of type bool");
            }
        }
        if (index_info->depth != 0) {
           if (snprintf(error_msg, sizeof (error_msg), "Index at position %u of access of depth-%u array %s is array-valued", $$.depth, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Index of array access is array-valued");
            }
        }
        $$.is_indexed[$$.depth] = true;
        if (index_info->qualifier == CONST_T) {
            $$.index_is_const[$$.depth] = true;
            $$.indices[($$.depth)++].const_index = ((const_node_t *) $3)->values[0].uval;
        } else {
            $$.index_is_const[$$.depth] = false;
            $$.indices[($$.depth)++].node_index = $3;
        }
    }
    | array_access LBRACKET COLON RBRACKET {
        $$ = $1;
        if ($$.entry->depth == 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Array access of scalar %s", $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Array access of scalar");
            }
        } else if ($$.depth == $$.entry->depth) {
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", $$.depth + 1, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too deep acess of array");
            }
        }
        $$.is_indexed[($$.depth)] = false;
        $$.index_is_const[($$.depth)++] = true;
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
        $$ = new_const_node(BOOL_T, $1);
    }
    | ICONST {
        $$ = new_const_node(INT_T, $1);
    }
	;

function_call:
	ID LPAREN argument_expr_l RPAREN {
	    $$ = new_func_call_node(insert($1, strlen($1), yylineno, false), NULL, 0); /* dummy implementation */
        if ($$ == NULL) {
            if (snprintf(error_msg, sizeof (error_msg), "Tried to call non-function %s", $1) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Tried to call non-function");
            }
        }
	}
	| ID LPAREN RPAREN {
	    $$ = new_func_call_node(insert($1, strlen($1), yylineno, false), NULL, 0);
	    if ($$ == NULL) {
	        if (snprintf(error_msg, sizeof (error_msg), "Tried to call non-function %s", $1) > 0) {
	            yyerror(error_msg);
	        } else {
	            yyerror("Tried to call non-function");
	        }
	    }
	}

argument_expr_l:
	logical_or_expr
	| argument_expr_l COMMA logical_or_expr
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
