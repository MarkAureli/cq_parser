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
    VAR_DECL_NODE_T,
    VAR_DEF_NODE_T,
    CONST_NODE_T,
    REFERENCE_NODE_T,
    FUNC_CALL_NODE_T,
    FUNC_SP_NODE_T,
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
    MEASURE_NODE_T,
    BREAK_NODE_T,
    CONTINUE_NODE_T,
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
 * \brief                               Array value union
 */
typedef union array_value {
    value_t const_value;                    /*!< Constant value */
    struct node *node_value;                /*!< Node value */
} array_value_t;

typedef struct node {
    node_type_t node_type;
    struct node *left;
    struct node *right;
} node_t;

typedef struct stmt_list_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    node_t **stmt_list;
    unsigned num_of_stmts;
} stmt_list_node_t;

typedef struct func_decl_node {
    node_type_t node_type;
    entry_t *entry;
    node_t *func_tail;
} func_decl_node_t;

typedef struct var_decl_node {
    node_type_t node_type;
    entry_t *entry;
} var_decl_node_t;

typedef struct var_def_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    entry_t *entry;
    bool is_init_list;
    union {
        node_t *node;
        struct {
            qualified_type_t *qualified_types;
            array_value_t *values;
        };
    };
    unsigned length;
} var_def_node_t;

typedef struct const_node {
    node_type_t node_type;
    type_info_t type_info;
    value_t *values;
} const_node_t;

typedef struct reference_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    bool index_is_const[MAX_ARRAY_DEPTH];
    index_t indices[MAX_ARRAY_DEPTH];
    entry_t *entry;
} reference_node_t;

typedef struct func_call_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    entry_t *entry;
    bool inverse;
    bool sp;
    node_t **pars;
    unsigned num_of_pars;
} func_call_node_t;

typedef struct func_sp_node {
    node_type_t node_type;
    entry_t *entry;
} func_sp_node_t;

typedef struct logical_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    logical_op_t op;
    node_t *left;
    node_t *right;
} logical_op_node_t;

typedef struct comparison_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    comparison_op_t op;
    node_t *left;
    node_t *right;
} comparison_op_node_t;

typedef struct equality_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    equality_op_t op;
    node_t *left;
    node_t *right;
} equality_op_node_t;

typedef struct not_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    node_t *child;
} not_op_node_t;

typedef struct integer_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    integer_op_t op;
    node_t *left;
    node_t *right;
} integer_op_node_t;

typedef struct invert_op_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_info_t type_info;
    node_t *child;
} invert_op_node_t;

typedef struct if_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    node_t *condition;
    node_t *if_branch;
    node_t **else_if_branches;
    unsigned num_of_else_ifs;
    node_t *else_branch;
} if_node_t;

typedef struct else_if_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    node_t *condition;
    node_t *else_if_branch;
} else_if_node_t;

typedef struct switch_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    node_t *expression;
    node_t **case_branches;
    unsigned num_of_cases;
} switch_node_t;

typedef struct case_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    type_t case_const_type;
    value_t case_const_value;
    node_t *case_branch;
} case_node_t;

typedef struct for_node {
    node_type_t node_type;
    node_t *initialize;
    node_t *condition;
    node_t *increment;
    node_t *for_branch;
} for_node_t;

typedef struct do_node {
    node_type_t node_type;
    node_t *do_branch;
    node_t *condition;
} do_node_t;

typedef struct while_node {
    node_type_t node_type;
    node_t *condition;
    node_t *while_branch;
} while_node_t;

typedef struct assign_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_quantizable;
    assign_op_t op;
    node_t *left;
    node_t *right;
} assign_node_t;

typedef struct phase_node {
    node_type_t node_type;
    bool is_unitary;
    bool is_positive;
    node_t *left;
    node_t *right;
} phase_node_t;

typedef struct measure_node {
    node_type_t node_type;
    type_info_t type_info;
    node_t *child;
} measure_node_t;

typedef struct break_node {
    node_type_t node_type;
} break_node_t;

typedef struct continue_node {
    node_type_t node_type;
} continue_node_t;

typedef struct return_node {
    node_type_t node_type;
    type_info_t type_info;
    node_t *return_value;
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

unsigned get_length_of_array(const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth);

node_t *new_stmt_list_node(bool is_unitary, bool is_quantizable, node_t **stmt_list, unsigned num_of_stmts,
                           char error_msg[ERROR_MSG_LENGTH]);

node_t *new_func_decl_node(entry_t *entry, node_t *func_tail, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_var_decl_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_var_def_node(entry_t *entry, bool is_init_list, node_t *node, qualified_type_t *qualified_types,
                         array_value_t *values, unsigned length, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_const_node(type_t type, value_t value, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_reference_node(entry_t *entry, const bool index_is_const[MAX_ARRAY_DEPTH],
                           const index_t indices[MAX_ARRAY_DEPTH], unsigned index_depth,
                           char error_msg[ERROR_MSG_LENGTH]);

node_t *new_func_call_node(bool sp, entry_t *entry, node_t **pars, unsigned num_of_pars,
                           char error_msg[ERROR_MSG_LENGTH]);

node_t *new_func_sp_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_not_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_invert_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **else_if_branches, unsigned num_of_else_ifs,
                    node_t *else_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_switch_node(node_t *expression, node_t **case_branches, unsigned num_of_cases,
                        char error_msg[ERROR_MSG_LENGTH]);

node_t *new_case_node(node_t *case_const, node_t *case_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                     char error_msg[ERROR_MSG_LENGTH]);

node_t *new_do_node(node_t *do_branch, node_t *condition, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_while_node(node_t *condition, node_t *while_branch, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_phase_node(node_t *left, bool is_positive, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_measure_node(node_t *node, char error_msg[ERROR_MSG_LENGTH]);

node_t *new_break_node(char error_msg[ERROR_MSG_LENGTH]);

node_t *new_continue_node(char error_msg[ERROR_MSG_LENGTH]);

node_t *new_return_node(node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool is_quantizable(const node_t *node);

bool is_unitary(const node_t *node);

void copy_type_info_of_entry(type_info_t *type_info, const entry_t *entry);

bool copy_type_info_of_node(type_info_t *type_info, const node_t *node);

bool are_matching_types(type_t type_1, type_t type_2);

void print_type_info(const type_info_t *type_info);

void print_node(const node_t *node);

void tree_traversal(const node_t *node);

void free_node(node_t *node);

/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AST_H */
