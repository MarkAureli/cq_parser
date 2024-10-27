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
 * \brief                               Return style enumeration
 */
typedef enum return_style {
    NONE_ST,                                /*!< No return style */
    CONDITIONAL_ST,                         /*!< Conditional return style*/
    DEFINITE_ST,                            /*!< Definite return style */
} return_style_t;

/**
 * \brief                               Node type enumeration
 */
typedef enum node_type {
    BASIC_NODE_T,                           /*!< Basic node type */
    STMT_LIST_NODE_T,                       /*!< Statement list node type */
    VAR_DECL_NODE_T,                        /*!< Variable declaration node type */
    VAR_DEF_NODE_T,                         /*!< Variable definition node type */
    FUNC_DEF_NODE_T,                        /*!< Function definition node type */
    CONST_NODE_T,                           /*!< Constant node type */
    REFERENCE_NODE_T,                       /*!< Reference node type */
    FUNC_CALL_NODE_T,                       /*!< Function call node type */
    FUNC_SP_NODE_T,                         /*!< Function superposition node type */
    LOGICAL_OP_NODE_T,                      /*!< Logical operator node type */
    COMPARISON_OP_NODE_T,                   /*!< Comparison operator node type */
    EQUALITY_OP_NODE_T,                     /*!< Equality operator node type */
    NOT_OP_NODE_T,                          /*!< Not-operator node type */
    INTEGER_OP_NODE_T,                      /*!< Integer operator node type */
    INVERT_OP_NODE_T,                       /*!< Invert-operator node type */
    IF_NODE_T,                              /*!< If node type */
    ELSE_IF_NODE_T,                         /*!< Else-if node type */
    SWITCH_NODE_T,                          /*!< Switch node type */
    CASE_NODE_T,                            /*!< Case node type */
    FOR_NODE_T,                             /*!< For-loop node type */
    DO_NODE_T,                              /*!< Do-while-loop node type */
    WHILE_NODE_T,                           /*!< While-loop node type */
    ASSIGN_NODE_T,                          /*!< Assignment node type */
    PHASE_NODE_T,                           /*!< Phase node type */
    MEASURE_NODE_T,                         /*!< Measurement node type */
    BREAK_NODE_T,                           /*!< Break node type */
    CONTINUE_NODE_T,                        /*!< Continue node type */
    RETURN_NODE_T,                          /*!< Return node type */
} node_type_t;

/**
 * \brief                               Index union
 */
typedef union index {
    unsigned const_index;                   /*!< Constant index */
    struct node *node_index;                /*!< Pointer to node as index */
} index_t;

/**
 * \brief                               Array value union
 */
typedef union array_value {
    value_t const_value;                    /*!< Constant value */
    struct node *node_value;                /*!< Node-pointer value */
} array_value_t;

/**
 * \brief                               Basic node struct
 * \note                                This structure defines a basic-node with two child nodes
 */
typedef struct node {
    node_type_t node_type;                  /*!< Node type */
    struct node *left;                      /*!< Pointer to left child */
    struct node *right;                     /*!< Pointer to right child */
} node_t;

/**
 * \brief                               Statement list node struct
 * \note                                This structure defines a statement-list-node with at least one child node
 */
typedef struct stmt_list_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether all statements are quantizable */
    bool is_unitary;                        /*!< Whether all statements are unitary */
    node_t **stmt_list;                     /*!< List of statements (pointers to child nodes) */
    unsigned num_of_stmts;                  /*!< Number of statements */
    return_style_t return_style;            /*!< Return style of statements */
    type_info_t return_type_info;           /*!< Return type information of statements */
} stmt_list_node_t;

/**
 * \brief                               Variable declaration node struct
 * \note                                This structure defines a variable-declaration-node with no child nodes
 */
typedef struct var_decl_node {
    node_type_t node_type;                  /*!< Node type */
    entry_t *entry;                         /*!< Pointer to corresponding entry in the symbol table */
} var_decl_node_t;

/**
 * \brief                               Variable definition node struct
 * \note                                This structure defines a variable-definition-node with an arbitrary number of
 *                                          child nodes
 */
typedef struct var_def_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether the variable definition is quantizable */
    bool is_unitary;                        /*!< Whether the variable definition is unitary */
    entry_t *entry;                         /*!< Pointer to corresponding entry in the symbol table */
    bool is_init_list;                      /*!< Whether the variable is initialized with an initializer list */
    union {
        node_t *node;                       /*!< Pointer to right-hand side of variable definition (child node) */
        struct {
            q_type_t *q_types;              /*!< Array of qualified types of initializer list entries */
            array_value_t *values;          /*!< Array of initializer list entries (possibly child nodes) */
        };
    };
    unsigned length;                        /*!< Length of initializer list */
} var_def_node_t;

/**
 * \brief                               Function definition node struct
 * \note                                This structure defines a function-definition-node with one child node
 */
typedef struct func_def_node {
    node_type_t node_type;                  /*!< Node type */
    entry_t *entry;                         /*!< Pointer to corresponding entry in the symbol table */
    node_t *func_tail;                      /*!< Pointer to function tail (child node) */
} func_def_node_t;

/**
 * \brief                               Constant node struct
 * \note                                This structure defines a constant-node with no child nodes
 */
typedef struct const_node {
    node_type_t node_type;                  /*!< Node type */
    type_info_t type_info;                  /*!< Type information of constant */
    value_t *values;                        /*!< Array of constant values */
} const_node_t;

/**
 * \brief                               Reference node struct
 * \note                                This structure defines a reference-node with an arbitrary number of child nodes
 */
typedef struct reference_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether reference is quantizable */
    bool is_unitary;                        /*!< Whether reference is unitary */
    type_info_t type_info;                  /*!< Type information of reference */
    bool index_is_const[MAX_ARRAY_DEPTH];   /*!< Array of whether indices have constant values */
    index_t indices[MAX_ARRAY_DEPTH];       /*!< Array of indices (possibly child nodes) */
    entry_t *entry;                         /*!< Pointer to entry of referenced variable in the symbol table */
} reference_node_t;

/**
 * \brief                               Function call node struct
 * \note                                This structure defines a function-call-node with an arbitrary number of child
 *                                          nodes
 */
typedef struct func_call_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether function call is quantizable */
    bool is_unitary;                        /*!< Whether function call is unitary */
    type_info_t type_info;                  /*!< Type information of the function's return */
    entry_t *entry;                         /*!< Pointer to entry of called function in the symbol table */
    bool inverse;                           /*!< Whether the inverted function is called */
    bool sp;                                /*!< Whether it is a superposition-creating function call */
    node_t **pars;                          /*!< Array of function parameters (pointers to child nodes) */
    unsigned num_of_pars;                   /*!< Number of function parameters */
} func_call_node_t;

/**
 * \brief                               Function superposition node struct
 * \note                                This structure defines a function-superposition-node with no child nodes
 */
typedef struct func_sp_node {
    node_type_t node_type;                  /*!< Node type */
    entry_t *entry;                         /*!< Pointer to entry of the function in the symbol table */
} func_sp_node_t;

/**
 * \brief                               Logical operator node struct
 * \note                                This structure defines a logical-operator-node with two child nodes
 */
typedef struct logical_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether logical operation is quantizable */
    bool is_unitary;                        /*!< Whether logical operation is unitary */
    type_info_t type_info;                  /*!< Type information of the logical operation's result */
    logical_op_t op;                        /*!< Logical operator */
    node_t *left;                           /*!< Pointer to left operand (child node) */
    node_t *right;                          /*!< Pointer tp right operand (child node) */
} logical_op_node_t;

/**
 * \brief                               Comparison operator node struct
 * \note                                This structure defines a comparison-operator-node with two child nodes
 */
typedef struct comparison_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether comparison operation is quantizable */
    bool is_unitary;                        /*!< Whether comparison operation is unitary */
    type_info_t type_info;                  /*!< Type information of the comparison operation's result */
    comparison_op_t op;                     /*!< Comparison operator */
    node_t *left;                           /*!< Pointer to left operand (child node) */
    node_t *right;                          /*!< Pointer to right operand (child node) */
} comparison_op_node_t;

/**
 * \brief                               Equality operator node struct
 * \note                                This structure defines an equality-operator-node with two child nodes
 */
typedef struct equality_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether equality operation is quantizable */
    bool is_unitary;                        /*!< Whether equality operation is unitary */
    type_info_t type_info;                  /*!< Type information of the equality operation's result */
    equality_op_t op;                       /*!< Equality operator */
    node_t *left;                           /*!< Pointer to left operand (child node) */
    node_t *right;                          /*!< Pointer to right operand (child node) */
} equality_op_node_t;

/**
 * \brief                               Not-operator node struct
 * \note                                This structure defines a not-operator-node with one child node
 */
typedef struct not_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether not-operation is quantizable */
    bool is_unitary;                        /*!< Whether not-operation is unitary */
    type_info_t type_info;                  /*!< Type information of the not-operation's result */
    node_t *child;                          /*!< Pointer to operand (child node) */
} not_op_node_t;

/**
 * \brief                               Integer operator node struct
 * \note                                This structure defines an integer-operator-node with two child nodes
 */
typedef struct integer_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether integer operation is quantizable */
    bool is_unitary;                        /*!< Whether integer operation is unitary */
    type_info_t type_info;                  /*!< Type information of the integer operation's result */
    integer_op_t op;                        /*!< Integer operator */
    node_t *left;                           /*!< Pointer to left operand (child node) */
    node_t *right;                          /*!< Pointer to right operand (child node) */
} integer_op_node_t;

/**
 * \brief                               Invert-operator node struct
 * \note                                This structure defines an invert-operator-node with one child node
 */
typedef struct invert_op_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether invert-operation is quantizable */
    bool is_unitary;                        /*!< Whether invert-operation is unitary */
    type_info_t type_info;                  /*!< Type information of the invert-operation's result */
    node_t *child;                          /*!< Pointer to operand (child node) */
} invert_op_node_t;

/**
 * \brief                               If node struct
 * \note                                This structure defines an if-node with at least two child nodes
 */
typedef struct if_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether if(-else)-statement is quantizable */
    bool is_unitary;                        /*!< Whether if(-else)-statement is unitary */
    node_t *condition;                      /*!< Pointer to if-condition (child node) */
    node_t *if_branch;                      /*!< Pointer to if-branch (child node) */
    node_t **else_ifs;                      /*!< Array of else-ifs (pointers to child nodes) */
    unsigned num_of_else_ifs;               /*!< Number of else-ifs */
    node_t *else_branch;                    /*!< Optional pointer to else-branch (child node) */
    return_style_t return_style;            /*!< Return style of if(-else)-statement */
    type_info_t return_type_info;           /*!< Return type information of if(-else)-statement */
} if_node_t;

/**
 * \brief                               Else-if node struct
 * \note                                This structure defines an else-if-node with two child nodes
 */
typedef struct else_if_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether else-if-statement is quantizable */
    bool is_unitary;                        /*!< Whether else-if-statement is unitary */
    node_t *condition;                      /*!< Pointer to else-if-condition (child node) */
    node_t *else_if_branch;                 /*!< Pointer to else-if-branch (child node) */
    return_style_t return_style;            /*!< Return style of else-if-statement */
    type_info_t return_type_info;           /*!< Return type information of else-if-statement */
} else_if_node_t;

/**
 * \brief                               Switch node struct
 * \note                                This structure defines a switch-node with at least one child node
 */
typedef struct switch_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether switch-statement is quantizable */
    bool is_unitary;                        /*!< Whether switch-statement is unitary */
    node_t *expression;                     /*!< Pointer to switch-expression (child node) */
    node_t **cases;                         /*!< Array of cases (pointers to child nodes) */
    unsigned num_of_cases;                  /*!< Number of cases */
    return_style_t return_style;            /*!< Return style of switch-statement */
    type_info_t return_type_info;           /*!< Return type information of switch-statement */
} switch_node_t;

/**
 * \brief                               Case node struct
 * \note                                This structure defines a case-node with one child nodes
 */
typedef struct case_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether case is quantizable */
    bool is_unitary;                        /*!< Whether case is unitary */
    type_t case_const_type;                 /*!< Type of case value */
    value_t case_const_value;               /*!< Case value */
    node_t *case_branch;                    /*!< Pointer to case branch (child node) */
    return_style_t return_style;            /*!< Return style of case */
    type_info_t return_type_info;           /*!< Return type information of case */
} case_node_t;

/**
 * \brief                               For-loop node struct
 * \note                                This structure defines a for-loop-node with four child nodes
 */
typedef struct for_node {
    node_type_t node_type;                  /*!< Node type */
    node_t *initialize;                     /*!< Pointer to for-loop-initialization statement (child node) */
    node_t *condition;                      /*!< Pointer to for-loop-condition (child node) */
    node_t *increment;                      /*!< Pointer to for-loop-increment (child node) */
    node_t *for_branch;                     /*!< Pointer to for-loop-branch (child node) */
} for_node_t;

/**
 * \brief                               Do-while-loop node struct
 * \note                                This structure defines a do-while-loop-node with two child nodes
 */
typedef struct do_node {
    node_type_t node_type;                  /*!< Node type */
    node_t *do_branch;                      /*!< Pointer to do-while-loop-branch (child node) */
    node_t *condition;                      /*!< Pointer to do-while-loop-condition (child node) */
} do_node_t;

/**
 * \brief                               While-loop node struct
 * \note                                This structure defines a while-loop-node with two child nodes
 */
typedef struct while_node {
    node_type_t node_type;                  /*!< Node type */
    node_t *condition;                      /*!< Pointer to while-loop-condition (child node) */
    node_t *while_branch;                   /*!< Pointer to while-loop-branch (child node) */
} while_node_t;

/**
 * \brief                               Assignment node struct
 * \note                                This structure defines an assignment-node with two child nodes
 */
typedef struct assign_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether assignment is quantizable */
    bool is_unitary;                        /*!< Whether assignment is unitary */
    assign_op_t op;                         /*!< Assignment operator */
    node_t *left;                           /*!< Pointer to left-hand side of assignment (child node) */
    node_t *right;                          /*!< Pointer to right-hand side of assignment (child node) */
} assign_node_t;

/**
 * \brief                               Phase node struct
 * \note                                This structure defines a phase-node with two child nodes
 */
typedef struct phase_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_unitary;                        /*!< Whether change of phase is unitary */
    bool is_positive;                       /*!< Whether change of phase is positive */
    node_t *left;                           /*!< Pointer to variable whose phase is changed (child node) */
    node_t *right;                          /*!< Pointer to change of phase (child node) */
} phase_node_t;

/**
 * \brief                               Measurement node struct
 * \note                                This structure defines a measurement-node with one child node
 */
typedef struct measure_node {
    node_type_t node_type;                  /*!< Node type */
    type_info_t type_info;                  /*!< Type information of measurement result */
    node_t *child;                          /*!< Pointer to quantity to be measured */
} measure_node_t;

/**
 * \brief                               Break node struct
 * \note                                This structure defines a break-node with zero child nodes
 */
typedef struct break_node {
    node_type_t node_type;                  /*!< Node type */
} break_node_t;

/**
 * \brief                               Continue node struct
 * \note                                This structure defines a continue-node with zero child nodes
 */
typedef struct continue_node {
    node_type_t node_type;                  /*!< Node type */
} continue_node_t;

/**
 * \brief                               Return node struct
 * \note                                This structure defines a return-node with one child node
 */
typedef struct return_node {
    node_type_t node_type;                  /*!< Node type */
    bool is_quantizable;                    /*!< Whether return statement is quantizable */
    bool is_unitary;                        /*!< Whether return statement is unitary */
    type_info_t type_info;                  /*!< Return style */
    node_t *return_value;                   /*!< Pointer to returned quantity (child node) */
} return_node_t;


/*
 * =====================================================================================================================
 *                                                function declarations
 * =====================================================================================================================
 */

/**
 * \brief                               Convert logical operator to printable string
 * \param[in]                           logical_op: Logical operator
 * \return                              String representing input logical operator
 */
char *logical_op_to_str(logical_op_t logical_op);

/**
 * \brief                               Convert comparison operator to printable string
 * \param[in]                           comparison_op: Comparison operator
 * \return                              String representing input comparison operator
 */
char *comparison_op_to_str(comparison_op_t comparison_op);

/**
 * \brief                               Convert equality operator to printable string
 * \param[in]                           equality_op: Equality operator
 * \return                              String representing input equality operator
 */
char *equality_op_to_str(equality_op_t equality_op);

/**
 * \brief                               Convert integer operator to printable string
 * \param[in]                           integer_op: Integer operator
 * \return                              String representing input integer operator
 */
char *integer_op_to_str(integer_op_t integer_op);

/**
 * \brief                               Convert assignment operator to printable string
 * \param[in]                           assign_op: Assignment operator
 * \return                              String representing input assignment operator
 */
char *assign_op_to_str(assign_op_t assign_op);

/**
 * \brief                               Copy type information from a node to given address
 * \param[out]                          type_info: Address to copy the type information to
 * \param[in]                           node: Pointer to node whose type information is to be copied
 * \return                              Whether copying type information was successful
 */
bool copy_type_info_of_node(type_info_t *type_info, const node_t *node);

/**
 * \brief                               Check the quantizable-attribute of a node
 * \param[in]                           node: Pointer to node whose quantizable-attribute is to be checked
 * \return                              Node's Quantizable-attribute
 */
bool is_quantizable(const node_t *node);

/**
 * \brief                               Check the unitary-attribute of a node
 * \param[in]                           node: Pointer to node whose unitary-attribute is to be checked
 * \return                              Node's unitary-attribute
 */
bool is_unitary(const node_t *node);

/**
 * \brief                               Allocate new statement-list-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           is_quantizable: Whether statement list is quantizable
 * \param[in]                           is_unitary: Whether statement list is unitary
 * \param[in]                           stmt_list: Statement list
 * \param[in]                           num_of_stmts: Number of statements
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated statement-list-node or `NULL` upon failure
 */
node_t *new_stmt_list_node(bool is_quantizable, bool is_unitary, node_t **stmt_list, unsigned num_of_stmts,
                           char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new variable-declaration-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           entry: Pointer to entry of declared variable in the symbol table
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated variable-declaration-node or `NULL` upon failure
 */
node_t *new_var_decl_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new variable-definition-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           entry: Pointer to entry of defined variable in the symbol table
 * \param[in]                           is_init_list: Whether variable definition is done via an initializer list
 * \param[in]                           node: Pointer to right-hand side of list-free variable definition
 * \param[in]                           q_types: Array of qualified types of values within initializer list
 * \param[in]                           values: Array of values with initializer list
 * \param[in]                           length: Length of initializer list
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated variable-definition-node or `NULL` upon failure
 */
node_t *new_var_def_node(entry_t *entry, bool is_init_list, node_t *node, q_type_t *q_types,
                         array_value_t *values, unsigned length, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new function-definition-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           entry: Pointer to entry of defined function in the symbol table
 * \param[in]                           func_tail: Pointer to tail of function definition
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated function-definition-node or `NULL` upon failure
 */
node_t *new_func_def_node(entry_t *entry, node_t *func_tail, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new constant-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           type: Type of constant value
 * \param[in]                           value: Constant value
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated constant-node or `NULL` upon failure
 */
node_t *new_const_node(type_t type, value_t value, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new reference-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           entry: Pointer to entry of referenced variable in the symbol table
 * \param[in]                           index_is_const: Array of whether indices in reference are constant
 * \param[in]                           indices: Array of indices of reference
 * \param[in]                           index_depth: Number of indices in reference
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated reference-node or `NULL` upon failure
 */
node_t *new_reference_node(entry_t *entry, const bool index_is_const[MAX_ARRAY_DEPTH],
                           const index_t indices[MAX_ARRAY_DEPTH], unsigned index_depth,
                           char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new function-call-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           sp: Whether function is called as a superposition-creating function
 * \param[in]                           entry: Pointer to entry of called function in the symbol table
 * \param[in]                           pars: Parameters of function call
 * \param[in]                           num_of_pars: Number of parameters of function call
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated function-call-node or `NULL` upon failure
 */
node_t *new_func_call_node(bool sp, entry_t *entry, node_t **pars, unsigned num_of_pars,
                           char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new function-superposition-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           entry: Pointer to entry of called function in the symbol table
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated function-superposition-node or `NULL` upon failure
 */
node_t *new_func_sp_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new logical-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to left-hand side of logical operation
 * \param[in]                           op: Logical operator
 * \param[in]                           right: Pointer to right-hand side of logical operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated logical-operator-node or `NULL` upon failure
 */
node_t *new_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new comparison-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to left-hand side of comparison operation
 * \param[in]                           op: Comparison operator
 * \param[in]                           right: Pointer to right-hand side of comparison operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated comparison-operator-node or `NULL` upon failure
 */
node_t *new_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new equality-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to left-hand side of equality operation
 * \param[in]                           op: Equality operator
 * \param[in]                           right: Pointer to right-hand side of equality operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated equality-operator-node or `NULL` upon failure
 */
node_t *new_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new not-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           child: Pointer to operand of not-operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated not-operator-node or `NULL` upon failure
 */
node_t *new_not_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new integer-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to left-hand side of integer operation
 * \param[in]                           op: Integer operator
 * \param[in]                           right: Pointer to right-hand side of integer operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated integer-operator-node or `NULL` upon failure
 */
node_t *new_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new invert-operator-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           child: Pointer to operand of invert-operation
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated invert-operator-node or `NULL` upon failure
 */
node_t *new_invert_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new if-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           condition: Pointer to if-condition
 * \param[in]                           if_branch: Pointer to if-branch
 * \param[in]                           else_ifs: Pointer to else-if-statements
 * \param[in]                           num_of_else_ifs: Number of else-if-statements
 * \param[in]                           else_branch: Optional pointer to else-branch
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated if-node or `NULL` upon failure
 */
node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **else_ifs, unsigned num_of_else_ifs,
                    node_t *else_branch, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new else-if-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           condition: Pointer to else-if-condition
 * \param[in]                           else_if_branch: Pointer to else-if-branch
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated else-if-node or `NULL` upon failure
 */
node_t *new_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new switch-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           expression: Pointer to switch-expression
 * \param[in]                           cases: Cases
 * \param[in]                           num_of_cases: Number of cases
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated switch-node or `NULL` upon failure
 */
node_t *new_switch_node(node_t *expression, node_t **cases, unsigned num_of_cases, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new case-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           case_const: Pointer to case constant (qualified type and value)
 * \param[in]                           case_branch: Pointer to case-branch
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated case-node or `NULL` upon failure
 */
node_t *new_case_node(node_t *case_const, node_t *case_branch, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new for-loop-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           initialize: Pointer to for-loop-initialization statement
 * \param[in]                           condition: Pointer to for-loop-condition statement
 * \param[in]                           increment: Pointer to for-loop-increment statement
 * \param[in]                           for_branch: Pointer to for-loop-branch
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated for-loop-node or `NULL` upon failure
 */
node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                     char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new do-while-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           do_branch: Pointer to do-while-loop-branch
 * \param[in]                           condition: Pointer to do-while-loop-condition
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated do-while-loop-node or `NULL` upon failure
 */
node_t *new_do_node(node_t *do_branch, node_t *condition, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new while-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           condition: Pointer to while-loop-condition
 * \param[in]                           while_branch: Pointer to while-loop-branch
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated while-loop-node or `NULL` upon failure
 */
node_t *new_while_node(node_t *condition, node_t *while_branch, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new assignment-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to left-hand side of assignment
 * \param[in]                           op: Assignment operator
 * \param[in]                           right: Pointer to right-hand side of assignment
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated assignment-node or `NULL` upon failure
 */
node_t *new_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new phase-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           left: Pointer to variable whose phase is changed
 * \param[in]                           positive: Whether change of phase is positive
 * \param[in]                           right: Pointer to change of phase
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated phase-node or `NULL` upon failure
 */
node_t *new_phase_node(node_t *left, bool is_positive, node_t *right, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new measurement-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           child: Pointer to quantity to be measured
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated measurement-node or `NULL` upon failure
 */
node_t *new_measure_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new break-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated break-node or `NULL` upon failure
 */
node_t *new_break_node(char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new continue-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated continue-node or `NULL` upon failure
 */
node_t *new_continue_node(char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Allocate new return-node and return pointer to it
 * \note                                Memory is allocated dynamically and must therefore be freed manually
 * \param[in]                           return_value: Pointer to returned quantity
 * \param[out]                          error_msg: Message to be written in case of an error or illegal parameters
 * \return                              Pointer to newly allocated return-node or `NULL` upon failure
 */
node_t *new_return_node(node_t *return_value, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Recursively free the tree emerging from a root node
 * \param[in]                           root: Pointer to root node of the tree to be freed
 */
void free_tree(node_t *root);

/**
 * \brief                               Write node information to output file
 * \param[out]                          output_file: Pointer to output file for node information
 * \param[in]                           node: Pointer to node whose information is to be written
 */
void fprint_node(FILE *output_file, const node_t *node);

/**
 * \brief                               Write tree information to output file
 * \param[out]                          output_file: Pointer to output file for tree information
 * \param[in]                           root: Pointer to root node of the tree whose information is to be written
 */
void fprint_tree(FILE *output_file, const node_t *root);


/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AST_H */
