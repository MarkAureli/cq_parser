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
            out->b_val = in_1.b_val || in_2.b_val;
            break;
        }
        case LXOR_OP: {
            out->b_val = in_1.b_val && !in_2.b_val || !in_1.b_val && in_2.b_val;
            break;
        }
        case LAND_OP: {
            out->b_val = in_1.b_val && in_2.b_val;
            break;
        }
    }
}

void comparison_op_application(comparison_op_t op, value_t *out, type_t in_type_1,
                               value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    switch (op) {
        case GE_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->b_val = in_value_1.i_val > in_value_2.i_val;
            } else {
                out->b_val = in_value_1.u_val > in_value_2.u_val;
            }
            break;
        }
        case GEQ_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->b_val = in_value_1.i_val >= in_value_2.i_val;
            } else {
                out->b_val = in_value_1.i_val >= in_value_2.u_val;
            }
            break;
        }
        case LE_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->b_val = in_value_1.i_val < in_value_2.i_val;
            } else {
                out->b_val = in_value_1.i_val < in_value_2.u_val;
            }
            break;
        }
        case LEQ_OP: {
            if (in_type_1 == INT_T && in_type_2 == INT_T) {
                out->b_val = in_value_1.i_val >= in_value_2.i_val;
            } else {
                out->b_val = in_value_1.i_val >= in_value_2.u_val;
            }
            break;
        }
    }
}

void apply_equality_op(equality_op_t op, value_t *out, type_t in_type_1,
                       value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    if (in_type_1 == BOOL_T) {
        if (op == EQ_OP) {
            out->b_val = in_value_1.b_val == in_value_2.b_val;
        } else {
            out->b_val = in_value_1.b_val != in_value_2.b_val;
        }
    } else if (in_type_1 == INT_T && in_type_2 == INT_T) {
        if (op == EQ_OP) {
            out->b_val = in_value_1.i_val == in_value_2.i_val;
        } else {
            out->b_val = in_value_1.i_val != in_value_2.i_val;
        }
    } else {
        if (op == EQ_OP) {
            out->b_val = in_value_1.u_val == in_value_2.u_val;
        } else {
            out->b_val = in_value_1.u_val != in_value_2.u_val;
        }
    }
}

int integer_op_application(integer_op_t op, value_t *out, type_t in_type_1,
                           value_t in_value_1, type_t in_type_2, value_t in_value_2) {
    if (in_type_1 == INT_T && in_type_2 == INT_T) {
        switch (op) {
            case ADD_OP: {
                out->i_val = in_value_1.i_val + in_value_2.i_val;
                break;
            }
            case AND_OP: {
                out->i_val = in_value_1.i_val & in_value_2.i_val;
                break;
            }
            case DIV_OP: {
                if (in_value_2.i_val == 0) {
                    return 1;
                }
                out->i_val = in_value_1.i_val / in_value_2.i_val;
                break;
            }
            case MOD_OP: {
                if (in_value_2.i_val == 0) {
                    return 2;
                }
                out->i_val = in_value_1.i_val % in_value_2.i_val;
                break;
            }
            case MUL_OP: {
                out->i_val = in_value_1.i_val * in_value_2.i_val;
                break;
            }
            case OR_OP: {
                out->i_val = in_value_1.i_val | in_value_2.i_val;
                break;
            }
            case SUB_OP: {
                out->i_val = in_value_1.i_val - in_value_2.i_val;
                break;
            }
            case XOR_OP: {
                out->i_val = in_value_1.i_val ^ in_value_2.i_val;
                break;
            }
        }
    } else {
        switch (op) {
            case ADD_OP: {
                out->u_val = in_value_1.u_val + in_value_2.u_val;
                break;
            }
            case AND_OP: {
                out->u_val = in_value_1.u_val & in_value_2.u_val;
                break;
            }
            case DIV_OP: {
                if (in_value_2.u_val == 0) {
                    return 1;
                }
                out->u_val = in_value_1.u_val / in_value_2.u_val;
                break;
            }
            case MOD_OP: {
                if (in_value_2.u_val == 0) {
                    return 2;
                }
                out->u_val = in_value_1.u_val % in_value_2.u_val;
                break;
            }
            case MUL_OP: {
                out->u_val = in_value_1.u_val * in_value_2.u_val;
                break;
            }
            case OR_OP: {
                out->u_val = in_value_1.u_val | in_value_2.u_val;
                break;
            }
            case SUB_OP: {
                out->u_val = in_value_1.u_val - in_value_2.u_val;
                break;
            }
            case XOR_OP: {
                out->u_val = in_value_1.u_val ^ in_value_2.u_val;
                break;
            }
        }
    }
    return 0;
}

unsigned get_length_of_array(const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth) {
    unsigned result = 1;
    for (unsigned i = 0; i < depth; ++i) {
        result *= sizes[i];
    }
    return result;
}

void print_array(type_t type, const value_t *values, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                 unsigned current_depth, unsigned index) {
    if (current_depth == depth - 1) {
        putchar('{');
        for (unsigned i = 0; i < sizes[current_depth]; ++i) {
            switch (type) {
                case BOOL_T: {
                    printf("%s", (values[index + i].b_val) ? "true" : "false");
                    break;
                }
                case INT_T: {
                    printf("%d", values[index + i].i_val);
                    break;
                }
                case UNSIGNED_T: {
                    printf("%u", values[index + i].u_val);
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

value_t *get_reduced_array(const value_t *values, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                           const unsigned indices[MAX_ARRAY_DEPTH], unsigned index_depth) {
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
    stmt_list_node_view->is_unitary = stmt_list_node_view->is_unitary && stmt_is_unitary(stmt);
}

node_t *new_func_decl_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not a function", entry->name);
        return NULL;
    }

    func_decl_node_t *new_node = calloc(1, sizeof (func_decl_node_t));
    new_node->type = FUNC_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_sp_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not a function", entry->name);
        return NULL;
    } else if (!entry->is_sp) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s cannot be used to create a superposition", entry->name);
        return NULL;
    }
    func_decl_node_t *new_node = calloc(1, sizeof (func_sp_node_t));
    new_node->type = FUNC_SP_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_decl_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is a function", entry->name);
        return NULL;
    }

    var_decl_node_t *new_node = calloc(1, sizeof (var_decl_node_t));
    new_node->type = VAR_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_const_node(type_t type, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, value_t *values) {
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

node_t *new_reference_node(const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, bool index_is_const[MAX_ARRAY_DEPTH],
                           const index_t indices[MAX_ARRAY_DEPTH], entry_t *entry) {
    reference_node_t *new_node = calloc(1, sizeof (reference_node_t));
    new_node->type = REFERENCE_NODE_T;
    new_node->type_info.qualifier = entry->qualifier;
    new_node->type_info.type = entry->type;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    memcpy(new_node->index_is_const, index_is_const, entry->depth * sizeof(bool));
    memcpy(new_node->indices, indices, entry->depth * sizeof(index_t));
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_call_node(bool sp, entry_t *entry, node_t **pars, unsigned num_of_pars,
                           char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to call non-function %s", entry->name);
        return NULL;
    } else if (entry->num_of_pars != num_of_pars) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s requires %u parameter%s, but is called with %u parameter%s",
                 entry->name, entry->num_of_pars, (entry->num_of_pars == 1) ? "" : "s",
                 num_of_pars, (num_of_pars == 1) ? "" : "s");
        return NULL;
    }

    if (sp) {
        if (!entry->is_sp) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s cannot be used to create a superposition", entry->name);
            return NULL;
        } else if (pars[0]->type != REFERENCE_NODE_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is not a quantum variable",
                     entry->name);
            return NULL;
        }

        reference_node_t *reference_node_view = (reference_node_t *) pars[0];
        if (reference_node_view->type_info.qualifier != QUANTUM_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is not a quantum variable",
                     entry->name);
            return NULL;
        } else if (reference_node_view->type_info.type != entry->pars_type_info[0].type) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is of type %s instead of %s",
                     entry->name, type_to_str(reference_node_view->type_info.type),
                     type_to_str(entry->pars_type_info[0].type));
            return NULL;
        }
    } else {
        for (unsigned i = 0; i < num_of_pars; ++i) {
            type_info_t type_info_of_par;
            if (!copy_type_info_of_node(&type_info_of_par, pars[i])) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter %u in call to function %s is not an expression",
                         i + 1, entry->name);
                return NULL;
            }
            if (entry->pars_type_info[i].qualifier != QUANTUM_T && type_info_of_par.qualifier == QUANTUM_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Parameter %u in call to function %s is required to be classical, but is quantum",
                         i + 1, entry->name);
                return NULL;
            } else if (!are_matching_types(entry->pars_type_info[i].type, type_info_of_par.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Parameter %u in call to function %s is required to be of type %s, but is of type %s",
                         i + 1, entry->name, type_to_str(entry->pars_type_info[i].type),
                         type_to_str(type_info_of_par.type));
                return NULL;
            } else if (entry->pars_type_info[i].depth != type_info_of_par.depth) {
                if (entry->pars_type_info[i].depth == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a scalar, but is a depth-%u array",
                             i + 1, entry->name, type_info_of_par.depth);
                    return NULL;
                } else if (type_info_of_par.depth == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a depth-%u array, but is a scalar",
                             i + 1, entry->name, entry->pars_type_info[i].depth);
                    return NULL;
                } else {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a depth-%u array, but has depth %u",
                             i + 1, entry->name, entry->pars_type_info[i].depth, type_info_of_par.depth);
                    return NULL;
                }
            }
            for (unsigned j = 0; j < type_info_of_par.depth; ++j) {
                if (entry->pars_type_info[i].sizes[j] != type_info_of_par.sizes[j]) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s has size %u instead of %u in dimension %u",
                             i + 1, entry->name, entry->pars_type_info[i].sizes[j], type_info_of_par.sizes[j], j + 1);
                    return NULL;
                }
            }
        }
    }

    func_call_node_t *new_node = calloc(1, sizeof (func_call_node_t));
    new_node->type = FUNC_CALL_NODE_T;
    new_node->type_info.qualifier = entry->qualifier;
    new_node->type_info.type = entry->type;
    memcpy(new_node->type_info.sizes, entry->sizes, entry->depth * sizeof (unsigned));
    new_node->type_info.depth = entry->depth;
    new_node->entry = entry;
    new_node->inverse = false;
    new_node->sp = sp;
    new_node->pars = pars;
    new_node->num_of_pars = num_of_pars;
    return (node_t *) new_node;
}

node_t *new_logical_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                            logical_op_t op, node_t *left, node_t *right) {
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

node_t *new_comparison_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
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

node_t *new_equality_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
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

node_t *new_not_op_node(qualifier_t qualifier, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth, node_t *child) {
    not_op_node_t *new_node = calloc(1, sizeof (not_op_node_t));
    new_node->type = NOT_OP_NODE_T;
    new_node->type_info.qualifier = qualifier;
    new_node->type_info.type = BOOL_T;
    memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
    new_node->type_info.depth = depth;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_integer_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
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

node_t *new_invert_op_node(qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
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

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **else_if_branches, unsigned num_of_else_ifs,
                    node_t *else_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t if_condition_type_info;
    if (!copy_type_info_of_node(&if_condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition is not an expression");
        return NULL;
    } else if (if_condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition must be of type bool, but is of type %s",
                 type_to_str(if_condition_type_info.type));
        return NULL;
    } else if (if_condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition must be a single bool, but is an array of depth %u",
                 if_condition_type_info.depth);
        return NULL;
    } else if (if_condition_type_info.qualifier == QUANTUM_T) {
        if (!(((stmt_list_node_t *) if_branch)->is_unitary)) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "If-condition is quantum, but statements in if-branch are not unitary");
            return NULL;
        } else if (else_branch != NULL && !(((stmt_list_node_t *) else_branch)->is_unitary)) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "If-condition is quantum, but statements in else-branch are not unitary");
            return NULL;
        }
    }

    type_info_t else_if_condition_type_info;
    for (unsigned i = 0; i < num_of_else_ifs; ++i) {
        else_if_node_t *else_if_node_view = (else_if_node_t *) (else_if_node_t *) else_if_branches[i];
        copy_type_info_of_node(&else_if_condition_type_info, else_if_node_view->condition);

        if (else_if_condition_type_info.qualifier != if_condition_type_info.qualifier) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition %u is %s while if condition is %s",
                     i + 1, (else_if_condition_type_info.qualifier  == QUANTUM_T) ? "quantum" : "classical",
                     (if_condition_type_info.qualifier == QUANTUM_T) ? "quantum" : "classical");
            return NULL;
        }
    }

    if_node_t *new_node = calloc(1, sizeof (if_node_t));
    new_node->type = IF_NODE_T;
    new_node->condition = condition;
    new_node->if_branch = if_branch;
    new_node->else_if_branches = else_if_branches;
    new_node->num_of_else_ifs = num_of_else_ifs;
    new_node->else_branch = else_branch;
    return (node_t *) new_node;
}

node_t *new_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition is not an expression");
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T && !(((stmt_list_node_t *) else_if_branch)->is_unitary)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition is quantum, but statements are not unitary");
        return NULL;
    }

    else_if_node_t *new_node = calloc(1, sizeof (else_if_node_t));
    new_node->type = ELSE_IF_NODE_T;
    new_node->condition = condition;
    new_node->else_if_branch = else_if_branch;
    return (node_t *) new_node;
}

node_t *new_switch_node(node_t *expression, node_t **case_branches, unsigned num_of_cases,
                        char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t expression_type_info;
    if (!copy_type_info_of_node(&expression_type_info, expression)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "No valid switch expression ");
        return NULL;
    } else if (expression_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Switch-expression must be a scalar, but is an array of depth %u",
                 expression_type_info.depth);
        return NULL;
    }

    for (unsigned i = 0; i < num_of_cases; ++i) {
        case_node_t *case_node_view = (case_node_t *) case_branches[i];
        stmt_list_node_t *case_branch_view = (stmt_list_node_t *) case_node_view->case_branch;
        const_node_t *case_const_view = (const_node_t *) case_node_view->case_const;
        if (case_const_view != NULL
            && !are_matching_types(expression_type_info.type, case_const_view->type_info.type)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Case %u is of type %s while switch expression is of type %s",
                     i + 1, type_to_str(case_const_view->type_info.type), type_to_str(expression_type_info.type));
            return NULL;
        } else if (expression_type_info.qualifier == QUANTUM_T && !case_branch_view->is_unitary) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Switch-expression is quantum, but statements in case %u are not unitary", i + 1);
            return NULL;
        }

        if (case_const_view == NULL) { /* default statement reached, discard all following cases */
            num_of_cases = i + 1;
            case_branches = realloc(case_branches, num_of_cases * sizeof (node_t *));
            break;
        }

        for (unsigned j = 0; j < i; ++j) {
            case_node_t *prior_case_node_view = (case_node_t *) case_branches[j];
            const_node_t *prior_case_const_view = (const_node_t *) prior_case_node_view->case_const;
            if ((case_const_view->type_info.type == BOOL_T
                 && case_const_view->values[0].b_val == prior_case_const_view->values[0].b_val)
                || (case_const_view->type_info.type == INT_T
                    && case_const_view->values[0].i_val == prior_case_const_view->values[0].i_val)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Cases %u and %u have the same value", j + 1, i + 1);
                return NULL;
            }
        }
    }

    switch_node_t *new_node = calloc(1, sizeof (switch_node_t));
    new_node->type = SWITCH_NODE_T;
    new_node->expression = expression;
    new_node->case_branches = case_branches;
    new_node->num_of_cases = num_of_cases;
    return (node_t *) new_node;
}

node_t *new_case_node(node_t *case_const, node_t *case_branch) {
    case_node_t *new_node = calloc(1, sizeof (case_node_t));
    new_node->type = CASE_NODE_T;
    new_node->case_const = case_const;
    new_node->case_branch = case_branch;
    return (node_t *) new_node;
}

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                     char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition is not an expression");
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop condition cannot be quantum");
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        return NULL;
    }

    for_node_t *new_node = calloc(1, sizeof (for_node_t));
    new_node->type = FOR_NODE_T;
    new_node->initialize = initialize;
    new_node->condition = condition;
    new_node->increment = increment;
    new_node->for_branch = for_branch;
    return (node_t *) new_node;
}

node_t *new_do_node(node_t *do_branch, node_t *condition, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Do-while-loop-condition is not an expression");
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Do-while-loop-condition cannot be quantum");
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Do-while-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Do-while-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        return NULL;
    }

    do_node_t *new_node = calloc(1, sizeof (do_node_t));
    new_node->type = DO_NODE_T;
    new_node->do_branch = do_branch;
    new_node->condition = condition;
    return (node_t *) new_node;
}

node_t *new_while_node(node_t *condition, node_t *while_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition is not an expression");
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition cannot be quantum");
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        return NULL;
    }

    while_node_t *new_node = calloc(1, sizeof (while_node_t));
    new_node->type = WHILE_NODE_T;
    new_node->condition = condition;
    new_node->while_branch = while_branch;
    return (node_t *) new_node;
}

node_t *new_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    if (left->type == CONST_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to reassign constant value");
        return NULL;
    } else if (left->type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left-hand side of assignment is not a variable");
        return NULL;
    }

    type_info_t left_type_info;
    copy_type_info_of_node(&left_type_info, left);
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of assignment is not an expression");
        return NULL;
    } else if (left_type_info.qualifier != QUANTUM_T && right_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Classical left-hand side of assignment, but quantum right-hand side");
        return NULL;
    }
    switch (op) {
        case ASSIGN_OP: {
            if (!are_matching_types(left_type_info.type, right_type_info.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Assigning %s to %s",
                         type_to_str(right_type_info.type), type_to_str(left_type_info.type));
                return NULL;
            }
            break;
        }
        case ASSIGN_OR_OP: case ASSIGN_XOR_OP: case ASSIGN_AND_OP: {
            if (!are_matching_types(left_type_info.type, right_type_info.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "%s Assigning %s to %s",
                         (left_type_info.type == BOOL_T) ? "Logically" : "Bitwisely", type_to_str(right_type_info.type),
                         type_to_str(left_type_info.type));
                return NULL;
            }
            break;
        }
        case ASSIGN_ADD_OP: case ASSIGN_SUB_OP: case ASSIGN_MUL_OP: case ASSIGN_DIV_OP: case ASSIGN_MOD_OP: {
            if (left_type_info.type == BOOL_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning to bool");
                return NULL;
            } else if (right_type_info.type == BOOL_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning bool to %s",
                         type_to_str(left_type_info.type));
                return NULL;
            } else if (left_type_info.type == INT_T && right_type_info.type == UNSIGNED_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning unsigned to int");
                return NULL;
            }
            break;
        }
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand side of \"%s\" is a scalar, right-hand side is an array of depth %u)",
                 assign_op_to_str(op), right_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand side of \"%s\" is an array of depth %u, right-hand side is a scalar)",
                 assign_op_to_str(op), left_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand and right-hand side of \"%s\" are arrays of different depth (%u != %u)",
                 assign_op_to_str(op), left_type_info.depth, right_type_info.depth);
        return NULL;
    }
    unsigned depth = left_type_info.depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Left-hand and right-hand side of \"%s\" are arrays of different sizes in dimension %u (%u != %u)",
                     assign_op_to_str(op), depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info.sizes;
    unsigned length = get_length_of_array(sizes, depth);

    if (right_type_info.qualifier == CONST_T) { /* right is of node_type CONST_NODE_T */
        const_node_t *const_node_view_right = (const_node_t *) right;

        if (op == ASSIGN_DIV_OP) {
            for (unsigned i = 0; i < length; ++i) {
                if (const_node_view_right->values[i].i_val == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Division by zero");
                    return NULL;
                }
            }
        } else if (op == ASSIGN_MOD_OP) {
            for (unsigned i = 0; i < length; ++i) {
                if (const_node_view_right->values[i].i_val == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Modulo by zero");
                    return NULL;
                }
            }
        }
    }
    assign_node_t *new_node = calloc(1, sizeof (assign_node_t));
    new_node->type = ASSIGN_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_phase_node(node_t *left, bool is_positive, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    if (left->type == CONST_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to change the phase of a classical variable");
        return NULL;
    } else if (left->type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left-hand side of assignment is not a variable");
        return NULL;
    }

    type_info_t left_type_info;
    copy_type_info_of_node(&left_type_info, left);
    if (left_type_info.qualifier != QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to change the phase of a classical variable");
        return NULL;
    }

    type_info_t right_type_info;
    if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of phase change is not an expression");
        return NULL;
    } else if (right_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Cannot change the phase by a quantum value");
        return NULL;
    } else if (right_type_info.type == BOOL_T || right_type_info.type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Cannot change the phase by a value of type %s",
                 type_to_str(right_type_info.type));
        return NULL;
    } else if (right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of phase change is an array of depth %u",
                 right_type_info.depth);
        return NULL;
    }

    phase_node_t *new_node = calloc(1, sizeof (phase_node_t));
    new_node->type = PHASE_NODE_T;
    new_node->is_positive = is_positive;
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

bool stmt_is_unitary(const node_t *node) {
    if (node == NULL) {
        return false;
    }

    type_info_t type_info;
    switch (node->type) {
        case VAR_DECL_NODE_T: case VAR_DEF_NODE_T: case ASSIGN_NODE_T: {
            copy_type_info_of_node(&type_info, node);
            return type_info.qualifier == QUANTUM_T;
        }
        case FUNC_CALL_NODE_T: {
            return ((func_call_node_t *) node)->entry->is_unitary;
        }
        case IF_NODE_T: {
            copy_type_info_of_node(&type_info, ((if_node_t *) node)->condition);
            return type_info.qualifier == QUANTUM_T;
        }
        case PHASE_NODE_T: {
            return true;
        }
        default: {
            return false;
        }
    }
}

void copy_type_info_of_entry(type_info_t *type_info, const entry_t *entry) {
    type_info->qualifier = entry->qualifier;
    type_info->type = entry->type;
    memcpy(type_info->sizes, entry->sizes, entry->depth * sizeof (unsigned));
    type_info->depth = entry->depth;
}

bool copy_type_info_of_node(type_info_t *type_info, const node_t *node) {
    switch (node->type) {
        case VAR_DECL_NODE_T: {
            copy_type_info_of_entry(type_info, ((var_decl_node_t *) node)->entry);
            return true;
        }
        case VAR_DEF_NODE_T: {
            copy_type_info_of_entry(type_info, ((var_def_node_t *) node)->entry);
            return true;
        }
        case CONST_NODE_T: {
            type_info->qualifier = ((const_node_t *) node)->type_info.qualifier;
            type_info->type = ((const_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((const_node_t *) node)->type_info.sizes,
                   ((const_node_t *) node)->type_info.depth);
            type_info->depth = ((const_node_t *) node)->type_info.depth;
            return true;
        }
        case FUNC_SP_NODE_T: {
            copy_type_info_of_entry(type_info, ((func_sp_node_t *) node)->entry);
            return true;
        }
        case REFERENCE_NODE_T: {
            type_info->qualifier = ((reference_node_t *) node)->type_info.qualifier;
            type_info->type = ((reference_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((reference_node_t *) node)->type_info.sizes,
                   ((reference_node_t *) node)->type_info.depth);
            type_info->depth = ((reference_node_t *) node)->type_info.depth;
            return true;
        }
        case ASSIGN_NODE_T: {
            switch (((assign_node_t *) node)->left->type) {
                case CONST_NODE_T: {
                    type_info->qualifier = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.qualifier;
                    type_info->type = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.type;
                    memcpy(type_info->sizes, ((const_node_t *) ((assign_node_t *) node)->left)->type_info.sizes,
                           ((const_node_t *) ((assign_node_t *) node)->left)->type_info.depth);
                    type_info->depth = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.depth;
                    return true;
                }
                case REFERENCE_NODE_T: {
                    type_info->qualifier = ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.qualifier;
                    type_info->type = ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.type;
                    memcpy(type_info->sizes, ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.sizes,
                           ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.depth);
                    type_info->depth = ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.depth;
                    return true;
                }
                default: {
                    return false;
                }
            }
        }
        case LOGICAL_OP_NODE_T: {
            type_info->qualifier = ((logical_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((logical_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((logical_op_node_t *) node)->type_info.sizes,
                   ((logical_op_node_t *) node)->type_info.depth);
            type_info->depth = ((logical_op_node_t *) node)->type_info.depth;
            return true;
        }
        case COMPARISON_OP_NODE_T: {
            type_info->qualifier = ((comparison_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((comparison_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((comparison_op_node_t *) node)->type_info.sizes,
                   ((comparison_op_node_t *) node)->type_info.depth);
            type_info->depth = ((comparison_op_node_t *) node)->type_info.depth;
            return true;
        }
        case EQUALITY_OP_NODE_T: {
            type_info->qualifier = ((equality_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((equality_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((equality_op_node_t *) node)->type_info.sizes,
                   ((equality_op_node_t *) node)->type_info.depth);
            type_info->depth = ((equality_op_node_t *) node)->type_info.depth;
            return true;
        }
        case NOT_OP_NODE_T: {
            type_info->qualifier = ((not_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((not_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((not_op_node_t *) node)->type_info.sizes,
                   ((not_op_node_t *) node)->type_info.depth);
            type_info->depth = ((not_op_node_t *) node)->type_info.depth;
            return true;
        }
        case INTEGER_OP_NODE_T: {
            type_info->qualifier = ((integer_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((integer_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((integer_op_node_t *) node)->type_info.sizes,
                   ((integer_op_node_t *) node)->type_info.depth);
            type_info->depth = ((integer_op_node_t *) node)->type_info.depth;
            return true;
        }
        case INVERT_OP_NODE_T: {
            type_info->qualifier = ((invert_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((invert_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((invert_op_node_t *) node)->type_info.sizes,
                   ((invert_op_node_t *) node)->type_info.depth);
            type_info->depth = ((invert_op_node_t *) node)->type_info.depth;
            return true;
        }
        case FUNC_CALL_NODE_T: {
            type_info->qualifier = ((func_call_node_t *) node)->type_info.qualifier;
            type_info->type = ((func_call_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((func_call_node_t *) node)->type_info.sizes,
                   ((func_call_node_t *) node)->type_info.depth);
            type_info->depth = ((func_call_node_t *) node)->type_info.depth;
            return true;
        }
        default: {
            return false;
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

node_t *new_var_def_node(entry_t *entry, bool is_init_list, node_t *node, qualified_type_t *qualified_types,
                         array_value_t *values, unsigned length, char error_msg[ERROR_MSG_LENGTH]) {
    if (is_init_list) {
        if (entry->depth == 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not an array, but is initialized as such", entry->name);
            return NULL;
        } else if (length > entry->length) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Too many (%u) elements initialized for array %s of total length %u",
                     length, entry->name, entry->length);
            return NULL;
        }

        for (unsigned i = 0; i < length; ++i) {
            if (entry->qualifier == QUANTUM_T
                && qualified_types[i].qualifier != CONST_T
                && values[i].node_value->type == FUNC_SP_NODE_T) {
                entry_t *current_entry = ((func_sp_node_t *) values[i].node_value)->entry;
                unsigned num_of_pars = current_entry->num_of_pars;
                if (current_entry->num_of_pars != 1) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Element %u: Quantizable function %s must take exactly 1 parameter",
                             i, current_entry->name);
                    return NULL;
                } else if (current_entry->pars_type_info[0].qualifier != NONE_T) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Element %u: Quantizable function %s must take a classical parameter",
                             i, current_entry->name);
                    return NULL;
                } else if (current_entry->pars_type_info[0].type != entry->type) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Element %u: Quantizable function %s takes %s instead of %s",
                             i, current_entry->name, type_to_str(current_entry->pars_type_info[0].type),
                             type_to_str(entry->type));
                    return NULL;
                } else if (current_entry->pars_type_info[0].depth != 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Element %u: Quantizable function %s takes an array of depth %u instead of a scalar",
                             i, current_entry->name, current_entry->pars_type_info[0].depth);
                    return NULL;
                }
            } else if (entry->qualifier != QUANTUM_T
                       && qualified_types[i].qualifier != CONST_T
                       && values[i].node_value->type == FUNC_SP_NODE_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of classical array %s is a superposition instruction",
                         i, entry->name);
                return NULL;
            } else if (entry->qualifier != QUANTUM_T
                       && qualified_types[i].qualifier == QUANTUM_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Element %u in initialization of classical array %s is quantum",
                         i, entry->name);
                return NULL;
            } else if (entry->qualifier == CONST_T
                       && qualified_types[i].qualifier != CONST_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of constant array %s is not constant", i, entry->name);
                return NULL;
            } else if (!are_matching_types(entry->type, qualified_types[i].type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of %s-array %s is of type %s",
                         i, type_to_str(entry->type), entry->name,
                         type_to_str(qualified_types[i].type));
                return NULL;
            }
        }
    } else { /* no initializer list */
        type_info_t type_info;
        if (!copy_type_info_of_node(&type_info, node)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side in initialization of %s is not an expression",
                     entry->name);
            return NULL;
        } else if (entry->qualifier == QUANTUM_T && node->type == FUNC_SP_NODE_T) {
            entry_t *current_entry = ((func_sp_node_t *) node)->entry;
            if (current_entry->num_of_pars != 1) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s must take exactly 1 parameter",
                         ((func_sp_node_t *) node)->entry->name);
                return NULL;
            } else if (current_entry->pars_type_info[0].qualifier != NONE_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s must take a classical parameter",
                         ((func_sp_node_t *) node)->entry->name);
                return NULL;
            } else if (current_entry->pars_type_info[0].type != entry->type) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s takes %s instead of %s",
                         ((func_sp_node_t *) node)->entry->name,
                         type_to_str(current_entry->pars_type_info[0].type), type_to_str(entry->type));
                return NULL;
            } else if (current_entry->pars_type_info[0].depth != 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Quantizable function %s takes an array of depth %u instead of a scalar",
                         ((func_sp_node_t *) node)->entry->name, current_entry->pars_type_info[0].depth);
                return NULL;
            }
        } else if (entry->qualifier != QUANTUM_T && node->type == FUNC_SP_NODE_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Classical variable %s cannot be initialized in superposition",
                     entry->name);
        } else if (entry->depth == 0 && type_info.depth != 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not an array, but is initialized as such", entry->name);
            return NULL;
        } else if (entry->depth != type_info.depth) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Non-matching depths in array initialization of %s (%u != %u)",
                     entry->name, entry->depth, type_info.depth);
            return NULL;
        }

        for (unsigned i = 0; i < entry->depth; ++i) {
            if (entry->sizes[i] != type_info.sizes[i]) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Non-matching sizes at position %u in array initialization of %s (%u != %u)",
                         i, entry->name, entry->sizes[i], type_info.sizes[i]);
                return NULL;
            }
        }

        if (entry->qualifier != QUANTUM_T && type_info.qualifier == QUANTUM_T) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of classical scalar %s with quantum value",
                         entry->name);
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of classical array %s with quantum array",
                         entry->name);
                return NULL;
            }
        } else if (entry->qualifier == CONST_T && type_info.qualifier != CONST_T) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of constant scalar %s with non-constant value",
                         entry->name);
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of constant array %s with non-constant array",
                         entry->name);
                return NULL;
            }
        } else if (node->type != FUNC_SP_NODE_T
                   && !are_matching_types(entry->type, type_info.type)) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of scalar %s of type %s with value of type %s",
                         entry->name, type_to_str(entry->type), type_to_str(type_info.type));
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of %s-array %s with %s-array",
                         type_to_str(entry->type), entry->name, type_to_str(type_info.type));
                return NULL;
            }
        }
    }

    var_def_node_t *new_node = calloc(1, sizeof (var_def_node_t));
    new_node->type = VAR_DEF_NODE_T;
    new_node->entry = entry;
    new_node->is_init_list = is_init_list;
    if (is_init_list) {
        new_node->qualified_types = qualified_types;
        new_node->values = values;
    } else {
        new_node->node = node;
    }
    new_node->length = length;
    return (node_t *) new_node;
}

node_t *build_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t left_type_info;
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&left_type_info, left)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left operand of \"%s\" is not an expression",
                 logical_op_to_str(op));
        return NULL;
    } else if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right operand of \"%s\" is not an expression",
                 logical_op_to_str(op));
        return NULL;
    }

    qualifier_t result_qualifier = propagate_qualifier(left_type_info.qualifier,
                                                       right_type_info.qualifier);
    type_t result_type = propagate_type(LOGICAL_OP, left_type_info.type, right_type_info.type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to %s and %s", logical_op_to_str(op),
                 type_to_str(left_type_info.type), type_to_str(right_type_info.type));
        return NULL;
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to scalar and array of depth %u)",
                 logical_op_to_str(op), right_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to array of depth %u and scalar)",
                 logical_op_to_str(op), left_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to arrays of different depth (%u != %u)",
                 logical_op_to_str(op), left_type_info.depth, right_type_info.depth);
        return NULL;
    }
    unsigned depth = left_type_info.depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Applying \"%s\" to arrays of different sizes in dimension %u (%u != %u)",
                     logical_op_to_str(op), depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info.sizes;
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

node_t *build_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t left_type_info;
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&left_type_info, left)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left operand of \"%s\" is not an expression",
                 comparison_op_to_str(op));
        return NULL;
    } else if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right operand of \"%s\" is not an expression",
                 comparison_op_to_str(op));
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info.qualifier,
                                                       right_type_info.qualifier);
    type_t result_type = propagate_type(COMPARISON_OP, left_type_info.type, right_type_info.type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "\"%s\"-comparison of %s and %s", comparison_op_to_str(op),
                 type_to_str(left_type_info.type), type_to_str(right_type_info.type));
        return NULL;
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "\"%s\"-comparison of scalar and array of depth %u)",
                 comparison_op_to_str(op), right_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "\"%s\"-comparison of array of depth %u and scalar)",
                 comparison_op_to_str(op), left_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "\"%s\"-comparison of arrays of different depths (%u != %u)",
                 comparison_op_to_str(op), left_type_info.depth, right_type_info.depth);
        return NULL;
    }
    unsigned depth = left_type_info.depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "\"%s\"-comparison of arrays of different sizes in dimension %u (%u != %u)",
                     comparison_op_to_str(op), depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info.sizes;
    unsigned length = get_length_of_array(sizes, depth);

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            comparison_op_application(op, const_node_view_result->values + i, left_type_info.type,
                                      const_node_view_left->values[i], right_type_info.type,
                                      const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_comparison_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t left_type_info;
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&left_type_info, left)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left-hand side of %sequality is not an expression",
                 (op == EQ_OP) ? "" : "in");
        return NULL;
    } else if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of %sequality is not an expression",
                 (op == EQ_OP) ? "" : "in");
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info.qualifier,
                                                       right_type_info.qualifier);
    type_t result_type = propagate_type(EQUALITY_OP, left_type_info.type, right_type_info.type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Checking %sequality of %s and %s", (op == EQ_OP) ? "" : "in",
                 type_to_str(left_type_info.type), type_to_str(right_type_info.type));
        return NULL;
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Checking %sequality of scalar and array of depth %u)",
                 (op == EQ_OP) ? "" : "in", right_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Checking %sequality of array of depth %u and scalar)",
                 (op == EQ_OP) ? "" : "in", left_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Checking %sequality of arrays of different depths (%u != %u)",
                 (op == EQ_OP) ? "" : "in", left_type_info.depth, right_type_info.depth);
        return NULL;
    }
    unsigned depth = left_type_info.depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Checking %sequality of arrays of different sizes in dimension %u (%u != %u)",
                     (op == EQ_OP) ? "" : "in", depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info.sizes;
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
                              left_type_info.type,
                              const_node_view_left->values[i],
                              right_type_info.type,
                              const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
    } else {
        result = new_equality_op_node(result_qualifier, sizes, depth, op, left, right);
    }
    return result;
}

node_t *build_not_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t child_type_info;
    if (!copy_type_info_of_node(&child_type_info, child)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"!\" to a non-expression");
        return NULL;
    }
    qualifier_t result_qualifier = child_type_info.qualifier;
    type_t result_type = propagate_type(NOT_OP, child_type_info.type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"!\" to expression of type %s",
                 type_to_str(child_type_info.type));
        return NULL;
    }

    node_t *result;
    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info.sizes, child_type_info.depth);
        result = child;
        const_node_t *const_node_view_child = (const_node_t *) child;
        const_node_t *const_node_view_result = (const_node_t *) result;
        for (unsigned i = 0; i < length; ++i) {
            const_node_view_result->values[i].b_val = !(const_node_view_child->values[i].b_val);
        }
    } else if (child->type == NOT_OP_NODE_T) {
        not_op_node_t *not_op_node_view_child = (not_op_node_t *) child;
        result = not_op_node_view_child->child;
        free(not_op_node_view_child);
    } else {
        result = new_not_op_node(result_qualifier, child_type_info.sizes, child_type_info.depth, child);
    }
    return result;
}

node_t *build_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t left_type_info;
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&left_type_info, left)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left operand of \"%s\" is not an expression",
                 integer_op_to_str(op));
        return NULL;
    } else if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right operand of \"%s\" is not an expression",
                 integer_op_to_str(op));
        return NULL;
    }
    qualifier_t result_qualifier = propagate_qualifier(left_type_info.qualifier,
                                                       right_type_info.qualifier);
    type_t result_type = propagate_type(INTEGER_OP,
                                        left_type_info.type,
                                        right_type_info.type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to %s and %s", integer_op_to_str(op),
                 type_to_str(left_type_info.type), type_to_str(right_type_info.type));
        return NULL;
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to scalar and array of depth %u)",
                 integer_op_to_str(op), right_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to array of depth %u and scalar)",
                 integer_op_to_str(op), left_type_info.depth);
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"%s\" to arrays of different depths (%u != %u)",
                 integer_op_to_str(op), left_type_info.depth, right_type_info.depth);
        return NULL;
    }
    unsigned depth = left_type_info.depth;

    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Applying \"%s\" to arrays of different sizes in dimension %u (%u != %u)",
                     integer_op_to_str(op), depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            return NULL;
        }
    }
    unsigned *sizes = left_type_info.sizes;
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
                                                        left_type_info.type,
                                                        const_node_view_left->values[i],
                                                        right_type_info.type,
                                                        const_node_view_right->values[i]);
            if (validity_check == 1) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Division by zero");
                return NULL;
            } else if (validity_check == 2) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Modulo by zero");
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

node_t *build_invert_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t child_type_info;
    if (!copy_type_info_of_node(&child_type_info, child)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"~\" to a non-expression");
        return NULL;
    }
    qualifier_t result_qualifier = child_type_info.qualifier;
    type_t result_type = propagate_type(INVERT_OP, child_type_info.type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Applying \"~\" to expression of type %s",
                 type_to_str(child_type_info.type));
        return NULL;
    }

    node_t *result;
    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info.sizes, child_type_info.depth);
        result = child;
        const_node_t *const_node_view_child = (const_node_t *) child;
        const_node_t *const_node_view_result = (const_node_t *) result;
        if (result_type == INT_T) {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_result->values[i].i_val = ~(const_node_view_child->values[i].i_val);
            }
        } else {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_result->values[i].u_val = ~(const_node_view_child->values[i].u_val);
            }
        }

    } else if (child->type == INVERT_OP_NODE_T) {
        invert_op_node_t *invert_op_node_view_child = (invert_op_node_t *) child;
        result = invert_op_node_view_child->child;
        free(invert_op_node_view_child);
    } else {
        result = new_invert_op_node(result_qualifier, result_type, child_type_info.sizes,
                                    child_type_info.depth, child);
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

void print_node(const node_t *node) {
    type_info_t type_info;
    switch (node->type) {
        case BASIC_NODE_T: {
            printf("Basic node\n");
            break;
        }
        case STMT_LIST_NODE_T: {
            printf("Statement entry node with %u statements\n", ((stmt_list_node_t *) node)->num_of_stmt);
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
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(" %s\n", var_decl_node_view->entry->name);
            break;
        }
        case VAR_DEF_NODE_T: {
            var_def_node_t *var_def_node_view = ((var_def_node_t *) node);
            printf("Definition: ");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(" %s\n", var_def_node_view->entry->name);
            break;
        }
        case CONST_NODE_T: {
            copy_type_info_of_node(&type_info, node);
            if (type_info.depth == 0) {
                switch (type_info.type) {
                    case BOOL_T: {
                        printf("%s\n", ((const_node_t *) node)->values[0].b_val ? "true" : "false");
                        break;
                    }
                    case INT_T: {
                        printf("%d\n", ((const_node_t *) node)->values[0].i_val);
                        break;
                    }
                    case UNSIGNED_T: {
                        printf("%u\n", ((const_node_t *) node)->values[0].u_val);
                        break;
                    }
                    case VOID_T: {
                        printf("undefined\n");
                        break;
                    }
                }
            } else {
                print_array(type_info.type, ((const_node_t *) node)->values, type_info.sizes,
                            type_info.depth, 0, 0);
            }
            break;
        }
        case REFERENCE_NODE_T: {
            copy_type_info_of_node(&type_info, node);
            printf("reference to ");
            print_type_info(&type_info);
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
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s (", logical_op_to_str(((logical_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case COMPARISON_OP_NODE_T: {
            putchar('(');
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s (", comparison_op_to_str(((comparison_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case EQUALITY_OP_NODE_T: {
            putchar('(');
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s (", equality_op_to_str(((equality_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case NOT_OP_NODE_T: {
            printf("!(");
            copy_type_info_of_node(&type_info, ((invert_op_node_t *) node)->child);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case INTEGER_OP_NODE_T: {
            putchar('(');
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s (", integer_op_to_str(((integer_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case INVERT_OP_NODE_T: {
            printf("~(");
            copy_type_info_of_node(&type_info, ((invert_op_node_t *) node)->child);
            print_type_info(&type_info);
            printf(") -> (");
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case IF_NODE_T: {
            printf("If node with %u \"else if\"s\n", ((if_node_t *) node)->num_of_else_ifs);
            break;
        }
        case ELSE_IF_NODE_T: {
            printf("Else-if node\n");
            break;
        }
        case SWITCH_NODE_T: {
            printf("Switch node with %u cases\n", ((switch_node_t *) node)->num_of_cases);
            break;
        }
        case CASE_NODE_T: {
            printf("Case node\n");
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
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s (", logical_op_to_str(((logical_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
            printf(") %s (", assign_op_to_str(((assign_node_t *) node)->op));
            copy_type_info_of_node(&type_info, node);
            print_type_info(&type_info);
            printf(")\n");
            break;
        }
        case PHASE_NODE_T: {
            printf("phase (");
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            print_type_info(&type_info);
            printf(") %s= (", (((phase_node_t *) node)->is_positive) ? "+" : "-");
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            print_type_info(&type_info);
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
                for (unsigned i = 0; i < get_length_of_array(((var_def_node_t *) node)->entry->sizes,
                                                             ((var_def_node_t *) node)->entry->depth); ++i) {
                    if ((((var_def_node_t *) node)->qualified_types[i].qualifier != CONST_T)) {
                        tree_traversal(((var_def_node_t *) node)->values[i].node_value);
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
        case SWITCH_NODE_T: {
            tree_traversal(((switch_node_t *) node)->expression);
            for (unsigned i = 0; i < ((switch_node_t *) node)->num_of_cases; ++i) {
                tree_traversal(((switch_node_t *) node)->case_branches[i]);
            }
            break;
        }
        case CASE_NODE_T: {
            tree_traversal(((case_node_t *) node)->case_const);
            tree_traversal(((case_node_t *) node)->case_branch);
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
        case PHASE_NODE_T: {
            tree_traversal(((phase_node_t *) node)->left);
            tree_traversal(((phase_node_t *) node)->right);
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
