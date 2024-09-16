#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

char *arithmetic_op_to_str(arithmetic_op_t arithmetic_op) {
    switch (arithmetic_op) {
        case ADD_OP: {
            return "+";
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
        case SUB_OP: {
            return "-";
        }
    }
}

char *increment_op_to_str(increment_op_t increment_op) {
    switch (increment_op) {
        case INCR_OP: {
            return "++";
        }
        case DECR_OP: {
            return "--";
        }
    }
}

char *bitwise_op_to_str(bitwise_op_t bitwise_op) {
    switch (bitwise_op) {
        case AND_OP: {
            return "&";
        }
        case OR_OP: {
            return "|";
        }
        case XOR_OP: {
            return "^";
        }
    }
}

char *shift_op_to_str(shift_op_t shift_op) {
    switch (shift_op) {
        case LSHIFT_OP: {
            return "<<";
        }
        case RSHIFT_OP: {
            return ">>";
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

char *relation_op_to_str(relation_op_t relation_op) {
    switch (relation_op) {
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

node_t *new_node(node_type_t type, node_t *left, node_t *right) {
    node_t *new_node = calloc(1, sizeof (node_t));
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

node_t *new_var_decl_node(list_t *entry) {
    var_decl_node_t *new_node = calloc(1, sizeof (var_decl_node_t));
    new_node->type = VAR_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_func_decl_node(list_t *entry) {
    func_decl_node_t *new_node = calloc(1, sizeof (func_decl_node_t));
    new_node->type = FUNC_DECL_NODE_T;
    new_node->entry = entry;
    return (node_t *) new_node;
}

node_t *new_const_node(type_t type, value_t value) {
    const_node_t *new_node = calloc(1, sizeof (const_node_t));
    new_node->type = CONST_NODE_T;
    new_node->var_info.qualifier = true;
    new_node->var_info.type = type;
    new_node->var_info.value = value;
    return (node_t *) new_node;
}

node_t *new_func_call_node(list_t *entry, node_t **pars, unsigned num_of_pars) {
    if (!entry->is_function) {
        return NULL;
    }
    func_call_node_t *new_node = calloc(1, sizeof (func_call_node_t));
    new_node->type = FUNC_CALL_NODE_T;
    new_node->var_info.qualifier = entry->qualifier;
    new_node->var_info.type = entry->type;
    new_node->entry = entry;
    new_node->pars = pars;
    new_node->num_of_pars = num_of_pars;
    return (node_t *) new_node;
}

node_t *new_arithmetic_node(arithmetic_op_t op, node_t *left, node_t *right) {
    arithmetic_node_t *new_node = calloc(1, sizeof (arithmetic_node_t));
    new_node->type = ARITHMETIC_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_bitwise_node(bitwise_op_t op, node_t *left, node_t *right) {
    bitwise_node_t *new_node = calloc(1, sizeof (bitwise_node_t));
    new_node->type = BITWISE_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_shift_node(shift_op_t op, node_t *left, node_t *right) {
    shift_node_t *new_node = calloc(1, sizeof (shift_node_t));
    new_node->type = SHIFT_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_inv_node(node_t *child) {
    inv_node_t *new_node = calloc(1, sizeof (inv_node_t));
    new_node->type = INV_NODE_T;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_logical_node(logical_op_t op, node_t *left, node_t *right) {
    logical_node_t *new_node = calloc(1, sizeof (logical_node_t));
    new_node->type = LOGICAL_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_relation_node(relation_op_t op, node_t *left, node_t *right) {
    relation_node_t *new_node = calloc(1, sizeof (relation_node_t));
    new_node->type = RELATION_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_equality_node(equality_op_t op, node_t *left, node_t *right) {
    equality_node_t *new_node = calloc(1, sizeof (equality_node_t));
    new_node->type = EQUALITY_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_not_op_node(node_t *child) {
    not_op_node_t *new_node = calloc(1, sizeof (not_op_node_t));
    new_node->type = NOT_OP_NODE_T;
    new_node->var_info.type = BOOL_T;
    new_node->child = child;
    return (node_t *) new_node;
}

node_t *new_reference_node(list_t *entry) {
    reference_node_t *new_node = calloc(1, sizeof (reference_node_t));
    new_node->type = REFERENCE_NODE_T;
    new_node->var_info.type = entry->type;
    new_node->var_info.qualifier = entry->qualifier;
    new_node->entry = entry;
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

var_info_t get_var_info_of_node(const node_t *node) {
    switch (node->type) {
        case CONST_NODE_T: {
            return ((const_node_t *) node)->var_info;
        }
        case ARITHMETIC_NODE_T: {
            return ((arithmetic_node_t *) node)->var_info;
        }
        case BITWISE_NODE_T: {
            return ((bitwise_node_t *) node)->var_info;
        }
        case SHIFT_NODE_T: {
            return ((shift_node_t *) node)->var_info;
        }
        case INV_NODE_T: {
            return ((inv_node_t *) node)->var_info;
        }
        case LOGICAL_NODE_T: {
            return ((logical_node_t *) node)->var_info;
        }
        case NOT_OP_NODE_T: {
            return ((not_op_node_t *) node)->var_info;
        }
        case RELATION_NODE_T: {
            return ((relation_node_t *) node)->var_info;
        }
        case EQUALITY_NODE_T: {
            return ((equality_node_t *) node)->var_info;
        }
        case REFERENCE_NODE_T: {
            return ((reference_node_t *) node)->var_info;
        }
        default: {
            var_info_t undefined_result = { .qualifier=NONE_T, .type=UNDEFINED_T, .value.bval=false};
            return undefined_result;
        }
    }
}

node_t *build_arithmetic_node(arithmetic_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    node_t *result;
    const_node_t *const_node_view_left, *const_node_view_right, *const_node_view_result;
    reference_node_t *reference_node_view_left, *reference_node_view_right, *reference_node_view_result;
    arithmetic_node_t *arithmetic_node_view_left, *arithmetic_node_view_right, *arithmetic_node_view_result;
    bitwise_node_t *bitwise_node_view_left, *bitwise_node_view_right, *bitwise_node_view_result;
    shift_node_t *shift_node_view_left, *shift_node_view_right, *shift_node_view_result;
    inv_node_t *inv_node_view_left, *inv_node_view_right, *inv_node_view_result;

    switch (left->type) {
        case CONST_NODE_T: {
            if (const_node_view_left->var_info.type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is a boolean expression",
                         arithmetic_op_to_str(op));
                return NULL;
            }
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 arithmetic_op_to_str(op));
                        return NULL;
                    }
                    result = left;
                    if (const_node_view_result->var_info.type == INT_T) {
                        if (const_node_view_right->var_info.type == INT_T) {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.ival += const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case DIV_OP: {
                                    if (const_node_view_right->var_info.value.ival == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.ival /= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case MOD_OP: {
                                    if (const_node_view_right->var_info.value.ival == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.ival %= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case MUL_OP: {
                                    const_node_view_result->var_info.value.ival *= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.ival -= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                            }
                        } else {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival + const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case DIV_OP: {
                                    if (const_node_view_right->var_info.value.uval == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival / const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case MOD_OP: {
                                    if (const_node_view_right->var_info.value.uval == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival % const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case MUL_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival * const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival - const_node_view_right->var_info.value.uval;
                                    break;
                                }
                            }
                        }
                    } else {
                        if (const_node_view_right->var_info.type == INT_T) {
                            const_node_view_result->var_info.type = UNSIGNED_T;
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.uval += const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case DIV_OP: {
                                    if (const_node_view_right->var_info.value.ival == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval /= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case MOD_OP: {
                                    if (const_node_view_right->var_info.value.ival == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval %= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case MUL_OP: {
                                    const_node_view_result->var_info.value.uval *= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval -= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                            }
                        } else {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.uval += const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case DIV_OP: {
                                    if (const_node_view_right->var_info.value.uval == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval /= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case MOD_OP: {
                                    if (const_node_view_right->var_info.value.uval == 0) {
                                        snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                                        return NULL;
                                    }
                                    const_node_view_result->var_info.value.uval %= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case MUL_OP: {
                                    const_node_view_result->var_info.value.uval *= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval -= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                            }
                        }
                    }
                    free(right);
                    return result;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(op, left, right);
                    arithmetic_node_view_result->var_info.qualifier = arithmetic_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(op, left, right);
                    arithmetic_node_view_result->var_info.qualifier = bitwise_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(op, left, right);
                    arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    if (const_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(op, left, right);
                    arithmetic_node_view_result->var_info.qualifier = inv_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression", arithmetic_op_to_str(op));
                        return NULL;
                    }
                    result = new_arithmetic_node(op, left, right);
                    arithmetic_node_view_result->var_info.qualifier = reference_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return NULL;
                }
                default: {
                    snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                             arithmetic_op_to_str(op));
                    return NULL;
                }
            }
            break;
        }
        case ARITHMETIC_NODE_T: {
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = arithmetic_node_view_left->var_info.qualifier;
                    if (arithmetic_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (arithmetic_node_view_left->var_info.qualifier == QUANTUM_T || arithmetic_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (arithmetic_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (arithmetic_node_view_left->var_info.qualifier == QUANTUM_T || bitwise_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (arithmetic_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = arithmetic_node_view_left->var_info.qualifier;
                    if (arithmetic_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (arithmetic_node_view_left->var_info.qualifier == QUANTUM_T || inv_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (arithmetic_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (arithmetic_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (arithmetic_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = arithmetic_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                default: {
                    yyerror("Right operand of \"*\" is a boolean expression");
                }
            }
            break;
        }
        case BITWISE_NODE_T: {
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = bitwise_node_view_left->var_info.qualifier;
                    if (bitwise_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (bitwise_node_view_left->var_info.qualifier == QUANTUM_T || arithmetic_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (bitwise_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (bitwise_node_view_left->var_info.qualifier == QUANTUM_T || bitwise_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (bitwise_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = bitwise_node_view_left->var_info.qualifier;
                    if (bitwise_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (bitwise_node_view_left->var_info.qualifier == QUANTUM_T || inv_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (bitwise_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (bitwise_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (bitwise_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = bitwise_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                default: {
                    yyerror("Right operand of \"*\" is a boolean expression");
                }
            }
            break;
        }
        case SHIFT_NODE_T: {
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    if (shift_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = arithmetic_node_view_right->var_info.qualifier;
                    if (shift_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = bitwise_node_view_right->var_info.qualifier;
                    if (shift_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    if (shift_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = inv_node_view_right->var_info.qualifier;
                    if (shift_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = reference_node_view_right->var_info.qualifier;
                    if (shift_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = shift_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                default: {
                    yyerror("Right operand of \"*\" is a boolean expression");
                }
            }
            break;
        }
        case INV_NODE_T: {
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = inv_node_view_left->var_info.qualifier;
                    if (inv_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (inv_node_view_left->var_info.qualifier == QUANTUM_T || arithmetic_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (inv_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (inv_node_view_left->var_info.qualifier == QUANTUM_T || bitwise_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (inv_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = inv_node_view_left->var_info.qualifier;
                    if (inv_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (inv_node_view_left->var_info.qualifier == QUANTUM_T || inv_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (inv_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (inv_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (inv_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = inv_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                default: {
                    yyerror("Right operand of \"*\" is a boolean expression");
                }
            }
            break;
        }
        case REFERENCE_NODE_T: {
            if (reference_node_view_left->var_info.type == BOOL_T) {
                yyerror("Left operand of \"*\" is a boolean expression");
            }
            switch (right->type) {
                case CONST_NODE_T: {
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = reference_node_view_left->var_info.qualifier;
                    if (reference_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case ARITHMETIC_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || arithmetic_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == arithmetic_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case BITWISE_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || bitwise_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == bitwise_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case SHIFT_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    arithmetic_node_view_result->var_info.qualifier = reference_node_view_left->var_info.qualifier;
                    if (reference_node_view_left->var_info.type == shift_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case INV_NODE_T: {
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || inv_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == inv_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                case REFERENCE_NODE_T: {
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        yyerror("Right operand of \"*\" is a boolean expression");
                    }
                    result = new_arithmetic_node(MUL_OP, left, right);
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        arithmetic_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        arithmetic_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        arithmetic_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        arithmetic_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    break;
                }
                default: {
                    yyerror("Right operand of \"*\" is a boolean expression");
                }
            }
            break;
        }
        default: {
            yyerror("Left operand of \"*\" is a boolean expression");
        }
    }
}

void print_node(const node_t *node) {
    switch (node->type) {
        case BASIC_NODE_T: {
            printf("Basic node\n");
            break;
        }
        case VAR_DECL_NODE_T: {
            switch (((var_decl_node_t *) node)->entry->qualifier) {
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
            printf("%s", type_to_str(((var_decl_node_t *) node)->entry->type));
            for (unsigned i = 0; i < ((var_decl_node_t *) node)->entry->depth; ++i) {
                printf("[]");
            }
            putchar('\n');
            break;
        }
        case FUNC_DECL_NODE_T: {
            printf("Function declaration node for %s\n", ((func_decl_node_t *) node)->entry->name);
            break;
        }
        case CONST_NODE_T: {
            switch (((const_node_t *) node)->var_info.type) {
                case BOOL_T: {
                    printf("%s\n", ((const_node_t *) node)->var_info.value.bval ? "true" : "false");
                    break;
                }
                case INT_T: {
                    printf("%d\n", ((const_node_t *) node)->var_info.value.ival);
                    break;
                }
                case UNSIGNED_T: {
                    printf("%u\n", ((const_node_t *) node)->var_info.value.uval);
                    break;
                }
                default: {
                    printf("undefined\n");
                    break;
                }
            }
            break;
        }
        case FUNC_CALL_NODE_T: {
            printf("Function call node for %s with %u parameters\n", ((func_call_node_t *) node)->entry->name, ((func_call_node_t *) node)->num_of_pars);
            break;
        }
        case ARITHMETIC_NODE_T: {
            putchar('(');
            var_info_t left_info = get_var_info_of_node(((arithmetic_node_t *) node)->left);
            if (left_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) %s (", type_to_str(left_info.type), arithmetic_op_to_str(((arithmetic_node_t *) node)->op));
            var_info_t right_info = get_var_info_of_node(((arithmetic_node_t *) node)->right);
            if (right_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) -> (", type_to_str(right_info.type));
            var_info_t info = get_var_info_of_node(node);
            if (info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s)\n", type_to_str(info.type));
            break;
        }
        case BITWISE_NODE_T: {
            printf("Bitwise node of operator %s\n", bitwise_op_to_str(((bitwise_node_t *) node)->op));
            break;
        }
        case SHIFT_NODE_T: {
            printf("Shift node of operator %s\n", shift_op_to_str(((shift_node_t *) node)->op));
            break;
        }
        case INV_NODE_T: {
            printf("Inversion node\n");
            break;
        }
        case LOGICAL_NODE_T: {
            printf("Logical node of operator %s\n", logical_op_to_str(((logical_node_t *) node)->op));
            break;
        }
        case RELATION_NODE_T: {
            printf("Relation node of operator %s\n", relation_op_to_str(((relation_node_t *) node)->op));
            break;
        }
        case EQUALITY_NODE_T: {
            printf("Equation node of operator %s\n", equality_op_to_str(((equality_node_t *) node)->op));
            break;
        }
        case NOT_OP_NODE_T: {
            printf("Not operator node\n");
            break;
        }
        case REFERENCE_NODE_T: {
            var_info_t info = get_var_info_of_node(node);
            printf("reference to %s%s %s", (info.qualifier == QUANTUM_T) ? "quantum " : "", type_to_str(info.type), ((reference_node_t *) node)->entry->name);
            for (unsigned i = 0; i < ((reference_node_t *) node)->depth; ++i) {
                printf("[%u]", ((reference_node_t *) node)->indices[i]);
            }
            putchar('\n');
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
        case ARITHMETIC_NODE_T: {
            tree_traversal(((arithmetic_node_t *)node)->left);
            tree_traversal(((arithmetic_node_t *)node)->right);
            break;
        }
        case BITWISE_NODE_T: {
            tree_traversal(((bitwise_node_t *)node)->left);
            tree_traversal(((bitwise_node_t *)node)->right);
            break;
        }
        case SHIFT_NODE_T: {
            tree_traversal(((shift_node_t *)node)->left);
            tree_traversal(((shift_node_t *)node)->right);
            break;
        }
        case LOGICAL_NODE_T: {
            tree_traversal(((logical_node_t *)node)->left);
            tree_traversal(((logical_node_t *)node)->right);
            break;
        }
        case RELATION_NODE_T: {
            tree_traversal(((relation_node_t *)node)->left);
            tree_traversal(((relation_node_t *)node)->right);
            break;
        }
        case EQUALITY_NODE_T: {
            tree_traversal(((equality_node_t *)node)->left);
            tree_traversal(((equality_node_t *)node)->right);
            break;
        }
        case INV_NODE_T: {
            tree_traversal(((inv_node_t *) node)->child);
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

// int main(int argc, const char *argv[]) {
//     value_t val1, val2;
//     val1.bval = true;
//     val2.bval = false;
//     node_t *const_node1 = new_const_node(BOOL_T, val1);
//     node_t *const_node2 = new_const_node(BOOL_T, val2);
//     node_t *logical_node = new_logical_node(LOR_OP, const_node1, const_node2);
//     node_t *jump_node = new_jump_node(0);
//     node_t *if_node = new_if_node(logical_node, jump_node, NULL, 0, NULL);
//     tree_traversal(if_node);
// }
