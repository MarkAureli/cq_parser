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

bool is_sp(const entry_t *entry) {
    return (entry->is_quantizable && entry->qualifier == NONE_T && entry->type == BOOL_T && entry->depth == 0
            && entry->num_of_pars == 1 && entry->pars_type_info[0].qualifier == NONE_T
            && entry->pars_type_info[0].depth == 0);
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

value_t *get_reduced_array(const value_t *values, const unsigned sizes[MAX_ARRAY_DEPTH], unsigned depth,
                           const unsigned indices[MAX_ARRAY_DEPTH], unsigned index_depth) {
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

    value_t* output = malloc( out_length * sizeof (value_t));
    memcpy(output, values + reduced_index, out_length * sizeof (value_t));
    return output;
}

void free_node(node_t *node) {
    if (node == NULL) {
        return;
    }

    switch (node->node_type) {
        case BASIC_NODE_T: {
            free_node(node->left);
            free_node(node->right);
            free(node);
            return;
        }
        case STMT_LIST_NODE_T: {
            for (unsigned i = 0; i < ((stmt_list_node_t *) node)->num_of_stmts; ++i) {
                free_node(((stmt_list_node_t *) node)->stmt_list[i]);
            }
            free(((stmt_list_node_t *) node)->stmt_list);
            free((stmt_list_node_t *) node);
            return;
        }
        case FUNC_DECL_NODE_T: {
            free_node(((func_decl_node_t *) node)->func_tail);
            free((func_decl_node_t *) node);
            return;
        }
        case VAR_DECL_NODE_T: {
            free((var_decl_node_t *) node);
            return;
        }
        case VAR_DEF_NODE_T: {
            if (((var_def_node_t *) node)->is_init_list) {
                free(((var_def_node_t *) node)->qualified_types);
                free(((var_def_node_t *) node)->values);
            } else {
                free_node(((var_def_node_t *) node)->node);
            }
            free((var_def_node_t *) node);
            return;
        }
        case CONST_NODE_T: {
            free(((const_node_t *) node)->values);
            free((const_node_t *) node);
            return;
        }
        case REFERENCE_NODE_T: {
            free((reference_node_t *) node);
            return;
        }
        case FUNC_CALL_NODE_T: {
            for (unsigned i = 0; i < ((func_call_node_t *) node)->num_of_pars; ++i) {
                free_node(((func_call_node_t *) node)->pars[i]);
            }
            free(((func_call_node_t *) node)->pars);
            free((func_call_node_t *) node);
            return;
        }
        case FUNC_SP_NODE_T: {
            free((func_sp_node_t *) node);
            return;
        }
        case LOGICAL_OP_NODE_T: {
            free_node(((logical_op_node_t *) node)->left);
            free_node(((logical_op_node_t *) node)->right);
            free((logical_op_node_t *) node);
            return;
        }
        case COMPARISON_OP_NODE_T: {
            free_node(((comparison_op_node_t *) node)->left);
            free_node(((comparison_op_node_t *) node)->right);
            free((comparison_op_node_t *) node);
            return;
        }
        case EQUALITY_OP_NODE_T: {
            free_node(((equality_op_node_t *) node)->left);
            free_node(((equality_op_node_t *) node)->right);
            free((equality_op_node_t *) node);
            return;
        }
        case NOT_OP_NODE_T: {
            free_node(((not_op_node_t *) node)->child);
            free((not_op_node_t *) node);
            return;
        }
        case INTEGER_OP_NODE_T: {
            free_node(((integer_op_node_t *) node)->left);
            free_node(((integer_op_node_t *) node)->right);
            free((integer_op_node_t *) node);
            return;
        }
        case INVERT_OP_NODE_T: {
            free_node(((invert_op_node_t *) node)->child);
            free((invert_op_node_t *) node);
            return;
        }
        case IF_NODE_T: {
            free_node(((if_node_t *) node)->condition);
            free_node(((if_node_t *) node)->if_branch);
            for (unsigned i = 0; i < ((if_node_t *) node)->num_of_else_ifs; ++i) {
                free_node(((if_node_t *) node)->else_if_branches[i]);
            }
            free_node(((if_node_t *) node)->else_branch);
            free((if_node_t *) node);
            return;
        }
        case ELSE_IF_NODE_T: {
            free_node(((else_if_node_t *) node)->condition);
            free_node(((else_if_node_t *) node)->else_if_branch);
            free((else_if_node_t *) node);
            return;
        }
        case SWITCH_NODE_T: {
            free_node(((switch_node_t *) node)->expression);
            for (unsigned i = 0; i < ((switch_node_t *) node)->num_of_cases; ++i) {
                free_node(((switch_node_t *) node)->case_branches[i]);
            }
            free((switch_node_t *) node);
            return;
        }
        case CASE_NODE_T: {
            free_node(((case_node_t *) node)->case_branch);
            free((case_node_t *) node);
            return;
        }
        case FOR_NODE_T: {
            free_node(((for_node_t *) node)->initialize);
            free_node(((for_node_t *) node)->condition);
            free_node(((for_node_t *) node)->increment);
            free_node(((for_node_t *) node)->for_branch);
            free((for_node_t *) node);
            return;
        }
        case DO_NODE_T: {
            free_node(((do_node_t *) node)->do_branch);
            free_node(((do_node_t *) node)->condition);
            free((do_node_t *) node);
            return;
        }
        case WHILE_NODE_T: {
            free_node(((while_node_t *) node)->condition);
            free_node(((while_node_t *) node)->while_branch);
            free((while_node_t *) node);
            return;
        }
        case ASSIGN_NODE_T: {
            free_node(((assign_node_t *) node)->left);
            free_node(((assign_node_t *) node)->right);
            free((assign_node_t *) node);
            return;
        }
        case PHASE_NODE_T: {
            free_node(((phase_node_t *) node)->left);
            free_node(((phase_node_t *) node)->right);
            free((phase_node_t *) node);
            return;
        }
        case MEASURE_NODE_T: {
            free_node(((measure_node_t *) node)->child);
            free((measure_node_t *) node);
            return;
        }
        case BREAK_NODE_T: {
            free((break_node_t *) node);
            return;
        }
        case CONTINUE_NODE_T: {
            free((continue_node_t *) node);
            return;
        }
        case RETURN_NODE_T: {
            free_node(((return_node_t *) node)->return_value);
            free((return_node_t *) node);
            return;
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

node_t *new_stmt_list_node(bool is_unitary, bool is_quantizable, node_t **stmt_list, unsigned num_of_stmts,
                           char error_msg[ERROR_MSG_LENGTH]) {
    return_style_t result_return_style = NONE_ST;
    type_info_t result_return_type_info;
    for (unsigned i = 0; i < num_of_stmts; ++i) {
        return_style_t current_return_style = get_return_style(stmt_list[i]);
        if ((stmt_list[i]->node_type == BREAK_NODE_T || stmt_list[i]->node_type == CONTINUE_NODE_T
            || current_return_style == DEFINITE_ST) && i < num_of_stmts - 1) {
            for (unsigned j = i + 1; j < num_of_stmts; ++j) {
                free_node(stmt_list[j]);
            }
            num_of_stmts = i + 1;
            node_t **temp = realloc(stmt_list, num_of_stmts * sizeof (node_t *));
            if (temp == NULL) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating memory for statement list node failed");
                for (unsigned j = 0; j < num_of_stmts; ++j) {
                    free_node(stmt_list[j]);
                }
                free(stmt_list);
                free_symbol_table();
                return NULL;
            }

            stmt_list = temp;
        }

        if (current_return_style != NONE_ST) {
            type_info_t current_return_type_info;
            copy_return_type_info_of_node(&current_return_type_info, stmt_list[i]);
            if (result_return_style != NONE_ST) {
                if (result_return_type_info.qualifier != current_return_type_info.qualifier) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Non-matching qualifiers in return statements");
                    return NULL;
                } else if (result_return_type_info.type != current_return_type_info.type) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Non-matching types (%s and %s) in return statements",
                             type_to_str(result_return_type_info.type), type_to_str(current_return_type_info.type));
                    return NULL;
                } else if (result_return_type_info.depth != current_return_type_info.depth) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Non-matching depths (%u and %u) in return statements",
                             result_return_type_info.depth, current_return_type_info.depth);
                    return NULL;
                }

                for (unsigned j = 0; j < result_return_type_info.depth; ++j) {
                    if (result_return_type_info.sizes[j] != current_return_type_info.sizes[j]) {
                        snprintf(error_msg, ERROR_MSG_LENGTH,
                                 "Non-matching sizes in depth %u (%u and %u) in return statements",
                                 j, result_return_type_info.sizes[j], current_return_type_info.sizes[j]);
                        return NULL;
                    }
                }
            } else {
                copy_return_type_info_of_node(&result_return_type_info, stmt_list[i]);
            }

            result_return_style = current_return_style;
        }
    }
    stmt_list_node_t *new_node = malloc(sizeof (stmt_list_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for statement list node failed");
        for (unsigned j = 0; j < num_of_stmts; ++j) {
            free_node(stmt_list[j]);
        }
        free(stmt_list);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = STMT_LIST_NODE_T;
    new_node->is_unitary = is_unitary;
    new_node->is_quantizable = is_quantizable;
    new_node->stmt_list = stmt_list;
    new_node->num_of_stmts = num_of_stmts;
    new_node->return_style = result_return_style;
    if (result_return_style != NONE_ST) {
        memcpy(&(new_node->return_type_info), &(result_return_type_info), sizeof (result_return_type_info));
    }
    return (node_t *) new_node;
}

node_t *new_func_decl_node(entry_t *entry, node_t *func_tail, char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not a function", entry->name);
        free_node(func_tail);
        free_symbol_table();
        return NULL;
    }

    return_style_t func_tail_return_style = get_return_style(func_tail);
    type_info_t func_tail_return_type_info;
    if (func_tail_return_style != NONE_ST) {
        copy_return_type_info_of_node(&func_tail_return_type_info, func_tail);
    }
    if (entry->type == VOID_T) {
        if (func_tail_return_style != NONE_ST && func_tail_return_type_info.type != VOID_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Function %s is declared to return void, but has non-void return statement", entry->name);
            return NULL;
        }
    } else {
        if (func_tail_return_style == NONE_ST) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Non-void function %s has no return statement", entry->name);
            return NULL;
        } else if (func_tail_return_style == CONDITIONAL_ST) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Non-void function %s does not return in all branches", entry->name);
            return NULL;
        } else {
            if (entry->qualifier == QUANTUM_T && func_tail_return_type_info.qualifier != QUANTUM_T
                || entry->qualifier == NONE_T && func_tail_return_type_info.qualifier == QUANTUM_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Declared return qualifier of function %s does not match the actually returned qualifier",
                         entry->name);
                return NULL;
            } else if (entry->type != func_tail_return_type_info.type) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Declared return type %s of function %s does not match the actually returned type %s",
                         type_to_str(entry->type), entry->name, type_to_str(func_tail_return_type_info.type));
                return NULL;
            } else if (entry->depth != func_tail_return_type_info.depth) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Declared return depth %u of function %s does not match the actually returned depth %u",
                         entry->depth, entry->name, func_tail_return_type_info.depth);
                return NULL;
            }

            for (unsigned j = 0; j < entry->depth; ++j) {
                if (entry->sizes[j] != func_tail_return_type_info.sizes[j]) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Declared return size of %u in depth %u of function %s does not match the actually "
                             "returned size of %u",
                             entry->sizes[j], j, entry->name, func_tail_return_type_info.sizes[j]);
                    return NULL;
                }
            }
        }
    }

    func_decl_node_t *new_node = malloc(sizeof (func_decl_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function declaration node failed");
        free_node(func_tail);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = FUNC_DECL_NODE_T;
    new_node->entry = entry;
    new_node->func_tail = func_tail;
    return (node_t *) new_node;
}

node_t *new_var_decl_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is a function", entry->name);
        free_symbol_table();
        return NULL;
    }

    var_decl_node_t *new_node = malloc(sizeof (var_decl_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for variable declaration node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = VAR_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_var_def_node(entry_t *entry, bool is_init_list, node_t *node, qualified_type_t *qualified_types,
                         array_value_t *values, unsigned length, char error_msg[ERROR_MSG_LENGTH]) {
    bool result_is_quantizable = true;
    bool result_is_unitary = true;
    if (is_init_list) {
        if (entry->depth == 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not an array, but is initialized as such", entry->name);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        } else if (length > entry->length) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Too many (%u) elements initialized for array %s of total length %u",
                     length, entry->name, entry->length);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        }

        for (unsigned i = 0; i < length; ++i) {
            if (entry->qualifier == QUANTUM_T
                && qualified_types[i].qualifier != CONST_T
                && values[i].node_value->node_type == FUNC_SP_NODE_T) {
                entry_t *current_entry = ((func_sp_node_t *) values[i].node_value)->entry;
                unsigned num_of_pars = current_entry->num_of_pars;
                if (current_entry->pars_type_info[0].type != entry->type) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Element %u: Quantizable function %s takes %s instead of %s",
                             i, current_entry->name, type_to_str(current_entry->pars_type_info[0].type),
                             type_to_str(entry->type));
                    free_node(node);
                    free(qualified_types);
                    free(values);
                    free_symbol_table();
                    return NULL;
                }
            } else if (entry->qualifier != QUANTUM_T
                       && qualified_types[i].qualifier != CONST_T
                       && values[i].node_value->node_type == FUNC_SP_NODE_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of classical array %s is a superposition instruction",
                         i, entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (entry->qualifier != QUANTUM_T
                       && qualified_types[i].qualifier == QUANTUM_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Element %u in initialization of classical array %s is quantum",
                         i, entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (entry->qualifier == CONST_T
                       && qualified_types[i].qualifier != CONST_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of constant array %s is not constant", i, entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (!are_matching_types(entry->type, qualified_types[i].type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Element %u in initialization of %s-array %s is of type %s",
                         i, type_to_str(entry->type), entry->name,
                         type_to_str(qualified_types[i].type));
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }

            if (qualified_types[i].qualifier != CONST_T) {
                result_is_quantizable = result_is_quantizable && is_quantizable(values[i].node_value);
                result_is_unitary = result_is_unitary && is_unitary(values[i].node_value);
            }
        }
    } else { /* no initializer list */
        type_info_t type_info;
        if (!copy_type_info_of_node(&type_info, node)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side in initialization of %s is not an expression",
                     entry->name);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        } else if (entry->qualifier == QUANTUM_T && node->node_type == FUNC_SP_NODE_T) {
            entry_t *current_entry = ((func_sp_node_t *) node)->entry;
            if (current_entry->num_of_pars != 1) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s must take exactly 1 parameter",
                         ((func_sp_node_t *) node)->entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (current_entry->pars_type_info[0].qualifier != NONE_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s must take a classical parameter",
                         ((func_sp_node_t *) node)->entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (current_entry->pars_type_info[0].type != entry->type) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Quantizable function %s takes %s instead of %s",
                         ((func_sp_node_t *) node)->entry->name,
                         type_to_str(current_entry->pars_type_info[0].type), type_to_str(entry->type));
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else if (current_entry->pars_type_info[0].depth != 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Quantizable function %s takes an array of depth %u instead of a scalar",
                         ((func_sp_node_t *) node)->entry->name, current_entry->pars_type_info[0].depth);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }
        } else if (entry->qualifier != QUANTUM_T && node->node_type == FUNC_SP_NODE_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Classical variable %s cannot be initialized in superposition",
                     entry->name);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        } else if (entry->depth == 0 && type_info.depth != 0) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not an array, but is initialized as such", entry->name);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        } else if (entry->depth != type_info.depth) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Non-matching depths in array initialization of %s (%u != %u)",
                     entry->name, entry->depth, type_info.depth);
            free_node(node);
            free(qualified_types);
            free(values);
            free_symbol_table();
            return NULL;
        }

        for (unsigned i = 0; i < entry->depth; ++i) {
            if (entry->sizes[i] != type_info.sizes[i]) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Non-matching sizes at position %u in array initialization of %s (%u != %u)",
                         i, entry->name, entry->sizes[i], type_info.sizes[i]);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }
        }

        if (entry->qualifier != QUANTUM_T && type_info.qualifier == QUANTUM_T) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of classical scalar %s with quantum value",
                         entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of classical array %s with quantum array",
                         entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }
        } else if (entry->qualifier == CONST_T && type_info.qualifier != CONST_T) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of constant scalar %s with non-constant value",
                         entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of constant array %s with non-constant array",
                         entry->name);
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }
        } else if (node->node_type != FUNC_SP_NODE_T
                   && !are_matching_types(entry->type, type_info.type)) {
            if (entry->depth == 0) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of scalar %s of type %s with value of type %s",
                         entry->name, type_to_str(entry->type), type_to_str(type_info.type));
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            } else {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Initialization of %s-array %s with %s-array",
                         type_to_str(entry->type), entry->name, type_to_str(type_info.type));
                free_node(node);
                free(qualified_types);
                free(values);
                free_symbol_table();
                return NULL;
            }
        }

        result_is_quantizable = is_quantizable(node);
        result_is_unitary = is_unitary(node);
    }

    var_def_node_t *new_node = malloc(sizeof (var_def_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for variable declaration node failed");
        free_node(node);
        free(qualified_types);
        free(values);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = VAR_DEF_NODE_T;
    new_node->is_quantizable = entry->qualifier != QUANTUM_T && result_is_quantizable;
    new_node->is_unitary = entry->qualifier == QUANTUM_T && result_is_unitary;
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

node_t *new_const_node(type_t type, value_t value, char error_msg[ERROR_MSG_LENGTH]) {
    const_node_t *new_node = malloc(sizeof (const_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for constant node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = CONST_NODE_T;
    new_node->type_info.qualifier = CONST_T;
    new_node->type_info.type = type;
    new_node->type_info.depth = 0;
    new_node->values = malloc(sizeof (value_t));
    if (new_node->values == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for value array of constant node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->values[0] = value;
    return (node_t *) new_node;
}

node_t *new_reference_node(entry_t *entry, const bool index_is_const[MAX_ARRAY_DEPTH],
                           const index_t indices[MAX_ARRAY_DEPTH], unsigned index_depth,
                           char error_msg[ERROR_MSG_LENGTH]) {
    bool all_indices_const = true;
    for (unsigned i = 0; i < index_depth; ++i) {
        all_indices_const &= index_is_const[i];
        if (index_is_const[i] && indices[i].const_index >= entry->sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "%u-th index (%u) of array %s%s out of bounds (%u)",
                     i, indices[i].const_index, (entry->is_function) ? "returned by " : "", entry->name,
                     entry->sizes[i]);
            free_symbol_table();
            return NULL;
        }
    }

    if (entry->qualifier == CONST_T && all_indices_const) {
        unsigned const_indices[MAX_ARRAY_DEPTH];
        for (unsigned i = 0; i < index_depth; ++i) {
            const_indices[i] = indices[i].const_index;
        }
        value_t *values = get_reduced_array(entry->values, entry->sizes, entry->depth, const_indices,
                                                index_depth);
        if (values == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for value extraction of %s failed", entry->name);
            free_symbol_table();
            return NULL;
        }

        const_node_t *new_node = malloc(sizeof (const_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for constant reference node failed");
            free_symbol_table();
            return NULL;
        }

        new_node->node_type = CONST_NODE_T;
        new_node->type_info.qualifier = CONST_T;
        new_node->type_info.type = entry->type;
        memcpy(new_node->type_info.sizes, entry->sizes + index_depth, (entry->depth - index_depth) * sizeof (unsigned));
        new_node->type_info.depth = entry->depth - index_depth;
        new_node->values = values;
        return (node_t *) new_node;
    } else {
        reference_node_t *new_node = malloc(sizeof (reference_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for reference node failed");
            free_symbol_table();
            return NULL;
        }

        new_node->node_type = REFERENCE_NODE_T;
        new_node->is_quantizable = entry->scope != 0 && entry->qualifier != QUANTUM_T && all_indices_const;
        new_node->is_unitary = entry->qualifier == QUANTUM_T && all_indices_const;
        new_node->type_info.qualifier = (entry->qualifier == CONST_T) ? NONE_T : entry->qualifier;
        new_node->type_info.type = entry->type;
        memcpy(new_node->type_info.sizes, entry->sizes + index_depth, (entry->depth - index_depth) * sizeof (unsigned));
        new_node->type_info.depth = entry->depth - index_depth;
        memcpy(new_node->index_is_const, index_is_const, index_depth * sizeof (bool));
        memcpy(new_node->indices, indices, index_depth * sizeof (index_t));
        new_node->entry = entry;
        return (node_t *) new_node;
    }
}

node_t *new_func_call_node(bool sp, entry_t *entry, node_t **pars, unsigned num_of_pars,
                           char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to call non-function %s", entry->name);
        for (unsigned i = 0; i < num_of_pars; ++i) {
            free_node(pars[i]);
        }
        free(pars);
        free_symbol_table();
        return NULL;
    } else if (entry->num_of_pars != num_of_pars) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s requires %u parameter%s, but is called with %u parameter%s",
                 entry->name, entry->num_of_pars, (entry->num_of_pars == 1) ? "" : "s",
                 num_of_pars, (num_of_pars == 1) ? "" : "s");
        for (unsigned i = 0; i < num_of_pars; ++i) {
            free_node(pars[i]);
        }
        free(pars);
        free_symbol_table();
        return NULL;
    }

    bool is_quantized = false;
    if (sp) {
        if (!is_sp(entry)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s cannot be used to create a superposition", entry->name);
            for (unsigned i = 0; i < num_of_pars; ++i) {
                free_node(pars[i]);
            }
            free(pars);
            free_symbol_table();
            return NULL;
        } else if (pars[0]->node_type != REFERENCE_NODE_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is not a quantum variable",
                     entry->name);
            for (unsigned i = 0; i < num_of_pars; ++i) {
                free_node(pars[i]);
            }
            free(pars);
            free_symbol_table();
            return NULL;
        }

        reference_node_t *reference_node_view = (reference_node_t *) pars[0];
        if (reference_node_view->type_info.qualifier != QUANTUM_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is not a quantum variable",
                     entry->name);
            for (unsigned i = 0; i < num_of_pars; ++i) {
                free_node(pars[i]);
            }
            free(pars);
            free_symbol_table();
            return NULL;
        } else if (reference_node_view->type_info.type != entry->pars_type_info[0].type) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter in call to function %s is of type %s instead of %s",
                     entry->name, type_to_str(reference_node_view->type_info.type),
                     type_to_str(entry->pars_type_info[0].type));
            for (unsigned i = 0; i < num_of_pars; ++i) {
                free_node(pars[i]);
            }
            free(pars);
            free_symbol_table();
            return NULL;
        }
    } else {
        for (unsigned i = 0; i < num_of_pars; ++i) {
            type_info_t type_info_of_par;
            if (!copy_type_info_of_node(&type_info_of_par, pars[i])) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Parameter %u in call to function %s is not an expression",
                         i + 1, entry->name);
                for (unsigned j = 0; j < num_of_pars; ++j) {
                    free_node(pars[j]);
                }
                free(pars);
                free_symbol_table();
                return NULL;
            }

            if (entry->pars_type_info[i].qualifier != QUANTUM_T && type_info_of_par.qualifier == QUANTUM_T) {
                if (!entry->is_quantizable) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be classical, but is quantum",
                             i + 1, entry->name);
                    for (unsigned j = 0; j < num_of_pars; ++j) {
                        free_node(pars[j]);
                    }
                    free(pars);
                    free_symbol_table();
                    return NULL;
                } else {
                    is_quantized = true;
                }
            }

            if (!are_matching_types(entry->pars_type_info[i].type, type_info_of_par.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Parameter %u in call to function %s is required to be of type %s, but is of type %s",
                         i + 1, entry->name, type_to_str(entry->pars_type_info[i].type),
                         type_to_str(type_info_of_par.type));
                for (unsigned j = 0; j < num_of_pars; ++j) {
                    free_node(pars[j]);
                }
                free(pars);
                free_symbol_table();
                return NULL;
            } else if (entry->pars_type_info[i].depth != type_info_of_par.depth) {
                if (entry->pars_type_info[i].depth == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a scalar, but is a depth-%u array",
                             i + 1, entry->name, type_info_of_par.depth);
                    for (unsigned j = 0; j < num_of_pars; ++j) {
                        free_node(pars[j]);
                    }
                    free(pars);
                    free_symbol_table();
                    return NULL;
                } else if (type_info_of_par.depth == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a depth-%u array, but is a scalar",
                             i + 1, entry->name, entry->pars_type_info[i].depth);
                    for (unsigned j = 0; j < num_of_pars; ++j) {
                        free_node(pars[j]);
                    }
                    free(pars);
                    free_symbol_table();
                    return NULL;
                } else {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s is required to be a depth-%u array, but has depth %u",
                             i + 1, entry->name, entry->pars_type_info[i].depth, type_info_of_par.depth);
                    for (unsigned j = 0; j < num_of_pars; ++j) {
                        free_node(pars[j]);
                    }
                    free(pars);
                    free_symbol_table();
                    return NULL;
                }
            }
            for (unsigned j = 0; j < type_info_of_par.depth; ++j) {
                if (entry->pars_type_info[i].sizes[j] != type_info_of_par.sizes[j]) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Parameter %u in call to function %s has size %u instead of %u in dimension %u",
                             i + 1, entry->name, entry->pars_type_info[i].sizes[j], type_info_of_par.sizes[j], j + 1);
                    for (unsigned k = 0; k < num_of_pars; ++k) {
                        free_node(pars[k]);
                    }
                    free(pars);
                    free_symbol_table();
                    return NULL;
                }
            }
        }
    }

    func_call_node_t *new_node = malloc(sizeof (func_call_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for reference node failed");
        for (unsigned i = 0; i < num_of_pars; ++i) {
            free_node(pars[i]);
        }
        free(pars);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = FUNC_CALL_NODE_T;
    if (sp) {
        new_node->is_quantizable = false;
        new_node->is_unitary = true;
        new_node->type_info.qualifier = NONE_T;
        new_node->type_info.type = VOID_T;
        new_node->type_info.depth = 0;
    } else if (is_quantized) {
        new_node->is_quantizable = false;
        new_node->is_unitary = true;
        copy_type_info_of_entry(&new_node->type_info, entry);
        new_node->type_info.qualifier = QUANTUM_T;
    } else {
        new_node->is_quantizable = entry->is_quantizable;
        new_node->is_unitary = entry->is_unitary;
        copy_type_info_of_entry(&new_node->type_info, entry);
    }
    new_node->entry = entry;
    new_node->inverse = false;
    new_node->sp = sp;
    new_node->pars = pars;
    new_node->num_of_pars = num_of_pars;
    return (node_t *) new_node;
}

node_t *new_func_sp_node(entry_t *entry, char error_msg[ERROR_MSG_LENGTH]) {
    if (!entry->is_function) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "%s is not a function", entry->name);
        free_symbol_table();
        return NULL;
    } else if (!is_sp(entry)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Function %s cannot be used to create a superposition", entry->name);
        free_symbol_table();
        return NULL;
    }

    func_decl_node_t *new_node = malloc(sizeof (func_sp_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for function superposition node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = FUNC_SP_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}


node_t *new_logical_op_node(node_t *left, logical_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
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
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_view_left->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            logical_op_application(op, const_node_view_left->values + i, const_node_view_left->values[i],
                                   const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
        return left;
    } else {
        logical_op_node_t *new_node = malloc(sizeof (logical_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for logical operator node failed");
            return NULL;
        }

        new_node->node_type = LOGICAL_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(left) && is_quantizable(right);
        new_node->is_unitary = is_unitary(left) && is_unitary(right);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = BOOL_T;
        memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
        new_node->type_info.depth = depth;
        new_node->op = op;
        new_node->left = left;
        new_node->right = right;
        return (node_t *) new_node;
    }
}

node_t *new_comparison_op_node(node_t *left, comparison_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
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
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_view_left->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            comparison_op_application(op, const_node_view_left->values + i, left_type_info.type,
                                      const_node_view_left->values[i], right_type_info.type,
                                      const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
        return left;
    } else {
        comparison_op_node_t *new_node = malloc(sizeof (comparison_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for comparison operator node failed");
            return NULL;
        }

        new_node->node_type = COMPARISON_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(left) && is_quantizable(right);
        new_node->is_unitary = is_unitary(left) && is_unitary(right);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = BOOL_T;
        memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
        new_node->type_info.depth = depth;
        new_node->op = op;
        new_node->left = left;
        new_node->right = right;
        return (node_t *) new_node;
    }
}

node_t *new_equality_op_node(node_t *left, equality_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
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
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_view_left->type_info.type = BOOL_T;

        for (unsigned i = 0; i < length; ++i) {
            apply_equality_op(op,
                              const_node_view_left->values + i,
                              left_type_info.type,
                              const_node_view_left->values[i],
                              right_type_info.type,
                              const_node_view_right->values[i]);
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
        return left;
    } else {
        equality_op_node_t *new_node = malloc( sizeof (equality_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for equality operator node failed");
            return NULL;
        }

        new_node->node_type = EQUALITY_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(left) && is_quantizable(right);
        new_node->is_unitary = is_unitary(left) && is_unitary(right);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = BOOL_T;
        memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
        new_node->type_info.depth = depth;
        new_node->op = op;
        new_node->left = left;
        new_node->right = right;
        return (node_t *) new_node;
    }
}

node_t *new_not_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]) {
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

    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info.sizes, child_type_info.depth);
        const_node_t *const_node_view_child = (const_node_t *) child;
        for (unsigned i = 0; i < length; ++i) {
            const_node_view_child->values[i].b_val = !(const_node_view_child->values[i].b_val);
        }
        return child;
    } else if (child->node_type == NOT_OP_NODE_T) {
        not_op_node_t *not_op_node_view_child = (not_op_node_t *) child;
        node_t *result = not_op_node_view_child->child;
        free(not_op_node_view_child);
        return result;
    } else {
        not_op_node_t *new_node = malloc(sizeof (not_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for not-operator node failed");
            return NULL;
        }

        new_node->node_type = NOT_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(child);
        new_node->is_unitary = is_unitary(child);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = BOOL_T;
        memcpy(new_node->type_info.sizes, child_type_info.sizes, child_type_info.depth * sizeof (unsigned));
        new_node->type_info.depth = child_type_info.depth;
        new_node->child = child;
        return (node_t *) new_node;
    }
}

node_t *new_integer_op_node(node_t *left, integer_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
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

    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_view_left->type_info.type = INT_T;

        for (unsigned i = 0; i < length; ++i) {
            int validity_check = integer_op_application(op,
                                                        const_node_view_left->values + i,
                                                        left_type_info.type,
                                                        const_node_view_left->values[i],
                                                        right_type_info.type,
                                                        const_node_view_right->values[i]);
            if (validity_check == 1) {
                free(const_node_view_left->values);
                free(const_node_view_left);
                free(const_node_view_right->values);
                free(const_node_view_right);
                snprintf(error_msg, ERROR_MSG_LENGTH, "Division by zero");
                return NULL;
            } else if (validity_check == 2) {
                free(const_node_view_left->values);
                free(const_node_view_left);
                free(const_node_view_right->values);
                free(const_node_view_right);
                snprintf(error_msg, ERROR_MSG_LENGTH, "Modulo by zero");
                return NULL;
            }
        }
        free(const_node_view_right->values);
        free(const_node_view_right);
        return left;
    } else {
        integer_op_node_t *new_node = malloc(sizeof (integer_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for integer operator node failed");
            return NULL;
        }

        new_node->node_type = INTEGER_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(left) && is_quantizable(right);
        new_node->is_unitary = is_unitary(left) && is_unitary(right);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = result_type;
        memcpy(new_node->type_info.sizes, sizes, depth * sizeof (unsigned));
        new_node->type_info.depth = depth;
        new_node->op = op;
        new_node->left = left;
        new_node->right = right;
        return (node_t *) new_node;
    }
}

node_t *new_invert_op_node(node_t *child, char error_msg[ERROR_MSG_LENGTH]) {
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

    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        unsigned length = get_length_of_array(child_type_info.sizes, child_type_info.depth);
        const_node_t *const_node_view_child = (const_node_t *) child;
        if (result_type == INT_T) {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_child->values[i].i_val = ~(const_node_view_child->values[i].i_val);
            }
        } else {
            for (unsigned i = 0; i < length; ++i) {
                const_node_view_child->values[i].u_val = ~(const_node_view_child->values[i].u_val);
            }
        }
        return child;
    } else if (child->node_type == INVERT_OP_NODE_T) {
        invert_op_node_t *invert_op_node_view_child = (invert_op_node_t *) child;
        node_t *result = invert_op_node_view_child->child;
        free(invert_op_node_view_child);
        return result;
    } else {
        invert_op_node_t *new_node = malloc(sizeof(invert_op_node_t));
        if (new_node == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for invert-operator node failed");
            return NULL;
        }

        new_node->node_type = INVERT_OP_NODE_T;
        new_node->is_quantizable = is_quantizable(child);
        new_node->is_unitary = is_unitary(child);
        new_node->type_info.qualifier = result_qualifier;
        new_node->type_info.type = result_type;
        memcpy(new_node->type_info.sizes, child_type_info.sizes, child_type_info.depth * sizeof (unsigned));
        new_node->type_info.depth = child_type_info.depth;
        new_node->child = child;
        return (node_t *) new_node;
    }
}

node_t *new_if_node(node_t *condition, node_t *if_branch, node_t **else_if_branches, unsigned num_of_else_ifs,
                    node_t *else_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t if_condition_type_info;
    return_style_t if_return_style = get_return_style(if_branch);
    return_style_t else_return_style = get_return_style(else_branch);
    return_style_t result_return_style = NONE_ST;
    type_info_t result_return_type_info;
    bool result_is_quantizable = is_quantizable(condition) && is_quantizable(if_branch)
                                 && is_quantizable(else_branch);
    bool result_is_unitary = is_unitary(condition);
    if (!copy_type_info_of_node(&if_condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition is not an expression");
        free_node(condition);
        free_node(if_branch);
        for (unsigned i = 0; i < num_of_else_ifs; ++i) {
            free_node(else_if_branches[i]);
        }
        free(else_if_branches);
        free_node(else_branch);
        free_symbol_table();
        return NULL;
    } else if (if_condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition must be of type bool, but is of type %s",
                 type_to_str(if_condition_type_info.type));
        free_node(condition);
        free_node(if_branch);
        for (unsigned i = 0; i < num_of_else_ifs; ++i) {
            free_node(else_if_branches[i]);
        }
        free(else_if_branches);
        free_node(else_branch);
        free_symbol_table();
        return NULL;
    } else if (if_condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "If-condition must be a single bool, but is an array of depth %u",
                 if_condition_type_info.depth);
        free_node(condition);
        free_node(if_branch);
        for (unsigned i = 0; i < num_of_else_ifs; ++i) {
            free_node(else_if_branches[i]);
        }
        free(else_if_branches);
        free_node(else_branch);
        free_symbol_table();
        return NULL;
    } else if (if_condition_type_info.qualifier == QUANTUM_T) {
        if (!is_unitary(if_branch) || if_return_style != NONE_ST) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "If-condition is quantum, but statements in if-branch are not unitary");
            free_node(condition);
            free_node(if_branch);
            for (unsigned i = 0; i < num_of_else_ifs; ++i) {
                free_node(else_if_branches[i]);
            }
            free(else_if_branches);
            free_node(else_branch);
            free_symbol_table();
            return NULL;
        } else if (else_branch != NULL && (!is_unitary(else_branch) || else_return_style != NONE_ST)) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "If-condition is quantum, but statements in else-branch are not unitary");
            free_node(condition);
            free_node(if_branch);
            for (unsigned i = 0; i < num_of_else_ifs; ++i) {
                free_node(else_if_branches[i]);
            }
            free(else_if_branches);
            free_node(else_branch);
            free_symbol_table();
            return NULL;
        }
    }

    if (if_return_style != NONE_ST) {
        copy_return_type_info_of_node(&result_return_type_info, if_branch);
        result_return_style = if_return_style;
    }

    type_info_t else_if_condition_type_info;
    for (unsigned i = 0; i < num_of_else_ifs; ++i) {
        return_style_t current_return_style = get_return_style(else_if_branches[i]);
        else_if_node_t *else_if_node_view = (else_if_node_t *) (else_if_node_t *) else_if_branches[i];
        copy_type_info_of_node(&else_if_condition_type_info, else_if_node_view->condition);

        if (else_if_condition_type_info.qualifier == QUANTUM_T && if_condition_type_info.qualifier != QUANTUM_T
            || else_if_condition_type_info.qualifier != QUANTUM_T && if_condition_type_info.qualifier == QUANTUM_T) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition %u is %s while if-condition is %s",
                     i + 1, (else_if_condition_type_info.qualifier  == QUANTUM_T) ? "quantum" : "classical",
                     (if_condition_type_info.qualifier == QUANTUM_T) ? "quantum" : "classical");
            free_node(condition);
            free_node(if_branch);
            for (unsigned j = 0; j < num_of_else_ifs; ++j) {
                free_node(else_if_branches[j]);
            }
            free(else_if_branches);
            free_node(else_branch);
            free_symbol_table();
            return NULL;
        }

        result_is_quantizable = result_is_quantizable && is_quantizable(else_if_branches[i]);
        result_is_unitary = result_is_unitary && is_unitary(else_if_branches[i]);

        if (current_return_style != NONE_ST) {
            type_info_t elif_return_type_info;
            copy_return_type_info_of_node(&elif_return_type_info, else_if_branches[i]);
            if (result_return_style != NONE_ST) {
                if (result_return_type_info.qualifier == QUANTUM_T && elif_return_type_info.qualifier != QUANTUM_T
                    || result_return_type_info.qualifier != QUANTUM_T && elif_return_type_info.qualifier == QUANTUM_T) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching qualifiers in return statements in if-branch and else-if-branch %u", i + 1);
                    return NULL;
                } else if (result_return_type_info.type != elif_return_type_info.type) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching types (%s and %s) in return statements in if-branch and else-if-branch %u",
                             type_to_str(result_return_type_info.type), type_to_str(elif_return_type_info.type), i + 1);
                    return NULL;
                } else if (result_return_type_info.depth != elif_return_type_info.depth) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching depths (%u and %u) in return statements in if-branch and else-if-branch %u",
                             result_return_type_info.depth, elif_return_type_info.depth, i + 1);
                    return NULL;
                }

                for (unsigned j = 0; j < result_return_type_info.depth; ++j) {
                    if (result_return_type_info.sizes[j] != elif_return_type_info.sizes[j]) {
                        snprintf(error_msg, ERROR_MSG_LENGTH,
                                 "Non-matching sizes in depth %u (%u and %u) in return statements in if-branch"
                                 "and else-if-branch %u",
                                 j, result_return_type_info.sizes[j], elif_return_type_info.sizes[j], i + 1);
                        return NULL;
                    }
                }
                result_return_style = (current_return_style == DEFINITE_ST && result_return_style == DEFINITE_ST) ?
                DEFINITE_ST : CONDITIONAL_ST;
            } else {
                memcpy(&result_return_type_info, &elif_return_type_info, sizeof (elif_return_type_info));
                result_return_style = CONDITIONAL_ST;
            }
        }
    }

    if (result_return_style != NONE_ST) {
        if (else_branch != NULL && else_return_style != NONE_ST) {
            type_info_t else_return_type_info;
            copy_return_type_info_of_node(&else_return_type_info, else_branch);
            if (result_return_type_info.qualifier == QUANTUM_T && else_return_type_info.qualifier != QUANTUM_T
                || result_return_type_info.qualifier != QUANTUM_T && else_return_type_info.qualifier == QUANTUM_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Non-matching qualifiers in return statements in if-branch and else-branch");
                return NULL;
            } else if (result_return_type_info.type != else_return_type_info.type) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Non-matching types (%s and %s) in return statements in if-branch and else-branch",
                         type_to_str(result_return_type_info.type), type_to_str(else_return_type_info.type));
                return NULL;
            } else if (result_return_type_info.depth != else_return_type_info.depth) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Non-matching depths (%u and %u) in return statements in if-branch and else-branch",
                         result_return_type_info.depth, else_return_type_info.depth);
                return NULL;
            }

            for (unsigned j = 0; j < result_return_type_info.depth; ++j) {
                if (result_return_type_info.sizes[j] != else_return_type_info.sizes[j]) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching sizes in depth %u (%u and %u) in return statements in if-branch"
                             " and else-branch",
                             j, result_return_type_info.sizes[j], else_return_type_info.sizes[j]);
                    return NULL;
                }
            }

            result_return_style = (else_return_style == DEFINITE_ST && result_return_style == DEFINITE_ST) ?
                                  DEFINITE_ST : CONDITIONAL_ST;
        } else {
            result_return_style = CONDITIONAL_ST;
        }
    } else if (else_branch != NULL && else_return_style != NONE_ST) {
        result_return_style = CONDITIONAL_ST;
        copy_return_type_info_of_node(&result_return_type_info, else_branch);
    }

    if_node_t *new_node = malloc(sizeof (if_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for if node failed");
        free_node(condition);
        free_node(if_branch);
        for (unsigned i = 0; i < num_of_else_ifs; ++i) {
            free_node(else_if_branches[i]);
        }
        free(else_if_branches);
        free_node(else_branch);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = IF_NODE_T;
    new_node->is_quantizable = result_is_quantizable;
    new_node->is_unitary = result_is_unitary;
    new_node->condition = condition;
    new_node->if_branch = if_branch;
    new_node->else_if_branches = else_if_branches;
    new_node->num_of_else_ifs = num_of_else_ifs;
    new_node->else_branch = else_branch;
    new_node->return_style = result_return_style;
    if (result_return_style != NONE_ST) {
        memcpy(&(new_node->return_type_info), &result_return_type_info, sizeof (result_return_type_info));
    }
    return (node_t *) new_node;
}

node_t *new_else_if_node(node_t *condition, node_t *else_if_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    return_style_t else_if_return_style = get_return_style(else_if_branch);
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition is not an expression");
        free_node(condition);
        free_node(else_if_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        free_node(condition);
        free_node(else_if_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        free_node(condition);
        free_node(else_if_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T && (!(((stmt_list_node_t *) else_if_branch)->is_unitary)
                || else_if_return_style != NONE_ST)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Else-if-condition is quantum, but statements are not unitary");
        free_node(condition);
        free_node(else_if_branch);
        free_symbol_table();
        return NULL;
    }

    else_if_node_t *new_node = malloc(sizeof (else_if_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for else-if node failed");
        free_node(condition);
        free_node(else_if_branch);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = ELSE_IF_NODE_T;
    if (else_if_return_style == NONE_ST) {
        new_node->is_quantizable = is_quantizable(condition) && is_quantizable(else_if_branch);
        new_node->is_unitary = is_unitary(condition);
    } else {
        new_node->is_quantizable = false;
        new_node->is_unitary = false;
        copy_return_type_info_of_node(&(new_node->return_type_info), else_if_branch);
    }
    new_node->condition = condition;
    new_node->else_if_branch = else_if_branch;
    new_node->return_style = else_if_return_style;
    return (node_t *) new_node;
}

node_t *new_switch_node(node_t *expression, node_t **case_branches, unsigned num_of_cases,
                        char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t expression_type_info;
    return_style_t result_return_style = NONE_ST;
    type_info_t result_return_type_info;
    bool result_is_quantizable = is_quantizable(expression);
    bool result_is_unitary = is_unitary(expression);
    if (!copy_type_info_of_node(&expression_type_info, expression)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "No valid switch-expression");
        free_node(expression);
        for (unsigned i = 0; i < num_of_cases; ++i) {
            free_node(case_branches[i]);
        }
        free(case_branches);
        free_symbol_table();
        return NULL;
    } else if (expression_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Switch-expression must be a scalar, but is an array of depth %u",
                 expression_type_info.depth);
        free_node(expression);
        for (unsigned i = 0; i < num_of_cases; ++i) {
            free_node(case_branches[i]);
        }
        free(case_branches);
        free_symbol_table();
        return NULL;
    }
    bool has_default_case = false;
    for (unsigned i = 0; i < num_of_cases; ++i) {
        return_style_t current_return_style = get_return_style(case_branches[i]);
        case_node_t *case_node_view = (case_node_t *) case_branches[i];
        stmt_list_node_t *case_branch_view = (stmt_list_node_t *) case_node_view->case_branch;
        if (case_node_view->case_const_type != VOID_T
            && !are_matching_types(expression_type_info.type, case_node_view->case_const_type)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Case %u is of type %s while switch-expression is of type %s",
                     i + 1, type_to_str(case_node_view->case_const_type), type_to_str(expression_type_info.type));
            free_node(expression);
            for (unsigned j = 0; j < num_of_cases; ++j) {
                free_node(case_branches[j]);
            }
            free(case_branches);
            free_symbol_table();
            return NULL;
        } else if (expression_type_info.qualifier == QUANTUM_T && !(case_node_view->is_unitary)) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Switch-expression is quantum, but statements in case %u are not unitary", i + 1);
            free_node(expression);
            for (unsigned j = 0; j < num_of_cases; ++j) {
                free_node(case_branches[j]);
            }
            free(case_branches);
            free_symbol_table();
            return NULL;
        }

        if (current_return_style != NONE_ST) {
            type_info_t case_return_type_info;
            copy_return_type_info_of_node(&case_return_type_info, case_branches[i]);
            if (result_return_style != NONE_ST) {
                if (result_return_type_info.qualifier == QUANTUM_T && case_return_type_info.qualifier != QUANTUM_T
                    || result_return_type_info.qualifier != QUANTUM_T && case_return_type_info.qualifier == QUANTUM_T) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching qualifiers in return statements in case-branches");
                    return NULL;
                } else if (result_return_type_info.type != case_return_type_info.type) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching types (%s and %s) in return statements in case-branches",
                             type_to_str(result_return_type_info.type), type_to_str(case_return_type_info.type));
                    return NULL;
                } else if (result_return_type_info.depth != case_return_type_info.depth) {
                    snprintf(error_msg, ERROR_MSG_LENGTH,
                             "Non-matching depths (%u and %u) in return statements in case-branches",
                             result_return_type_info.depth, case_return_type_info.depth);
                    return NULL;
                }

                for (unsigned j = 0; j < result_return_type_info.depth; ++j) {
                    if (result_return_type_info.sizes[j] != case_return_type_info.sizes[j]) {
                        snprintf(error_msg, ERROR_MSG_LENGTH,
                                 "Non-matching sizes in depth %u (%u and %u) in return statements in case-branches",
                                 j, result_return_type_info.sizes[j], case_return_type_info.sizes[j]);
                        return NULL;
                    }
                }
                result_return_style = (current_return_style == DEFINITE_ST && result_return_style == DEFINITE_ST) ?
                                      DEFINITE_ST : CONDITIONAL_ST;
            } else {
                memcpy(&result_return_type_info, &case_return_type_info, sizeof (case_return_type_info));
                result_return_style = CONDITIONAL_ST;
            }
        }

        if (case_node_view->case_const_type == VOID_T) {
            has_default_case = true;
            if (i < num_of_cases - 1) { /* default statement reached */
                for (unsigned j = i + 1; j < num_of_cases; ++j) {
                    free_node(case_branches[j]);
                }
                num_of_cases = i + 1;
                node_t **temp = realloc(case_branches, num_of_cases * sizeof(node_t *));
                if (temp == NULL) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Reallocating case list in switch node failed");
                    free_node(expression);
                    for (unsigned j = 0; j < num_of_cases; ++j) {
                        free_node(case_branches[j]);
                    }
                    free(case_branches);
                    free_symbol_table();
                    return NULL;
                }
                case_branches = temp;
                break;
            }
        }

        for (unsigned j = 0; j < i; ++j) {
            case_node_t *prior_case_node_view = (case_node_t *) case_branches[j];
            if ((case_node_view->case_const_type == BOOL_T
                 && case_node_view->case_const_value.b_val == prior_case_node_view->case_const_value.b_val)
                || (case_node_view->case_const_type == INT_T
                    && case_node_view->case_const_value.i_val == prior_case_node_view->case_const_value.i_val)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Cases %u and %u have the same value", j + 1, i + 1);
                free_node(expression);
                for (unsigned k = 0; k < num_of_cases; ++k) {
                    free_node(case_branches[k]);
                }
                free(case_branches);
                free_symbol_table();
                return NULL;
            }
        }
    }

    for (unsigned i = 0; i < num_of_cases; ++i) {
        result_is_quantizable = result_is_quantizable && is_quantizable(case_branches[i]);
        result_is_unitary = result_is_unitary && is_unitary(case_branches[i]);
    }
    if (!has_default_case) {
        result_return_style = (result_return_style == NONE_ST) ? NONE_ST : CONDITIONAL_ST;
    }
    switch_node_t *new_node = malloc(sizeof (switch_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for switch node failed");
        free_node(expression);
        for (unsigned i = 0; i < num_of_cases; ++i) {
            free_node(case_branches[i]);
        }
        free(case_branches);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = SWITCH_NODE_T;
    new_node->is_quantizable = result_is_quantizable;
    new_node->is_unitary = result_is_unitary;
    new_node->expression = expression;
    new_node->case_branches = case_branches;
    new_node->num_of_cases = num_of_cases;
    new_node->return_style = result_return_style;
    if (result_return_style != NONE_ST) {
        memcpy(&(new_node->return_type_info), &result_return_type_info, sizeof (result_return_type_info));
    }
    return (node_t *) new_node;
}

node_t *new_case_node(node_t *case_const, node_t *case_branch, char error_msg[ERROR_MSG_LENGTH]) {
    return_style_t case_return_style = get_return_style(case_branch);
    case_node_t *new_node = malloc(sizeof (case_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for case node failed");
        free_node(case_const);
        free_node(case_branch);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = CASE_NODE_T;
    if (case_return_style == NONE_ST) {
        new_node->is_quantizable = is_quantizable(case_branch);
        new_node->is_unitary = is_unitary(case_branch);
    } else {
        new_node->is_quantizable = false;
        new_node->is_unitary = false;
        copy_return_type_info_of_node(&(new_node->return_type_info), case_branch);
    }
    if (case_const != NULL) {
        new_node->case_const_type = ((const_node_t *) case_const)->type_info.type;
        new_node->case_const_value = ((const_node_t *) case_const)->values[0];
        free(case_const);
    } else {
        new_node->case_const_type = VOID_T;
    }
    new_node->case_branch = case_branch;
    new_node->return_style = case_return_style;
    return (node_t *) new_node;
}

node_t *new_for_node(node_t *initialize, node_t *condition, node_t *increment, node_t *for_branch,
                     char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition is not an expression");
        free_node(initialize);
        free_node(condition);
        free_node(increment);
        free_node(for_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop condition cannot be quantum");
        free_node(initialize);
        free_node(condition);
        free_node(increment);
        free_node(for_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        free_node(initialize);
        free_node(condition);
        free_node(increment);
        free_node(for_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "For-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        free_node(initialize);
        free_node(condition);
        free_node(increment);
        free_node(for_branch);
        free_symbol_table();
        return NULL;
    }

    for_node_t *new_node = malloc(sizeof (for_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for for-loop node failed");
        free_node(initialize);
        free_node(condition);
        free_node(increment);
        free_node(for_branch);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = FOR_NODE_T;
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
        free_node(do_branch);
        free_node(condition);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Do-while-loop-condition cannot be quantum");
        free_node(do_branch);
        free_node(condition);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Do-while-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        free_node(do_branch);
        free_node(condition);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Do-while-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        free_node(do_branch);
        free_node(condition);
        free_symbol_table();
        return NULL;
    }

    do_node_t *new_node = malloc(sizeof (do_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for do-while-loop node failed");
        free_node(do_branch);
        free_node(condition);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = DO_NODE_T;
    new_node->do_branch = do_branch;
    new_node->condition = condition;
    return (node_t *) new_node;
}

node_t *new_while_node(node_t *condition, node_t *while_branch, char error_msg[ERROR_MSG_LENGTH]) {
    type_info_t condition_type_info;
    if (!copy_type_info_of_node(&condition_type_info, condition)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition is not an expression");
        free_node(condition);
        free_node(while_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition cannot be quantum");
        free_node(condition);
        free_node(while_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.type != BOOL_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition must be of type bool, but is of type %s",
                 type_to_str(condition_type_info.type));
        free_node(condition);
        free_node(while_branch);
        free_symbol_table();
        return NULL;
    } else if (condition_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "While-loop-condition must be a single bool, but is an array of depth %u",
                 condition_type_info.depth);
        free_node(condition);
        free_node(while_branch);
        free_symbol_table();
        return NULL;
    }

    while_node_t *new_node = malloc(sizeof (while_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for while-loop node failed");
        free_node(condition);
        free_node(while_branch);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = WHILE_NODE_T;
    new_node->condition = condition;
    new_node->while_branch = while_branch;
    return (node_t *) new_node;
}

node_t *new_assign_node(node_t *left, assign_op_t op, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    if (left->node_type == CONST_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to reassign constant value");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (left->node_type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left-hand side of assignment is not a variable");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    type_info_t left_type_info;
    copy_type_info_of_node(&left_type_info, left);
    type_info_t right_type_info;
    if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of assignment is not an expression");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (left_type_info.qualifier != QUANTUM_T && right_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Classical left-hand side of assignment, but quantum right-hand side");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    switch (op) {
        case ASSIGN_OP: {
            if (!are_matching_types(left_type_info.type, right_type_info.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Assigning %s to %s",
                         type_to_str(right_type_info.type), type_to_str(left_type_info.type));
                free_node(left);
                free_node(right);
                free_symbol_table();
                return NULL;
            }
            break;
        }
        case ASSIGN_OR_OP: case ASSIGN_XOR_OP: case ASSIGN_AND_OP: {
            if (!are_matching_types(left_type_info.type, right_type_info.type)) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "%s Assigning %s to %s",
                         (left_type_info.type == BOOL_T) ? "Logically" : "Bitwisely", type_to_str(right_type_info.type),
                         type_to_str(left_type_info.type));
                free_node(left);
                free_node(right);
                free_symbol_table();
                return NULL;
            }
            break;
        }
        case ASSIGN_ADD_OP: case ASSIGN_SUB_OP: case ASSIGN_MUL_OP: case ASSIGN_DIV_OP: case ASSIGN_MOD_OP: {
            if (left_type_info.type == BOOL_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning to bool");
                free_node(left);
                free_node(right);
                free_symbol_table();
                return NULL;
            } else if (right_type_info.type == BOOL_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning bool to %s",
                         type_to_str(left_type_info.type));
                free_node(left);
                free_node(right);
                free_symbol_table();
                return NULL;
            } else if (left_type_info.type == INT_T && right_type_info.type == UNSIGNED_T) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Arithmetically assigning unsigned to int");
                free_node(left);
                free_node(right);
                free_symbol_table();
                return NULL;
            }
            break;
        }
    }

    if (left_type_info.depth == 0 && right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand side of \"%s\" is a scalar, right-hand side is an array of depth %u)",
                 assign_op_to_str(op), right_type_info.depth);
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (left_type_info.depth != 0 && right_type_info.depth == 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand side of \"%s\" is an array of depth %u, right-hand side is a scalar)",
                 assign_op_to_str(op), left_type_info.depth);
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (left_type_info.depth != right_type_info.depth) {
        snprintf(error_msg, ERROR_MSG_LENGTH,
                 "Left-hand and right-hand side of \"%s\" are arrays of different depth (%u != %u)",
                 assign_op_to_str(op), left_type_info.depth, right_type_info.depth);
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    unsigned depth = left_type_info.depth;
    for (unsigned i = 0; i < depth; ++i) {
        if (left_type_info.sizes[i] != right_type_info.sizes[i]) {
            snprintf(error_msg, ERROR_MSG_LENGTH,
                     "Left-hand and right-hand side of \"%s\" are arrays of different sizes in dimension %u (%u != %u)",
                     assign_op_to_str(op), depth, left_type_info.sizes[i], right_type_info.sizes[i]);
            free_node(left);
            free_node(right);
            free_symbol_table();
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
                    free_node(left);
                    free_node(right);
                    free_symbol_table();
                    return NULL;
                }
            }
        } else if (op == ASSIGN_MOD_OP) {
            for (unsigned i = 0; i < length; ++i) {
                if (const_node_view_right->values[i].i_val == 0) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Modulo by zero");
                    free_node(left);
                    free_node(right);
                    free_symbol_table();
                    return NULL;
                }
            }
        }
    }

    assign_node_t *new_node = malloc(sizeof (assign_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for assignment node failed");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = ASSIGN_NODE_T;
    new_node->is_quantizable = is_quantizable(left) && is_quantizable(right);
    new_node->is_unitary = is_unitary(left) && is_unitary(right);
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_phase_node(node_t *left, bool is_positive, node_t *right, char error_msg[ERROR_MSG_LENGTH]) {
    if (left->node_type == CONST_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to change the phase of a classical value");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (left->node_type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Left-hand side of assignment is not a variable");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    type_info_t left_type_info;
    copy_type_info_of_node(&left_type_info, left);
    if (left_type_info.qualifier != QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to change the phase of a classical value");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    type_info_t right_type_info;
    if (!copy_type_info_of_node(&right_type_info, right)) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of phase change is not an expression");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (right_type_info.qualifier == QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Cannot change the phase by a quantum value");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (right_type_info.type == BOOL_T || right_type_info.type == VOID_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Cannot change the phase by a value of type %s",
                 type_to_str(right_type_info.type));
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    } else if (right_type_info.depth != 0) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Right-hand side of phase change is an array of depth %u",
                 right_type_info.depth);
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    phase_node_t *new_node = malloc(sizeof (phase_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for phase node failed");
        free_node(left);
        free_node(right);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = PHASE_NODE_T;
    new_node->is_unitary = is_unitary(right);
    new_node->is_positive = is_positive;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_measure_node(node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    if (node->node_type == CONST_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to measure a classical variable");
        free_node(node);
        free_symbol_table();
        return NULL;
    } else if (node->node_type != REFERENCE_NODE_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to measure a non-variable");
        free_node(node);
        free_symbol_table();
        return NULL;
    }

    type_info_t type_info;
    copy_type_info_of_node(&type_info, node);
    if (type_info.qualifier != QUANTUM_T) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Trying to measure a classical variable");
        free_node(node);
        free_symbol_table();
        return NULL;
    }

    measure_node_t *new_node = malloc(sizeof (measure_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for measure node failed");
        free_node(node);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = MEASURE_NODE_T;
    new_node->type_info = type_info;
    new_node->type_info.qualifier = NONE_T;
    new_node->child = node;
    return (node_t *) new_node;
}

node_t *new_break_node(char error_msg[ERROR_MSG_LENGTH]) {
    break_node_t *new_node = malloc(sizeof (break_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for break node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = BREAK_NODE_T;
    return (node_t *) new_node;
}

node_t *new_continue_node(char error_msg[ERROR_MSG_LENGTH]) {
    continue_node_t *new_node = malloc(sizeof (continue_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for continue node failed");
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = CONTINUE_NODE_T;
    return (node_t *) new_node;
}

node_t *new_return_node(node_t *node, char error_msg[ERROR_MSG_LENGTH]) {
    return_node_t *new_node = malloc(sizeof (return_node_t));
    if (new_node == NULL) {
        snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for return node failed");
        free_node(node);
        free_symbol_table();
        return NULL;
    }

    new_node->node_type = RETURN_NODE_T;
    if (node != NULL) {
        new_node->is_quantizable = is_quantizable(node);
        new_node->is_unitary = is_unitary(node);
        if (!copy_type_info_of_node(&(new_node->type_info), node)) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Return value is not an expression");
            free_node(node);
            free_symbol_table();
            return NULL;
        }
        new_node->type_info.qualifier = (new_node->type_info.qualifier == QUANTUM_T) ? QUANTUM_T : NONE_T;
    } else {
        new_node->is_quantizable = true;
        new_node->is_unitary = true;
        new_node->type_info.qualifier = NONE_T;
        new_node->type_info.type = VOID_T;
        new_node->type_info.depth = 0;
    }
    new_node->return_value = node;
    return (node_t *) new_node;
}

bool copy_return_type_info_of_node(type_info_t *type_info, const node_t *node) {
    if (node == NULL) {
        return false;
    }

    switch (node->node_type) {
        case STMT_LIST_NODE_T: {
            type_info->qualifier = ((stmt_list_node_t *) node)->return_type_info.qualifier;
            type_info->type = ((stmt_list_node_t *) node)->return_type_info.type;
            memcpy(type_info->sizes, ((stmt_list_node_t *) node)->return_type_info.sizes,
                   ((stmt_list_node_t *) node)->return_type_info.depth * sizeof (unsigned));
            type_info->depth = ((stmt_list_node_t *) node)->return_type_info.depth;
            return true;
        }
        case IF_NODE_T: {
            type_info->qualifier = ((if_node_t *) node)->return_type_info.qualifier;
            type_info->type = ((if_node_t *) node)->return_type_info.type;
            memcpy(type_info->sizes, ((if_node_t *) node)->return_type_info.sizes,
                   ((if_node_t *) node)->return_type_info.depth * sizeof (unsigned));
            type_info->depth = ((if_node_t *) node)->return_type_info.depth;
            return true;
        }
        case ELSE_IF_NODE_T: {
            type_info->qualifier = ((else_if_node_t *) node)->return_type_info.qualifier;
            type_info->type = ((else_if_node_t *) node)->return_type_info.type;
            memcpy(type_info->sizes, ((else_if_node_t *) node)->return_type_info.sizes,
                   ((else_if_node_t *) node)->return_type_info.depth * sizeof (unsigned));
            type_info->depth = ((else_if_node_t *) node)->return_type_info.depth;
            return true;
        }
        case SWITCH_NODE_T: {
            type_info->qualifier = ((switch_node_t *) node)->return_type_info.qualifier;
            type_info->type = ((switch_node_t *) node)->return_type_info.type;
            memcpy(type_info->sizes, ((switch_node_t *) node)->return_type_info.sizes,
                   ((switch_node_t *) node)->return_type_info.depth * sizeof (unsigned));
            type_info->depth = ((switch_node_t *) node)->return_type_info.depth;
            return true;
        }
        case CASE_NODE_T: {
            type_info->qualifier = ((case_node_t *) node)->return_type_info.qualifier;
            type_info->type = ((case_node_t *) node)->return_type_info.type;
            memcpy(type_info->sizes, ((case_node_t *) node)->return_type_info.sizes,
                   ((case_node_t *) node)->return_type_info.depth * sizeof (unsigned));
            type_info->depth = ((case_node_t *) node)->return_type_info.depth;
            return true;
        }
        case RETURN_NODE_T: {
            type_info->qualifier = ((return_node_t *) node)->type_info.qualifier;
            type_info->type = ((return_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((return_node_t *) node)->type_info.sizes,
                   ((return_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((return_node_t *) node)->type_info.depth;
            return true;
        }
        default: {
            return false;
        }
    }
}

return_style_t get_return_style(const node_t *node) {
    if (node == NULL) {
        return NONE_ST;
    }

    switch (node->node_type) {
        case STMT_LIST_NODE_T: {
            return ((stmt_list_node_t *) node)->return_style;
        }
        case IF_NODE_T: {
            return ((if_node_t *) node)->return_style;
        }
        case ELSE_IF_NODE_T: {
            return ((else_if_node_t *) node)->return_style;
        }
        case SWITCH_NODE_T: {
            return ((switch_node_t *) node)->return_style;
        }
        case CASE_NODE_T: {
            return ((case_node_t *) node)->return_style;
        }
        case RETURN_NODE_T: {
            return DEFINITE_ST;
        }
        default: {
            return NONE_ST;
        }
    }
}

bool is_quantizable(const node_t *node) {
    if (node == NULL) {
        return false;
    }

    switch (node->node_type) {
        case STMT_LIST_NODE_T: {
            return ((stmt_list_node_t *) node)->is_quantizable;
        }
        case FUNC_SP_NODE_T: {
            return true;
        }
        case VAR_DECL_NODE_T: {
            return ((var_decl_node_t *) node)->entry->qualifier != QUANTUM_T;
        }
        case VAR_DEF_NODE_T: {
            return ((var_def_node_t *) node)->is_quantizable;
        }
        case CONST_NODE_T: {
            return true;
        }
        case REFERENCE_NODE_T: {
            return ((reference_node_t *) node)->is_quantizable;
        }
        case FUNC_CALL_NODE_T: {
            return ((func_call_node_t *) node)->is_quantizable;
        }
        case LOGICAL_OP_NODE_T: {
            return ((logical_op_node_t *) node)->is_quantizable;
        }
        case COMPARISON_OP_NODE_T: {
            return ((comparison_op_node_t *) node)->is_quantizable;
        }
        case EQUALITY_OP_NODE_T: {
            return ((equality_op_node_t *) node)->is_quantizable;
        }
        case NOT_OP_NODE_T: {
            return ((not_op_node_t *) node)->is_quantizable;
        }
        case INTEGER_OP_NODE_T: {
            return ((integer_op_node_t *) node)->is_quantizable;
        }
        case INVERT_OP_NODE_T: {
            return ((invert_op_node_t *) node)->is_quantizable;
        }
        case IF_NODE_T: {
            return ((if_node_t *) node)->is_quantizable;
        }
        case ELSE_IF_NODE_T: {
            return ((else_if_node_t *) node)->is_quantizable;
        }
        case SWITCH_NODE_T: {
            return ((switch_node_t *) node)->is_quantizable;
        }
        case CASE_NODE_T: {
            return ((case_node_t *) node)->is_quantizable;
        }
        case ASSIGN_NODE_T: {
            return ((assign_node_t *) node)->is_quantizable;
        }
        case PHASE_NODE_T: {
            return false;
        }
        case RETURN_NODE_T: {
            return ((return_node_t *) node)->is_quantizable;
        }
        default: {
            return false;
        }
    }
}

bool is_unitary(const node_t *node) {
    if (node == NULL) {
        return false;
    }

    switch (node->node_type) {
        case STMT_LIST_NODE_T: {
            return ((stmt_list_node_t *) node)->is_unitary;
        }
        case VAR_DECL_NODE_T: {
            return ((var_decl_node_t *) node)->entry->qualifier == QUANTUM_T;
        }
        case VAR_DEF_NODE_T: {
            return ((var_def_node_t *) node)->is_unitary;
        }
        case CONST_NODE_T: {
            return true;
        }
        case REFERENCE_NODE_T: {
            return ((reference_node_t *) node)->is_unitary;
        }
        case FUNC_CALL_NODE_T: {
            return ((func_call_node_t *) node)->is_unitary;
        }
        case FUNC_SP_NODE_T: {
            return true;
        }
        case LOGICAL_OP_NODE_T: {
            return ((logical_op_node_t *) node)->is_unitary;
        }
        case COMPARISON_OP_NODE_T: {
            return ((comparison_op_node_t *) node)->is_unitary;
        }
        case EQUALITY_OP_NODE_T: {
            return ((equality_op_node_t *) node)->is_unitary;
        }
        case NOT_OP_NODE_T: {
            return ((not_op_node_t *) node)->is_unitary;
        }
        case INTEGER_OP_NODE_T: {
            return ((integer_op_node_t *) node)->is_unitary;
        }
        case INVERT_OP_NODE_T: {
            return ((invert_op_node_t *) node)->is_unitary;
        }
        case IF_NODE_T: {
            return ((if_node_t *) node)->is_unitary;
        }
        case ELSE_IF_NODE_T: {
            return ((else_if_node_t *) node)->is_unitary;
        }
        case SWITCH_NODE_T: {
            return ((switch_node_t *) node)->is_unitary;
        }
        case CASE_NODE_T: {
            return ((case_node_t *) node)->is_unitary;
        }
        case ASSIGN_NODE_T: {
            return ((assign_node_t *) node)->is_unitary;
        }
        case PHASE_NODE_T: {
            return ((phase_node_t *) node)->is_unitary;
        }
        case RETURN_NODE_T: {
            return ((return_node_t *) node)->is_unitary;
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
    switch (node->node_type) {
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
                   ((const_node_t *) node)->type_info.depth * sizeof (unsigned));
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
                   ((reference_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((reference_node_t *) node)->type_info.depth;
            return true;
        }
        case ASSIGN_NODE_T: {
            switch (((assign_node_t *) node)->left->node_type) {
                case CONST_NODE_T: {
                    type_info->qualifier = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.qualifier;
                    type_info->type = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.type;
                    memcpy(type_info->sizes, ((const_node_t *) ((assign_node_t *) node)->left)->type_info.sizes,
                           ((const_node_t *) ((assign_node_t *) node)->left)->type_info.depth * sizeof (unsigned));
                    type_info->depth = ((const_node_t *) ((assign_node_t *) node)->left)->type_info.depth;
                    return true;
                }
                case REFERENCE_NODE_T: {
                    type_info->qualifier = ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.qualifier;
                    type_info->type = ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.type;
                    memcpy(type_info->sizes, ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.sizes,
                           ((reference_node_t *) ((assign_node_t *) node)->left)->type_info.depth * sizeof (unsigned));
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
                   ((logical_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((logical_op_node_t *) node)->type_info.depth;
            return true;
        }
        case COMPARISON_OP_NODE_T: {
            type_info->qualifier = ((comparison_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((comparison_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((comparison_op_node_t *) node)->type_info.sizes,
                   ((comparison_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((comparison_op_node_t *) node)->type_info.depth;
            return true;
        }
        case EQUALITY_OP_NODE_T: {
            type_info->qualifier = ((equality_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((equality_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((equality_op_node_t *) node)->type_info.sizes,
                   ((equality_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((equality_op_node_t *) node)->type_info.depth;
            return true;
        }
        case NOT_OP_NODE_T: {
            type_info->qualifier = ((not_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((not_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((not_op_node_t *) node)->type_info.sizes,
                   ((not_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((not_op_node_t *) node)->type_info.depth;
            return true;
        }
        case INTEGER_OP_NODE_T: {
            type_info->qualifier = ((integer_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((integer_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((integer_op_node_t *) node)->type_info.sizes,
                   ((integer_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((integer_op_node_t *) node)->type_info.depth;
            return true;
        }
        case INVERT_OP_NODE_T: {
            type_info->qualifier = ((invert_op_node_t *) node)->type_info.qualifier;
            type_info->type = ((invert_op_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((invert_op_node_t *) node)->type_info.sizes,
                   ((invert_op_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((invert_op_node_t *) node)->type_info.depth;
            return true;
        }
        case MEASURE_NODE_T: {
            type_info->qualifier = ((measure_node_t *) node)->type_info.qualifier;
            type_info->type = ((measure_node_t *) node)->type_info.type;
            memcpy(type_info->sizes, ((measure_node_t *) node)->type_info.sizes,
                   ((measure_node_t *) node)->type_info.depth * sizeof (unsigned));
            type_info->depth = ((measure_node_t *) node)->type_info.depth;
            return true;
        }
        case FUNC_CALL_NODE_T: {
            copy_type_info_of_entry(type_info, ((func_call_node_t *) node)->entry);
            return true;
        }
        default: {
            return false;
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

void fprint_const_value(FILE *output_file, type_t type, value_t value) {
    switch (type) {
        case VOID_T: {
            return;
        }
        case BOOL_T: {
            if (value.b_val) {
                fprintf(output_file, "true");
            } else {
                fprintf(output_file, "false");
            }
            return;
        }
        case INT_T: {
            fprintf(output_file, "%d", value.i_val);
            return;
        }
        case UNSIGNED_T: {
            fprintf(output_file, "%u", value.u_val);
        }
        return;
    }
}

void fprint_type_info(FILE *output_file, const type_info_t *type_info) {
    switch (type_info->qualifier) {
        case NONE_T: {
            break;
        }
        case CONST_T: {
            fprintf(output_file, "const ");
            break;
        }
        case QUANTUM_T: {
            fprintf(output_file, "quantum ");
            break;
        }
    }
    fprintf(output_file, "%s", type_to_str(type_info->type));
    for (unsigned i = 0; i < type_info->depth; ++i) {
        fprintf(output_file, "[%u]", type_info->sizes[i]);
    }
}

void fprint_node(FILE *output_file, const node_t *node) {
    type_info_t type_info;
    switch (node->node_type) {
        case BASIC_NODE_T: {
            fprintf(output_file, "Basic node\n");
            break;
        }
        case STMT_LIST_NODE_T: {
            fprintf(output_file, "Statement list node with %u statements\n", ((stmt_list_node_t *) node)->num_of_stmts);
            break;
        }
        case FUNC_DECL_NODE_T: {
            fprintf(output_file, "Function declaration node for %s\n", ((func_decl_node_t *) node)->entry->name);
            break;
        }
        case VAR_DECL_NODE_T: {
            var_decl_node_t *var_decl_node_view = ((var_decl_node_t *) node);
            fprintf(output_file, "Declaration: ");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, " %s\n", var_decl_node_view->entry->name);
            break;
        }
        case VAR_DEF_NODE_T: {
            var_def_node_t *var_def_node_view = ((var_def_node_t *) node);
            fprintf(output_file, "Definition: ");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, " %s\n", var_def_node_view->entry->name);
            break;
        }
        case CONST_NODE_T: {
            copy_type_info_of_node(&type_info, node);
            if (type_info.depth == 0) {
                switch (type_info.type) {
                    case BOOL_T: {
                        fprintf(output_file, "%s\n", ((const_node_t *) node)->values[0].b_val ? "true" : "false");
                        break;
                    }
                    case INT_T: {
                        fprintf(output_file, "%d\n", ((const_node_t *) node)->values[0].i_val);
                        break;
                    }
                    case UNSIGNED_T: {
                        fprintf(output_file, "%u\n", ((const_node_t *) node)->values[0].u_val);
                        break;
                    }
                    case VOID_T: {
                        fprintf(output_file, "undefined\n");
                        break;
                    }
                }
            } else {
                fprintf(output_file, "{");
                for (unsigned i = 0; i < get_length_of_array(type_info.sizes, type_info.depth); ++i) {
                    if (i != 0) {
                        fprintf(output_file, ", ");
                    }
                    switch (type_info.type) {
                        case BOOL_T: {
                            fprintf(output_file, "%s", ((const_node_t *) node)->values[i].b_val ? "true" : "false");
                            break;
                        }
                        case INT_T: {
                            fprintf(output_file, "%d", ((const_node_t *) node)->values[i].i_val);
                            break;
                        }
                        case UNSIGNED_T: {
                            fprintf(output_file, "%u", ((const_node_t *) node)->values[i].u_val);
                            break;
                        }
                        case VOID_T: {
                            fprintf(output_file, "undefined");
                            break;
                        }
                    }
                }
                fprintf(output_file, "}\n");
            }
            break;
        }
        case REFERENCE_NODE_T: {
            copy_type_info_of_node(&type_info, node);
            fprintf(output_file, "Reference to ");
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, " %s\n", ((reference_node_t *) node)->entry->name);
            break;
        }
        case FUNC_CALL_NODE_T: {
            if (((func_call_node_t *) node)->sp) {
                fprintf(output_file, "%s[%s](...) -> (", (((func_call_node_t *) node)->inverse) ? "~" : "",
                        ((func_call_node_t *) node)->entry->name);
                fprint_type_info(output_file, &(((func_call_node_t *) node)->type_info));
                fprintf(output_file, ")\n");
            } else {
                fprintf(output_file, "%s%s(...) -> (", (((func_call_node_t *) node)->inverse) ? "~" : "",
                        ((func_call_node_t *) node)->entry->name);
                fprint_type_info(output_file, &(((func_call_node_t *) node)->type_info));
                fprintf(output_file, ")\n");
            }
            break;
        }
        case FUNC_SP_NODE_T: {
            fprintf(output_file, "Function superposition node for %s\n", ((func_sp_node_t *) node)->entry->name);
            break;
        }
        case LOGICAL_OP_NODE_T: {
            fprintf(output_file, "(");
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s (", logical_op_to_str(((logical_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((logical_op_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case COMPARISON_OP_NODE_T: {
            fprintf(output_file, "(");
            copy_type_info_of_node(&type_info, ((comparison_op_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s (", comparison_op_to_str(((comparison_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((comparison_op_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case EQUALITY_OP_NODE_T: {
            fprintf(output_file, "(");
            copy_type_info_of_node(&type_info, ((equality_op_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s (", equality_op_to_str(((equality_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((equality_op_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case NOT_OP_NODE_T: {
            fprintf(output_file, "!(");
            copy_type_info_of_node(&type_info, ((not_op_node_t *) node)->child);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case INTEGER_OP_NODE_T: {
            fprintf(output_file, "(");
            copy_type_info_of_node(&type_info, ((integer_op_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s (", integer_op_to_str(((integer_op_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((integer_op_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case INVERT_OP_NODE_T: {
            fprintf(output_file, "~(");
            copy_type_info_of_node(&type_info, ((invert_op_node_t *) node)->child);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case IF_NODE_T: {
            fprintf(output_file, "If statement with %u \"else if\"s and%s \"else\"\n",
                   ((if_node_t *) node)->num_of_else_ifs, (((if_node_t *) node)->else_branch != NULL) ? "" : " no");
            break;
        }
        case ELSE_IF_NODE_T: {
            fprintf(output_file, "Else-if branch\n");
            break;
        }
        case SWITCH_NODE_T: {
            fprintf(output_file, "Switch statement with %u cases\n", ((switch_node_t *) node)->num_of_cases);
            break;
        }
        case CASE_NODE_T: {
            if (((case_node_t *) node)->case_const_type == VOID_T) {
                fprintf(output_file, "default:\n");
            } else {
                fprintf(output_file, "case ");
                fprint_const_value(output_file, ((case_node_t *) node)->case_const_type,
                                  ((case_node_t *) node)->case_const_value);
                fprintf(output_file, ":\n");
            }
            break;
        }
        case FOR_NODE_T: {
            fprintf(output_file, "For loop\n");
            break;
        }
        case DO_NODE_T: {
            fprintf(output_file, "Do-while loop\n");
            break;
        }
        case WHILE_NODE_T: {
            fprintf(output_file, "While loop\n");
            break;
        }
        case ASSIGN_NODE_T: {
            fprintf(output_file, "(");
            copy_type_info_of_node(&type_info, ((assign_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s (", assign_op_to_str(((assign_node_t *) node)->op));
            copy_type_info_of_node(&type_info, ((assign_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            copy_type_info_of_node(&type_info, node);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case PHASE_NODE_T: {
            fprintf(output_file, "phase (");
            copy_type_info_of_node(&type_info, ((phase_node_t *) node)->left);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") %s= (", (((phase_node_t *) node)->is_positive) ? "+" : "-");
            copy_type_info_of_node(&type_info, ((phase_node_t *) node)->right);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ")\n");
            break;
        }
        case MEASURE_NODE_T: {
            fprintf(output_file, "measure (");
            copy_type_info_of_node(&type_info, ((measure_node_t *) node)->child);
            fprint_type_info(output_file, &type_info);
            fprintf(output_file, ") -> (");
            fprint_type_info(output_file, &(((measure_node_t *) node)->type_info));
            fprintf(output_file, ")\n");
            break;
        }
        case BREAK_NODE_T: {
            fprintf(output_file, "Break\n");
            break;
        }
        case CONTINUE_NODE_T: {
            fprintf(output_file, "Continue\n");
            break;
        }
        case RETURN_NODE_T: {
            fprintf(output_file, "Return ");
            fprint_type_info(output_file, &(((return_node_t *) node)->type_info));
            fprintf(output_file, "\n");
            break;
        }
    }
}

void fprint_tree(FILE *output_file, const node_t *node) {
    if (node == NULL) {
        return;
    }
    fprint_node(output_file, node);
    switch (node->node_type) {
        case BASIC_NODE_T: {
            fprint_tree(output_file, node->left);
            fprint_tree(output_file, node->right);
            break;
        }
        case STMT_LIST_NODE_T: {
            for (unsigned i = 0; i < ((stmt_list_node_t *) node)->num_of_stmts; ++i) {
                fprint_tree(output_file, ((stmt_list_node_t *) node)->stmt_list[i]);
            }
            break;
        }
        case FUNC_DECL_NODE_T: {
            fprint_tree(output_file, ((func_decl_node_t *) node)->func_tail);
            break;
        }
        case VAR_DEF_NODE_T: {
            if (((var_def_node_t *) node)->is_init_list) {
                for (unsigned i = 0; i < get_length_of_array(((var_def_node_t *) node)->entry->sizes,
                                                             ((var_def_node_t *) node)->entry->depth); ++i) {
                    if ((((var_def_node_t *) node)->qualified_types[i].qualifier != CONST_T)) {
                        fprint_tree(output_file, ((var_def_node_t *) node)->values[i].node_value);
                    }
                }
            } else {
                fprint_tree(output_file, ((var_def_node_t *) node)->node);
            }
            break;
        }
        case LOGICAL_OP_NODE_T: {
            fprint_tree(output_file, ((logical_op_node_t *) node)->left);
            fprint_tree(output_file, ((logical_op_node_t *) node)->right);
            break;
        }
        case COMPARISON_OP_NODE_T: {
            fprint_tree(output_file, ((comparison_op_node_t *) node)->left);
            fprint_tree(output_file, ((comparison_op_node_t *) node)->right);
            break;
        }
        case EQUALITY_OP_NODE_T: {
            fprint_tree(output_file, ((equality_op_node_t *) node)->left);
            fprint_tree(output_file, ((equality_op_node_t *) node)->right);
            break;
        }
        case NOT_OP_NODE_T: {
            fprint_tree(output_file, ((not_op_node_t *) node)->child);
            break;
        }
        case INTEGER_OP_NODE_T: {
            fprint_tree(output_file, ((integer_op_node_t *) node)->left);
            fprint_tree(output_file, ((integer_op_node_t *) node)->right);
            break;
        }
        case INVERT_OP_NODE_T: {
            fprint_tree(output_file, ((invert_op_node_t *) node)->child);
            break;
        }
        case FUNC_CALL_NODE_T: {
            for (unsigned i = 0; i < ((func_call_node_t *) node)->num_of_pars; ++i) {
                fprint_tree(output_file, ((func_call_node_t *) node)->pars[i]);
            }
            break;
        }
        case IF_NODE_T: {
            fprint_tree(output_file, ((if_node_t *) node)->condition);
            fprint_tree(output_file, ((if_node_t *) node)->if_branch);
            for (unsigned i = 0; i < ((if_node_t *) node)->num_of_else_ifs; ++i) {
                fprint_tree(output_file, ((if_node_t *) node)->else_if_branches[i]);
            }
            fprint_tree(output_file, ((if_node_t *) node)->else_branch);
            break;
        }
        case ELSE_IF_NODE_T: {
            fprint_tree(output_file, ((else_if_node_t *) node)->condition);
            fprint_tree(output_file, ((else_if_node_t *) node)->else_if_branch);
            break;
        }
        case SWITCH_NODE_T: {
            fprint_tree(output_file, ((switch_node_t *) node)->expression);
            for (unsigned i = 0; i < ((switch_node_t *) node)->num_of_cases; ++i) {
                fprint_tree(output_file, ((switch_node_t *) node)->case_branches[i]);
            }
            break;
        }
        case CASE_NODE_T: {
            fprint_tree(output_file, ((case_node_t *) node)->case_branch);
            break;
        }
        case FOR_NODE_T: {
            fprint_tree(output_file, ((for_node_t *) node)->initialize);
            fprint_tree(output_file, ((for_node_t *) node)->condition);
            fprint_tree(output_file, ((for_node_t *) node)->increment);
            fprint_tree(output_file, ((for_node_t *) node)->for_branch);
            break;
        }
        case DO_NODE_T: {
            fprint_tree(output_file, ((do_node_t *) node)->do_branch);
            fprint_tree(output_file, ((do_node_t *) node)->condition);
            break;
        }
        case WHILE_NODE_T: {
            fprint_tree(output_file, ((while_node_t *) node)->condition);
            fprint_tree(output_file, ((while_node_t *) node)->while_branch);
            break;
        }
        case ASSIGN_NODE_T: {
            fprint_tree(output_file, ((assign_node_t *) node)->left);
            fprint_tree(output_file, ((assign_node_t *) node)->right);
            break;
        }
        case PHASE_NODE_T: {
            fprint_tree(output_file, ((phase_node_t *) node)->left);
            fprint_tree(output_file, ((phase_node_t *) node)->right);
            break;
        }
        case MEASURE_NODE_T: {
            fprint_tree(output_file, ((measure_node_t *) node)->child);
            break;
        }
        case RETURN_NODE_T: {
            fprint_tree(output_file, ((return_node_t *) node)->return_value);
            break;
        }
        default: {
            break;
        }
    }
}
