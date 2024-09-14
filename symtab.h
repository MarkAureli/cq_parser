#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>

/* maximum size of hash table */
#define SIZE 211

/* maximum size of tokens-identifiers */
#define MAXTOKENLEN 40

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
typedef enum type {
    UNDEFINED_T,
    BOOL_T,
    INT_T,
    UNSIGNED_T,
    VOID_T,
    ARRAY_T,
    FUNCTION_T
} type_t;

/* parameter struct */
typedef struct param {
    type_t type;
    char name[MAXTOKENLEN];
    value_t value;
} param_t;

/* a linked list of references (lineno's) for each variable */
typedef struct ref_list { 
    unsigned line_num;
    struct ref_list *next;
    type_t type;
} ref_list_t;

// struct that represents a list node
typedef struct list {
    char st_name[MAXTOKENLEN];
    unsigned st_size;
    unsigned scope;
    ref_list_t *lines;
    value_t value;
    type_t st_type;
    type_t inf_type; // for arrays (info type) and functions (return type)
    value_t *values;
    unsigned array_size;
    param_t *pars;
    unsigned num_of_pars;
    struct list *next;
} list_t;

/* the hash table */
static list_t **hash_table;

char *type_to_str(type_t type);

void init_hash_table();

unsigned hash(const char *key);

list_t *insert(const char *name, unsigned length, type_t type, unsigned line_num, bool declaration);

list_t *lookup(const char *name);

void hide_scope();

void incr_scope();

void set_type(const char *name, type_t st_type, type_t inf_type);

type_t get_type(const char *name);

param_t def_param(const char *name, type_t type);

void decl_function(const char *name, type_t ret_type, unsigned num_of_pars, param_t *pars);

void check_function_pars(const char *name, unsigned num_of_pars, param_t *pars);

void symtab_dump(FILE *of);

#endif /* SYMTAB_H */
