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
typedef enum qualifier {
    NONE_T,
    CONST_T,
    QUANTUM_T
} qualifier_t;

typedef enum type {
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

/* a linked list of references (lineno's) for each variable */
typedef struct ref_list { 
    unsigned line_num;
    struct ref_list *next;
} ref_list_t;

// struct that represents a list node
typedef struct list {
    char name[MAXTOKENLEN];
    unsigned scope;
    ref_list_t *lines;
    qualifier_t qualifier;
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

char *qualifier_to_str(qualifier_t qualifier);

char *type_to_str(type_t type);

void init_hash_table();

unsigned hash(const char *key);

list_t *insert(const char *name, unsigned length, unsigned line_num, bool declaration);

list_t *lookup(const char *name);

void hide_scope();

void incr_scope();

void set_type_of_elem(list_t *entry, qualifier_t qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]);

void set_type(const char *name, qualifier_t qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]);

void set_values_of_elem(list_t *entry, value_t *values, unsigned length);

type_t get_type(const char *name);

param_t def_param(const char *name, type_t type);

void symtab_dump(FILE *of);

#endif /* SYMTAB_H */
