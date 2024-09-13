#ifndef SEMANTICS_H
#define SEMANTICS_H

typedef enum op_type {
    NONE_OP,
    ARITHMETIC_OP,
    BIT_OP,
    SHIFT_OP,
    LOGICAL_OP,
    NOT_OP,
    REL_OP,
    EQU_OP
} op_type_t;

char *op_type_to_string(op_type_t op_type);

type_t get_result_type(type_t type_1, type_t type_2, op_type_t op_type);

void type_error(type_t type_1, type_t type_2, op_type_t op_type);

#endif // SEMANTICS_H
