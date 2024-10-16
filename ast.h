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
    STMT_LIST_NODE_T,
    FUNC_DECL_NODE_T,
    FUNC_SP_NODE_T,
    VAR_DECL_NODE_T,
    VAR_DEF_NODE_T,
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

typedef struct node {
    node_type_t type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct stmt_list_node {
    node_type_t type;
    node_t **stmt_list;
    unsigned num_of_stmt;
} stmt_list_node_t;

typedef union array_index {
    unsigned const_index;
    node_t *node_index;
} array_index_t;

typedef union array_value {
    value_t const_value;
    node_t *node_value;
} array_value_t;

typedef struct init_list {
    qualified_type_t *qualified_types;
    array_value_t *values;
    unsigned length;
} init_list_t;

typedef struct init_info {
    bool is_init_list;
    union {
        node_t *node;
        init_list_t init_list;
    };
} init_info_t;

typedef struct array_access_info {
    list_t *entry;
    bool index_is_const[MAXARRAYDEPTH];
    array_index_t indices[MAXARRAYDEPTH];
    unsigned depth;
} array_access_info_t;

typedef struct array_values_info {
    bool *value_is_const;
    array_value_t *values;
} array_values_info_t;

typedef struct arg_list {
    node_t **pars;
    unsigned num_of_pars;
} arg_list_t;

typedef struct func_decl_node {
    node_type_t type;
    list_t *entry;
} func_decl_node_t;

typedef struct func_sp_node {
    node_type_t type;
    list_t *entry;
} func_sp_node_t;

typedef struct var_decl_node {
    node_type_t type;
    list_t *entry;
} var_decl_node_t;

typedef struct var_def_node {
    node_type_t type;
    list_t *entry;
    bool is_init_list;
    union {
        node_t *node;
        array_values_info_t array_values_info;
    };
} var_def_node_t;

typedef struct const_node {
    node_type_t type;
    type_info_t type_info;
    value_t *values;
} const_node_t;

typedef struct reference_node {
    node_type_t type;
    type_info_t type_info;
    bool index_is_const[MAXARRAYDEPTH];
    array_index_t indices[MAXARRAYDEPTH];
    list_t *entry;
} reference_node_t;

typedef struct func_call_node {
    node_type_t type;
    type_info_t type_info;
    list_t *entry;
    bool inverse;
    node_t **pars;
    unsigned num_of_pars;
} func_call_node_t;

typedef struct logical_op_node {
    node_type_t type;
    type_info_t type_info;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_op_node_t;

typedef struct comparison_op_node {
    node_type_t type;
    type_info_t type_info;
    comparison_op_t op;
    node_t *left;
    node_t *right;
} comparison_op_node_t;

typedef struct equality_op_node {
    node_type_t type;
    type_info_t type_info;
    equality_op_t op;
    node_t *left;
    node_t *right;
} equality_op_node_t;

typedef struct not_op_node {
    node_type_t type;
    type_info_t type_info;
    node_t *child;
} not_op_node_t;

typedef struct integer_op_node {
    node_type_t type;
    type_info_t type_info;
    integer_op_t op;
    node_t *left;
    node_t *right;
} integer_op_node_t;

typedef struct invert_op_node {
    node_type_t type;
    type_info_t type_info;
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
    assign_op_t op;
    node_t *left;
    node_t *right;
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

void apply_logical_op(logical_op_t op, value_t *out, value_t in_1, value_t in_2);

void apply_comparison_op(comparison_op_t op, value_t *out, type_t in_type_1, value_t in_value_1, type_t in_type_2,
                         value_t in_value_2);

int apply_integer_op(integer_op_t op, value_t *out, type_t in_type_1, value_t in_value_1, type_t in_type_2,
                     value_t in_value_2);

unsigned get_length_of_array(const unsigned sizes[MAXARRAYDEPTH], unsigned depth);

value_t *get_reduced_array(const value_t *values, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                          const unsigned indices[MAXARRAYDEPTH], unsigned index_depth);

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_stmt_list_node(node_t *stmt);

void append_to_stmt_list(node_t *stmt_list_node, node_t *stmt);

node_t *new_func_decl_node(list_t *entry);

node_t *new_func_sp_node(list_t *entry);

node_t *new_var_decl_node(list_t *entry);

node_t *new_var_def_node(list_t *entry, bool is_init_list, node_t *node, array_values_info_t array_values_info);

node_t *new_var_def_node_from_node(list_t *entry, node_t *node);

node_t *new_var_def_node_from_init_list(list_t *entry, bool *value_is_const, array_value_t *values);

node_t *new_const_node(type_t type, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, value_t *values);

node_t *new_reference_node(const unsigned sizes[MAXARRAYDEPTH], unsigned depth, bool index_is_const[MAXARRAYDEPTH],
                           array_index_t indices[MAXARRAYDEPTH], list_t *entry);

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars);

node_t *new_logical_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                            logical_op_t op, node_t *left, node_t *right);

node_t *new_comparison_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                               comparison_op_t op, node_t *left, node_t *right);

node_t *new_equality_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                             equality_op_t op, node_t *left, node_t *right);

node_t *new_not_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, node_t *child);

node_t *new_integer_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH],
                            unsigned depth, integer_op_t op, node_t *left, node_t *right);

node_t *new_invert_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH],
                           unsigned depth, node_t *child);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned elseif_count,
                    node_t *else_branch);

node_t *new_else_node(node_t *condition, node_t *elseif_branch);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch);

node_t *new_do_node(node_t *do_branch, node_t *condition);

node_t *new_while_node(node_t *condition, node_t *while_branch);

node_t *new_assign_node(assign_op_t op, node_t *left, node_t *right);

node_t *new_jump_node(int statement_type);

node_t *new_return_node(type_t ret_type, node_t *ret_val);

type_info_t type_info_init(type_t type, unsigned depth);

init_info_t *new_array_init_info_from_node(node_t *node);

init_info_t *new_array_init_info_from_init_list(qualified_type_t qualified_type, array_value_t value);

void append_to_array_init_info(init_info_t *array_init_info, qualified_type_t qualified_type,
                               array_value_t value);

array_access_info_t array_access_info_init(list_t *entry);

arg_list_t arg_list_init(node_t *node);

void append_to_arg_list(arg_list_t *arg_list, node_t *node);

type_info_t *get_type_info_of_node(const node_t *node);

bool are_matching_types(type_t type_1, type_t type_2);

node_t *build_func_sp_node(list_t * entry, char error_msg[ERRORMSGLENGTH]);

node_t *build_var_def_node(list_t *entry, init_info_t *init_info, char error_msg[ERRORMSGLENGTH]);

node_t *build_do_node(node_t *do_branch, node_t *condition, char error_msg[ERRORMSGLENGTH]);

node_t *build_while_node(node_t *condition, node_t *while_branch, char error_msg[ERRORMSGLENGTH]);

node_t *build_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                       char error_msg[ERRORMSGLENGTH]);

node_t *build_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_not_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]);

node_t *build_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]);

node_t *build_invert_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]);

node_t *build_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars, char error_msg[ERRORMSGLENGTH]);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);

#endif // AST_H
