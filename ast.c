#include <__stddef_null.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

char *op_type_to_string(op_type_t op_type) {
    switch (op_type) {
        case LOGICAL_OP: {
            return "LOGICAL_OP";
        }
        case RELATION_OP: {
            return "RELATION_OP";
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

node_t *new_const_node(type_t type, value_t value) {
    const_node_t *new_node = calloc(1, sizeof (const_node_t));
    new_node->type = CONST_NODE_T;
    new_node->var_info.qualifier = true;
    new_node->var_info.type = type;
    new_node->var_info.value = value;
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

node_t *new_logical_op_node(logical_op_t op, node_t *left, node_t *right) {
    logical_op_node_t *new_node = calloc(1, sizeof (logical_op_node_t));
    new_node->type = LOGICAL_OP_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_relation_op_node(relation_op_t op, node_t *left, node_t *right) {
    relation_op_node_t *new_node = calloc(1, sizeof (relation_op_node_t));
    new_node->type = RELATION_OP_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_equality_op_node(equality_op_t op, node_t *left, node_t *right) {
    equality_op_node_t *new_node = calloc(1, sizeof (equality_op_node_t));
    new_node->type = EQUALITY_OP_NODE_T;
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

node_t *new_integer_op_node(integer_op_t op, node_t *left, node_t *right) {
    integer_op_node_t *new_node = calloc(1, sizeof (integer_op_node_t));
    new_node->type = INTEGER_OP_NODE_T;
    new_node->op = op;
    new_node->left = left;
    new_node->right = right;
    return (node_t *) new_node;
}

node_t *new_invert_op_node(node_t *child) {
    invert_op_node_t *new_node = calloc(1, sizeof (invert_op_node_t));
    new_node->type = INVERT_OP_NODE_T;
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

var_info_t get_var_info_of_node(const node_t *node) {
    switch (node->type) {
        case CONST_NODE_T: {
            return ((const_node_t *) node)->var_info;
        }
        case REFERENCE_NODE_T: {
            return ((reference_node_t *) node)->var_info;
        }
        case LOGICAL_OP_NODE_T: {
            return ((logical_op_node_t *) node)->var_info;
        }
        case RELATION_OP_NODE_T: {
            return ((relation_op_node_t *) node)->var_info;
        }
        case EQUALITY_OP_NODE_T: {
            return ((equality_op_node_t *) node)->var_info;
        }
        case NOT_OP_NODE_T: {
            return ((not_op_node_t *) node)->var_info;
        }
        case INTEGER_OP_NODE_T: {
            return ((integer_op_node_t *) node)->var_info;
        }
        case INVERT_OP_NODE_T: {
            return ((invert_op_node_t *) node)->var_info;
        }
        default: {
            var_info_t undefined_result = { .qualifier=NONE_T, .type=VOID_T, .value.bval=false};
            return undefined_result;
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
        case RELATION_OP: {
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

node_t *build_equality_op_node(equality_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    var_info_t left_var_info = get_var_info_of_node(left);
    var_info_t right_var_info = get_var_info_of_node(right);
    /* implement error when nodes are no expression nodes */
    qualifier_t result_qualifier = propagate_qualifier(left_var_info.qualifier, right_var_info.qualifier);
    type_t result_type = propagate_type(EQUALITY_OP, left_var_info.type, right_var_info.type);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Checking %s of %s and %s", (op == EQ_OP) ? "equality" : "inequality",
                 type_to_str(left_var_info.type), type_to_str(right_var_info.type));
        return NULL;
    }

    node_t *result;
    if (result_qualifier == CONST_T) { /* left and right are of node_type CONST_NODE_T */
        result = left;
        const_node_t *const_node_view_left = (const_node_t *) left;
        const_node_t *const_node_view_right = (const_node_t *) right;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->var_info.type = BOOL_T;
        switch (left_var_info.type) {
            case BOOL_T: { /* left and right are booleans */
                if (op == EQ_OP) {
                    const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.bval == const_node_view_right->var_info.value.bval;
                } else {
                    const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.bval != const_node_view_right->var_info.value.bval;
                }
                break;
            }
            case INT_T: { /* left is a signed integer, right is either a signed or unsigned integer */
                if (right_var_info.type == INT_T) { /* left and right are integers */
                    if (op == EQ_OP) {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.ival == const_node_view_right->var_info.value.ival;
                    } else {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.ival != const_node_view_right->var_info.value.ival;
                    }
                } else {
                    if (op == EQ_OP) { /* left is a signed integer, right is an unsigned integer */
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.ival == const_node_view_right->var_info.value.uval;
                    } else {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.ival != const_node_view_right->var_info.value.uval;
                    }
                }
                break;
            }
            case UNSIGNED_T: { /* left is an unsigned integer, right is either a signed or unsigned integer */
                if (right_var_info.type == INT_T) { /* left is an unsigned integer, right is a signed integer */
                    if (op == EQ_OP) {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.uval == const_node_view_right->var_info.value.ival;
                    } else {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.uval != const_node_view_right->var_info.value.ival;
                    }
                } else {
                    if (op == EQ_OP) { /* left and right are unsigned integers */
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.uval == const_node_view_right->var_info.value.uval;
                    } else {
                        const_node_view_result->var_info.value.bval = const_node_view_left->var_info.value.uval != const_node_view_right->var_info.value.uval;
                    }
                }
                break;
            }
            case VOID_T: { /* case not possible due to previous type checking */
                break;
            }
        }
        free(const_node_view_right);
        return result;
    }

    result = new_equality_op_node(op, left, right);
    equality_op_node_t *equality_op_node_view_result = (equality_op_node_t *) result;
    equality_op_node_view_result->var_info.qualifier = result_qualifier;
    equality_op_node_view_result->var_info.type = result_type;
    return result;
}

node_t *build_not_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]) {
    var_info_t child_var_info = get_var_info_of_node(child);
    /* implement error when child is no expression node */
    qualifier_t result_qualifier = child_var_info.qualifier;
    type_t result_type = propagate_type(NOT_OP, child_var_info.type, VOID_T);
    if (result_type == VOID_T) {
        snprintf(error_msg, ERRORMSGLENGTH, "Applying \"!\" to expression of type %s", type_to_str(child_var_info.type));
        return NULL;
    }
    node_t *result;
    if (result_qualifier == CONST_T) { /* child is of node_type CONST_NODE_T */
        result = child;
        const_node_t *const_node_view_child = (const_node_t *) child;
        const_node_t *const_node_view_result = (const_node_t *) result;
        const_node_view_result->var_info.value.bval = !(const_node_view_result->var_info.value.bval);
        return result;
    } else if (child->type == NOT_OP_NODE_T) {
        not_op_node_t *not_op_node_view_child = (not_op_node_t *) child;
        result = not_op_node_view_child->child;
        free(not_op_node_view_child);
    } else {
        result = new_not_op_node(child);
        not_op_node_t *not_op_node_view_result = (not_op_node_t *) result;
        not_op_node_view_result->var_info.qualifier = result_qualifier;
        not_op_node_view_result->var_info.type = result_type;
    }
    return result;
}

node_t *build_integer_op_node(integer_op_t op, node_t *left, node_t *right, char error_msg[ERRORMSGLENGTH]) {
    node_t *result;
    const_node_t *const_node_view_left, *const_node_view_right, *const_node_view_result;
    reference_node_t *reference_node_view_left, *reference_node_view_right;
    integer_op_node_t *integer_op_node_view_left, *integer_op_node_view_right, *integer_op_node_view_result;
    invert_op_node_t *invert_op_node_view_left, *invert_op_node_view_right;

    switch (left->type) {
        case CONST_NODE_T: {
            const_node_view_left = (const_node_t *) left;
            if (const_node_view_left->var_info.type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is a boolean expression",
                         integer_op_to_str(op));
                return NULL;
            }
            switch (right->type) {
                case CONST_NODE_T: {
                    const_node_view_right = (const_node_t *) right;
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    }
                    result = left;
                    const_node_view_result = (const_node_t *) result;
                    if (const_node_view_result->var_info.type == INT_T) {
                        if (const_node_view_right->var_info.type == INT_T) {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.ival += const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case AND_OP: {
                                    const_node_view_result->var_info.value.ival &= const_node_view_right->var_info.value.ival;
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
                                case OR_OP: {
                                    const_node_view_result->var_info.value.ival |= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.ival -= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case XOR_OP: {
                                    const_node_view_result->var_info.value.ival ^= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                            }
                        } else {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival + const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case AND_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival & const_node_view_right->var_info.value.uval;
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
                                case OR_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival | const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival - const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case XOR_OP: {
                                    const_node_view_result->var_info.value.uval = const_node_view_left->var_info.value.ival ^ const_node_view_right->var_info.value.uval;
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
                                case AND_OP: {
                                    const_node_view_result->var_info.value.uval &= const_node_view_right->var_info.value.ival;
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
                                case OR_OP: {
                                    const_node_view_result->var_info.value.uval |= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval -= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                                case XOR_OP: {
                                    const_node_view_result->var_info.value.uval ^= const_node_view_right->var_info.value.ival;
                                    break;
                                }
                            }
                        } else {
                            switch (op) {
                                case ADD_OP: {
                                    const_node_view_result->var_info.value.uval += const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case AND_OP: {
                                    const_node_view_result->var_info.value.uval &= const_node_view_right->var_info.value.uval;
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
                                case OR_OP: {
                                    const_node_view_result->var_info.value.uval |= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case SUB_OP: {
                                    const_node_view_result->var_info.value.uval -= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                                case XOR_OP: {
                                    const_node_view_result->var_info.value.uval ^= const_node_view_right->var_info.value.uval;
                                    break;
                                }
                            }
                        }
                    }
                    free(const_node_view_right);
                    return result;
                }
                case REFERENCE_NODE_T: {
                    reference_node_view_right = (reference_node_t *) right;
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = reference_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INTEGER_OP_NODE_T: {
                    integer_op_node_view_right = (integer_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = integer_op_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == integer_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INVERT_OP_NODE_T: {
                    invert_op_node_view_right = (invert_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = invert_op_node_view_right->var_info.qualifier;
                    if (const_node_view_left->var_info.type == invert_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = const_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                default: {
                    snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                             integer_op_to_str(op));
                    return NULL;
                }
            }
        }
        case REFERENCE_NODE_T: {
            reference_node_view_left = (reference_node_t *) left;
            if (reference_node_view_left->var_info.type == BOOL_T) {
                snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is a boolean expression",
                         integer_op_to_str(op));
                return NULL;
            }
            switch (right->type) {
                case CONST_NODE_T: {
                    const_node_view_right = (const_node_t *) right;
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    } else if (const_node_view_right->var_info.value.ival == 0) {
                        if (op == DIV_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                            return NULL;
                        } else if (op == MOD_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                            return NULL;
                        }
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = reference_node_view_left->var_info.qualifier;
                    if (reference_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case REFERENCE_NODE_T: {
                    reference_node_view_right = (reference_node_t *) right;
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INTEGER_OP_NODE_T: {
                    integer_op_node_view_right = (integer_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || integer_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == integer_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INVERT_OP_NODE_T: {
                    invert_op_node_view_right = (invert_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (reference_node_view_left->var_info.qualifier == QUANTUM_T || invert_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (reference_node_view_left->var_info.type == invert_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = reference_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                default: {
                    snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                             integer_op_to_str(op));
                    return NULL;
                }
            }
        }
        case INTEGER_OP_NODE_T: {
            integer_op_node_view_left = (integer_op_node_t *) left;
            switch (right->type) {
                case CONST_NODE_T: {
                    const_node_view_right = (const_node_t *) right;
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    } else if (const_node_view_right->var_info.value.ival == 0) {
                        if (op == DIV_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                            return NULL;
                        } else if (op == MOD_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                            return NULL;
                        }
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = integer_op_node_view_left->var_info.qualifier;
                    if (integer_op_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = integer_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case REFERENCE_NODE_T: {
                    reference_node_view_right = (reference_node_t *) right;
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (integer_op_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (integer_op_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = integer_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INTEGER_OP_NODE_T: {
                    integer_op_node_view_right = (integer_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (integer_op_node_view_left->var_info.qualifier == QUANTUM_T || integer_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (integer_op_node_view_left->var_info.type == integer_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = integer_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INVERT_OP_NODE_T: {
                    invert_op_node_view_right = (invert_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (integer_op_node_view_left->var_info.qualifier == QUANTUM_T || invert_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (integer_op_node_view_left->var_info.type == invert_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = integer_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                default: {
                    snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                             integer_op_to_str(op));
                    return NULL;
                }
            }
        }
        case INVERT_OP_NODE_T: {
            invert_op_node_view_left = (invert_op_node_t *) left;
            switch (right->type) {
                case CONST_NODE_T: {
                    const_node_view_right = (const_node_t *) right;
                    if (const_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    } else if (const_node_view_right->var_info.value.ival == 0) {
                        if (op == DIV_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Division by zero");
                            return NULL;
                        } else if (op == MOD_OP) {
                            snprintf(error_msg, ERRORMSGLENGTH, "Modulo by zero");
                            return NULL;
                        }
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    integer_op_node_view_result->var_info.qualifier = invert_op_node_view_left->var_info.qualifier;
                    if (invert_op_node_view_left->var_info.type == const_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = invert_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case REFERENCE_NODE_T: {
                    reference_node_view_right = (reference_node_t *) right;
                    if (reference_node_view_right->var_info.type == BOOL_T) {
                        snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                                 integer_op_to_str(op));
                        return NULL;
                    }
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (invert_op_node_view_left->var_info.qualifier == QUANTUM_T || reference_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (invert_op_node_view_left->var_info.type == reference_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = invert_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INTEGER_OP_NODE_T: {
                    integer_op_node_view_right = (integer_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (invert_op_node_view_left->var_info.qualifier == QUANTUM_T || integer_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (invert_op_node_view_left->var_info.type == integer_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = invert_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                case INVERT_OP_NODE_T: {
                    invert_op_node_view_right = (invert_op_node_t *) right;
                    result = new_integer_op_node(op, left, right);
                    integer_op_node_view_result = (integer_op_node_t *) result;
                    if (invert_op_node_view_left->var_info.qualifier == QUANTUM_T || invert_op_node_view_right->var_info.qualifier == QUANTUM_T) {
                        integer_op_node_view_result->var_info.qualifier = QUANTUM_T;
                    } else {
                        integer_op_node_view_result->var_info.qualifier = NONE_T;
                    }
                    if (invert_op_node_view_left->var_info.type == invert_op_node_view_right->var_info.type) {
                        integer_op_node_view_result->var_info.type = invert_op_node_view_left->var_info.type;
                    } else {
                        integer_op_node_view_result->var_info.type = UNSIGNED_T;
                    }
                    return result;
                }
                default: {
                    snprintf(error_msg, ERRORMSGLENGTH, "Right operand of \"%s\" is a boolean expression",
                             integer_op_to_str(op));
                    return NULL;
                }
            }
        }
        default: {
            snprintf(error_msg, ERRORMSGLENGTH, "Left operand of \"%s\" is a boolean expression",
                     integer_op_to_str(op));
            return NULL;
        }
    }
}

node_t *build_invert_op_node(node_t *child, char error_msg[ERRORMSGLENGTH]) {
    node_t *result;
    const_node_t *const_node_view_child, *const_node_view_result;
    reference_node_t *reference_node_view_child;
    integer_op_node_t *integer_op_node_view_child;
    invert_op_node_t *invert_op_node_view_child, *invert_op_node_view_result;

    switch (child->type) {
        case CONST_NODE_T: {
            const_node_view_child = (const_node_t *) child;
            if (const_node_view_child->var_info.type == INT_T) {
                result = child;
                const_node_view_result = (const_node_t *) result;
                const_node_view_result->var_info.value.ival = ~(const_node_view_result->var_info.value.ival);
            } else if (const_node_view_child->var_info.type == UNSIGNED_T) {
                result = child;
                const_node_view_result = (const_node_t *) result;
                const_node_view_result->var_info.value.uval = ~(const_node_view_result->var_info.value.uval);
            } else {
                snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to a boolean expression");
                return NULL;
            }
            return result;
        }
        case REFERENCE_NODE_T: {
            reference_node_view_child = (reference_node_t *) child;
            if (reference_node_view_child->var_info.type == INT_T || reference_node_view_child->var_info.type == UNSIGNED_T) {
                result = new_invert_op_node(child);
                invert_op_node_view_result = (invert_op_node_t *) result;
                invert_op_node_view_result->var_info.qualifier = reference_node_view_child->var_info.qualifier;
                invert_op_node_view_result->var_info.type = reference_node_view_child->var_info.type;
            } else {
                snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to boolean variable %s", reference_node_view_child->entry->name);
                return NULL;
            }
            return result;
        }
        case INTEGER_OP_NODE_T: {
            integer_op_node_view_child = (integer_op_node_t *) child;
            result = new_invert_op_node(child);
            invert_op_node_view_result = (invert_op_node_t *) result;
            invert_op_node_view_result->var_info.qualifier = integer_op_node_view_child->var_info.qualifier;
            invert_op_node_view_result->var_info.type = integer_op_node_view_child->var_info.type;
            return result;
        }
        case INVERT_OP_NODE_T: {
            invert_op_node_view_child = (invert_op_node_t *) child;
            result = invert_op_node_view_child->child;
            free(invert_op_node_view_child);
            return result;
        }
        default: {
            snprintf(error_msg, ERRORMSGLENGTH, "Applying \"~\" to boolean expression");
            return NULL;
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
                case VOID_T: {
                    printf("undefined\n");
                    break;
                }
            }
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
        case FUNC_CALL_NODE_T: {
            printf("Function call node for %s with %u parameters\n", ((func_call_node_t *) node)->entry->name, ((func_call_node_t *) node)->num_of_pars);
            break;
        }
        case LOGICAL_OP_NODE_T: {
            putchar('(');
            var_info_t left_info = get_var_info_of_node(((logical_op_node_t *) node)->left);
            if (left_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) %s (", type_to_str(left_info.type), logical_op_to_str(((logical_op_node_t *) node)->op));
            var_info_t right_info = get_var_info_of_node(((logical_op_node_t *) node)->right);
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
        case RELATION_OP_NODE_T: {
            putchar('(');
            var_info_t left_info = get_var_info_of_node(((relation_op_node_t *) node)->left);
            if (left_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) %s (", type_to_str(left_info.type), relation_op_to_str(((relation_op_node_t *) node)->op));
            var_info_t right_info = get_var_info_of_node(((relation_op_node_t *) node)->right);
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
        case EQUALITY_OP_NODE_T: {
            putchar('(');
            var_info_t left_info = get_var_info_of_node(((equality_op_node_t *) node)->left);
            if (left_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) %s (", type_to_str(left_info.type), equality_op_to_str(((equality_op_node_t *) node)->op));
            var_info_t right_info = get_var_info_of_node(((equality_op_node_t *) node)->right);
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
        case NOT_OP_NODE_T: {
            printf("!(");
            var_info_t child_info = get_var_info_of_node(((invert_op_node_t *) node)->child);
            if (child_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) -> (", type_to_str(child_info.type));
            var_info_t info = get_var_info_of_node(node);
            if (info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s)\n", type_to_str(info.type));
            break;
        }
        case INTEGER_OP_NODE_T: {
            putchar('(');
            var_info_t left_info = get_var_info_of_node(((integer_op_node_t *) node)->left);
            if (left_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) %s (", type_to_str(left_info.type), integer_op_to_str(((integer_op_node_t *) node)->op));
            var_info_t right_info = get_var_info_of_node(((integer_op_node_t *) node)->right);
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
        case INVERT_OP_NODE_T: {
            printf("~(");
            var_info_t child_info = get_var_info_of_node(((invert_op_node_t *) node)->child);
            if (child_info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s) -> (", type_to_str(child_info.type));
            var_info_t info = get_var_info_of_node(node);
            if (info.qualifier == QUANTUM_T) {
                printf("quantum ");
            }
            printf("%s)\n", type_to_str(info.type));
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
        case RELATION_OP_NODE_T: {
            tree_traversal(((relation_op_node_t *)node)->left);
            tree_traversal(((relation_op_node_t *)node)->right);
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
