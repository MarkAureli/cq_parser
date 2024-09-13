#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

char *type_to_string(type_t type) {
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
        case ARRAY_T: {
            return "ARRAY_T";
        }
        case FUNCTION_T: {
            return "FUNCTION_T";
        }
    }
}

void init_hash_table() {
    hash_table = calloc(SIZE, sizeof(list_t*));
}

unsigned hash(char *key) {
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

void insert(char *name, unsigned length, type_t type, unsigned line_num, bool declaration) {
    unsigned hashval = hash(name);
    list_t *l = hash_table[hashval];
	
    while ((l != NULL) && (strcmp(name,l->st_name) != 0)) {
        l = l->next;
    }

    /* variable not yet in table */
    if (l == NULL) {
        l = (list_t*) malloc(sizeof (list_t));
        strncpy(l->st_name, name, length);  
        /* add to hashtable */
        l->st_type = type;
        l->scope = cur_scope;
        l->lines = malloc(sizeof (ref_list_t));
        l->lines->line_num = line_num;
        l->lines->next = NULL;
        l->next = hash_table[hashval];
        hash_table[hashval] = l;
    } else {
        if (declaration == true) {
            if (l->scope == cur_scope) {
                fprintf(stderr, "Multiple declaration of variable %s at line %u (previous declaration in line %u)\n", name, line_num, l->lines->line_num);
                exit(1);
            } else {
                l = malloc(sizeof (list_t));
                strncpy(l->st_name, name, length);
                l->st_type = type;
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
}

list_t *lookup(char *name) { /* return symbol if found or NULL if not found */
    unsigned hashval = hash(name);
    list_t *l = hash_table[hashval];
    while ((l != NULL) && (strcmp(name,l->st_name) != 0)) {
        l = l->next;
    }
    return l; // NULL is not found
}

void hide_scope() { /* hide the current scope */
    for (unsigned i = 0; i < SIZE; ++i) {
        if (hash_table[i] != NULL) {
            list_t *l = hash_table[i];
            while (l != NULL && l->scope == cur_scope) {
                l = l->next;
            }
            hash_table[i] = l;
        }
    }
    if(cur_scope > 0) {
        --cur_scope;
    }
}

void incr_scope() { /* go to next scope */
    ++cur_scope;
}

void set_type(char *name, type_t st_type, type_t inf_type) {
    list_t *l = lookup(name);
    l->st_type = st_type;
    if (inf_type != UNDEFINED_T) {
        l->inf_type = inf_type;
    }
}

type_t get_type(char *name) {
    list_t *l = lookup(name);
    switch (l->st_type) {
        case UNDEFINED_T: case BOOL_T: case INT_T: case UNSIGNED_T: {
            return l->st_type;
        }
        case ARRAY_T: case FUNCTION_T: {
            return l->inf_type;
        }
    }
}

/* print to stdout by default */ 
void symtab_dump(FILE * of){  
    fprintf(of,"---------------------------------------- -------------- ------ -------------\n");
    fprintf(of,"Name                                     Type           Scope  Line Numbers \n");
    fprintf(of,"---------------------------------------- -------------- ------ -------------\n");
    for (unsigned i = 0; i < SIZE; ++i) { 
        if (hash_table[i] != NULL) { 
            list_t *l = hash_table[i];
            while (l != NULL) { 
                ref_list_t *t = l->lines;
                fprintf(of,"%-41s",l->st_name);
                switch (l->st_type) {
                    case UNDEFINED_T: {
                        fprintf(of, "%-15s", "undefined");
                        break;
                    }
                    case BOOL_T: {
                        fprintf(of, "%-15s", "bool");
                        break;
                    }
                    case INT_T: {
                        fprintf(of, "%-15s", "int");
                        break;
                    }
                    case UNSIGNED_T: {
                        fprintf(of, "%-15s", "unsigned");
                        break;
                    }
                    case ARRAY_T: {
                        switch (l->inf_type) {
                            case UNDEFINED_T: {
                                fprintf(of, "%-15s", "undefined[]");
                                break;
                            }
                            case BOOL_T: {
                                fprintf(of, "%-15s", "bool[]");
                                break;
                            }
                            case INT_T: {
                                fprintf(of, "%-15s", "int[]");
                                break;
                            }
                            case UNSIGNED_T: {
                                fprintf(of, "%-15s", "unsigned[]");
                                break;
                            }
                            default: {
                                fprintf(of, "%-15s", "ERROR[]");
                                break;
                            }
                        }
                        break;
                    }
                    case FUNCTION_T: {
                        switch (l->inf_type) {
                            case UNDEFINED_T: {
                                fprintf(of, "%-15s", "-> undefined");
                                break;
                            }
                            case BOOL_T: {
                                fprintf(of, "%-15s", "-> bool");
                                break;
                            }
                            case INT_T: {
                                fprintf(of, "%-15s", "-> int");
                                break;
                            }
                            case UNSIGNED_T: {
                                fprintf(of, "%-15s", "-> unsigned");
                                break;
                            }
                            case ARRAY_T: {
                                switch (l->inf_type) {
                                    case UNDEFINED_T: {
                                        fprintf(of, "%-15s", "-> undefined[]");
                                        break;
                                    }
                                    case BOOL_T: {
                                        fprintf(of, "%-15s", "-> bool[]");
                                        break;
                                    }
                                    case INT_T: {
                                        fprintf(of, "%-15s", "-> int[]");
                                        break;
                                    }
                                    case UNSIGNED_T: {
                                        fprintf(of, "%-15s", "-> unsigned[]");
                                        break;
                                    }
                                    default: {
                                        fprintf(of, "%-15s", "-> ERROR[]");
                                        break;
                                    }
                                }
                                break;
                            }
                            default: {
                                fprintf(of, "%-15s", "-> ERROR");
                                break;
                            }
                        }
                        break;
                    }
                    default: {
                        fprintf(of, "%-15s", "ERROR");
                        break;
                    }
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
