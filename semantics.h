#ifndef SEMANTICS_H
#define SEMANTICS_H
#include "symtab.h"

typedef enum op_type {
    NONE_OP,
    INTEGER_OP,
    INVERT_OP,
    LOGICAL_OP,
    LOGICAL_NOT_OP,
    RELATION_OP,
    EQUALITY_OP
} op_type_t;

char *op_type_to_str(op_type_t op_type);

type_t get_result_type(type_t type_1, type_t type_2, op_type_t op_type);

void type_error(type_t type_1, type_t type_2, op_type_t op_type);

#endif // SEMANTICS_H
