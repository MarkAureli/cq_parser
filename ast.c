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
            return "%%";
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

void apply_logical_op(logical_op_t op, value_t *out, value_t in_1, value_t in_2) {
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

void apply_comparison_op(comparison_op_t op, value_t *out, type_t in_type_1, value_t in_value_1, type_t in_type_2, value_t in_value_2) {
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

void apply_equality_op(equality_op_t op, value_t *out, type_t in_type_1, value_t in_value_1, type_t in_type_2, value_t in_value_2) {
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

int apply_integer_op(integer_op_t op, value_t *out, type_t in_type_1, value_t in_value_1, type_t in_type_2, value_t in_value_2) {
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

void slice_array(const value_t *in_values, const unsigned sizes[MAXARRAYDEPTH], const bool is_indexed[MAXARRAYDEPTH],
                 const unsigned indices[MAXARRAYDEPTH], value_t *out_values, unsigned *output_index,
                 unsigned current_dim, unsigned depth, unsigned current_index) {
    if (current_dim == depth) {
        out_values[(*output_index)++] = in_values[current_index];
        return;
    }

    if (!is_indexed[current_dim]) { // full axis is addressed
        for (unsigned i = 0; i < sizes[current_dim]; ++i) {
            unsigned next_index = current_index + i;
            for (unsigned j = current_dim + 1; j < depth; ++j) {
                next_index *= sizes[j];
            }
            slice_array(in_values, sizes, is_indexed, indices, out_values, output_index,
                        current_dim + 1, depth, next_index);
        }
    } else { // specific index is addressed
        unsigned next_index = current_index + indices[current_dim];
        for (unsigned j = current_dim + 1; j < depth; ++j) {
            next_index *= sizes[j];
        }
        slice_array(in_values, sizes, is_indexed, indices, out_values, output_index,
                    current_dim + 1, depth, next_index);
    }
}

value_t *get_sliced_array(const value_t *values, const unsigned sizes[MAXARRAYDEPTH],
                          const bool is_indexed[MAXARRAYDEPTH], const unsigned indices[MAXARRAYDEPTH], unsigned depth) {
    // Calculate the length of the output values
    unsigned out_length = 1;
    for (unsigned i = 0; i < depth; ++i) {
        if (indices[i] == -1) {
            out_length *= sizes[i];
        }
    }

    // Allocate memory for the output values
    value_t* output = malloc( out_length * sizeof (value_t));
    if (output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Initialize the output index
    unsigned output_index = 0;

    // Start the recursive slicing
    slice_array(values, sizes, is_indexed, indices, output, &output_index, 0, depth, 0);

    return output;
}

node_t *new_node(node_type_t type, node_t *left, node_t *right) {
    node_t *new_node = calloc(1, sizeof (node_t));
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

node_t *new_func_decl_node(list_t *entry) {
    func_decl_node_t *new_node = calloc(1, sizeof (func_decl_node_t));
    new_node->type = FUNC_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_decl_node(list_t *entry) {
    var_decl_node_t *new_node = calloc(1, sizeof (var_decl_node_t));
    new_node->type = VAR_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_def_node(list_t *entry, bool *value_is_const, array_value_t *values) {
    var_def_node_t *new_node = calloc(1, sizeof (var_def_node_t));
    new_node->type = VAR_DEF_NODE_T;
    new_node->entry = entry;
    new_node->value_is_const = value_is_const;
    new_node->values = values;
    return (node_t *) new_node;
}

node_t *new_const_node(type_t type, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, value_t *values) {
    const_node_t *new_node = calloc(1, sizeof (const_node_t));
    new_node->type = CONST_NODE_T;
    new_node->type_info.qualifier = CONST_T;
    new_node->type_info.type = type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->values = values;
    return (node_t *) new_node;
}

node_t *new_reference_node(const unsigned sizes[MAXARRAYDEPTH], unsigned depth,
                           bool is_indexed[MAXARRAYDEPTH], bool index_is_const[MAXARRAYDEPTH],
                           array_index_t indices[MAXARRAYDEPTH], list_t *entry) {
    reference_node_t *new_node = calloc(1, sizeof (reference_node_t));
    new_node->type = REFERENCE_NODE_T;
    new_node->type_info.qualifier = entry->qualifier;
    new_node->type_info.type = entry->type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned ));
    new_node->type_info.depth = depth;
    memcpy(new_node->is_indexed, is_indexed, entry->depth * sizeof(bool));
    memcpy(new_node->index_is_const, index_is_const, entry->depth * sizeof(bool));
    memcpy(new_node->indices, indices, entry->depth * sizeof(array_index_t));
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars) {
    if (!entry->is_function) {
        return NULL;
    }
    func_call_node_t *new_node = calloc(1, sizeof (func_call_node_t));
    new_node->type = FUNC_CALL_NODE_T;
    // TODO: properly set the sizes and depth when altered in reference (maybe not necessary)
    new_node->type_info.qualifier = entry->qualifier;
    new_node->type_info.type = entry->type;
    new_node->entry = entry;
    new_node->pars = pars;
    new_node->num_of_pars = num_of_pars;
    return (node_t *) new_node;
}

node_t *new_logical_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, logical_op_t op, node_t *left, node_t *right) {
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

node_t *new_comparison_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, comparison_op_t op, node_t *left, node_t *right) {
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

node_t *new_equality_op_node(qualifier_t qualifier, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, equality_op_t op, node_t *left, node_t *right) {
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

node_t *new_integer_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, integer_op_t op, node_t *left, node_t *right) {
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

node_t *new_invert_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, node_t *child) {
    invert_op_node_t *new_node = calloc(1, sizeof(invert_op_node_t));
    new_node->type = INVERT_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof(unsigned));
    new_node->type_info.depth = depth;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **elseif_branches, unsigned elseif_count, node_t *else_branch) {
    if_node_t *new_node = calloc(1, sizeof (if_node_t));
    new_node->type = IF_NODE_T;
    new_node->condition = condition;
    new_node->if_branch = if_branch;
    new_node->elseif_branches = elseif_branches;
    new_node->elseif_count = elseif_count;
    new_node->else_branch = else_branch;
    return (node_t *) new_node;
}

node_t *new_else_node(node_t *condition, node_t *elseif_branch) {
    else_node_t *new_node = calloc(1, sizeof (else_node_t));
    new_node->type = ELSE_NODE_T;
    new_node->condition = condition;
    new_node->elseif_branch = elseif_branch;
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

node_t *new_assign_node(list_t *entry, node_t *assign_val) {
    assign_node_t *new_node = calloc(1, sizeof (assign_node_t));
    new_node->type = ASSIGN_NODE_T;
    new_node->entry = entry;
    new_node->assign_val = assign_val;
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

type_info_t type_info_init(type_t type, unsigned depth) {
    type_info_t new_type_info = { .type = type, .depth = depth};
    return new_type_info;
}

array_var_infos_t *new_array_var_infos(bool value_is_const, array_value_t value) {
    array_var_infos_t *new_infos = calloc(1, sizeof (array_var_infos_t));
    new_infos->is_array_init = false;
    new_infos->value_is_const = calloc(1, sizeof (bool));
    new_infos->value_is_const[0] = value_is_const;
    new_infos->values = calloc(1, sizeof (array_value_t));
    new_infos->values[0] = value;
    new_infos->length = 1;
    return new_infos;
}

void append_to_array_var_infos(array_var_infos_t *array_var_infos, bool value_is_const,
                               array_value_t value) {
    array_var_infos->value_is_const = realloc(array_var_infos->value_is_const,
                                          ++(array_var_infos->length) * sizeof (bool));
    array_var_infos->values = realloc(array_var_infos->values,
                                      array_var_infos->length * sizeof (array_value_t));
}

array_access_info_t array_access_info_init(list_t *entry) {
    array_access_info_t new_array_access = { .entry=entry };
    return new_array_access;
}

type_info_t *get_type_info_of_node(const node_t *node) {
    switch (node->type) {
        case CONST_NODE_T: {
            return &(((const_node_t *) node)->type_info);
        }
        case REFERENCE_NODE_T: {
            return &(((reference_node_t *) node)->type_info);
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

node_t *build_logical_op_node(logical_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    /* implement error when nodes are no expression nodes */
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier, right_type_info->qualifier);
    type_t result_type = propagate_type(LOGICAL_OP, left_type_info->type, right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to %s and %s", logical_op_to_str(op),
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to scalar and array of depth %u)", logical_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to array of depth %u and scalar)", logical_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to arrays of different depth (%u != %u)", logical_op_to_str(op),
                 left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH, "Applying \"%s\" to arrays of different sizes in dimension %u (%u != %u)", logical_op_to_str(op),
                     depth, left_type_info->sizes[i], right_type_info->sizes[i]);
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
            apply_logical_op(op, const_node_view_result->values + i, const_node_view_left->values[i], const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_logical_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_comparison_op_node(comparison_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    /* implement error when nodes are no expression nodes */
    qualifier_t result_qualifier = propagate_qualifier(left_type_info->qualifier, right_type_info->qualifier);
    type_t result_type = propagate_type(COMPARISON_OP, left_type_info->type, right_type_info->type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of %s and %s", comparison_op_to_str(op),
                 type_to_str(left_type_info->type), type_to_str(right_type_info->type));
        return NULL;
    }

    if (left_type_info->depth == 0 && right_type_info->depth != 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of scalar and array of depth %u)", comparison_op_to_str(op), right_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != 0 && right_type_info->depth == 0) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of array of depth %u and scalar)", comparison_op_to_str(op), left_type_info->depth);
        return NULL;
    } else if (left_type_info->depth != right_type_info->depth) {
        snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of arrays of different depths (%u != %u)", comparison_op_to_str(op),
                 left_type_info->depth, right_type_info->depth);
        return NULL;
    }
    unsigned depth = left_type_info->depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info->sizes[i] != right_type_info->sizes[i]) {
            snprintf(error_msg, ERRORMSGLENGTH, "\"%s\"-comparison of arrays of different sizes in dimension %u (%u != %u)", comparison_op_to_str(op),
                     depth, left_type_info->sizes[i], right_type_info->sizes[i]);
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
            apply_comparison_op(op, const_node_view_result->values + i, left_type_info->type, const_node_view_left->values[i], right_type_info->type, const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_comparison_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_equality_op_node(equality_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    /* implement error when nodes are no expression nodes */
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
    /* implement error when child is no expression node */
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

node_t *build_integer_op_node(integer_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    type_info_t *left_type_info = get_type_info_of_node(left);
    type_info_t *right_type_info = get_type_info_of_node(right);
    /* implement error when nodes are no expression nodes */
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
        const_node_view_result->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            int validity_check = apply_integer_op(op,
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
    /* implement error when child is no expression node */
    qualifier_t result_qualifier = child_type_info->qualifier;
    type_t result_type = propagate_type(INVERT_OP, child_type_info->type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to expression of type %s", type_to_str(child_type_info->type));
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
        result = new_invert_op_node(result_qualifier, result_type, child_type_info->sizes, child_type_info->depth, child);
    }
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

void print_array(type_t type, const value_t *values, const unsigned sizes[MAXARRAYDEPTH], unsigned depth, unsigned current_depth, unsigned index) {
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

void print_node(const node_t *node) {
    switch (node->type) {
        case BASIC_NODE_T: {
            printf("Basic node\n");
            break;
        }
        case FUNC_DECL_NODE_T: {
            printf("Function declaration node for %s\n", ((func_decl_node_t *) node)->entry->name);
            break;
        }
        case VAR_DECL_NODE_T: {
            var_decl_node_t *var_decl_node_view = ((var_decl_node_t *) node);
            type_info_t info = { .qualifier = var_decl_node_view->entry->qualifier,
                                 .type = var_decl_node_view->entry->type,
                                 .depth = var_decl_node_view->entry->depth};
            memcpy(info.sizes, var_decl_node_view->entry->sizes, info.depth * sizeof (unsigned));
            print_type_info(&info);
            printf(" %s\n", var_decl_node_view->entry->name);
            break;
        }
        case VAR_DEF_NODE_T: {
            var_def_node_t *var_def_node_view = ((var_def_node_t *) node);
            type_info_t info = { .qualifier = var_def_node_view->entry->qualifier,
                    .type = var_def_node_view->entry->type,
                    .depth = var_def_node_view->entry->depth};
            memcpy(info.sizes, var_def_node_view->entry->sizes, info.depth * sizeof (unsigned));
            print_type_info(&info);
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
                print_array(info->type, ((const_node_t *) node)->values, info->sizes, info->depth, 0, 0);
            }
        }
        case REFERENCE_NODE_T: {
            type_info_t *info = get_type_info_of_node(node);
            printf("reference to ");
            print_type_info(info);
            printf(" %s\n", ((reference_node_t *) node)->entry->name);
            break;
        }
        case FUNC_CALL_NODE_T: {
            printf("Function call node for %s with %u parameters\n", ((func_call_node_t *) node)->entry->name, ((func_call_node_t *) node)->num_of_pars);
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
            printf("If node with %u \"else if\"s\n", ((if_node_t *) node)->elseif_count);
            break;
        }
        case ELSE_NODE_T: {
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
            printf("Assign node of entry %s\n", ((assign_node_t *) node)->entry->name);
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

    switch (node->type) {
        case BASIC_NODE_T: {
            tree_traversal(node->left);
            tree_traversal(node->right);
            break;
        }
        case VAR_DEF_NODE_T: {
            for (unsigned i = 0; i < get_length_of_array(((var_def_node_t *) node)->entry->sizes,
                                                         ((var_def_node_t *) node)->entry->depth); ++i) {
                if (!(((var_def_node_t *) node)->value_is_const)) {
                    tree_traversal(((var_def_node_t *) node)->values[i].node_value);
                }
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
            for (unsigned i = 0; i < ((if_node_t *) node)->elseif_count; ++i) {
                tree_traversal(((if_node_t *) node)->elseif_branches[i]);
            }
            tree_traversal(((if_node_t *) node)->else_branch);
            break;
        }
        case ELSE_NODE_T: {
            tree_traversal(((else_node_t *) node)->condition);
            tree_traversal(((else_node_t *) node)->elseif_branch);
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
            tree_traversal(((assign_node_t *) node)->assign_val);
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
    print_node(node);
}
