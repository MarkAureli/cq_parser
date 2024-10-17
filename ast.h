/**
 * \file                                ast.h
 * \brief                               AST include file
 */

/*
 * Copyright (c) 2024 Lennart BINKOWSKI
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of cq_compiler.
 *
 * Author:          Lennart BINKOWSKI <lennart.binkowski@itp.uni-hannover.de>
 */


/*
 * =====================================================================================================================
 *                                                header guard
 * =====================================================================================================================
 */

#ifndef AST_H
#define AST_H


/*
 * =====================================================================================================================
 *                                                includes
 * =====================================================================================================================
 */

#include <stdbool.h>
#include "symbol_table.h"


/*
 * =====================================================================================================================
 *                                                C++ check
 * =====================================================================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * =====================================================================================================================
 *                                                type definitions
 * =====================================================================================================================
 */

/**
 * \brief                               Operator type enumeration
 */
typedef enum op_type {
    LOGICAL_OP,                             /*!< Logical operation */
    COMPARISON_OP,                          /*!< Comparison operation */
    EQUALITY_OP,                            /*!< Equality operation */
    NOT_OP,                                 /*!< Not operation */
    INTEGER_OP,                             /*!< Integer operation */
    INVERT_OP,                              /*!< Invert operation */
} op_type_t;

/**
 * \brief                               Logical operator enumeration
 */
typedef enum logical_op {
    LAND_OP,                                /*!< Logical AND operation */
    LOR_OP,                                 /*!< Logical OR operation */
    LXOR_OP,                                /*!< Logical XOR operation */
} logical_op_t;

/**
 * \brief                               Comparison operator enumeration
 */
typedef enum comparison_op {
    GE_OP,                                  /*!< Greater-than operation */
    GEQ_OP,                                 /*!< Greater-or-equal operation */
    LE_OP,                                  /*!< Less-than operation */
    LEQ_OP,                                 /*!< Less-or-equal operation */
} comparison_op_t;

/**
 * \brief                               Equality operator enumeration
 */
typedef enum equality_op {
    EQ_OP,                                  /*!< Equality operation */
    NEQ_OP,                                 /*!< Inequality operation */
} equality_op_t;

/**
 * \brief                               Integer operator enumeration
 */
typedef enum integer_op {
    OR_OP,                                  /*!< Bitwise OR operation */
    XOR_OP,                                 /*!< Bitwise XOR operation */
    AND_OP,                                 /*!< Bitwise AND operation */
    ADD_OP,                                 /*!< Addition operation */
    SUB_OP,                                 /*!< Subtraction operation */
    MUL_OP,                                 /*!< Multiplication operation */
    DIV_OP,                                 /*!< Division operation */
    MOD_OP,                                 /*!< Modulo operation */
} integer_op_t;

/**
 * \brief                               Assignment operator enumeration
 */
typedef enum assign_op {
    ASSIGN_OP,                              /*!< Assignment operation */
    ASSIGN_OR_OP,                           /*!< Assignment-OR operation */
    ASSIGN_XOR_OP,                          /*!< Assignment-XOR operation */
    ASSIGN_AND_OP,                          /*!< Assignment-AND operation */
    ASSIGN_ADD_OP,                          /*!< Assignment-addition operation */
    ASSIGN_SUB_OP,                          /*!< Assignment-subtraction operation */
    ASSIGN_MUL_OP,                          /*!< Assignment-multiplication operation */
    ASSIGN_DIV_OP,                          /*!< Assignment-division operation */
    ASSIGN_MOD_OP,                          /*!< Assignment-modulo operation */
} assign_op_t;

/**
 * \brief                               Node type enumeration
 */
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
    ELSE_IF_NODE_T,
    SWITCH_NODE_T,
    CASE_NODE_T,
    FOR_NODE_T,
    DO_NODE_T,
    WHILE_NODE_T,
    ASSIGN_NODE_T,
    PHASE_NODE_T,
    JUMP_NODE_T,
    RETURN_NODE_T,
} node_type_t;

/**
 * \brief                               Index union
 */
typedef union index {
    unsigned const_index;                   /*!< Constant index */
    struct node *node_index;                /*!< Node index */
} index_t;

/**
 * \brief                               Array value index
 */
typedef union array_value {
    value_t const_value;                    /*!< Constant value */
    struct node *node_value;                /*!< Node value */
} array_value_t;

typedef struct init_list {
    qualified_type_t *qualified_types;
    array_value_t *values;
    unsigned length;
} init_list_t;

typedef struct init_info {
    bool is_init_list;
    union {
        struct node *node;
        init_list_t init_list;
    };
} init_info_t;

typedef struct access_info {
    entry_t *entry;
    bool index_is_const[MAX_ARRAY_DEPTH];
    index_t indices[MAX_ARRAY_DEPTH];
    unsigned depth;
} access_info_t;

typedef struct array_values_info {
    bool *value_is_const;
    array_value_t *values;
} array_values_info_t;

typedef struct else_if_list {
    struct node **else_if_nodes;
    unsigned num_of_else_ifs;
} else_if_list_t;

typedef struct case_list {
    struct node **case_nodes;
    unsigned num_of_cases;
} case_list_t;

typedef struct arg_list {
    struct node **args;
    unsigned num_of_args;
} arg_list_t;

typedef struct node {
    node_type_t type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct stmt_list_node {
    node_type_t type;
    bool is_unitary;
    node_t **stmt_list;
    unsigned num_of_stmt;
} stmt_list_node_t;

typedef struct func_decl_node {
    node_type_t type;
    entry_t *entry;
} func_decl_node_t;

typedef struct func_sp_node {
    node_type_t type;
    entry_t *entry;
} func_sp_node_t;

typedef struct var_decl_node {
    node_type_t type;
    entry_t *entry;
} var_decl_node_t;

typedef struct var_def_node {
    node_type_t type;
    entry_t *entry;
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
    bool index_is_const[MAX_ARRAY_DEPTH];
    index_t indices[MAX_ARRAY_DEPTH];
    entry_t *entry;
} reference_node_t;

typedef struct func_call_node {
    node_type_t type;
    type_info_t type_info;
    entry_t *entry;
    bool inverse;
    bool sp;
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
    node_t **else_if_branches;
    unsigned num_of_else_ifs;
    node_t *else_branch;
} if_node_t;

typedef struct else_if_node {
    node_type_t type;
    node_t *condition;
    node_t *else_if_branch;
} else_if_node_t;

typedef struct switch_node {
    node_type_t type;
    node_t *expression;
    node_t **case_branches;
    unsigned num_of_cases;
} switch_node_t;

typedef struct case_node {
    node_type_t type;
    node_t *case_const;
    node_t *case_branch;
} case_node_t;

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

typedef struct phase_node {
    node_type_t type;
    bool is_positive;
    node_t *left;
    node_t *right;
} phase_node_t;

typedef struct jump_node {
    node_type_t type;
    int statement_type;
} jump_node_t;

typedef struct return_node {
    node_type_t type;
    type_t ret_type;
    node_t *ret_val;
} return_node_t;


/*
 * =====================================================================================================================
 *                                                function declarations
 * =====================================================================================================================
 */

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

unsigned get_length_of_array(const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth);

value_t *get_reduced_array(const value_t *values, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                           const unsigned indices[MAX_ARRAY_DEPTH], unsigned index_depth);

node_t *new_node(node_type_t type, node_t *left, node_t *right);

node_t *new_stmt_list_node(node_t *stmt);

void append_to_stmt_list(node_t *stmt_list_node, node_t *stmt);

type_info_t create_type_info(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth);

func_info_t create_empty_func_info();

func_info_t create_func_info(type_info_t type_info);

void append_to_func_info(func_info_t *func_info, type_info_t type_info);

node_t *new_func_decl_node(entry_t *entry);

node_t *new_func_sp_node(entry_t *entry);

node_t *new_var_decl_node(entry_t *entry);

node_t *new_var_def_node(entry_t *entry, bool is_init_list, node_t *node, array_values_info_t array_values_info);

node_t *new_var_def_node_from_node(entry_t *entry, node_t *node);

node_t *new_var_def_node_from_init_list(entry_t *entry, bool *value_is_const, array_value_t *values);

node_t *new_const_node(type_t type, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, value_t *values);

node_t *new_reference_node(const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, bool index_is_const[MAX_ARRAY_DEPTH],
                           index_t indices[MAX_ARRAY_DEPTH], entry_t *entry);

node_t *new_func_call_node(entry_t *entry, bool sp, node_t **pars, unsigned num_of_pars);

node_t *new_logical_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                            logical_op_t op, node_t *left, node_t *right);

node_t *new_comparison_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                               comparison_op_t op, node_t *left, node_t *right);

node_t *new_equality_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                             equality_op_t op, node_t *left, node_t *right);

node_t *new_not_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, node_t *child);

node_t *new_integer_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
                            unsigned depth, integer_op_t op, node_t *left, node_t *right);

node_t *new_invert_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
                           unsigned depth, node_t *child);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned num_of_else_ifs,
                    node_t *else_branch);

node_t *new_else_if_node(node_t *condition, node_t *elseif_branch);

node_t *new_switch_node(node_t *expression, node_t **case_branches, unsigned num_of_cases);

node_t *new_case_node(node_t *case_const, node_t *case_branch);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch);

node_t *new_do_node(node_t *do_branch, node_t *condition);

node_t *new_while_node(node_t *condition, node_t *while_branch);

node_t *new_assign_node(assign_op_t op, node_t *left, node_t *right);

node_t *new_phase_node(bool is_positive, node_t *left, node_t *right);

node_t *new_jump_node(int statement_type);

node_t *new_return_node(type_t ret_type, node_t *ret_val);

type_info_t create_atomic_type_info(type_t type);

init_info_t *new_init_info_from_node(node_t *node);

init_info_t *new_init_info_from_init_list(qualified_type_t qualified_type, array_value_t value);

void append_to_init_info(init_info_t *array_init_info, qualified_type_t qualified_type, array_value_t value);

bool stmt_is_unitary(const node_t *node);

access_info_t create_access_info(entry_t *entry);

else_if_list_t create_else_if_list(node_t *node);

void append_to_else_if_list(else_if_list_t *else_if_list, node_t *node);

case_list_t create_case_list(node_t *node);

void append_to_case_list(case_list_t *case_list, node_t *node);

arg_list_t create_arg_list(node_t *node);

void append_to_arg_list(arg_list_t *arg_list, node_t *node);

type_info_t *get_type_info_of_node(const node_t *node);

bool are_matching_types(type_t type_1, type_t type_2);

node_t *build_func_sp_node(entry_t * entry, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_var_def_node(entry_t *entry, init_info_t *init_info, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_if_node(node_t *condition, node_t *if_branch, node_t **else_if_branches, unsigned num_of_else_ifs,
                      node_t *else_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_switch_node(node_t *expression, node_t **case_branches, unsigned num_of_cases,
                          char error_msg[ERROR_MSG_LENGTH]);

node_t *build_do_node(node_t *do_branch, node_t *condition, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_while_node(node_t *condition, node_t *while_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                       char error_msg[ERROR_MSG_LENGTH]);

node_t *build_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_phase_node(node_t *left, bool is_positive, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_not_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_invert_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

node_t *build_func_call_node(bool sp, entry_t *entry, node_t **pars, unsigned num_of_pars,
                             char error_msg[ERROR_MSG_LENGTH]);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);


/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AST_H */
