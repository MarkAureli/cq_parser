#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

char *type_qualifier_to_str(type_qualifier_t type_qualifier) {
    switch (type_qualifier) {
        case NONE_T: {
            return "NONE_T";
        }
        case CONST_T: {
            return "CONST_T";
        }
        case QUANTUM_T: {
            return "QUANTUM_T";
        }
    }
}

char *type_to_str(type_t type) {
    switch (type) {
        case UNDEFINED_T: {
            return "UNDEFINED_T";
        }
        case BOOL_T: {
            return "BOOL_T";
        }
        case INT_T: {
            return "INT_T";
        }
        case UNSIGNED_T: {
            return "UNSIGNED_T";
        }
        case VOID_T: {
            return "VOID_T";
        }
    }
}

type_info_t type_info_init(type_t type, unsigned depth) {
    type_info_t new_type_info = { .type = type, .depth = depth};
    return new_type_info;
}

array_values_t array_values_init(value_t *values, unsigned old_length, unsigned length) {
    array_values_t new_array_values = { .values = calloc(length, sizeof (value_t)), .length = length};
    if (values != NULL) {
        for (unsigned i = 0; i < old_length; ++i) {
            new_array_values.values[i] = values[i];
        }
    }
    return new_array_values;
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

list_t *insert(const char *name, unsigned length, type_t type, unsigned line_num, bool declaration) {
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
        l = (list_t*) malloc(sizeof (list_t));
        strncpy(l->name, name, length);
        /* add to hashtable */
        l->type = type;
        l->scope = cur_scope;
        l->lines = malloc(sizeof (ref_list_t));
        l->lines->line_num = line_num;
        l->lines->next = NULL;
        l->next = hash_table[hashval];
        hash_table[hashval] = l;
    } else {
        if (declaration == true) {
            if (l->scope == cur_scope) {
                fprintf(stderr, "Multiple declaration of identifier %s at line %u (previous declaration in line %u)\n", name, line_num, l->lines->line_num);
                exit(1);
            } else {
                l = malloc(sizeof (list_t));
                strncpy(l->name, name, length);
                l->type = type;
                l->scope = cur_scope;
                l->lines = malloc(sizeof (ref_list_t));
                l->lines->line_num = line_num;
                l->lines->next = NULL;
                l->next = hash_table[hashval];
                hash_table[hashval] = l;
            }
        } else {
            ref_list_t *t = l->lines;
            while (t->next != NULL) {
                t = t->next;
            }
            /* add line number to reference list */
            t->next = malloc(sizeof (ref_list_t));
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
    if (no_hide) {
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
    if(cur_scope > 0) {
        --cur_scope;
    }
}

void incr_scope() { /* go to next scope */
    ++cur_scope;
}

void set_type_of_elem(list_t *symtab_elem, type_qualifier_t type_qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]) {
    symtab_elem->type_qualifier = type_qualifier;
    symtab_elem->type = type;
    symtab_elem->is_function = is_function;
    symtab_elem->depth = depth;
    unsigned length = 1;
    if (sizes != NULL) {
        memcpy(symtab_elem->sizes, sizes, MAXARRAYDEPTH * sizeof (unsigned));
        for (unsigned i = 0; i < depth; ++i) {
            length *= sizes[i];
        }
    }
    symtab_elem->length = length;
}

void set_type(const char *name, type_qualifier_t type_qualifier, type_t type, bool is_function, unsigned depth, const unsigned sizes[MAXARRAYDEPTH]) {
    list_t *l = lookup(name);
    set_type_of_elem(l, type_qualifier, type, is_function, depth, sizes);
}

void set_values_of_elem(list_t *symtab_elem, value_t *values, unsigned length) {
    symtab_elem->values = calloc(symtab_elem->length, sizeof (value_t));
    memcpy(symtab_elem->values, values, length * sizeof (value_t));
}

type_t get_type(const char *name) {
    list_t *l = lookup(name);
    return l->type;
}

param_t def_param(const char *name, type_t type) {
    param_t param;
    param.type = type;
    strncpy(param.name, name, MAXTOKENLEN);
    return param;
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
                switch (l->type_qualifier) {
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
                switch (l->type) {
                    case UNDEFINED_T: {
                        fprintf(of, "undefined");
                        type_str_length += 9;
                        break;
                    }
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
                for (unsigned j = 0; j < l->depth; ++j) {
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
