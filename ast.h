#ifndef AST_H
#define AST_H
#include <stdbool.h>
#include <stdlib.h>
#include "symtab.h"

typedef union value {
    bool bval;
    int ival;
    unsigned uval;
} value_t;

typedef enum arithm_op {
    ADD_OP,
    DIV_OP,
    MOD_OP,
    MUL_OP,
    SUB_OP
} arithm_op_t;

typedef enum bit_op {
    AND_OP,
    OR_OP,
    XOR_OP
} bit_op_t;

typedef enum logical_op {
    LAND_OP,
    LOR_OP,
    LXOR_OP
} logical_op_t;

typedef enum rel_op {
    GE_OP,
    GEQ_OP,
    LE_OP,
    LEQ_OP
} rel_op_t;

typedef enum equ_op {
    EQ_OP,
    NEQ_OP
} equ_op_t;

typedef enum node_type {
    BASIC_NODE_T,
    DECL_NODE_T,
    CONST_NODE_T,
    IF_NODE_T,
    ELSE_NODE_T,
    FOR_NODE_T,
    DO_NODE_T,
    WHILE_NODE_T,
    ASSIGN_NODE_T,
    JUMP_NODE_T,
    FUNC_CALL_NODE_T,
    ARITHM_NODE_T,
    BIT_NODE_T,
    SHIFT_NODE_T,
    LOGICAL_NODE_T,
    REL_NODE_T,
    EQU_NODE_T,
    FUNC_DECL_NODE_T,
    RETURN_NODE_T
} node_type_t;

typedef struct node {
    node_type_t type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct decl_node {
    node_type_t type;
    type_t data_type;
    list_t *name;
} decl_node_t;

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

typedef struct func_call_node {
    node_type_t type;
    list_t *entry;
    node_t **pars;
} func_call_node_t;

typedef struct arithm_node {
    node_type_t type;
    arithm_op_t op;
    node_t *left;
    node_t *right;
} arithm_node_t;

typedef struct bit_node {
    node_type_t type;
    bit_op_t op;
    node_t *left;
    node_t *right;
} bit_node_t;

typedef struct logical_node {
    node_type_t type;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_node_t;

typedef struct rel_node {
    node_type_t type;
    rel_op_t op;
    node_t *left;
    node_t *right;
} rel_node_t;

typedef struct equ_node {
    node_type_t type;
    equ_op_t op;
    node_t *left;
    node_t *right;
} equ_node_t;

typedef struct func_decl_node {
    node_type_t type;
    type_t ret_type;
    list_t *entry;
} func_decl_node_t;

typedef struct return_node {
    node_type_t type;
    type_t ret_type;
    node_t *ret_val;
} return_node_t;

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_decl_node(type_t data_type, list_t *name);

node_t *new_const_node(type_t data_type, value_t value);

#endif // AST_H
