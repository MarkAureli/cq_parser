/**
 * \file                                pars_utils.h
 * \brief                               Parse utilities include file
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
            q_type_t *qualified_types;
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

/**
 * \brief                               Setup type information at a given address with primitive type
 * \param[out]                          type_info: Address to setup the type information at
 * \param[in]                           type: Primitive type of type information
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the type information was successful
 */
bool setup_type_info(type_info_t *type_info, type_t type, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append size to type information at a given address
 * \param[out]                          type_info: Address of type information
 * \param[in]                           node: Pointer to node carrying the appended size
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending size to type information was successful
 */
bool append_to_type_info(type_info_t *type_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup statement list at a given address with node
 * \param[out]                          stmt_list: Address to setup the statement list at
 * \param[in]                           node: Pointer to initial statement node
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the statement list was successful
 */
bool setup_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append statement to statement list at a given address
 * \param[out]                          stmt_list: Address of statement list
 * \param[in]                           node: Pointer to appended statement node
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending statement to statement list was successful
 */
bool append_to_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup empty function information at a given address
 * \param[out]                          func_info: Address to setup the empty function information at
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the empty function information was successful
 */
bool setup_empty_func_info(func_info_t *func_info, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup function information at a given address with return type information
 * \param[out]                          func_info: Address to setup the function information at
 * \param[in]                           type_info: Return type information
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the function information was successful
 */
bool setup_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append return type information to function information at a given address
 * \param[out]                          func_info: Address of function information
 * \param[in]                           type_info: Appended return type information
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending return type information to function information was successful
 */
bool append_to_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup access information at a given address with symbol table entry
 * \param[out]                          access_info: Address to setup the access information at
 * \param[in]                           entry: Pointer to symbol table entry of variable that is accessed
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the access information was successful
 */
bool setup_access_info(access_info_t *access_info, entry_t *entry, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append index to access information at a given address
 * \param[out]                          access_info: Address of access information
 * \param[in]                           node: Pointer to node representing the access index
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending index to access information was successful
 */
bool append_to_access_info(access_info_t *access_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup initialization information at a given address with node
 * \param[out]                          init_info: Address to setup the initialization information at
 * \param[in]                           is_init_list: Whether initialization is given by an initializer list
 * \param[in]                           node: Pointer to node representing first value of initialization
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the initialization information was successful
 */
bool setup_init_info(init_info_t *init_info, bool is_init_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append value to initialization information at a given address
 * \param[out]                          init_info: Address of initialization information
 * \param[in]                           node: Pointer to node representing appended value of initialization
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending value to initialization information was successful
 */
bool append_to_init_info(init_info_t *init_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup else-if list at a given address with node
 * \param[out]                          else_if_list: Address to setup the else-if list at
 * \param[in]                           node: Pointer to node representing first else-if
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the else-if list was successful
 */
bool setup_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append else-if to else-if list at a given address
 * \param[out]                          else_if_list: Address to setup the else-if list at
 * \param[in]                           node: Pointer to node representing appended else-if
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending else-if to else-if list was successful
 */
bool append_to_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup case list at a given address with node
 * \param[out]                          case_list: Address to setup the case list at
 * \param[in]                           node: Pointer to node representing first case
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the case list was successful
 */
bool setup_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append case to case list at a given address
 * \param[out]                          case_list: Address of case list
 * \param[in]                           node: Pointer to node representing appended case
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending case to case list was successful
 */
bool append_to_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Setup argument list at a given address with node
 * \param[out]                          arg_list: Address to setup the argument list at
 * \param[in]                           node: Pointer to node representing first argument
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting up the argument list was successful
 */
bool setup_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Append argument to argument list at a given address
 * \param[out]                          arg_list: Address of argument list
 * \param[in]                           node: Pointer to node representing appended argument
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether appending argument to argument list was successful
 */
bool append_to_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Increase nested-loop counter
 */
void incr_nested_loop_counter();

/**
 * \brief                               Decrease nested-loop counter
 */
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
