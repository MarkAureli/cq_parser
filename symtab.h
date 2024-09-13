#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>

/* maximum size of hash table */
#define SIZE 211

/* maximum size of tokens-identifiers */
#define MAXTOKENLEN 40

/* current scope */
static unsigned cur_scope = 0;

/* token types */
typedef enum type {
    UNDEFINED_T,
    BOOL_T,
    INT_T,
    UNSIGNED_T,
    ARRAY_T,
    FUNCTION_T
} type_t;

/* parameter struct */
typedef struct param {
    type_t type;
    char name[MAXTOKENLEN];
    bool bval; int ival; unsigned uval;
} param_t;

/* a linked list of references (lineno's) for each variable */
typedef struct ref_list { 
    int line_num;
    struct ref_list *next;
    int type;
} ref_list_t;

// struct that represents a list node
typedef struct list {
    char st_name[MAXTOKENLEN];
    unsigned st_size;
    unsigned scope;
    ref_list_t *lines;
    bool st_bval; int st_ival;  unsigned st_uval;
    type_t st_type;
    type_t inf_type; // for arrays (info type) and functions (return type)
    bool *b_vals; int *i_vals; unsigned *u_vals;
    unsigned array_size;
    param_t *pars;
    unsigned num_of_pars;
    struct list *next;
} list_t;

/* the hash table */
static list_t **hash_table;

char *type_to_string(type_t type);

void init_hash_table();

unsigned hash(const char *key);

void insert(const char *name, unsigned length, type_t type, unsigned line_num, bool declaration);

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
