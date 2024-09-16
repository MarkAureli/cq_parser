#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

char *arithmetic_op_to_str(arithmetic_op_t arithmetic_op) {
    switch (arithmetic_op) {
        case ADD_OP: {
            return "ADD_OP";
        }
        case DIV_OP: {
            return "DIV_OP";
        }
        case MOD_OP: {
            return "MOD_OP";
        }
        case MUL_OP: {
            return "MUL_OP";
        }
        case SUB_OP: {
            return "SUB_OP";
        }
    }
}

char *increment_op_to_str(increment_op_t increment_op) {
    switch (increment_op) {
        case INCR_OP: {
            return "INCR_OP";
        }
        case DECR_OP: {
            return "DECR_OP";
        }
    }
}

char *bitwise_op_to_str(bitwise_op_t bitwise_op) {
    switch (bitwise_op) {
        case AND_OP: {
            return "AND_OP";
        }
        case OR_OP: {
            return "OR_OP";
        }
        case XOR_OP: {
            return "XOR_OP";
        }
    }
}

char *shift_op_to_str(shift_op_t shift_op) {
    switch (shift_op) {
        case LSHIFT_OP: {
            return "LSHIFT_OP";
        }
        case RSHIFT_OP: {
            return "RSHIFT_OP";
        }
    }
}

char *logical_op_to_str(logical_op_t logical_op) {
    switch (logical_op) {
        case LAND_OP: {
            return "LAND_OP";
        }
        case LOR_OP: {
            return "LOR_OP";
        }
        case LXOR_OP: {
            return "LXOR_OP";
        }
    }
}

char *relation_op_to_str(relation_op_t relation_op) {
    switch (relation_op) {
        case GE_OP: {
            return "GE_OP";
        }
        case GEQ_OP: {
            return "GEQ_OP";
        }
        case LE_OP: {
            return "LE_OP";
        }
        case LEQ_OP: {
            return "LEQ_OP";
        }
    }
}

char *equality_op_to_str(equality_op_t equality_op) {
    switch (equality_op) {
        case EQ_OP: {
            return "EQ_OP";
        }
        case NEQ_OP: {
            return "NEQ_OP";
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

void print_node(const node_t *node) {
    switch (node->type) {
        case BASIC_NODE_T: {
            printf("Basic node\n");
            break;
        }
        case VAR_DECL_NODE_T: {
            printf("Variable declaration node for %s\n", ((var_decl_node_t *) node)->entry->name);
            break;
        }
        case FUNC_DECL_NODE_T: {
            printf("Function declaration node for %s\n", ((func_decl_node_t *) node)->entry->name);
            break;
        }
        case CONST_NODE_T: {
            printf("Constant node of data type %s\n", type_to_str(((const_node_t *) node)->var_info.type));
            break;
        }
        case FUNC_CALL_NODE_T: {
            printf("Function call node for %s with %u parameters\n", ((func_call_node_t *) node)->entry->name, ((func_call_node_t *) node)->num_of_pars);
            break;
        }
        case ARITHMETIC_NODE_T: {
            printf("Arithmetic node of operator %s\n", arithmetic_op_to_str(((arithmetic_node_t *) node)->op));
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
            printf("Reference node for %s\n", ((reference_node_t *) node)->entry->name);
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
