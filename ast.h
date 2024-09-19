#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include "symtab.h"

typedef enum op_type {
    LOGICAL_OP,
    COMPARISON_OP,
    EQUALITY_OP,
    NOT_OP,
    INTEGER_OP,
    INVERT_OP
} op_type_t;

typedef enum logical_op {
    LAND_OP,
    LOR_OP,
    LXOR_OP
} logical_op_t;

typedef enum comparison_op {
    GE_OP,
    GEQ_OP,
    LE_OP,
    LEQ_OP
} comparison_op_t;

typedef enum equality_op {
    EQ_OP,
    NEQ_OP
} equality_op_t;

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

typedef enum assign_op {
    ASSIGN_OP,
    ASSIGN_OR_OP,
    ASSIGN_XOR_OP,
    ASSIGN_AND_OP,
    ASSIGN_ADD_OP,
    ASSIGN_SUB_OP,
    ASSIGN_MUL_OP,
    ASSIGN_DIV_OP,
    ASSIGN_MOD_OP
} assign_op_t;

typedef enum node_type {
    BASIC_NODE_T,
    VAR_DECL_NODE_T,
    FUNC_DECL_NODE_T,
    CONST_NODE_T,
    REFERENCE_NODE_T,
    FUNC_CALL_NODE_T,
    LOGICAL_OP_NODE_T,
    COMPARISON_OP_NODE_T,
    EQUALITY_OP_NODE_T,
    NOT_OP_NODE_T,
    INTEGER_OP_NODE_T,
    INVERT_OP_NODE_T,
    IF_NODE_T,
    ELSE_NODE_T,
    FOR_NODE_T,
    DO_NODE_T,
    WHILE_NODE_T,
    ASSIGN_NODE_T,
    JUMP_NODE_T,
    RETURN_NODE_T
} node_type_t;

typedef struct type_info {
    type_t type;
    unsigned sizes[MAXARRAYDEPTH];
    unsigned depth;
} type_info_t;

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

typedef union array_index {
    unsigned const_index;
    node_t *node_index;
} array_index_t;

typedef struct array_var_infos {
    bool is_array_init;
    var_info_t *var_infos;
    unsigned length;
} array_var_infos_t;

typedef struct array_access_info {
    list_t *entry;
    bool index_is_const[MAXARRAYDEPTH];
    array_index_t indices[MAXARRAYDEPTH];
    unsigned depth;
} array_access_info_t;

typedef struct func_decl_node {
    node_type_t type;
    list_t *entry;
} func_decl_node_t;

typedef struct var_decl_node {
    node_type_t type;
    list_t *entry;
} var_decl_node_t;

typedef struct const_node {
    node_type_t type;
    var_info_t var_info;
} const_node_t;

typedef struct reference_node {
    node_type_t type;
    var_info_t var_info;
    bool index_is_const[MAXARRAYDEPTH];
    array_index_t indices[MAXARRAYDEPTH];
    unsigned depth;
    list_t *entry;
} reference_node_t;

typedef struct func_call_node {
    node_type_t type;
    var_info_t var_info;
    list_t *entry;
    node_t **pars;
    unsigned num_of_pars;
} func_call_node_t;

typedef struct logical_op_node {
    node_type_t type;
    var_info_t var_info;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_op_node_t;

typedef struct comparison_op_node {
    node_type_t type;
    var_info_t var_info;
    comparison_op_t op;
    node_t *left;
    node_t *right;
} comparison_op_node_t;

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

typedef struct integer_op_node {
    node_type_t type;
    var_info_t var_info;
    integer_op_t op;
    node_t *left;
    node_t *right;
} integer_op_node_t;

typedef struct invert_op_node {
    node_type_t type;
    var_info_t var_info;
    node_t *child;
} invert_op_node_t;

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

char *op_type_to_str(op_type_t op_type);

char *logical_op_to_str(logical_op_t logical_op);

char *comparison_op_to_str(comparison_op_t comparison_op);

char *equality_op_to_str(equality_op_t equality_op);

char *integer_op_to_str(integer_op_t integer_op);

char *assign_op_to_str(assign_op_t assign_op);

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_func_decl_node(list_t *entry);

node_t *new_var_decl_node(list_t *entry);

node_t *new_const_node(type_t type, value_t value);

node_t *new_reference_node(list_t *entry);

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars);

node_t *new_logical_op_node(logical_op_t op, node_t *left, node_t *right);

node_t *new_comparison_op_node(comparison_op_t op, node_t *left, node_t *right);

node_t *new_equality_op_node(equality_op_t op, node_t *left, node_t *right);

node_t *new_not_op_node(node_t *child);

node_t *new_integer_op_node(integer_op_t op, node_t *left, node_t *right);

node_t *new_invert_op_node(node_t *child);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned elseif_count, node_t *else_branch);

node_t *new_else_node(node_t *condition, node_t *elseif_branch);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch);

node_t *new_do_node(node_t *do_branch, node_t *condition);

node_t *new_while_node(node_t *condition, node_t *while_branch);

node_t *new_assign_node(list_t *entry, node_t *assign_val);

node_t *new_jump_node(int statement_type);

node_t *new_return_node(type_t ret_type, node_t *ret_val);

type_info_t type_info_init(type_t type, unsigned depth);

array_var_infos_t array_var_infos_init(const var_info_t *var_infos, unsigned old_length, unsigned length);

array_access_info_t array_access_info_init(list_t *entry);

var_info_t get_var_info_of_node(const node_t *node);

node_t *build_logical_op_node(logical_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_comparison_op_node(comparison_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_equality_op_node(equality_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_not_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]);

node_t *build_integer_op_node(integer_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_invert_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);

#endif // AST_H
