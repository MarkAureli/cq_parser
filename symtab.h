#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>
#include "rules.h"

/* maximum size of hash table */
#define SIZE 211

/* control scope hide for visual debugging via --dump */
static bool no_hide = false;

/* current scope */
static unsigned cur_scope = 0;

typedef union value {
    bool bval;
    int ival;
    unsigned uval;
} value_t;

/* token types */
typedef enum type_qualifier {
    NONE_T,
    CONST_T,
    QUANTUM_T
} type_qualifier_t;

typedef enum type {
    UNDEFINED_T,
    BOOL_T,
    INT_T,
    UNSIGNED_T,
    VOID_T
} type_t;

/* parameter struct */
typedef struct param {
    type_t type;
    char name[MAXTOKENLEN];
    value_t value;
} param_t;

typedef struct func_info {
    param_t *pars;
    unsigned num_of_pars;
} func_info_t;

typedef struct type_info {
    type_t type;
    unsigned sizes[MAXARRAYDEPTH];
    unsigned depth;
} type_info_t;

typedef struct array_values {
    value_t *values;
    unsigned length;
} array_values_t;

/* a linked list of references (lineno's) for each variable */
typedef struct ref_list { 
    unsigned line_num;
    struct ref_list *next;
    type_t type;
} ref_list_t;

// struct that represents a list node
typedef struct list {
    char name[MAXTOKENLEN];
    unsigned scope;
    ref_list_t *lines;
    type_qualifier_t type_qualifier;
    type_t type;
    bool is_function;
    unsigned sizes[MAXARRAYDEPTH];
    unsigned length;
    unsigned depth;
    union {
        value_t *values;
        func_info_t func_info;
    };
    struct list *next;
} list_t;

/* the hash table */
static list_t **hash_table;

char *type_qualifier_to_str(type_qualifier_t type_qualifier);

char *type_to_str(type_t type);

type_info_t type_info_init(type_t type, unsigned depth);

array_values_t array_values_init(value_t *values, unsigned old_length, unsigned length);

void init_hash_table();

unsigned hash(const char *key);

list_t *insert(const char *name, unsigned length, type_t type, unsigned line_num, bool declaration);

list_t *lookup(const char *name);

void hide_scope();

void incr_scope();

void set_type_of_elem(list_t *symtab_elem, type_qualifier_t type_qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]);

void set_type(const char *name, type_qualifier_t type_qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]);

void set_values_of_elem(list_t *symtab_elem, value_t *values, unsigned length);

type_t get_type(const char *name);

param_t def_param(const char *name, type_t type);

void symtab_dump(FILE *of);

#endif /* SYMTAB_H */
