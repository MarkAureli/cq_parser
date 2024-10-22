/**
 * \file                                parsing.h
 * \brief                               Parsing include file
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

#ifndef PARSING_H
#define PARSING_H


/*
 * =====================================================================================================================
 *                                                includes
 * =====================================================================================================================
 */

#include <stdbool.h>
#include "ast.h"
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

typedef struct stmt_list {
    bool is_unitary;
    bool is_quantizable;
    node_t **stmt_nodes;
    unsigned num_of_stmts;
} stmt_list_t;

/**
 * \brief                               Function info struct
 * \note                                This structure defines all characteristics (except return type) of a function
 */
typedef struct func_info {
    bool is_unitary;                        /*!< Whether function is unitary */
    bool is_quantizable;                    /*!< Whether function can be quantized */
    type_info_t *pars_type_info;            /*!< Type information of function parameters */
    unsigned num_of_pars;                   /*!< Number of function parameters */
} func_info_t;

typedef struct access_info {
    entry_t *entry;
    bool index_is_const[MAX_ARRAY_DEPTH];
    index_t indices[MAX_ARRAY_DEPTH];
    unsigned index_depth;
} access_info_t;

typedef struct init_info {
    bool is_init_list;
    union {
        node_t *node;
        struct {
            qualified_type_t *qualified_types;
            array_value_t *values;
            unsigned length;
        };
    };
} init_info_t;

typedef struct else_if_list {
    node_t **else_if_nodes;
    unsigned num_of_else_ifs;
} else_if_list_t;

typedef struct case_list {
    node_t **case_nodes;
    unsigned num_of_cases;
} case_list_t;

typedef struct arg_list {
    node_t **args;
    unsigned num_of_args;
} arg_list_t;


/*
 * =====================================================================================================================
 *                                                function declarations
 * =====================================================================================================================
 */

bool setup_atomic_type_info(type_info_t *type_info, type_t type, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_type_info(type_info_t *type_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_empty_func_info(func_info_t *func_info, char error_msg[ERROR_MSG_LENGTH]);

bool setup_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]);

bool setup_access_info(access_info_t *access_info, entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_access_info(access_info_t *access_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_init_info(init_info_t *init_info, bool is_init_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_init_info(init_info_t *init_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool setup_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

bool append_to_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

void incr_nested_loop_counter();

void decr_nested_loop_counter();


/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PARSING_H */
