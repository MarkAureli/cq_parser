#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NULL
#include <__stddef_null.h>
#endif
#include "symtab.h"

/* control scope hide for visual debugging via --dump */
volatile bool hide = true;

char *qualifier_to_str(qualifier_t qualifier) {
    switch (qualifier) {
        case NONE_T: {
            return "";
        }
        case CONST_T: {
            return "const";
        }
        case QUANTUM_T: {
            return "quantum";
        }
    }
}

char *type_to_str(type_t type) {
    switch (type) {
        case BOOL_T: {
            return "bool";
        }
        case INT_T: {
            return "int";
        }
        case UNSIGNED_T: {
            return "unsigned";
        }
        case VOID_T: {
            return "void";
        }
    }
}

void init_hash_table() {
    hash_table = calloc(SIZE, sizeof(list_t*));
}

unsigned hash(const char *key) {
    if (key == NULL) {
        return -1;
    }
    char first_char = key[0];
    unsigned hashval = 0;
    while (*key != '\0') {
        hashval += *key;
        ++key;
    }
    hashval += first_char % 11 + (first_char << 3) - first_char;
    return hashval % SIZE;
}

list_t *insert(const char *name, unsigned length, unsigned line_num, bool declaration) {
    unsigned hashval = hash(name);
    list_t *l = hash_table[hashval];
	
    while ((l != NULL) && (strcmp(name,l->name) != 0)) {
        l = l->next;
    }

    /* variable not yet in table */
    if (l == NULL) {
        if (declaration == false) {
            fprintf(stderr, "Undeclared identifier %s at line %u\n", name, line_num);
            exit(1);
        }
        l = (list_t*) calloc(1, sizeof (list_t));
        strncpy(l->name, name, length);
        l->scope = cur_scope;
        l->lines = calloc(1, sizeof (ref_list_t));
        l->lines->line_num = line_num;
        l->lines->next = NULL;
        l->type_info.qualifier = NONE_T;
        l->type_info.type = VOID_T;
        l->next = hash_table[hashval];
        hash_table[hashval] = l;
    } else {
        if (declaration == true) {
            if (l->scope == cur_scope) {
                fprintf(stderr, "Multiple declaration of identifier %s at line %u (previous declaration in line %u)\n", name, line_num, l->lines->line_num);
                exit(1);
            } else {
                l = calloc(1, sizeof (list_t));
                strncpy(l->name, name, length);
                l->scope = cur_scope;
                l->lines = calloc(1, sizeof (ref_list_t));
                l->lines->line_num = line_num;
                l->lines->next = NULL;
                l->type_info.qualifier = NONE_T;
                l->type_info.type = VOID_T;
                l->next = hash_table[hashval];
                hash_table[hashval] = l;
            }
        } else {
            ref_list_t *t = l->lines;
            while (t->next != NULL) {
                t = t->next;
            }
            /* add line number to reference list */
            t->next = calloc(1, sizeof (ref_list_t));
            t->next->line_num = line_num;
            t->next->next = NULL;
        }
    }
    return l;
}

list_t *lookup(const char *name) { /* return symbol if found or NULL if not found */
    unsigned hashval = hash(name);
    list_t *l = hash_table[hashval];
    while ((l != NULL) && (strcmp(name,l->name) != 0)) {
        l = l->next;
    }
    return l; // NULL is not found
}

void hide_scope() { /* hide the current scope */
    if (hide) {
        for (unsigned i = 0; i < SIZE; ++i) {
            if (hash_table[i] != NULL) {
                list_t *l = hash_table[i];
                while (l != NULL && l->scope == cur_scope) {
                    l = l->next;
                }
                hash_table[i] = l;
            }
        }
    }
    if (cur_scope > 0) {
        --cur_scope;
    }
}

void incr_scope() { /* go to next scope */
    ++cur_scope;
}

void set_type_info_of_elem(list_t *entry, qualifier_t qualifier, type_t type, unsigned depth,
                           const unsigned sizes[MAXARRAYDEPTH], bool is_function) {
    entry->type_info.qualifier = qualifier;
    entry->type_info.type = type;
    entry->type_info.depth = depth;
    unsigned length = 1;
    if (sizes != NULL) {
        memcpy(entry->type_info.sizes, sizes, MAXARRAYDEPTH * sizeof (unsigned));
        for (unsigned i = 0; i < depth; ++i) {
            length *= sizes[i];
        }
    }
    entry->length = length;
    entry->is_function = is_function;
    if (qualifier == CONST_T) {
        entry->values = calloc(length, sizeof (value_t));
    }
}

void set_type_info(const char *name, qualifier_t qualifier, type_t type, unsigned depth,
                   const unsigned sizes[MAXARRAYDEPTH], bool is_function) {
    list_t *l = lookup(name);
    set_type_info_of_elem(l, qualifier, type, depth, sizes, is_function);
}

func_info_t empty_func_info_init() {
    func_info_t new_func_info = { .pars_type_info = NULL, .num_of_pars = 0};
    return new_func_info;
}

func_info_t func_info_init(type_info_t type_info) {
    func_info_t new_func_info = { .pars_type_info = calloc(1, sizeof (type_info_t)), .num_of_pars = 1};
    new_func_info.pars_type_info[0] = type_info;
    return new_func_info;
}

void append_to_func_info(func_info_t *func_info, type_info_t type_info) {
    unsigned current_num_of_pars = (func_info->num_of_pars)++;
    func_info->pars_type_info = realloc(func_info->pars_type_info,
                                        (current_num_of_pars + 1) * sizeof (type_info_t));
    func_info->pars_type_info[current_num_of_pars] = type_info;
}

void set_func_info_of_elem(list_t *entry, func_info_t func_info) {
    entry->func_info = func_info;
}

/* print to stdout by default */ 
void symtab_dump(FILE * of){
    for (unsigned i = 0; i < MAXTOKENLEN; ++i) {
        fputc('-', of);
    }
    fputc(' ', of);
    fprintf(of, "---------- ");
    for (unsigned i = 0; i < 11 + 2 * MAXARRAYDEPTH; ++i) {
        fputc('-', of);
    }
    fputc(' ', of);
    fprintf(of, "------ -------------\n");

    fprintf(of, "Name");
    for (unsigned i = 0; i < MAXTOKENLEN + 1- sizeof ("Name"); ++i) {
        fputc(' ', of);
    }
    fputc(' ', of);
    fprintf(of, "Qualifier  Type");
    for (unsigned i = 0; i < 12 + 2 * MAXARRAYDEPTH - sizeof ("Type"); ++i) {
        fputc(' ', of);
    }
    fputc(' ', of);
    fprintf(of, "Scope  Line Numbers \n");

    for (unsigned i = 0; i < MAXTOKENLEN; ++i) {
        fputc('-', of);
    }
    fputc(' ', of);
    fprintf(of, "---------- ");
    for (unsigned i = 0; i < 11 + 2 * MAXARRAYDEPTH; ++i) {
        fputc('-', of);
    }
    fputc(' ', of);
    fprintf(of, "------ -------------\n");
    for (unsigned i = 0; i < SIZE; ++i) { 
        if (hash_table[i] != NULL) { 
            list_t *l = hash_table[i];
            while (l != NULL) { 
                ref_list_t *t = l->lines;
                fprintf(of,"%-*s", MAXTOKENLEN + 1, l->name);
                switch (l->type_info.qualifier) {
                    case NONE_T: {
                        fprintf(of, "%-11s", "");
                        break;
                    }
                    case CONST_T: {
                        fprintf(of, "%-11s", "const");
                        break;
                    }
                    case QUANTUM_T: {
                        fprintf(of, "%-11s", "quantum");
                        break;
                    }
                }
                unsigned type_str_length = 0;
                if (l->is_function) {
                    fprintf(of, "-> ");
                    type_str_length += 3;
                }
                switch (l->type_info.type) {
                    case BOOL_T: {
                        fprintf(of, "bool");
                        type_str_length += 4;
                        break;
                    }
                    case INT_T: {
                        fprintf(of, "int");
                        type_str_length += 3;
                        break;
                    }
                    case UNSIGNED_T: {
                        fprintf(of, "unsigned");
                        type_str_length += 8;
                        break;
                    }
                    case VOID_T: {
                        fprintf(of, "void");
                        type_str_length += 4;
                        break;
                    }
                }
                for (unsigned j = 0; j < l->type_info.depth; ++j) {
                    fprintf(of, "[]");
                    type_str_length += 2;
                }
                for (unsigned k = type_str_length; k < 12  + MAXARRAYDEPTH * 2; ++k) {
                    fprintf(of, " ");
                }
                fprintf(of, "%-7u", l->scope);
                while (t != NULL) {
                    fprintf(of, "%-4u ", t->line_num);
                    t = t->next;
                }
                fprintf(of,"\n");
                l = l->next;
	        }
        }
    }
}
