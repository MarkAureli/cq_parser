#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "symtab.h"

char *op_type_to_string(op_type_t op_type) {
    switch (op_type) {
        case NONE_OP: {
            return "NONE_OP";
        }
        case INTEGER_OP: {
            return "INTEGER_OP";
        }
        case INVERT_OP: {
            return "INVERT_OP";
        }
        case LOGICAL_OP: {
            return "LOGICAL_OP";
        }
        case LOGICAL_NOT_OP: {
            return "LOGICAL_NOT_OP";
        }
        case RELATION_OP: {
            return "RELATION_OP";
        }
        case EQUALITY_OP: {
            return "EQUALITY_OP";
        }
    }
}

type_t get_result_type(type_t type_1, type_t type_2, op_type_t op_type) {
    switch (op_type) {
        case NONE_OP: {
            if (type_1 == type_2) {
                return type_1;
            }
            break;
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
                            break;
                    }
                    break;
                }
                case UNSIGNED_T: {
                    switch (type_2) {
                        case INT_T: case UNSIGNED_T: {
                            return UNSIGNED_T;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case INVERT_OP: {
            if (type_1 == INT_T) {
                return INT_T;
            } else if (type_1 == UNSIGNED_T) {
                return UNSIGNED_T;
            }
        }
        case LOGICAL_OP: {
            if (type_1 == BOOL_T && type_2 == BOOL_T) {
                return BOOL_T;
            }
            break;
        }
        case LOGICAL_NOT_OP: {
            if (type_1 == BOOL_T) {
                return BOOL_T;
            }
            break;
        }
        case RELATION_OP: {
            if ((type_1 == INT_T || type_1 == UNSIGNED_T) && (type_2 == INT_T || type_2 == UNSIGNED_T)) {
                return BOOL_T;
            }
            break;
        }
        case EQUALITY_OP: {
            switch (type_1) {
                case BOOL_T: {
                    if (type_2 == BOOL_T) {
                        return BOOL_T;
                    }
                    break;
                }
                case INT_T: case UNSIGNED_T: {
                    if (type_2 == INT_T || type_2 == UNSIGNED_T) {
                        return BOOL_T;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    type_error(type_1, type_2, op_type);
    exit(1);
}

void type_error(type_t type_1, type_t type_2, op_type_t op_type) {
    fprintf(stderr, "Type conflict between %s and %s using an operator of type %s\n", type_to_str(type_1),
            type_to_str(type_2), op_type_to_string(op_type));
}
