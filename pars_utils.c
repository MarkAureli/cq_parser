/**
 * \file                                pars_utils.c
 * \brief                               Parse utilities source file
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
 *                                                includes
 * =====================================================================================================================
 */

#include <stdlib.h>
#include <string.h>
#include "pars_utils.h"


/*
 * =====================================================================================================================
 *                                                static variables
 * =====================================================================================================================
 */

/**
 * \brief                               Counter for loop depth (starts at `0`)
 */
static unsigned nested_loop_counter;


/*
 * =====================================================================================================================
 *                                                function definitions
 * =====================================================================================================================
 */

/* See header for documentation */
bool setup_type_info(type_info_t *type_info, type_t type, char error_msg[ERROR_MSG_LENGTH]) {
    if (type_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for type information failed");
        return false;
    }

    type_info->qualifier = NONE_T;
    type_info->type = type;
    type_info->depth = 0;
    return true;
}

/* See header for documentation */
bool append_to_type_info(type_info_t *type_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (type_info == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for type information failed");
        return false;
    } else if (type_info->depth == MAX_ARRAY_DEPTH) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Exceeding maximal array length of %i", MAX_ARRAY_DEPTH);
        return false;
    }

    type_info_t size_type_info;
    if (!copy_type_info_of_node(&size_type_info, node)) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Size parameter at position %u of array initialization is not an expression", type_info->depth);
        return false;
    } else if (size_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Size parameter at position %u of array initialization is itself an array", type_info->depth);
        return false;
    } else if (size_type_info.type == BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Size parameter at position %u of array initialization is of type bool", type_info->depth);
        return false;
    } else if (size_type_info.qualifier != CONST_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Size parameter at position %u of array initialization is not constant", type_info->depth);
        return false;
    }

    type_info->sizes[(type_info->depth)++] = ((const_node_t *) node)->values[0].u_val;
    return true;
}

/* See header for documentation */
bool setup_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (stmt_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for statement list failed");
        return false;
    } else if (nested_loop_counter == 0 && node->node_type == BREAK_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "There is no loop to break from");
        return false;
    } else if (nested_loop_counter == 0 && node->node_type == CONTINUE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "There is no loop to continue with");
        return false;
    }

    stmt_list->stmt_nodes = malloc(sizeof (node_t *));
    if (stmt_list->stmt_nodes == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for statement list failed");
        return false;
    }

    stmt_list->stmt_nodes[0] = node;
    stmt_list->is_unitary = is_unitary(node);
    stmt_list->is_quantizable = is_quantizable(node);
    stmt_list->num_of_stmts = 1;
    return true;
}

/* See header for documentation */
bool append_to_stmt_list(stmt_list_t *stmt_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (stmt_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for statement list failed");
        return false;
    } else if (nested_loop_counter == 0 && node->node_type == BREAK_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "There is no loop to break from");
        return false;
    } else if (nested_loop_counter == 0 && node->node_type == CONTINUE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "There is no loop to continue with");
        return false;
    }

    unsigned current_num_of_stmt = (stmt_list->num_of_stmts)++;
    node_t **temp = realloc(stmt_list->stmt_nodes, (current_num_of_stmt + 1) * sizeof (node_t *));
    if (temp == NULL) {
        free(stmt_list->stmt_nodes);
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for statement list failed");
        return false;
    }

    stmt_list->stmt_nodes = temp;
    stmt_list->stmt_nodes[current_num_of_stmt] = node;
    stmt_list->is_unitary = stmt_list->is_unitary && is_unitary(node);
    stmt_list->is_quantizable = stmt_list->is_quantizable && is_quantizable(node);
    return true;
}

/* See header for documentation */
bool setup_empty_func_info(func_info_t *func_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }

    func_info->is_unitary = true;
    func_info->is_quantizable = true;
    func_info->pars_type_info = NULL;
    func_info->num_of_pars = 0;
    return true;
}

/* See header for documentation */
bool setup_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }

    func_info->is_unitary = true;
    func_info->is_quantizable = true,
    func_info->pars_type_info = malloc(sizeof (type_info_t));
    if (func_info->pars_type_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }

    func_info->pars_type_info[0] = type_info;
    func_info->num_of_pars = 1;
    return true;
}

/* See header for documentation */
bool append_to_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }

    unsigned current_num_of_pars = (func_info->num_of_pars)++;
    type_info_t *temp = realloc(func_info->pars_type_info, (current_num_of_pars + 1) * sizeof (type_info_t));
    if (temp == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for function information failed");
        free(func_info->pars_type_info);
        return false;
    }

    func_info->pars_type_info = temp;
    func_info->pars_type_info[current_num_of_pars] = type_info;
    return true;
}

/* See header for documentation */
bool setup_init_info(init_info_t *init_info, bool is_init_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (init_info == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for initialization information failed");
        return false;
    }

    if (is_init_list) {
        type_info_t type_info;
        if (!copy_type_info_of_node(&type_info, node)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is not an expression");
            return false;
        } else if (type_info.depth != 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is itself a depth-%u array",
                     type_info.depth);
            return false;
        }

        init_info->is_init_list = true;
        init_info->qualified_types = malloc(sizeof (q_type_t));
        init_info->values = malloc(sizeof (array_value_t));
        if (init_info->qualified_types == NULL || init_info->values == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for initialization information failed");
            return false;
        }

        q_type_t qualified_type = { .qualifier=type_info.qualifier, .type=type_info.type };
        init_info->qualified_types[0] = qualified_type;
        if (type_info.qualifier == CONST_T) {
            init_info->values[0].const_value = ((const_node_t *) node)->values[0];
        } else {
            init_info->values[0].node_value = node;
        }
        init_info->length = 1;
    } else {
        init_info->is_init_list = false;
        init_info->node = node;
    }
    return true;
}

/* See header for documentation */
bool append_to_init_info(init_info_t *init_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (init_info == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Accessing memory for initialization information failed");
        return false;
    }

    type_info_t type_info;
    if (!copy_type_info_of_node(&type_info, node)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is not an expression");
        return false;
    } else if (type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is itself a depth-%u array",
                 type_info.depth);
        return false;
    }

    array_value_t value;
    if (type_info.qualifier == CONST_T) {
        value.const_value = ((const_node_t *) node)->values[0];
    } else {
        value.node_value = node;
    }
    q_type_t qualified_type = { .qualifier=type_info.qualifier, .type=type_info.type };
    unsigned current_length = (init_info->length)++;
    q_type_t *temp_1 = realloc(init_info->qualified_types,
                                     (current_length + 1) * sizeof (q_type_t));
    array_value_t *temp_2 = realloc(init_info->values, (current_length + 1) * sizeof (array_value_t));
    if (temp_1 == NULL || temp_2 == NULL) {
        free(init_info->qualified_types);
        free(init_info->values);
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for initialization information failed");
        return false;
    }

    init_info->qualified_types = temp_1;
    init_info->qualified_types[current_length] = qualified_type;
    init_info->values = temp_2;
    init_info->values[current_length] = value;
    return true;
}

/* See header for documentation */
bool setup_access_info(access_info_t *access_info, entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (access_info == NULL || entry == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for access information failed");
        return false;
    } else if (entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s is not called", entry->name);
        return false;
    }

    access_info->entry = entry;
    access_info->index_depth = 0;
    return true;
}

/* See header for documentation */
bool append_to_access_info(access_info_t *access_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (access_info == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Accessing memory for access information failed");
        return false;
    } else if (access_info->entry->depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Array access of scalar %s", access_info->entry->name);
        return false;
    } else if (access_info->index_depth == access_info->entry->depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Depth-%u access of depth-%u array %s",
                 access_info->index_depth + 1, access_info->entry->depth, access_info->entry->name);
        return false;
    }

    type_info_t index_type_info;
    copy_type_info_of_node(&index_type_info, node);
    if (index_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Index at position %u of access of depth-%u array %s is quantum",
                 access_info->index_depth, access_info->entry->depth, access_info->entry->name);
        return false;
    } else if (index_type_info.type == BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Index at position %u of access of depth-%u array %s is of type bool",
                 access_info->index_depth, access_info->entry->depth, access_info->entry->name);
        return false;
    } else if (index_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Index at position %u of access of depth-%u array %s is array-valued",
                 access_info->index_depth, access_info->entry->depth, access_info->entry->name);
        return false;
    }

    if (index_type_info.qualifier == CONST_T) {
        access_info->index_is_const[access_info->index_depth] = true;
        access_info->indices[(access_info->index_depth)++].const_index = ((const_node_t *) node)->values[0].u_val;
    } else {
        access_info->index_is_const[access_info->index_depth] = false;
        access_info->indices[(access_info->index_depth)++].node_index = node;
    }
    return true;
}

/* See header for documentation */
bool setup_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (else_if_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for else-if list failed");
        return false;
    }

    else_if_list->else_if_nodes = malloc(sizeof (node_t *));
    if (else_if_list->else_if_nodes == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for else-if list failed");
        return false;
    }

    else_if_list->else_if_nodes[0] = node;
    else_if_list->num_of_else_ifs = 1;
    return true;
}

/* See header for documentation */
bool append_to_else_if_list(else_if_list_t *else_if_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (else_if_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Accessing memory for else-if list failed");
        return false;
    }

    unsigned current_num_of_else_ifs = (else_if_list->num_of_else_ifs)++;
    node_t **temp = realloc(else_if_list->else_if_nodes, (current_num_of_else_ifs + 1) * sizeof (type_info_t));
    if (temp == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for else-if list failed");
        free(else_if_list->else_if_nodes);
        return false;
    }

    else_if_list->else_if_nodes = temp;
    else_if_list->else_if_nodes[current_num_of_else_ifs] = node;
    return true;
}

/* See header for documentation */
bool setup_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (case_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for case list failed");
        return false;
    }

    case_list->case_nodes = malloc(sizeof (node_t *));
    if (case_list->case_nodes == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for case list failed");
        return false;
    }

    case_list->case_nodes[0] = node;
    case_list->num_of_cases = 1;
    return true;
}

/* See header for documentation */
bool append_to_case_list(case_list_t *case_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (case_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Accessing memory for case list failed");
        return false;
    }

    unsigned current_num_of_cases = (case_list->num_of_cases)++;
    node_t **temp = realloc(case_list->case_nodes, (current_num_of_cases + 1) * sizeof (node_t *));
    if (temp == NULL) {
        free(case_list->case_nodes);
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for case list failed");
        return false;
    }

    case_list->case_nodes = temp;
    case_list->case_nodes[current_num_of_cases] = node;
    return true;
}

/* See header for documentation */
bool setup_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (arg_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for argument list failed");
        return false;
    }

    arg_list->args = malloc(sizeof (node_t *));
    if (arg_list->args == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for argument list failed");
        return false;
    }

    arg_list->args[0] = node;
    arg_list->num_of_args = 1;
    return true;
}

/* See header for documentation */
bool append_to_arg_list(arg_list_t *arg_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (arg_list == NULL || node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for argument list failed");
        return false;
    }

    unsigned current_num_of_args = (arg_list->num_of_args)++;
    node_t **temp = realloc(arg_list->args, (current_num_of_args + 1) * sizeof (type_info_t));
    if (temp == NULL) {
        free(arg_list->args);
        snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for argument list failed");
        return false;
    }

    arg_list->args = temp;
    arg_list->args[current_num_of_args] = node;
    return true;
}

/* See header for documentation */
void incr_nested_loop_counter() {
    ++nested_loop_counter;
}

/* See header for documentation */
void decr_nested_loop_counter() {
    if (nested_loop_counter > 0) {
        --nested_loop_counter;
    }
}
