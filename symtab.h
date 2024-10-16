#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>
#include "rules.h"

/* maximum size of hash table */
#define SIZE 211

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
    VOID_T,
    BOOL_T,
    INT_T,
    UNSIGNED_T,
} type_t;

typedef struct type_info {
    qualifier_t qualifier;
    type_t type;
    unsigned sizes[MAXARRAYDEPTH];
    unsigned depth;
} type_info_t;

typedef struct qualified_type {
    qualifier_t qualifier;
    type_t type;
} qualified_type_t;

typedef struct func_info {
    bool is_unitary;
    type_info_t *pars_type_info;
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
    type_info_t type_info;
    unsigned length;
    bool is_function;
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

void set_type_info_of_elem(list_t *entry, qualifier_t qualifier, type_t type, unsigned depth,
                           const unsigned sizes[MAXARRAYDEPTH], bool is_function);

void set_type_info(const char *name, qualifier_t qualifier, type_t type, unsigned depth,
                   const unsigned sizes[MAXARRAYDEPTH], bool is_function);

func_info_t create_empty_func_info();

func_info_t create_func_info(type_info_t type_info);

void append_to_func_info(func_info_t *func_info, type_info_t type_info);

void set_func_info_of_elem(list_t *entry, func_info_t func_info);

void symtab_dump(FILE *of);

#endif /* SYMTAB_H */
