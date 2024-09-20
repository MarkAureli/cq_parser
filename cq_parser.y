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
    array_init_info_t *array_init_info;
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
%type <array_init_info> init init_elem_l
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
	    set_type_of_elem($3, QUANTUM_T, $2.type, false, $2.depth, $2.sizes);
        if ($5->is_init_list) {
            if ($3->depth == 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($5->array_init_list.length > $3->length) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Too many (%u) elements initialized for array %s of total length %u",
                             $5->array_init_list.length, $3->name, $3->length) > 0) {
                yyerror(error_msg);
                } else {
                    yyerror("Too many elements initialized for array");
                }
            } else {
                bool *value_is_const = calloc($3->length, sizeof (bool));
                for (unsigned i = 0; i < $5->array_init_list.length; ++i) {
                    if ($2.type == BOOL_T && $5->array_init_list.qualified_types[i].type != BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of bool-array %s is of type %s", i, $3->name,
                                     type_to_str($5->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of bool-array is not of type bool");
                        }
                    } else if ($2.type == INT_T && $5->array_init_list.qualified_types[i].type != INT_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of int-array %s is of type %s", i, $3->name,
                                     type_to_str($5->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of int-array is not of type int");
                        }
                    } else if ($2.type == UNSIGNED_T && $5->array_init_list.qualified_types[i].type == BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of unsigned-array %s is of type bool", i,
                                     $3->name) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of unsigned-array is of type bool");
                        }
                    } else if ($5->array_init_list.qualified_types[i].qualifier == CONST_T) {
                        value_is_const[i] = true;
                    }
                }
                for (unsigned i = $5->array_init_list.length; i < $3->length; ++i) {
                    value_is_const[i] = true;
                }
                $$ = new_var_def_node_from_init_list($3, value_is_const, $5->array_init_list.values);
            }
        } else { /* no initializer list */
            type_info_t *init_info = get_type_info_of_node($5->node);
            printf("Depth of node %u\n", init_info->depth);
            if ($3->depth == 0 && init_info->depth != 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($3->depth != init_info->depth) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Unmatching depths in array initialization of %s (%u != %u)", $3->name, $3->depth,
                             init_info->depth) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Unmatching depths in array initialization");
                }
            }
            for (unsigned i = 0; i < $3->depth; ++i) {
                if ($3->sizes[i] != init_info->sizes[i]) {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Unmatching sizes at position %u in array initialization of %s (%u != %u)", i,
                                 $3->name, $3->sizes[i], init_info->sizes[i]) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Unmatching depths in array initialization");
                    }
                }
            }
            $$ = new_var_def_node_from_node($3, $5->node);
        }
        tree_traversal($$);
	}
	| CONST type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_of_elem($3, CONST_T, $2.type, false, $2.depth, $2.sizes);
        if ($5->is_init_list) {
            if ($3->depth == 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($5->array_init_list.length > $3->length) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Too many (%u) elements initialized for array %s of total length %u",
                             $5->array_init_list.length, $3->name, $3->length) > 0) {
                yyerror(error_msg);
                } else {
                    yyerror("Too many elements initialized for array");
                }
            } else {
                bool *value_is_const = calloc($3->length, sizeof (bool));
                for (unsigned i = 0; i < $5->array_init_list.length; ++i) {
                    if ($5->array_init_list.qualified_types[i].qualifier != CONST_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of constant array %s is not constant", i,
                                     $3->name) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of constant array is not constant");
                        }
                    } else if ($2.type == BOOL_T && $5->array_init_list.qualified_types[i].type != BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of bool-array %s is of type %s", i, $3->name,
                                     type_to_str($5->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of bool-array is not of type bool");
                        }
                    } else if ($2.type == INT_T && $5->array_init_list.qualified_types[i].type != INT_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of int-array %s is of type %s", i, $3->name,
                                     type_to_str($5->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of int-array is not of type int");
                        }
                    } else if ($2.type == UNSIGNED_T && $5->array_init_list.qualified_types[i].type == BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of unsigned-array %s is of type bool", i,
                                     $3->name) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of unsigned-array is of type bool");
                        }
                    }
                    value_is_const[i] = true;
                }
                for (unsigned i = 0; i < $5->array_init_list.length; ++i) {
                    $3->values[i] = $5->array_init_list.values[i].const_value;
                }
                for (unsigned i = $5->array_init_list.length; i < $3->length; ++i) {
                    value_is_const[i] = true;
                }
                $$ = new_var_def_node_from_init_list($3, value_is_const, $5->array_init_list.values);
            }
        } else { /* no initializer list */
            type_info_t *init_info = get_type_info_of_node($5->node);
            if ($3->depth == 0 && init_info->depth != 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $3->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($3->depth != init_info->depth) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Unmatching depths in array initialization of %s (%u != %u)", $3->name, $3->depth,
                             init_info->depth) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Unmatching depths in array initialization");
                }
            }
            for (unsigned i = 0; i < $3->depth; ++i) {
                if ($3->sizes[i] != init_info->sizes[i]) {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Unmatching sizes at position %u in array initialization of %s (%u != %u)", i,
                                 $3->name, $3->sizes[i], init_info->sizes[i]) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Unmatching depths in array initialization");
                    }
                }
            }
            if (init_info->qualifier != CONST_T) {
                if ($3->depth == 0) {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Initialization of constant scalar %s with non-constant value", $3->name) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Initialization of constant scalar with non-constant value");
                    }
                } else {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Initialization of constant array %s with non-constant array", $3->name) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Initialization of constant array with non-constant array");
                    }
                }
            }
            memcpy($3->values, ((const_node_t *) $5->node)->values,
                   get_length_of_array(init_info->sizes, init_info->depth) * sizeof (value_t));
            $$ = new_var_def_node_from_node($3, $5->node);
        }
        tree_traversal($$);
    }
	| type_specifier declarator ASSIGN init SEMICOLON {
	    set_type_of_elem($2, NONE_T, $1.type, false, $1.depth, $1.sizes);
        if ($4->is_init_list) {
            if ($2->depth == 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $2->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($4->array_init_list.length > $2->length) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Too many (%u) elements initialized for array %s of total length %u",
                             $4->array_init_list.length, $2->name, $2->length) > 0) {
                yyerror(error_msg);
                } else {
                    yyerror("Too many elements initialized for array");
                }
            } else {
                bool *value_is_const = calloc($2->length, sizeof (bool));
                for (unsigned i = 0; i < $4->array_init_list.length; ++i) {
                    if ($4->array_init_list.qualified_types[i].qualifier == QUANTUM_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of classical array %s is quantum", i, $2->name) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of classical array is quantum");
                        }
                    } else if ($1.type == BOOL_T && $4->array_init_list.qualified_types[i].type != BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of bool-array %s is of type %s", i, $2->name,
                                     type_to_str($4->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of bool-array is not of type bool");
                        }
                    } else if ($1.type == INT_T && $4->array_init_list.qualified_types[i].type != INT_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of int-array %s is of type %s", i, $2->name,
                                     type_to_str($4->array_init_list.qualified_types[i].type)) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of int-array is not of type int");
                        }
                    } else if ($1.type == UNSIGNED_T && $4->array_init_list.qualified_types[i].type == BOOL_T) {
                        free(value_is_const);
                        if (snprintf(error_msg, sizeof (error_msg),
                                     "Element %u in initialization of unsigned-array %s is of type bool", i,
                                     $2->name) > 0) {
                            yyerror(error_msg);
                        } else {
                            yyerror("Element in initialization of unsigned-array is of type bool");
                        }
                    } else if ($4->array_init_list.qualified_types[i].qualifier == CONST_T) {
                        value_is_const[i] = true;
                    }
                }
                for (unsigned i = $4->array_init_list.length; i < $2->length; ++i) {
                    value_is_const[i] = true;
                }
                $$ = new_var_def_node_from_init_list($2, value_is_const, $4->array_init_list.values);
            }
        } else { /* no initializer list */
            type_info_t *init_info = get_type_info_of_node($4->node);
            if ($2->depth == 0 && init_info->depth != 0) {
                if (snprintf(error_msg, sizeof (error_msg), "%s is not an array, but is initialized as such",
                             $2->name) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Attempt to initialize scalar as array");
                }
            }
            if ($2->depth != init_info->depth) {
                if (snprintf(error_msg, sizeof (error_msg),
                             "Unmatching depths in array initialization of %s (%u != %u)", $2->name, $2->depth,
                             init_info->depth) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Unmatching depths in array initialization");
                }
            }
            for (unsigned i = 0; i < $2->depth; ++i) {
                if ($2->sizes[i] != init_info->sizes[i]) {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Unmatching sizes at position %u in array initialization of %s (%u != %u)", i,
                                 $2->name, $2->sizes[i], init_info->sizes[i]) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Unmatching depths in array initialization");
                    }
                }
            }
            if (init_info->qualifier == QUANTUM_T) {
                if ($2->depth == 0) {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Initialization of classical scalar %s with quantum value", $2->name) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Initialization of classical scalar with quantum value");
                    }
                } else {
                    if (snprintf(error_msg, sizeof (error_msg),
                                 "Initialization of classical array %s with quantum array", $2->name) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Initialization of classical array with quantum array");
                    }
                }

            }
            $$ = new_var_def_node_from_node($2, $4->node);
        }
        tree_traversal($$);
    }
	;

declarator:
	ID {
	    $$ = insert($1, strlen($1), yylineno, true);
	}
	;

init:
    logical_or_expr {
        $$ = new_array_init_info_from_node($1);
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
    }
    ;

init_elem_l:
    logical_or_expr {
        type_info_t *info = get_type_info_of_node($1);
        if (info->depth != 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Element of array initializer list is itself a depth-%u array",
                         info->depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Element of array initializer list is itself an array");
            }
        }
        array_value_t value;
        if (info->qualifier == CONST_T) {
            value.const_value = ((const_node_t *) $1)->values[0];
        } else {
            value.node_value = $1;
        }
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        $$ = new_array_init_info_from_init_list(qualified_type, value);
        $$->is_init_list = true;
    }
    | init_elem_l COMMA logical_or_expr {
        type_info_t *info = get_type_info_of_node($3);
        if (info->depth != 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Element of array initializer list is itself a depth-%u array",
                         info->depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Element of array initializer list is itself an array");
            }
        }
        $$ = $1;
        array_value_t value;
        if (info->qualifier == CONST_T) {
            value.const_value = ((const_node_t *) $3)->values[0];
        } else {
            value.node_value = $3;
        }
        qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
        append_to_array_init_info($$, qualified_type, value);
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
	    type_info_t *size_info = get_type_info_of_node($3);
	    if (size_info->depth != 0) {
           if (snprintf(error_msg, sizeof (error_msg),
                        "Size parameter at position %u of array initialization is itself an array", $$.depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("One size parameter of array initialization is itself an array");
            }
	    }
	    if (size_info->type == BOOL_T) {
           if (snprintf(error_msg, sizeof (error_msg),
                        "Size parameter at position %u of array initialization is of type bool", $$.depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("One size parameter of array initialization is of type bool");
            }
	    } else if (size_info->qualifier != CONST_T) {
           if (snprintf(error_msg, sizeof (error_msg),
                        "Size parameter at position %u of array initialization is not constant", $$.depth) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("One size parameter of array initialization is not constant");
            }
	    } else {
            $$ = $1;
            $$.sizes[($$.depth)++] = ((const_node_t *) $3)->values[0].uval;
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
	}
	| comparison_expr GEQ equality_expr {
	    $$ = build_comparison_op_node(GEQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LE equality_expr {
	    $$ = build_comparison_op_node(LE_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| comparison_expr LEQ equality_expr {
	    $$ = build_comparison_op_node(LEQ_OP, $1, $3, error_msg);
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
	    $$ = build_equality_op_node(EQ_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| equality_expr NEQ or_expr {
	    $$ = build_equality_op_node(NEQ_OP, $1, $3, error_msg);
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
        $$ = build_integer_op_node(OR_OP, $1, $3, error_msg);
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
        $$ = build_integer_op_node(XOR_OP, $1, $3, error_msg);
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
        $$ = build_integer_op_node(AND_OP, $1, $3, error_msg);
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
        $$ = build_integer_op_node(ADD_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| add_expr SUB mul_expr {
        $$ = build_integer_op_node(SUB_OP, $1, $3, error_msg);
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
        $$ = build_integer_op_node(MUL_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr DIV unary_expr {
        $$ = build_integer_op_node(DIV_OP, $1, $3, error_msg);
        if ($$ == NULL) {
            yyerror(error_msg);
        }
	}
	| mul_expr MOD unary_expr {
        $$ = build_integer_op_node(MOD_OP, $1, $3, error_msg);
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
    | array_access_expr {
        $$ = $1;
    }
    ;

array_access_expr:
	array_access {
	    list_t *entry = $1.entry;
        bool all_indices_const = true;
        unsigned new_depth = entry->depth;
        unsigned new_sizes[entry->depth];
        memcpy(new_sizes, entry->sizes, entry->depth * sizeof (unsigned));
        for (unsigned i = 0; i < $1.depth; ++i) {
            all_indices_const &= $1.index_is_const[i];
            if ($1.is_indexed[i]) {
                if ($1.index_is_const[i] && $1.indices[i].const_index >= entry->sizes[i]) {
                    if (snprintf(error_msg, sizeof (error_msg), "%u-th index (%u) of array %s%s out of bounds (%u)", i,
                                 $1.indices[i].const_index, ($1.entry->is_function) ? "returned by " : "", entry->name,
                                 entry->sizes[i]) > 0) {
                        yyerror(error_msg);
                    } else {
                        yyerror("Array index out of bounds");
                    }
                }
                --new_depth;
                for (unsigned j = i; j < entry->depth; ++j) {
                    new_sizes[j] = new_sizes[j + 1];
                }
            }
        }
        if (entry->qualifier == CONST_T && all_indices_const && $1.depth == entry->depth) {
            unsigned const_indices[$1.depth];
            for (unsigned i = 0; i < $1.depth; ++i) {
                const_indices[i] = $1.indices[i].const_index;
            }
            value_t *new_values = get_sliced_array(entry->values, entry->sizes, $1.is_indexed, const_indices, $1.depth);
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
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", $$.depth + 1,
                         $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too deep access of array");
            }
        }
        type_info_t *index_info = get_type_info_of_node($3);
        if (index_info->qualifier == QUANTUM_T) {
           if (snprintf(error_msg, sizeof (error_msg), "Index at position %u of access of depth-%u array %s is quantum",
                        $$.depth, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Index of array access is quantum");
            }
        }
        if (index_info->type == BOOL_T) {
           if (snprintf(error_msg, sizeof (error_msg),
                        "Index at position %u of access of depth-%u array %s is of type bool", $$.depth,
                        $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Index of array access is of type bool");
            }
        }
        if (index_info->depth != 0) {
           if (snprintf(error_msg, sizeof (error_msg),
                        "Index at position %u of access of depth-%u array %s is array-valued", $$.depth,
                        $$.entry->depth, $$.entry->name) > 0) {
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
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", $$.depth + 1,
                         $$.entry->depth, $$.entry->name) > 0) {
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
