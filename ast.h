#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include "symtab.h"

typedef enum arithmetic_op {
    ADD_OP,
    DIV_OP,
    MOD_OP,
    MUL_OP,
    SUB_OP
} arithmetic_op_t;

typedef enum increment_op {
    INCR_OP,
    DECR_OP
} increment_op_t;

typedef enum bitwise_op {
    AND_OP,
    OR_OP,
    XOR_OP
} bitwise_op_t;

typedef enum shift_op {
    LSHIFT_OP,
    RSHIFT_OP
} shift_op_t;

typedef enum logical_op {
    LAND_OP,
    LOR_OP,
    LXOR_OP
} logical_op_t;

typedef enum relation_op {
    GE_OP,
    GEQ_OP,
    LE_OP,
    LEQ_OP
} relation_op_t;

typedef enum equality_op {
    EQ_OP,
    NEQ_OP
} equality_op_t;

typedef enum node_type {
    BASIC_NODE_T,
    VAR_DECL_NODE_T,
    FUNC_DECL_NODE_T,
    CONST_NODE_T,
    IF_NODE_T,
    ELSE_NODE_T,
    FOR_NODE_T,
    DO_NODE_T,
    WHILE_NODE_T,
    ASSIGN_NODE_T,
    JUMP_NODE_T,
    FUNC_CALL_NODE_T,
    ARITHMETIC_NODE_T,
    BITWISE_NODE_T,
    SHIFT_NODE_T,
    LOGICAL_NODE_T,
    RELATION_NODE_T,
    EQUALITY_NODE_T,
    REFERENCE_NODE_T,
    RETURN_NODE_T
} node_type_t;

typedef struct node {
    node_type_t type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct var_decl_node {
    node_type_t type;
    list_t *symtab_elem;
} var_decl_node_t;

typedef struct func_decl_node {
    node_type_t type;
    list_t *symtab_elem;
} func_decl_node_t;

typedef struct const_node {
    node_type_t type;
    type_t data_type;
    value_t value;
} const_node_t;

typedef struct if_node {
    node_type_t type;
    node_t *condition;
    node_t *if_branch;
    node_t **elseif_branches;
    unsigned elseif_count;
    node_t *else_branch;
} if_node_t;

typedef struct else_node {
    node_type_t type;
    node_t *condition;
    node_t *elseif_branch;
} else_node_t;

typedef struct for_node {
    node_type_t type;
    node_t *initialize;
    node_t *condition;
    node_t *increment;
    node_t *for_branch;
} for_node_t;

typedef struct do_node {
    node_type_t type;
    node_t *do_branch;
    node_t *condition;
} do_node_t;

typedef struct while_node {
    node_type_t type;
    node_t *condition;
    node_t *while_branch;
} while_node_t;

typedef struct assign_node {
    node_type_t type;
    list_t *symtab_elem;
    node_t *assign_val;
} assign_node_t;

typedef struct jump_node {
    node_type_t type;
    int statement_type;
} jump_node_t;

typedef struct func_call_node {
    node_type_t type;
    list_t *symtab_elem;
    node_t **pars;
    unsigned num_of_pars;
} func_call_node_t;

typedef struct arithmetic_node {
    node_type_t type;
    arithmetic_op_t op;
    node_t *left;
    node_t *right;
} arithmetic_node_t;

typedef struct bit_node {
    node_type_t type;
    bitwise_op_t op;
    node_t *left;
    node_t *right;
} bit_node_t;

typedef struct shift_node {
    node_type_t type;
    shift_op_t op;
    node_t *left;
    node_t *right;
} shift_node_t;

typedef struct logical_node {
    node_type_t type;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_node_t;

typedef struct relation_node {
    node_type_t type;
    relation_op_t op;
    node_t *left;
    node_t *right;
} relation_node_t;

typedef struct equality_node {
    node_type_t type;
    equality_op_t op;
    node_t *left;
    node_t *right;
} equality_node_t;

typedef struct reference_node {
    node_type_t type;
    list_t *symtab_elem;
    unsigned indices[MAXARRAYDEPTH];
    unsigned depth;
} reference_node_t;

typedef struct return_node {
    node_type_t type;
    type_t ret_type;
    node_t *ret_val;
} return_node_t;

char *arithmetic_op_to_str(arithmetic_op_t arithmetic_op);

char *increment_op_to_str(increment_op_t increment_op);

char *bitwise_op_to_str(bitwise_op_t bitwise_op);

char *shift_op_to_str(shift_op_t shift_op);

char *logical_op_to_str(logical_op_t logical_op);

char *relation_op_to_str(relation_op_t relation_op);

char *equality_op_to_str(equality_op_t equality_op);

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_var_decl_node(list_t *symtab_elem);

node_t *new_func_decl_node(list_t *symtab_elem);

node_t *new_const_node(type_t data_type, value_t value);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned elseif_count, node_t *else_branch);

node_t *new_else_node(node_t *condition, node_t *elseif_branch);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch);

node_t *new_do_node(node_t *do_branch, node_t *condition);

node_t *new_while_node(node_t *condition, node_t *while_branch);

node_t *new_assign_node(list_t *symtab_elem, node_t *assign_val);

node_t *new_jump_node(int statement_type);

node_t *new_func_call_node(list_t *symtab_elem, node_t **pars, unsigned num_of_pars);

node_t *new_arithmetic_node(arithmetic_op_t op, node_t *left, node_t *right);

node_t *new_bitwise_node(bitwise_op_t op, node_t *left, node_t *right);

node_t *new_shift_node(shift_op_t op, node_t *left, node_t *right);

node_t *new_logical_node(logical_op_t op, node_t *left, node_t *right);

node_t *new_relation_node(relation_op_t op, node_t *left, node_t *right);

node_t *new_equality_node(equality_op_t op, node_t *left, node_t *right);

node_t *new_reference_node(list_t *symtab_elem);

node_t *new_return_node(type_t ret_type, node_t *ret_val);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);

#endif // AST_H
