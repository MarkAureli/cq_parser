#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
#include <__stddef_null.h>
#endif
#include "ast.h"

char *op_type_to_string(op_type_t op_type) {
    switch (op_type) {
        case LOGICAL_OP: {
            return "LOGICAL_OP";
        }
        case COMPARISON_OP: {
            return "COMPARISON_OP";
        }
        case EQUALITY_OP: {
            return "EQUALITY_OP";
        }
        case NOT_OP: {
            return "NOT_OP";
        }
        case INTEGER_OP: {
            return "INTEGER_OP";
        }
        case INVERT_OP: {
            return "INVERT_OP";
        }
    }
}

char *logical_op_to_str(logical_op_t logical_op) {
    switch (logical_op) {
        case LAND_OP: {
            return "&&";
        }
        case LOR_OP: {
            return "||";
        }
        case LXOR_OP: {
            return "^^";
        }
    }
}

char *comparison_op_to_str(comparison_op_t comparison_op) {
    switch (comparison_op) {
        case GE_OP: {
            return ">";
        }
        case GEQ_OP: {
            return ">=";
        }
        case LE_OP: {
            return "<";
        }
        case LEQ_OP: {
            return "<=";
        }
    }
}

char *equality_op_to_str(equality_op_t equality_op) {
    switch (equality_op) {
        case EQ_OP: {
            return "==";
        }
        case NEQ_OP: {
            return "!=";
        }
    }
}

char *integer_op_to_str(integer_op_t integer_op) {
    switch (integer_op) {
        case ADD_OP: {
            return "+";
        }
        case AND_OP: {
            return "&";
        }
        case DIV_OP: {
            return "/";
        }
        case MOD_OP: {
            return "%";
        }
        case MUL_OP: {
            return "*";
        }
        case OR_OP: {
            return "|";
        }
        case SUB_OP: {
            return "-";
        }
        case XOR_OP: {
            return "^";
        }
    }
}

char *assign_op_to_str(assign_op_t assign_op) {
    switch (assign_op) {
        case ASSIGN_OP: {
            return "=";
        }
        case ASSIGN_OR_OP: {
            return "|=";
        }
        case ASSIGN_XOR_OP: {
            return "^=";
        }
        case ASSIGN_AND_OP: {
            return "&=";
        }
        case ASSIGN_ADD_OP: {
            return "+=";
        }
        case ASSIGN_SUB_OP: {
            return "-=";
        }
        case ASSIGN_MUL_OP: {
            return "*=";
        }
        case ASSIGN_DIV_OP: {
            return "/=";
        }
        case ASSIGN_MOD_OP: {
            return "%=";
        }
    }
}

void logical_op_application(logical_op_t op, value_t *out, value_t in_1, value_t in_2) {
    switch (op) {
        case LOR_OP: {
            out->bval = in_1.bval || in_2.bval;
            break;
        }
        case LXOR_OP: {
            out->bval = in_1.bval && !in_2.bval || !in_1.bval && in_2.bval;
            break;
        }
        case LAND_OP: {
            out->bval = in_1.bval && in_2.bval;
            break;
        }
    }
}

void comparison_op_application(comparison_op_t op, value_t *out, type_t in_type_1,
                               value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    switch (op) {
        case GE_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->bval = in_value_1.ival > in_value_2.ival;
            } else {
                out->bval = in_value_1.uval > in_value_2.uval;
            }
            break;
        }
        case GEQ_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->bval = in_value_1.ival >= in_value_2.ival;
            } else {
                out->bval = in_value_1.ival >= in_value_2.uval;
            }
            break;
        }
        case LE_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->bval = in_value_1.ival < in_value_2.ival;
            } else {
                out->bval = in_value_1.ival < in_value_2.uval;
            }
            break;
        }
        case LEQ_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->bval = in_value_1.ival >= in_value_2.ival;
            } else {
                out->bval = in_value_1.ival >= in_value_2.uval;
            }
            break;
        }
    }
}

void apply_equality_op(equality_op_t op, value_t *out, type_t in_type_1,
                       value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    if (in_type_1 == BOOL_T) {
        if (op == EQ_OP) {
            out->bval = in_value_1.bval == in_value_2.bval;
        } else {
            out->bval = in_value_1.bval != in_value_2.bval;
        }
    } else if (in_type_1 == INT_T && in_type_2 == INT_T) {
        if (op == EQ_OP) {
            out->bval = in_value_1.ival == in_value_2.ival;
        } else {
            out->bval = in_value_1.ival != in_value_2.ival;
        }
    } else {
        if (op == EQ_OP) {
            out->bval = in_value_1.uval == in_value_2.uval;
        } else {
            out->bval = in_value_1.uval != in_value_2.uval;
        }
    }
}

int integer_op_application(integer_op_t op, value_t *out, type_t in_type_1,
                           value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    if (in_type_1 == INT_T && in_type_2 == INT_T) {
        switch (op) {
            case ADD_OP: {
                out->ival = in_value_1.ival + in_value_2.ival;
                break;
            }
            case AND_OP: {
                out->ival = in_value_1.ival & in_value_2.ival;
                break;
            }
            case DIV_OP: {
                if (in_value_2.ival == 0) {
                    return 1;
                }
                out->ival = in_value_1.ival / in_value_2.ival;
                break;
            }
            case MOD_OP: {
                if (in_value_2.ival == 0) {
                    return 2;
                }
                out->ival = in_value_1.ival % in_value_2.ival;
                break;
            }
            case MUL_OP: {
                out->ival = in_value_1.ival * in_value_2.ival;
                break;
            }
            case OR_OP: {
                out->ival = in_value_1.ival | in_value_2.ival;
                break;
            }
            case SUB_OP: {
                out->ival = in_value_1.ival - in_value_2.ival;
                break;
            }
            case XOR_OP: {
                out->ival = in_value_1.ival ^ in_value_2.ival;
                break;
            }
        }
    } else {
        switch (op) {
            case ADD_OP: {
                out->uval = in_value_1.uval + in_value_2.uval;
                break;
            }
            case AND_OP: {
                out->uval = in_value_1.uval & in_value_2.uval;
                break;
            }
            case DIV_OP: {
                if (in_value_2.uval == 0) {
                    return 1;
                }
                out->uval = in_value_1.uval / in_value_2.uval;
                break;
            }
            case MOD_OP: {
                if (in_value_2.uval == 0) {
                    return 2;
                }
                out->uval = in_value_1.uval % in_value_2.uval;
                break;
            }
            case MUL_OP: {
                out->uval = in_value_1.uval * in_value_2.uval;
                break;
            }
            case OR_OP: {
                out->uval = in_value_1.uval | in_value_2.uval;
                break;
            }
            case SUB_OP: {
                out->uval = in_value_1.uval - in_value_2.uval;
                break;
            }
            case XOR_OP: {
                out->uval = in_value_1.uval ^ in_value_2.uval;
                break;
            }
        }
    }
    return 0;
}

unsigned get_length_of_array(const unsigned sizes[MAXARRAYDEPTH], unsigned depth) {
    unsigned result = 1;
    for (unsigned i = 0; i < depth; ++i) {
        result *= sizes[i];
    }
    return result;
}

void print_array(type_t type, const value_t *values, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                 unsigned current_depth, unsigned index) {
    if (current_depth == depth - 1) {
        putchar('{');
        for (unsigned i = 0; i < sizes[current_depth]; ++i) {
            switch (type) {
                case BOOL_T: {
                    printf("%s", (values[index + i].bval) ? "true" : "false");
                    break;
                }
                case INT_T: {
                    printf("%d", values[index + i].ival);
                    break;
                }
                case UNSIGNED_T: {
                    printf("%u", values[index + i].uval);
                    break;
                }
                case VOID_T: {
                    printf("undefined");
                    break;
                }
            }
            if (i < sizes[current_depth] - 1) {
                printf(", ");
            }
        }
    } else {
        putchar('{');
        for (unsigned i = 0; i < sizes[current_depth]; ++i) {
            unsigned stride = 1;
            for (unsigned j = current_depth + 1; j < depth; ++j) {
                stride *= sizes[j];
            }
            print_array(type, values, sizes, depth, current_depth + 1, index + i * stride);
            if (i < sizes[current_depth] - 1) {
                printf(", ");
            }
        }
        putchar('}');
    }
    putchar('\n');
}

value_t *get_reduced_array(const value_t *values, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                          const unsigned indices[MAXARRAYDEPTH], unsigned index_depth) {
    // Calculate the length of the output values
    unsigned out_length = 1;
    for (unsigned i = index_depth; i < depth; ++i) {
        out_length *= sizes[i];
    }

    unsigned reduced_index = 0;
    for (unsigned i = 0; i < index_depth; ++i) {
        unsigned factor = 1;
        for (unsigned j = i + 1; j <depth; ++j) {
            factor *= sizes[j];
        }
        reduced_index += factor * indices[i];
    }

    // Allocate memory for the output values
    value_t* output = malloc( out_length * sizeof (value_t));
    memcpy(output, values + reduced_index, out_length * sizeof (value_t));
    return output;
}

node_t *new_node(node_type_t type, node_t *left, node_t *right) {
    node_t *new_node = calloc(1, sizeof (node_t));
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

node_t *new_stmt_list_node(node_t *stmt) {
    stmt_list_node_t *new_node = calloc(1, sizeof (stmt_list_node_t));
    new_node->type = STMT_LIST_NODE_T;
    new_node->is_unitary = stmt_is_unitary(stmt);
    new_node->stmt_list = calloc(1, sizeof (node_t *));
    new_node->stmt_list[0] = stmt;
    new_node->num_of_stmt = 1;
    return (node_t *) new_node;
}

void append_to_stmt_list(node_t *stmt_list_node, node_t *stmt) {
    stmt_list_node_t *stmt_list_node_view = (stmt_list_node_t *) stmt_list_node;
    unsigned current_num_of_stmt = (stmt_list_node_view->num_of_stmt)++;
    stmt_list_node_view->stmt_list = realloc(stmt_list_node_view->stmt_list,
                                             (current_num_of_stmt + 1) * sizeof (node_t *));
    stmt_list_node_view->stmt_list[current_num_of_stmt] = stmt;
    stmt_list_node_view->is_unitary = stmt_is_unitary(stmt);
}

node_t *new_func_decl_node(list_t *entry) {
    func_decl_node_t *new_node = calloc(1, sizeof (func_decl_node_t));
    new_node->type = FUNC_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_sp_node(list_t *entry) {
    func_decl_node_t *new_node = calloc(1, sizeof (func_sp_node_t));
    new_node->type = FUNC_SP_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_decl_node(list_t *entry) {
    var_decl_node_t *new_node = calloc(1, sizeof (var_decl_node_t));
    new_node->type = VAR_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_def_node(list_t *entry, bool is_init_list, node_t *node, array_values_info_t array_values_info) {
    var_def_node_t *new_node = calloc(1, sizeof (var_def_node_t));
    new_node->type = VAR_DEF_NODE_T;
    new_node->entry = entry;
    new_node->is_init_list = is_init_list;
    if (is_init_list) {
        new_node->array_values_info = array_values_info;
    } else {
        new_node->node = node;
    }
    return (node_t *) new_node;
}

node_t *new_var_def_node_from_node(list_t *entry, node_t *node) {
    var_def_node_t *new_node = calloc(1, sizeof (var_def_node_t));
    new_node->type = VAR_DEF_NODE_T;
    new_node->entry = entry;
    new_node->is_init_list = false;
    new_node->node = node;
    return (node_t *) new_node;
}

node_t *new_var_def_node_from_init_list(list_t *entry, bool *value_is_const, array_value_t *values) {
    var_def_node_t *new_node = calloc(1, sizeof (var_def_node_t));
    new_node->type = VAR_DEF_NODE_T;
    new_node->entry = entry;
    new_node->is_init_list = true;
    new_node->array_values_info.value_is_const = value_is_const;
    new_node->array_values_info.values = values;
    return (node_t *) new_node;
}

node_t *new_const_node(type_t type, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, value_t *values) {
    const_node_t *new_node = calloc(1, sizeof (const_node_t));
    new_node->type = CONST_NODE_T;
    new_node->type_info.qualifier = CONST_T;
    new_node->type_info.type = type;
    if (sizes != NULL) {
        memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    }
    new_node->type_info.depth = depth;
    new_node->values = values;
    return (node_t *) new_node;
}

node_t *new_reference_node(const unsigned sizes[MAXARRAYDEPTH], unsigned depth, bool index_is_const[MAXARRAYDEPTH],
                           index_t indices[MAXARRAYDEPTH], list_t *entry) {
    reference_node_t *new_node = calloc(1, sizeof (reference_node_t));
    new_node->type = REFERENCE_NODE_T;
    new_node->type_info.qualifier = entry->type_info.qualifier;
    new_node->type_info.type = entry->type_info.type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    memcpy(new_node->index_is_const, index_is_const, entry->type_info.depth * sizeof(bool));
    memcpy(new_node->indices, indices, entry->type_info.depth * sizeof(index_t));
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars) {
    if (!entry->is_function) {
        return NULL;
    }
    func_call_node_t *new_node = calloc(1, sizeof (func_call_node_t));
    new_node->type = FUNC_CALL_NODE_T;
    new_node->type_info.qualifier = entry->type_info.qualifier;
    new_node->type_info.type = entry->type_info.type;
    new_node->entry = entry;
    new_node->inverse = false;
    new_node->pars = pars;
    new_node->num_of_pars = num_of_pars;
    return (node_t *) new_node;
}

node_t *new_logical_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, logical_op_t op,
                            node_t *left, node_t *right) {
    logical_op_node_t *new_node = calloc(1, sizeof (logical_op_node_t));
    new_node->type = LOGICAL_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = BOOL_T;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_comparison_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                               comparison_op_t op, node_t *left, node_t *right) {
    comparison_op_node_t *new_node = calloc(1, sizeof (comparison_op_node_t));
    new_node->type = COMPARISON_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = BOOL_T;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_equality_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                             equality_op_t op, node_t *left, node_t *right) {
    equality_op_node_t *new_node = calloc(1, sizeof (equality_op_node_t));
    new_node->type = EQUALITY_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = BOOL_T;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_not_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, node_t *child) {
    not_op_node_t *new_node = calloc(1, sizeof (not_op_node_t));
    new_node->type = NOT_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = BOOL_T;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_integer_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH],
                            unsigned depth, integer_op_t op, node_t *left, node_t *right) {
    integer_op_node_t *new_node = calloc(1, sizeof (integer_op_node_t));
    new_node->type = INTEGER_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_invert_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH],
                           unsigned depth, node_t *child) {
    invert_op_node_t *new_node = calloc(1, sizeof(invert_op_node_t));
    new_node->type = INVERT_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof(unsigned));
    new_node->type_info.depth = depth;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned num_of_else_ifs,
                    node_t *else_branch) {
    if_node_t *new_node = calloc(1, sizeof (if_node_t));
    new_node->type = IF_NODE_T;
    new_node->condition = condition;
    new_node->if_branch = if_branch;
    new_node->else_if_branches = elseif_branches;
    new_node->num_of_else_ifs = num_of_else_ifs;
    new_node->else_branch = else_branch;
    return (node_t *) new_node;
}

node_t *new_else_if_node(node_t *condition, node_t *elseif_branch) {
    else_if_node_t *new_node = calloc(1, sizeof (else_if_node_t));
    new_node->type = ELSE_IF_NODE_T;
    new_node->condition = condition;
    new_node->else_if_branch = elseif_branch;
    return (node_t *) new_node;
}

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch) {
    for_node_t *new_node = calloc(1, sizeof (for_node_t));
    new_node->type = FOR_NODE_T;
    new_node->initialize = initialize;
    new_node->condition = condition;
    new_node->increment = increment;
    new_node->for_branch = for_branch;
    return (node_t *) new_node;
}

node_t *new_do_node(node_t *do_branch, node_t *condition) {
    do_node_t *new_node = calloc(1, sizeof (do_node_t));
    new_node->type = DO_NODE_T;
    new_node->do_branch = do_branch;
    new_node->condition = condition;
    return (node_t *) new_node;
}

node_t *new_while_node(node_t *condition, node_t *while_branch) {
    while_node_t *new_node = calloc(1, sizeof (while_node_t));
    new_node->type = WHILE_NODE_T;
    new_node->condition = condition;
    new_node->while_branch = while_branch;
    return (node_t *) new_node;
}

node_t *new_assign_node(assign_op_t op, node_t *left, node_t *right) {
    assign_node_t *new_node = calloc(1, sizeof (assign_node_t));
    new_node->type = ASSIGN_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_jump_node(int statement_type) {
    jump_node_t *new_node = calloc(1, sizeof (jump_node_t));
    new_node->type = JUMP_NODE_T;
    new_node->statement_type = statement_type;
    return (node_t *) new_node;
}

node_t *new_return_node(type_t ret_type, node_t *ret_val) {
    return_node_t *new_node = calloc(1, sizeof (return_node_t));
    new_node->type = RETURN_NODE_T;
    new_node->ret_type = ret_type;
    new_node->ret_val = ret_val;
    return (node_t *) new_node;
}

type_info_t create_type_info(type_t type, unsigned depth) {
    type_info_t new_type_info = { .type = type, .depth = depth};
    return new_type_info;
}

init_info_t *new_init_info_from_node(node_t *node) {
    init_info_t *new_infos = calloc(1, sizeof (init_info_t));
    new_infos->is_init_list = false;
    new_infos->node = node;
    return new_infos;
}

init_info_t *new_init_info_from_init_list(qualified_type_t qualified_type, array_value_t value) {
    init_info_t *new_infos = calloc(1, sizeof (init_info_t));
    new_infos->is_init_list = true;
    new_infos->init_list.qualified_types = calloc(1, sizeof (qualified_type_t));
    new_infos->init_list.qualified_types[0] = qualified_type;
    new_infos->init_list.values = calloc(1, sizeof (array_value_t));
    new_infos->init_list.values[0] = value;
    new_infos->init_list.length = 1;
    return new_infos;
}

void append_to_init_info(init_info_t *array_init_info, qualified_type_t qualified_type, array_value_t value) {
    unsigned current_length = (array_init_info->init_list.length)++;
    array_init_info->init_list.qualified_types = realloc(array_init_info->init_list.qualified_types,
                                                               (current_length + 1)
                                                               * sizeof (qualified_type_t));
    array_init_info->init_list.values = realloc(array_init_info->init_list.values,
                                                      (current_length + 1)
                                                      * sizeof (array_value_t));
    array_init_info->init_list.qualified_types[current_length] = qualified_type;
    array_init_info->init_list.values[current_length] = value;
}

bool stmt_is_unitary(const node_t *node) {
    switch (node->type) {
        case VAR_DECL_NODE_T: case VAR_DEF_NODE_T: case ASSIGN_NODE_T: {
            return get_type_info_of_node(node)->type == QUANTUM_T;
        }
        case FUNC_CALL_NODE_T: {
            return ((func_call_node_t *) node)->entry->func_info.is_unitary;
        }
        default: {
            return false;
        }
    }
}

access_info_t create_access_info(list_t *entry) {
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

type_info_t *get_type_info_of_node(const node_t *node) {
    switch (node->type) {
        case VAR_DECL_NODE_T: {
            return &(((var_decl_node_t *) node)->entry->type_info);
        }
        case VAR_DEF_NODE_T: {
            return &(((var_def_node_t *) node)->entry->type_info);
        }
        case CONST_NODE_T: {
            return &(((const_node_t *) node)->type_info);
        }
        case FUNC_SP_NODE_T: {
            return &(((func_sp_node_t *) node)->entry->type_info);
        }
        case REFERENCE_NODE_T: {
            return &(((reference_node_t *) node)->type_info);
        }
        case ASSIGN_NODE_T: {
            assign_node_t *assign_node_view = ((assign_node_t *) node);
            switch (assign_node_view->left->type) {
                case CONST_NODE_T: {
                    return &((const_node_t *) assign_node_view->left)->type_info;
                }
                case REFERENCE_NODE_T: {
                    return &((reference_node_t *) assign_node_view->left)->type_info;
                }
                default: {
                    return NULL;
                }
            }
        }
        case LOGICAL_OP_NODE_T: {
            return &(((logical_op_node_t *) node)->type_info);
        }
        case COMPARISON_OP_NODE_T: {
            return &(((comparison_op_node_t *) node)->type_info);
        }
        case EQUALITY_OP_NODE_T: {
            return &(((equality_op_node_t *) node)->type_info);
        }
        case NOT_OP_NODE_T: {
            return &(((not_op_node_t *) node)->type_info);
        }
        case INTEGER_OP_NODE_T: {
            return &(((integer_op_node_t *) node)->type_info);
        }
        case INVERT_OP_NODE_T: {
            return &(((invert_op_node_t *) node)->type_info);
        }
        case FUNC_CALL_NODE_T: {
            return &(((func_call_node_t *) node)->type_info);
        }
        default: {
            return NULL;
        }
    }
}

qualifier_t propagate_qualifier(qualifier_t qualifier_1, qualifier_t qualifier_2) {
    if (qualifier_1 == CONST_T && qualifier_2 == CONST_T) {
        return CONST_T;
    } else if (qualifier_1 == QUANTUM_T || qualifier_2 == QUANTUM_T) {
        return QUANTUM_T;
    } else {
        return NONE_T;
    }
}

type_t propagate_type(op_type_t op_type, type_t type_1, type_t type_2) {
    switch (op_type) {
        case LOGICAL_OP: {
            if (type_1 == BOOL_T && type_2 == BOOL_T) {
                return BOOL_T;
            } else {
                return VOID_T;
            }
        }
        case COMPARISON_OP: {
            if ((type_1 == INT_T || type_1 == UNSIGNED_T) && (type_2 == INT_T || type_2 == UNSIGNED_T)) {
                return BOOL_T;
            } else {
                return VOID_T;
            }
        }
        case EQUALITY_OP: {
            switch (type_1) {
                case BOOL_T: {
                    if (type_2 == BOOL_T) {
                        return BOOL_T;
                    } else {
                        return VOID_T;
                    }
                }
                case INT_T: case UNSIGNED_T: {
                    if (type_2 == INT_T || type_2 == UNSIGNED_T) {
                        return BOOL_T;
                    } else {
                        return VOID_T;
                    }
                }
                default: {
                    return VOID_T;
                }
            }
        }
        case NOT_OP: {
            if (type_1 == BOOL_T) {
                return BOOL_T;
            } else {
                return VOID_T;
            }
        }
        case INTEGER_OP: {
            switch (type_1) {
                case INT_T: {
                    switch (type_2) {
                        case INT_T: {
                            return INT_T;
                        }
                        case UNSIGNED_T: {
                            return UNSIGNED_T;
                        }
                        default:
                            return VOID_T;
                    }
                }
                case UNSIGNED_T: {
                    switch (type_2) {
                        case INT_T: case UNSIGNED_T: {
                            return UNSIGNED_T;
                        }
                        default:
                            return VOID_T;
                    }
                }
                default: {
                    return VOID_T;
                }
            }
        }
        case INVERT_OP: {
            switch (type_1) {
                case INT_T: {
                    return INT_T;
                }
                case UNSIGNED_T: {
                    return UNSIGNED_T;
                }
                default: {
                    return VOID_T;
                }
            }
        }
    }
}

bool are_matching_types(type_t type_1, type_t type_2) {
    switch (type_1) {
        case VOID_T: {
            return false;
        }
        case BOOL_T: {
            if (type_2 != BOOL_T) {
                return false;
            }
            break;
        }
        case INT_T: {
            if (type_2 != INT_T) {
                return false;
            }
            break;
        }
        case UNSIGNED_T: {
            if (type_2 != INT_T && type_2 != UNSIGNED_T) {
                return false;
            }
            break;
        }
    }
    return true;
}

node_t *build_func_sp_node(list_t *entry, char error_msg[ERRORMSGLENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERRORMSGLENGTH, "%s is not a function", entry->name);
        return NULL;
    } else if (entry->type_info.qualifier != NONE_T || entry->type_info.type != BOOL_T
               || entry->type_info.depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Function %s does not return a single classical bool", entry->name);
        return NULL;
    }
    node_t *result = new_func_sp_node(entry);
    return result;
}

node_t *build_var_def_node(list_t *entry, init_info_t *init_info, char error_msg[ERRORMSGLENGTH]) {
    bool *value_is_const;
    if (init_info->is_init_list) {
        if (entry->type_info.depth == 0) {
            snprintf(error_msg, ERRORMSGLENGTH, "%s is not an array, but is initialized as such", entry->name);
            return NULL;
        } else if (init_info->init_list.length > entry->length) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Too many (%u) elements initialized for array %s of total length %u",
                     init_info->init_list.length, entry->name, entry->length);
            return NULL;
        }

        value_is_const = calloc(entry->length, sizeof (bool));
        for (unsigned i = 0; i < init_info->init_list.length; ++i) {
            if (entry->type_info.qualifier == QUANTUM_T && init_info->init_list.qualified_types[i].qualifier != CONST_T
                && init_info->init_list.values[i].node_value->type == FUNC_SP_NODE_T) {
                func_info_t func_info = ((func_sp_node_t *) init_info->init_list.values[i].node_value)->entry->func_info;
                if (func_info.num_of_pars != 1) {
                    free(value_is_const);
                    snprintf(error_msg, ERRORMSGLENGTH,
                             "Element %u: Quantizable function %s must take exactly 1 parameter",
                             i, ((func_sp_node_t *) init_info->init_list.values[i].node_value)->entry->name);
                    return NULL;
                } else if (func_info.pars_type_info[0].qualifier != NONE_T) {
                    free(value_is_const);
                    snprintf(error_msg, ERRORMSGLENGTH,
                             "Element %u: Quantizable function %s must take a classical parameter",
                             i, ((func_sp_node_t *) init_info->init_list.values[i].node_value)->entry->name);
                    return NULL;
                } else if (func_info.pars_type_info[0].type != entry->type_info.type) {
                    free(value_is_const);
                    snprintf(error_msg, ERRORMSGLENGTH,
                             "Element %u: Quantizable function %s takes %s instead of %s",
                             i, ((func_sp_node_t *) init_info->init_list.values[i].node_value)->entry->name,
                             type_to_str(func_info.pars_type_info[0].type), type_to_str(entry->type_info.type));
                    return NULL;
                } else if (func_info.pars_type_info[0].depth != 0) {
                    free(value_is_const);
                    snprintf(error_msg, ERRORMSGLENGTH,
                             "Element %u: Quantizable function %s takes an array of depth %u instead of a scalar",
                             i, ((func_sp_node_t *) init_info->init_list.values[i].node_value)->entry->name,
                             func_info.pars_type_info[0].depth);
                    return NULL;
                }
            } else if (entry->type_info.qualifier != QUANTUM_T
                       && init_info->init_list.qualified_types[i].qualifier != CONST_T
                       && init_info->init_list.values[i].node_value->type == FUNC_SP_NODE_T) {
                free(value_is_const);
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Element %u in initialization of classical array %s is a superposition instruction",
                         i, entry->name);
                return NULL;
            } else if (entry->type_info.qualifier != QUANTUM_T
                       && init_info->init_list.qualified_types[i].qualifier == QUANTUM_T) {
                free(value_is_const);
                snprintf(error_msg, ERRORMSGLENGTH, "Element %u in initialization of classical array %s is quantum",
                         i, entry->name);
                return NULL;
            } else if (entry->type_info.qualifier == CONST_T
                       && init_info->init_list.qualified_types[i].qualifier != CONST_T) {
                free(value_is_const);
                snprintf(error_msg, ERRORMSGLENGTH, "Element %u in initialization of constant array %s is not constant",
                         i, entry->name);
                return NULL;
            } else if (!are_matching_types(entry->type_info.type,
                                           init_info->init_list.qualified_types[i].type)) {
                free(value_is_const);
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Element %u in initialization of %s-array %s is of type %s",
                         i, type_to_str(entry->type_info.type), entry->name,
                         type_to_str(init_info->init_list.qualified_types[i].type));
                return NULL;
            }
            if (init_info->init_list.qualified_types[i].qualifier == CONST_T) {
                value_is_const[i] = true;
            }
        }
        for (unsigned i = init_info->init_list.length; i < entry->length; ++i) {
            value_is_const[i] = true;
        }
    } else { /* no initializer list */
        value_is_const = NULL;
        type_info_t *type_info = get_type_info_of_node(init_info->node);
        if (entry->type_info.qualifier == QUANTUM_T && init_info->node->type == FUNC_SP_NODE_T) {
            func_info_t func_info = ((func_sp_node_t *) init_info->node)->entry->func_info;
            if (func_info.num_of_pars != 1) {
                snprintf(error_msg, ERRORMSGLENGTH, "Quantizable function %s must take exactly 1 parameter",
                         ((func_sp_node_t *) init_info->node)->entry->name);
                return NULL;
            } else if (func_info.pars_type_info[0].qualifier != NONE_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Quantizable function %s must take a classical parameter",
                         ((func_sp_node_t *) init_info->node)->entry->name);
                return NULL;
            } else if (func_info.pars_type_info[0].type != entry->type_info.type) {
                snprintf(error_msg, ERRORMSGLENGTH, "Quantizable function %s takes %s instead of %s",
                         ((func_sp_node_t *) init_info->node)->entry->name,
                         type_to_str(func_info.pars_type_info[0].type), type_to_str(entry->type_info.type));
                return NULL;
            } else if (func_info.pars_type_info[0].depth != 0) {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Quantizable function %s takes an array of depth %u instead of a scalar",
                         ((func_sp_node_t *) init_info->node)->entry->name, func_info.pars_type_info[0].depth);
                return NULL;
            }
        } else if (entry->type_info.qualifier != QUANTUM_T && init_info->node->type == FUNC_SP_NODE_T) {
            snprintf(error_msg, ERRORMSGLENGTH, "Classical variable %s cannot be initialized in superposition",
                     entry->name);
        } else if (entry->type_info.depth == 0 && type_info->depth != 0) {
            snprintf(error_msg, ERRORMSGLENGTH, "%s is not an array, but is initialized as such", entry->name);
            return NULL;
        } else if (entry->type_info.depth != type_info->depth) {
            snprintf(error_msg, ERRORMSGLENGTH, "Unmatching depths in array initialization of %s (%u != %u)",
                     entry->name, entry->type_info.depth, type_info->depth);
            return NULL;
        }

        for (unsigned i = 0; i < entry->type_info.depth; ++i) {
            if (entry->type_info.sizes[i] != type_info->sizes[i]) {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Unmatching sizes at position %u in array initialization of %s (%u != %u)",
                         i, entry->name, entry->type_info.sizes[i], type_info->sizes[i]);
                return NULL;
            }
        }

        if (entry->type_info.qualifier != QUANTUM_T && type_info->qualifier == QUANTUM_T) {
            if (entry->type_info.depth == 0) {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of classical scalar %s with quantum value",
                         entry->name);
                return NULL;
            } else {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of classical array %s with quantum array",
                         entry->name);
                return NULL;
            }
        } else if (entry->type_info.qualifier == CONST_T && type_info->qualifier != CONST_T) {
            if (entry->type_info.depth == 0) {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of constant scalar %s with non-constant value",
                         entry->name);
                return NULL;
            } else {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of constant array %s with non-constant array",
                         entry->name);
                return NULL;
            }
        } else if (init_info->node->type != FUNC_SP_NODE_T
                   && !are_matching_types(entry->type_info.type, type_info->type)) {
            if (entry->type_info.depth == 0) {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of scalar %s of type %s with value of type %s",
                         entry->name, type_to_str(entry->type_info.type), type_to_str(type_info->type));
                return NULL;
            } else {
                snprintf(error_msg, ERRORMSGLENGTH, "Initialization of %s-array %s with %s-array",
                         type_to_str(entry->type_info.type), entry->name, type_to_str(type_info->type));
                return NULL;
            }
        }
    }
    array_values_info_t array_values_info = { .value_is_const = value_is_const, .values = init_info->init_list.values};
    node_t *result = new_var_def_node(entry, init_info->is_init_list, init_info->node, array_values_info);
    return result;
}

node_t *build_if_node(node_t *condition, node_t *if_branch, node_t **else_if_branches, unsigned num_of_else_ifs,
                      node_t *else_branch, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *type_info = get_type_info_of_node(condition);
    if (type_info->type != BOOL_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "If condition must be of type bool, but is of type %s",
                 type_to_str(type_info->type));
        return NULL;
    } else if (type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "If condition must be a single bool, but is an array of depth %u",
                 type_info->depth);
        return NULL;
    }

    if (type_info->qualifier == QUANTUM_T) {
        if (!(((stmt_list_node_t *) if_branch)->is_unitary)) {
            snprintf(error_msg, ERRORMSGLENGTH, "If condition is quantum, but statements are not unitary");
            return NULL;
        } else if (else_branch != NULL && !(((stmt_list_node_t *) else_branch)->is_unitary)) {
            snprintf(error_msg, ERRORMSGLENGTH, "Else condition is quantum, but statements are not unitary");
            return NULL;
        }
    }

    for (unsigned i = 0; i < num_of_else_ifs; ++i) {
        else_if_node_t *else_if_node_view = (else_if_node_t *) (else_if_node_t *) else_if_branches[i];
        qualifier_t condition_qualifier = get_type_info_of_node((else_if_node_view)->condition)->qualifier;
        if (condition_qualifier != type_info->qualifier) {
            snprintf(error_msg, ERRORMSGLENGTH, "Else-if condition %u is %s while if condition is %s",
                     i + 1, (condition_qualifier == QUANTUM_T) ? "quantum" : "classical",
                     (type_info->qualifier == QUANTUM_T) ? "quantum" : "classical");
            return NULL;
        }
    }


    node_t *result = new_if_node(condition, if_branch, else_if_branches, num_of_else_ifs, else_branch);
    return result;
}

node_t *build_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *type_info = get_type_info_of_node(condition);
    if (type_info->type != BOOL_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Else-if condition must be of type bool, but is of type %s",
                 type_to_str(type_info->type));
        return NULL;
    } else if (type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Else-if condition must be a single bool, but is an array of depth %u",
                 type_info->depth);
        return NULL;
    } else if (type_info->qualifier == QUANTUM_T && !(((stmt_list_node_t *) else_if_branch)->is_unitary)) {
        snprintf(error_msg, ERRORMSGLENGTH, "Else-if condition is quantum, but statements are not unitary");
        return NULL;
    }

    node_t *result = new_else_if_node(condition, else_if_branch);
    return result;
}

node_t *build_do_node(node_t *do_branch, node_t *condition, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *type_info = get_type_info_of_node(condition);
    if (type_info->qualifier == QUANTUM_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Do-while-loop condition cannot be quantum");
        return NULL;
    } else if (type_info->type != BOOL_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Do-while-loop condition must be of type bool, but is of type %s",
                 type_to_str(type_info->type));
        return NULL;
    } else if (type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH,
                 "Do-while-loop condition must be a single bool, but is an array of depth %u", type_info->depth);
        return NULL;
    }

    node_t *result = new_do_node(do_branch, condition);
    return result;
}

node_t *build_while_node(node_t *condition, node_t *while_branch, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *type_info = get_type_info_of_node(condition);
    if (type_info->qualifier == QUANTUM_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "While-loop condition cannot be quantum");
        return NULL;
    } else if (type_info->type != BOOL_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "While-loop condition must be of type bool, but is of type %s",
                 type_to_str(type_info->type));
        return NULL;
    } else if (type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "While-loop condition must be a single bool, but is an array of depth %u",
                 type_info->depth);
        return NULL;
    }

    node_t *result = new_while_node(condition, while_branch);
    return result;
}

node_t *build_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                       char error_msg[ERRORMSGLENGTH]) {
    type_info_t *type_info = get_type_info_of_node(condition);
    if (type_info->qualifier == QUANTUM_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "For-loop condition cannot be quantum");
        return NULL;
    } else if (type_info->type != BOOL_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "For-loop condition must be of type bool, but is of type %s",
                 type_to_str(type_info->type));
        return NULL;
    } else if (type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "For-loop condition must be a single bool, but is an array of depth %u",
                 type_info->depth);
        return NULL;
    }

    node_t *result = new_for_node(initialize, condition, increment, for_branch);
    return result;
}

node_t *build_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    if (left->type == CONST_NODE_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Trying to reassign constant value");
        return NULL;
    } else if (left->type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Left-hand side of assignment is not a variable");
        return NULL;
    }

    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    if (right_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Right-hand side of assignment is not an expression");
        return NULL;
    }
    if (left_type_info->qualifier != QUANTUM_T && right_type_info->qualifier == QUANTUM_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Classical left-hand side of assignment, but quantum right-hand side");
        return NULL;
    }
    switch (op) {
        case ASSIGN_OP: {
            if (left_type_info->type == BOOL_T && right_type_info->type != BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Assigning %s to bool", type_to_str(right_type_info->type));
                return NULL;
            } else if (left_type_info->type == INT_T && right_type_info->type != INT_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Assigning %s to int", type_to_str(right_type_info->type));
                return NULL;
            } else if (left_type_info->type == UNSIGNED_T && right_type_info->type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Assigning bool to unsigned");
                return NULL;
            }
            break;
        }
        case ASSIGN_OR_OP: case ASSIGN_XOR_OP: case ASSIGN_AND_OP: {
            if (left_type_info->type == BOOL_T && right_type_info->type != BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Logically assigning %s to bool",
                         type_to_str(right_type_info->type));
                return NULL;
            } else if (left_type_info->type == INT_T && right_type_info->type != INT_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Bitwisely assigning %s to int",
                         type_to_str(right_type_info->type));
                return NULL;
            } else if (left_type_info->type == UNSIGNED_T && right_type_info->type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Bitwisely assigning bool to unsigned");
                return NULL;
            }
            break;
        }
        case ASSIGN_ADD_OP: case ASSIGN_SUB_OP: case ASSIGN_MUL_OP: case ASSIGN_DIV_OP: case ASSIGN_MOD_OP: {
            if (left_type_info->type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Arithmetically assigning to bool");
                return NULL;
            } else if (right_type_info->type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Arithmetically assigning bool to %s",
                         type_to_str(left_type_info->type));
                return NULL;
            } else if (left_type_info->type == INT_T && right_type_info->type == UNSIGNED_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Arithmetically assigning unsigned to int");
                return NULL;
            }
            break;
        }
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH,
                 "Left-hand side of \"%s\" is a scalar, right-hand side is an array of depth %u)",
                 assign_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH,
                 "Left-hand side of \"%s\" is an array of depth %u, right-hand side is a scalar)",
                 assign_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH,
                 "Left-hand and right-hand side of \"%s\" are arrays of different depth (%u != %u)",
                 assign_op_to_str(op), left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Left-hand and right-hand side of \"%s\" are arrays of different sizes in dimension %u (%u != %u)",
                     assign_op_to_str(op), depth, left_type_info->sizes[i], right_type_info->sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info->sizes;
    unsigned length = get_length_of_array(sizes, depth);

    if (right_type_info->qualifier == CONST_T) { /* right is of node_type CONST_NODE_T */
        const_node_t *const_node_view_right = (const_node_t *) right;

        if (op == ASSIGN_DIV_OP) {
            for (unsigned i = 0; i < length; ++i) {
                if (const_node_view_right->values[i].ival == 0) {
                    snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                    return NULL;
                }
            }
        } else if (op == ASSIGN_MOD_OP) {
            for (unsigned i = 0; i < length; ++i) {
                if (const_node_view_right->values[i].ival == 0) {
                    snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                    return NULL;
                }
            }
        }
    }
    node_t *result = new_assign_node(op, left, right);
    return result;
}

node_t *build_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    if (left_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is not an expression",
                 logical_op_to_str(op));
        return NULL;
    } else if (right_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is not an expression",
                 logical_op_to_str(op));
        return NULL;
    }
    /* implement error when nodes are no expression nodes */
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier,
                                                       right_type_info->qualifier);
    type_t result_type = propagate_type(LOGICAL_OP,
                                        left_type_info->type,
                                        right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to %s and %s", logical_op_to_str(op),
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to scalar and array of depth %u)",
                 logical_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to array of depth %u and scalar)",
                 logical_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to arrays of different depth (%u != %u)",
                 logical_op_to_str(op), left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Applying \"%s\" to arrays of different sizes in dimension %u (%u != %u)",
                     logical_op_to_str(op), depth, left_type_info->sizes[i], right_type_info->sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info->sizes;
    unsigned length = get_length_of_array(sizes, depth);

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            logical_op_application(op,
                                   const_node_view_result->values + i,
                                   const_node_view_left->values[i],
                                   const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_logical_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    if (left_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is not an expression",
                 comparison_op_to_str(op));
        return NULL;
    } else if (right_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is not an expression",
                 comparison_op_to_str(op));
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier,
                                                       right_type_info->qualifier);
    type_t result_type = propagate_type(COMPARISON_OP,
                                        left_type_info->type,
                                        right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of %s and %s", comparison_op_to_str(op),
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of scalar and array of depth %u)",
                 comparison_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of array of depth %u and scalar)",
                 comparison_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of arrays of different depths (%u != %u)",
                 comparison_op_to_str(op), left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "\"%s\"-comparison of arrays of different sizes in dimension %u (%u != %u)",
                     comparison_op_to_str(op), depth, left_type_info->sizes[i], right_type_info->sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info->sizes;
    unsigned length = get_length_of_array(sizes, depth);

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            comparison_op_application(op, const_node_view_result->values + i, left_type_info->type,
                                      const_node_view_left->values[i], right_type_info->type,
                                      const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_comparison_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    if (left_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Left-hand side of %sequality is not an expression",
                 (op == EQ_OP) ? "" : "in");
        return NULL;
    } else if (right_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Right-hand side of %sequality is not an expression",
                 (op == EQ_OP) ? "" : "in");
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier,
                                                       right_type_info->qualifier);
    type_t result_type = propagate_type(EQUALITY_OP,
                                        left_type_info->type,
                                        right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Checking %sequality of %s and %s", (op == EQ_OP) ? "" : "in",
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Checking %sequality of scalar and array of depth %u)",
                 (op == EQ_OP) ? "" : "in", right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Checking %sequality of array of depth %u and scalar)",
                 (op == EQ_OP) ? "" : "in", left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "Checking %sequality of arrays of different depths (%u != %u)",
                 (op == EQ_OP) ? "" : "in", left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Checking %sequality of arrays of different sizes in dimension %u (%u != %u)",
                     (op == EQ_OP) ? "" : "in", depth, left_type_info->sizes[i], right_type_info->sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info->sizes;
    unsigned length = get_length_of_array(sizes, depth);

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            apply_equality_op(op,
                              const_node_view_result->values + i,
                              left_type_info->type,
                              const_node_view_left->values[i],
                              right_type_info->type,
                              const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_equality_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_not_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *child_type_info = get_type_info_of_node(child);
    if (child_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"!\" to a non-expression");
        return NULL;
    }
    qualifier_t result_qualifier = child_type_info->qualifier;
    type_t result_type = propagate_type(NOT_OP, child_type_info->type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"!\" to expression of type %s",
                 type_to_str(child_type_info->type));
        return NULL;
    }

    node_t *result;
    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info->sizes, child_type_info->depth);
        result = child;
        const_node_t *const_node_view_child = (const_node_t *) child;
        const_node_t *const_node_view_result = (const_node_t *) result;
        for (unsigned i = 0; i < length; ++i) {
            const_node_view_result->values[i].bval = !(const_node_view_child->values[i].bval);
        }
    } else if (child->type == NOT_OP_NODE_T) {
        not_op_node_t *not_op_node_view_child = (not_op_node_t *) child;
        result = not_op_node_view_child->child;
        free(not_op_node_view_child);
    } else {
        result = new_not_op_node(result_qualifier, child_type_info->sizes, child_type_info->depth, child);
    }
    return result;
}

node_t *build_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    if (left_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is not an expression",
                 integer_op_to_str(op));
        return NULL;
    } else if (right_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is not an expression",
                 integer_op_to_str(op));
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier,
                                                       right_type_info->qualifier);
    type_t result_type = propagate_type(INTEGER_OP,
                                        left_type_info->type,
                                        right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to %s and %s", integer_op_to_str(op),
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to scalar and array of depth %u)",
                 integer_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to array of depth %u and scalar)",
                 integer_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to arrays of different depths (%u != %u)",
                 integer_op_to_str(op), left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Applying \"%s\" to arrays of different sizes in dimension %u (%u != %u)",
                     integer_op_to_str(op), depth, left_type_info->sizes[i], right_type_info->sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info->sizes;
    unsigned length = get_length_of_array(sizes, depth);

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->type_info.type = INT_T;

        for (unsigned i = 0; i < length; ++i) {
            int validity_check = integer_op_application(op,
                                                        const_node_view_result->values + i,
                                                        left_type_info->type,
                                                        const_node_view_left->values[i],
                                                        right_type_info->type,
                                                        const_node_view_right->values[i]);
            if (validity_check == 1) {
                snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                return NULL;
            } else if (validity_check == 2) {
                snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                return NULL;
            }
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_integer_op_node(result_qualifier, result_type, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_invert_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *child_type_info = get_type_info_of_node(child);
    if (child_type_info == NULL) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to a non-expression");
        return NULL;
    }
    qualifier_t result_qualifier = child_type_info->qualifier;
    type_t result_type = propagate_type(INVERT_OP, child_type_info->type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to expression of type %s",
                 type_to_str(child_type_info->type));
        return NULL;
    }

    node_t *result;
    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info->sizes, child_type_info->depth);
        result = child;
        const_node_t *const_node_view_child = (const_node_t *) child;
        const_node_t *const_node_view_result = (const_node_t *) result;
        if (result_type == INT_T) {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_result->values[i].ival = ~(const_node_view_child->values[i].ival);
            }
        } else {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_result->values[i].uval = ~(const_node_view_child->values[i].uval);
            }
        }

    } else if (child->type == INVERT_OP_NODE_T) {
        invert_op_node_t *invert_op_node_view_child = (invert_op_node_t *) child;
        result = invert_op_node_view_child->child;
        free(invert_op_node_view_child);
    } else {
        result = new_invert_op_node(result_qualifier, result_type, child_type_info->sizes,
                                    child_type_info->depth, child);
    }
    return result;
}

node_t *build_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars, char error_msg[ERRORMSGLENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERRORMSGLENGTH, "Trying to call non-function %s", entry->name);
        return NULL;
    } else if (entry->func_info.num_of_pars != num_of_pars) {
        snprintf(error_msg, ERRORMSGLENGTH, "Function %s requires %u parameter%s, but is called with %u parameter%s",
                 entry->name, entry->func_info.num_of_pars, (entry->func_info.num_of_pars == 1) ? "" : "s",
                 num_of_pars, (num_of_pars == 1) ? "" : "s");
        return NULL;
    }
    for (unsigned i = 0; i < num_of_pars; ++i) {
        type_info_t *type_info_of_par = get_type_info_of_node(pars[i]);
        if (type_info_of_par == NULL) {
            snprintf(error_msg, ERRORMSGLENGTH, "Parameter %u in call to function %s is not an expression",
                     i, entry->name);
            return NULL;
        } else if (entry->func_info.pars_type_info[i].qualifier != QUANTUM_T
                   && type_info_of_par->qualifier == QUANTUM_T) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Parameter %u in call to function %s is required to be classical, but is quantum", i, entry->name);
            return NULL;
        } else if (!are_matching_types(entry->func_info.pars_type_info[i].type, type_info_of_par->type)) {
            snprintf(error_msg, ERRORMSGLENGTH,
                     "Parameter %u in call to function %s is required to be of type %s, but is of type %s",
                     i, entry->name, type_to_str(entry->func_info.pars_type_info[i].type),
                     type_to_str(type_info_of_par->type));
            return NULL;
        } else if (entry->func_info.pars_type_info[i].depth != type_info_of_par->depth) {
            if (entry->func_info.pars_type_info[i].depth == 0) {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Parameter %u in call to function %s is required to be a scalar, but is an array of depth %u",
                         i, entry->name, type_info_of_par->depth);
                return NULL;
            } else if (type_info_of_par->depth == 0) {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Parameter %u in call to function %s is required to be an array of depth %u, but is a scalar",
                         i, entry->name, entry->func_info.pars_type_info[i].depth);
                return NULL;
            } else {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Parameter %u in call to function %s is required to be an array of depth %u, but has depth %u",
                         i, entry->name, entry->func_info.pars_type_info[i].depth, type_info_of_par->depth);
                return NULL;
            }
        }
        for (unsigned j = 0; j < type_info_of_par->depth; ++j) {
            if (entry->func_info.pars_type_info[i].sizes[j] != type_info_of_par->sizes[j]) {
                snprintf(error_msg, ERRORMSGLENGTH,
                         "Parameter %u in call to function %s has size %u instead of %u in dimension %u",
                         i, entry->name, entry->func_info.pars_type_info[i].sizes[j], type_info_of_par->sizes[j], j);
                return NULL;
            }
        }
    }

    node_t *result = new_func_call_node(entry, pars, num_of_pars);
    return result;
}

void print_type_info(const type_info_t *type_info) {
    switch (type_info->qualifier) {
        case NONE_T: {
            break;
        }
        case CONST_T: {
            printf("const ");
            break;
        }
        case QUANTUM_T: {
            printf("quantum ");
            break;
        }
    }
    printf("%s", type_to_str(type_info->type));
    for (unsigned i = 0; i < type_info->depth; ++i) {
        printf("[%u]", type_info->sizes[i]);
    }
}

void print_node(const node_t *node) {
    switch (node->type) {
        case BASIC_NODE_T: {
            printf("Basic node\n");
            break;
        }
        case STMT_LIST_NODE_T: {
            printf("Statement list node with %u statements\n", ((stmt_list_node_t *) node)->num_of_stmt);
            break;
        }
        case FUNC_SP_NODE_T: {
            printf("Function superposition node for %s\n", ((func_sp_node_t *) node)->entry->name);
            break;
        }
        case FUNC_DECL_NODE_T: {
            printf("Function declaration node for %s\n", ((func_decl_node_t *) node)->entry->name);
            break;
        }
        case VAR_DECL_NODE_T: {
            var_decl_node_t *var_decl_node_view = ((var_decl_node_t *) node);
            printf("Declaration: ");
            print_type_info(&var_decl_node_view->entry->type_info);
            printf(" %s\n", var_decl_node_view->entry->name);
            break;
        }
        case VAR_DEF_NODE_T: {
            var_def_node_t *var_def_node_view = ((var_def_node_t *) node);
            printf("Definition: ");
            print_type_info(&var_def_node_view->entry->type_info);
            printf(" %s\n", var_def_node_view->entry->name);
            break;
        }
        case CONST_NODE_T: {
            type_info_t *info = get_type_info_of_node(node);
            if (info->depth == 0) {
                switch (info->type) {
                    case BOOL_T: {
                        printf("%s\n", ((const_node_t *) node)->values[0].bval ? "true" : "false");
                        break;
                    }
                    case INT_T: {
                        printf("%d\n", ((const_node_t *) node)->values[0].ival);
                        break;
                    }
                    case UNSIGNED_T: {
                        printf("%u\n", ((const_node_t *) node)->values[0].uval);
                        break;
                    }
                    case VOID_T: {
                        printf("undefined\n");
                        break;
                    }
                }
            } else {
                print_array(info->type, ((const_node_t *) node)->values, info->sizes,
                            info->depth, 0, 0);
            }
            break;
        }
        case REFERENCE_NODE_T: {
            type_info_t *info = get_type_info_of_node(node);
            printf("reference to ");
            print_type_info(info);
            printf(" %s\n", ((reference_node_t *) node)->entry->name);
            break;
        }
        case FUNC_CALL_NODE_T: {
            printf("Function call node for %s with %u parameters\n", ((func_call_node_t *) node)->entry->name,
                   ((func_call_node_t *) node)->num_of_pars);
            break;
        }
        case LOGICAL_OP_NODE_T: {
            putchar('(');
            type_info_t *left_info = get_type_info_of_node(((logical_op_node_t *) node)->left);
            type_info_t *right_info = get_type_info_of_node(((logical_op_node_t *) node)->right);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(left_info);
            printf(") %s (", logical_op_to_str(((logical_op_node_t *) node)->op));
            print_type_info(right_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case COMPARISON_OP_NODE_T: {
            putchar('(');
            type_info_t *left_info = get_type_info_of_node(((comparison_op_node_t *) node)->left);
            type_info_t *right_info = get_type_info_of_node(((comparison_op_node_t *) node)->right);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(left_info);
            printf(") %s (", comparison_op_to_str(((comparison_op_node_t *) node)->op));
            print_type_info(right_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case EQUALITY_OP_NODE_T: {
            putchar('(');
            type_info_t *left_info = get_type_info_of_node(((equality_op_node_t *) node)->left);
            type_info_t *right_info = get_type_info_of_node(((equality_op_node_t *) node)->right);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(left_info);
            printf(") %s (", equality_op_to_str(((equality_op_node_t *) node)->op));
            print_type_info(right_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case NOT_OP_NODE_T: {
            printf("!(");
            type_info_t *child_info = get_type_info_of_node(((invert_op_node_t *) node)->child);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(child_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case INTEGER_OP_NODE_T: {
            putchar('(');
            type_info_t *left_info = get_type_info_of_node(((integer_op_node_t *) node)->left);
            type_info_t *right_info = get_type_info_of_node(((integer_op_node_t *) node)->right);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(left_info);
            printf(") %s (", integer_op_to_str(((integer_op_node_t *) node)->op));
            print_type_info(right_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case INVERT_OP_NODE_T: {
            printf("~(");
            type_info_t *child_info = get_type_info_of_node(((invert_op_node_t *) node)->child);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(child_info);
            printf(") -> (");
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case IF_NODE_T: {
            printf("If node with %u \"else if\"s\n", ((if_node_t *) node)->num_of_else_ifs);
            break;
        }
        case ELSE_IF_NODE_T: {
            printf("Else node\n");
            break;
        }
        case FOR_NODE_T: {
            printf("For node\n");
            break;
        }
        case DO_NODE_T: {
            printf("Do node\n");
            break;
        }
        case WHILE_NODE_T: {
            printf("While node\n");
            break;
        }
        case ASSIGN_NODE_T: {
            putchar('(');
            type_info_t *left_info = get_type_info_of_node(((assign_node_t *) node)->left);
            type_info_t *right_info = get_type_info_of_node(((assign_node_t *) node)->right);
            type_info_t *result_info = get_type_info_of_node(node);
            print_type_info(left_info);
            printf(") %s (", logical_op_to_str(((logical_op_node_t *) node)->op));
            print_type_info(right_info);
            printf(") %s (", assign_op_to_str(((assign_node_t *) node)->op));
            print_type_info(result_info);
            printf(")\n");
            break;
        }
        case JUMP_NODE_T: {
            printf("Jump node of statement %d\n", ((jump_node_t *) node)->statement_type);
            break;
        }
        case RETURN_NODE_T: {
            printf("Return node of return type %s\n", type_to_str(((return_node_t *) node)->ret_type));
            break;
        }
    }
}

void tree_traversal(const node_t *node) {
    if (node == NULL) {
        return;
    }
    print_node(node);
    switch (node->type) {
        case BASIC_NODE_T: {
            tree_traversal(node->left);
            tree_traversal(node->right);
            break;
        }
        case STMT_LIST_NODE_T: {
            for (unsigned i = 0; i < ((stmt_list_node_t *) node)->num_of_stmt; ++i) {
                tree_traversal(((stmt_list_node_t *) node)->stmt_list[i]);
            }
            break;
        }
        case VAR_DEF_NODE_T: {
            if (((var_def_node_t *) node)->is_init_list) {
                for (unsigned i = 0; i < get_length_of_array(((var_def_node_t *) node)->entry->type_info.sizes,
                                                             ((var_def_node_t *) node)->entry->type_info.depth); ++i) {
                    if (!(((var_def_node_t *) node)->array_values_info.value_is_const[i])) {
                        tree_traversal(((var_def_node_t *) node)->array_values_info.values[i].node_value);
                    }
                }
            } else {
                tree_traversal(((var_def_node_t *) node)->node);
            }
            break;
        }
        case INTEGER_OP_NODE_T: {
            tree_traversal(((integer_op_node_t *)node)->left);
            tree_traversal(((integer_op_node_t *)node)->right);
            break;
        }
        case LOGICAL_OP_NODE_T: {
            tree_traversal(((logical_op_node_t *)node)->left);
            tree_traversal(((logical_op_node_t *)node)->right);
            break;
        }
        case COMPARISON_OP_NODE_T: {
            tree_traversal(((comparison_op_node_t *)node)->left);
            tree_traversal(((comparison_op_node_t *)node)->right);
            break;
        }
        case EQUALITY_OP_NODE_T: {
            tree_traversal(((equality_op_node_t *)node)->left);
            tree_traversal(((equality_op_node_t *)node)->right);
            break;
        }
        case INVERT_OP_NODE_T: {
            tree_traversal(((invert_op_node_t *) node)->child);
            break;
        }
        case NOT_OP_NODE_T: {
            tree_traversal(((not_op_node_t *) node)->child);
            break;
        }
        case FUNC_CALL_NODE_T: {
            for (unsigned i = 0; i < ((func_call_node_t *) node)->num_of_pars; ++i) {
                tree_traversal(((func_call_node_t *) node)->pars[i]);
            }
            break;
        }
        case IF_NODE_T: {
            tree_traversal(((if_node_t *) node)->condition);
            tree_traversal(((if_node_t *) node)->if_branch);
            for (unsigned i = 0; i < ((if_node_t *) node)->num_of_else_ifs; ++i) {
                tree_traversal(((if_node_t *) node)->else_if_branches[i]);
            }
            tree_traversal(((if_node_t *) node)->else_branch);
            break;
        }
        case ELSE_IF_NODE_T: {
            tree_traversal(((else_if_node_t *) node)->condition);
            tree_traversal(((else_if_node_t *) node)->else_if_branch);
            break;
        }
        case FOR_NODE_T: {
            tree_traversal(((for_node_t *) node)->initialize);
            tree_traversal(((for_node_t *) node)->condition);
            tree_traversal(((for_node_t *) node)->increment);
            tree_traversal(((for_node_t *) node)->for_branch);
            break;
        }
        case DO_NODE_T: {
            tree_traversal(((do_node_t *) node)->do_branch);
            tree_traversal(((do_node_t *) node)->condition);
            break;
        }
        case WHILE_NODE_T: {
            tree_traversal(((while_node_t *) node)->condition);
            tree_traversal(((while_node_t *) node)->while_branch);
            break;
        }
        case ASSIGN_NODE_T: {
            tree_traversal(((assign_node_t *) node)->left);
            tree_traversal(((assign_node_t *) node)->right);
            break;
        }
        case RETURN_NODE_T: {
            tree_traversal(((return_node_t *) node)->ret_val);
            break;
        }
        default: {
            break;
        }
    }
}
