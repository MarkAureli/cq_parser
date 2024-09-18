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
    array_values_t array_values;
    array_access_info_t array_access_info;
    integer_op_t integer_op;
    logical_op_t logical_op;
    comparison_op_t comparison_op;
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
%type <array_values> init init_elem_l
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
        $$ = new_var_decl_node($3);
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
        $$ = new_var_decl_node($2);
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
	    $$ = insert($1, strlen($1), yylineno, true);
	}
	;

init:
    const {
        $$ = array_values_init(NULL, 0, 1);
        $$.values[0] = ((const_node_t *) $1)->var_info.value;
    }
    | LBRACE init_elem_l RBRACE {
        $$ = $2;
    }
    ;

init_elem_l:
    const {
        $$ = array_values_init(NULL, 0, 1);
        $$.values[0] = ((const_node_t *) $1)->var_info.value;
    }
    | init_elem_l COMMA const {
        $$ = array_values_init($1.values, $1.length, $1.length + 1);
        $$.values[$1.length] = ((const_node_t *) $3)->var_info.value;
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
	comparison_expr
	| logical_and_expr LAND comparison_expr
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
	| array_access LBRACKET UCONST RBRACKET {
	    unsigned depth = $1.depth;
	    list_t *entry = $1.entry;
        if (depth == entry->depth) {
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", depth + 1, entry->depth, entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too deep access of array");
            }
        } else if (depth + 1 != entry->depth) {
            if (snprintf(error_msg, sizeof (error_msg), "Insufficient depth-%u access of depth-%u array %s", depth + 1, entry->depth, entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too shallow access of array");
            }
        }
        for (unsigned i = 0; i < depth; ++i) {
            if ($1.indices[i] >= entry->sizes[i]) {
                if (snprintf(error_msg, sizeof (error_msg), "%u-th index (%u) array index of %s out of bounds (%u)", i, $1.indices[i], entry->name, entry->sizes[i]) > 0) {
                    yyerror(error_msg);
                } else {
                    yyerror("Array index out of bounds");
                }
            }
        }
        if ($3.uval >= entry->sizes[depth]) {
            if (snprintf(error_msg, sizeof (error_msg), "%u-th index (%u) array index of %s out of bounds (%u)", depth, $3.uval, entry->name, entry->sizes[depth]) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Array index out of bounds");
            }
        }
        if (entry->qualifier == CONST_T) {
            unsigned index = 0;
            for (unsigned i = 0; i < depth; ++i) {
                unsigned factor = i;
                for (unsigned j = i + 1; j < depth; ++j) {
                    factor *= $1.indices[j];
                }
                index += factor;
            }
            index += $3.uval;
            $$ = new_const_node(entry->type, entry->values[index]);
        } else {
            $$ = new_reference_node(entry);
            memcpy(((reference_node_t *) $$)->indices, $1.indices, depth * sizeof (unsigned));
            ((reference_node_t *) $$)->indices[depth++] = $3.uval;
            ((reference_node_t *) $$)->depth = depth;
        }
	}
	| ID LPAREN argument_expr_l RPAREN {
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
	;

array_access:
    ID {
        $$ = array_access_info_init(insert($1, strlen($1), yylineno, false));
        if ($$.entry->depth == 0) {
            if (snprintf(error_msg, sizeof (error_msg), "Array access of scalar %s", $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Array access of scalar");
            }
        }
    }
    | array_access LBRACKET UCONST RBRACKET {
        $$ = $1;
        if ($$.depth == $$.entry->depth) {
            if (snprintf(error_msg, sizeof (error_msg), "Depth-%u access of depth-%u array %s", $$.depth + 1, $$.entry->depth, $$.entry->name) > 0) {
                yyerror(error_msg);
            } else {
                yyerror("Too deep acess of array");
            }
        }
        $$.indices[($$.depth)++] = $3.uval;
    }
    ;

primary_expr:
	ID {
	    list_t *entry = insert($1, strlen($1), yylineno, false);
	    if (entry->depth != 0) {
	        if (snprintf(error_msg, sizeof (error_msg), "Depth-%u array %s is not indexed", entry->depth, entry->name) > 0 ) {
	            yyerror(error_msg);
	        } else {
	            yyerror("Array is not index");
	        }
	    }
	    if (entry->qualifier == CONST_T) {
	        $$ = new_const_node(entry->type, entry->values[0]);
	    } else {
	        $$ = new_reference_node(entry);
	    }
	}
	| const {
	    $$ = $1;
	}
	| LPAREN logical_or_expr RPAREN {
	    $$ = new_node(BASIC_NODE_T, NULL, NULL); /* dummy node */
	}
	;

argument_expr_l:
	logical_or_expr
	| argument_expr_l COMMA logical_or_expr
	;

const:
    BCONST {
        $$ = new_const_node(BOOL_T, $1);
    }
    | ICONST {
        $$ = new_const_node(INT_T, $1);
    }
    | UCONST {
        $$ = new_const_node(UNSIGNED_T, $1);
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
