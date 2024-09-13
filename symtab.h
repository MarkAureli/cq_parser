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
    int par_type;
    char param_name[MAXTOKENLEN];
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
    param_t *parameters;
    unsigned num_of_pars;
    struct list *next;
} list_t;

/* the hash table */
static list_t **hash_table;

char *type_to_string(type_t type);

void init_hash_table();

unsigned hash(char *key);

void insert(char *name, unsigned length, type_t type, unsigned line_num, bool declaration);

list_t *lookup(char *name);

void hide_scope();

void incr_scope();

void set_type(char *name, type_t st_type, type_t inf_type);

type_t get_type(char *name);

void symtab_dump(FILE *of);

#endif /* SYMTAB_H */
