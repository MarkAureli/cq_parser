#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include "symtab.h"

typedef enum integer_op {
    ADD_OP,
    AND_OP,
    DIV_OP,
    MOD_OP,
    MUL_OP,
    OR_OP,
    SUB_OP,
    XOR_OP
} integer_op_t;

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
    FUNC_CALL_NODE_T,
    INTEGER_OP_NODE_T,
    SHIFT_OP_NODE_T,
    INVERT_OP_NODE_T,
    LOGICAL_OP_NODE_T,
    RELATION_OP_NODE_T,
    EQUALITY_OP_NODE_T,
    NOT_OP_NODE_T,
    REFERENCE_NODE_T,
    IF_NODE_T,
    ELSE_NODE_T,
    FOR_NODE_T,
    DO_NODE_T,
    WHILE_NODE_T,
    ASSIGN_NODE_T,
    JUMP_NODE_T,
    RETURN_NODE_T
} node_type_t;

typedef struct var_info {
    qualifier_t qualifier;
    type_t type;
    value_t value;
} var_info_t;

typedef struct node {
    node_type_t type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct var_decl_node {
    node_type_t type;
    list_t *entry;
} var_decl_node_t;

typedef struct func_decl_node {
    node_type_t type;
    list_t *entry;
} func_decl_node_t;

typedef struct const_node {
    node_type_t type;
    var_info_t var_info;
} const_node_t;

typedef struct func_call_node {
    node_type_t type;
    var_info_t var_info;
    list_t *entry;
    node_t **pars;
    unsigned num_of_pars;
} func_call_node_t;

typedef struct integer_op_node {
    node_type_t type;
    var_info_t var_info;
    integer_op_t op;
    node_t *left;
    node_t *right;
} integer_op_node_t;

typedef struct shift_op_node {
    node_type_t type;
    var_info_t var_info;
    shift_op_t op;
    node_t *left;
    node_t *right;
} shift_op_node_t;

typedef struct invert_op_node {
    node_type_t type;
    var_info_t var_info;
    node_t *child;
} invert_op_node_t;

typedef struct logical_op_node {
    node_type_t type;
    var_info_t var_info;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_op_node_t;

typedef struct relation_op_node {
    node_type_t type;
    var_info_t var_info;
    relation_op_t op;
    node_t *left;
    node_t *right;
} relation_op_node_t;

typedef struct equality_op_node {
    node_type_t type;
    var_info_t var_info;
    equality_op_t op;
    node_t *left;
    node_t *right;
} equality_op_node_t;

typedef struct not_op_node {
    node_type_t type;
    var_info_t var_info;
    node_t *child;
} not_op_node_t;

typedef struct reference_node {
    node_type_t type;
    var_info_t var_info;
    unsigned indices[MAXARRAYDEPTH];
    unsigned depth;
    list_t *entry;
} reference_node_t;

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
    list_t *entry;
    node_t *assign_val;
} assign_node_t;

typedef struct jump_node {
    node_type_t type;
    int statement_type;
} jump_node_t;

typedef struct return_node {
    node_type_t type;
    type_t ret_type;
    node_t *ret_val;
} return_node_t;

char *integer_op_to_str(integer_op_t integer_op);

char *shift_op_to_str(shift_op_t shift_op);

char *logical_op_to_str(logical_op_t logical_op);

char *relation_op_to_str(relation_op_t relation_op);

char *equality_op_to_str(equality_op_t equality_op);

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_var_decl_node(list_t *entry);

node_t *new_func_decl_node(list_t *entry);

node_t *new_const_node(type_t type, value_t value);

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars);

node_t *new_integer_op_node(integer_op_t op, node_t *left, node_t *right);

node_t *new_shift_op_node(shift_op_t op, node_t *left, node_t *right);

node_t *new_invert_op_node(node_t *child);

node_t *new_logical_op_node(logical_op_t op, node_t *left, node_t *right);

node_t *new_relation_op_node(relation_op_t op, node_t *left, node_t *right);

node_t *new_equality_op_node(equality_op_t op, node_t *left, node_t *right);

node_t *new_not_op_node(node_t *child);

node_t *new_reference_node(list_t *entry);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned elseif_count, node_t *else_branch);

node_t *new_else_node(node_t *condition, node_t *elseif_branch);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch);

node_t *new_do_node(node_t *do_branch, node_t *condition);

node_t *new_while_node(node_t *condition, node_t *while_branch);

node_t *new_assign_node(list_t *entry, node_t *assign_val);

node_t *new_jump_node(int statement_type);

node_t *new_return_node(type_t ret_type, node_t *ret_val);

node_t *build_arithmetic_node(integer_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);

#endif // AST_H
