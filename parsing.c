/**
 * \file                                parsing.c
 * \brief                               Parsing source file
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
#include "parsing.h"


/*
 * =====================================================================================================================
 *                                                function definitions
 * =====================================================================================================================
 */

type_info_t create_atomic_type_info(type_t type) {
    type_info_t new_type_info = { .type = type};
    return new_type_info;
}

type_info_t create_type_info(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
                             unsigned depth) {
    type_info_t new_type_info = { .qualifier = qualifier, .type = type, .depth = depth };
    memcpy(new_type_info.sizes, sizes, depth * sizeof (unsigned));
    return new_type_info;
}

bool setup_empty_func_info(func_info_t *func_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }
    func_info->is_unitary = true;
    func_info->is_sp = false;
    func_info->pars_type_info = NULL;
    func_info->num_of_pars = 0;
    return true;
}

bool setup_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }
    func_info->is_unitary = true;
    func_info->is_sp = type_info.depth == 0;
    func_info->pars_type_info = malloc(sizeof (type_info_t));
    func_info->pars_type_info[0] = type_info;
    func_info->num_of_pars = 0;
    return true;
}

bool append_to_func_info(func_info_t *func_info, type_info_t type_info, char error_msg[ERROR_MSG_LENGTH]) {
    if (func_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function information failed");
        return false;
    }

    func_info->is_sp = false;
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

bool setup_init_info(init_info_t *init_info, bool is_init_list, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (init_info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for initialization information failed");
        return false;
    }

    if (is_init_list) {
        type_info_t *type_info = get_type_info_of_node(node);
        if (type_info == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is not an expression");
            return false;
        } else if (type_info->depth != 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is itself a depth-%u array",
                     type_info->depth);
            return false;
        }

        init_info->is_init_list = true;
        init_info->qualified_types = malloc(sizeof (qualified_type_t));
        qualified_type_t qualified_type = { .qualifier=type_info->qualifier, .type=type_info->type };
        init_info->qualified_types[0] = qualified_type;
        init_info->values = malloc(sizeof (array_value_t));
        if (type_info->qualifier == CONST_T) {
            init_info->values[0].const_value = ((const_node_t *) node)->values[0];
        } else {
            init_info->values[0].node_value = node;
        }
    } else {
        init_info->is_init_list = false;
        init_info->node = node;
    }
    return true;
}

bool append_to_init_info(init_info_t *init_info, node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t *info = get_type_info_of_node(node);
    if (info == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is not an expression");
        return false;
    } else if (info->depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Element of array initializer entry is itself a depth-%u array",
                 info->depth);
        return false;
    }
    array_value_t value;
    if (info->qualifier == CONST_T) {
        value.const_value = ((const_node_t *) node)->values[0];
    } else {
        value.node_value = node;
    }
    qualified_type_t qualified_type = { .qualifier=info->qualifier, .type=info->type };
    unsigned current_length = (init_info->length)++;
    init_info->qualified_types = realloc(init_info->qualified_types,
                                               (current_length + 1) * sizeof (qualified_type_t));
    init_info->values = realloc(init_info->values,
                                      (current_length + 1) * sizeof (array_value_t));
    init_info->qualified_types[current_length] = qualified_type;
    init_info->values[current_length] = value;
    return true;
}

access_info_t create_access_info(entry_t *entry) {
    access_info_t new_array_access = { .entry=entry };
    return new_array_access;
}

else_if_list_t create_else_if_list(node_t *node) {
    else_if_list_t new_else_if_list = { .else_if_nodes = calloc(1, sizeof (node_t *)),
            .num_of_else_ifs = 1 };
    new_else_if_list.else_if_nodes[0] = node;
    return new_else_if_list;
}

void append_to_else_if_list(else_if_list_t *else_if_list, node_t *node) {
    unsigned current_num_of_else_ifs = (else_if_list->num_of_else_ifs)++;
    else_if_list->else_if_nodes = realloc(else_if_list->else_if_nodes,
                                          (current_num_of_else_ifs + 1) * sizeof (node_t *));
    else_if_list->else_if_nodes[current_num_of_else_ifs] = node;
}

case_list_t create_case_list(node_t *node) {
    case_list_t new_case_list = { .case_nodes = calloc(1, sizeof (node_t *)), .num_of_cases = 1 };
    new_case_list.case_nodes[0] = node;
    return new_case_list;
}

void append_to_case_list(case_list_t *case_list, node_t *node) {
    unsigned current_num_of_cases = (case_list->num_of_cases)++;
    case_list->case_nodes = realloc(case_list->case_nodes, (current_num_of_cases + 1) * sizeof (node_t *));
    case_list->case_nodes[current_num_of_cases] = node;
}

arg_list_t create_arg_list(node_t *node) {
    arg_list_t new_arg_list = { .args = calloc(1, sizeof (node_t *)), .num_of_args = 1 };
    new_arg_list.args[0] = node;
    return new_arg_list;
}

void append_to_arg_list(arg_list_t *arg_list, node_t *node) {
    unsigned current_num_of_args = (arg_list->num_of_args)++;
    arg_list->args = realloc(arg_list->args, (current_num_of_args) * sizeof (node_t *));
    arg_list->args[current_num_of_args] = node;
}
