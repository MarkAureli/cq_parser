#include "ast.h"

node_t *new_node(node_type_t type, node_t *left, node_t *right) {
    node_t *new_node = malloc(sizeof (node_t));
    new_node->type = type;
    new_node->left = left;
    new_node->right = right;
    return new_node;
}

node_t *new_decl_node(type_t data_type, list_t *name) {
    decl_node_t *new_node = malloc(sizeof (decl_node_t));
    new_node->type = DECL_NODE_T;
    new_node->data_type = data_type;
    new_node->name = name;
    return (node_t *) new_node;
}

node_t *new_const_node(type_t data_type, value_t value) {
    const_node_t *new_node = malloc(sizeof (const_node_t));
    new_node->type = CONST_NODE_T;
    new_node->data_type = data_type;
    new_node->value = value;
    return (node_t *) new_node;
}
