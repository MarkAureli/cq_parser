/**
 * \file                                symbol_table.c
 * \brief                               Symbol table source file
 */

/*
 * Copyright (c) 2024 Lennart BINKOWSKI
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of cq_compiler.
 *
 * Author:          Lennart BINKOWSKI <lennart.binkowski@itp.uni-hannover.de>
 */


/*
 * =====================================================================================================================
 *                                                includes
 * =====================================================================================================================
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NULL
#include <__stddef_null.h>
#endif
#include "symbol_table.h"


/*
 * =====================================================================================================================
 *                                                static variables
 * =====================================================================================================================
 */

/**
 * \brief                               Pointer to symbol table
 */
static struct entry *symbol_table[SYMBOL_TABLE_SIZE];

/**
 * \brief                               Pointer to shadow symbol table
 */
static struct entry *shadow_symbol_table[SYMBOL_TABLE_SIZE];

/**
 * \brief                               Counter for the current scope (starts at `0`)
 */
static unsigned cur_scope;


/*
 * =====================================================================================================================
 *                                                function definitions
 * =====================================================================================================================
 */

/* See header for documentation */
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

/* See header for documentation */
char *type_to_str(type_t type) {
    switch (type) {
        case VOID_T: {
            return "void";
        }
        case BOOL_T: {
            return "bool";
        }
        case INT_T: {
            return "int";
        }
        case UNSIGNED_T: {
            return "unsigned";
        }
    }
}

/* See header for documentation */
void init_symbol_table() {
    memset(symbol_table, 0, sizeof (symbol_table));
    memset(shadow_symbol_table, 0, sizeof (shadow_symbol_table));
    cur_scope = 0;
}

/**
 * \brief                               Free content of symbol table entry
 * \note                                The pointers to the symbol table entry and to the next entry are not freed
 * \param[in]                           entry: Pointer to symbol table entry
 */
void free_entry_content(entry_t *entry) {
    if (entry == NULL) {
        return;
    }

    ref_list_t *current_ref = entry->lines;
    ref_list_t *next_ref;
    while (current_ref != NULL) {
        next_ref = current_ref->next;
        free(current_ref);
        current_ref = next_ref;
    }

    if (entry->is_function) {
        free(entry->pars_type_info);
    } else if (entry->qualifier == CONST_T) {
        free(entry->values);
    }
}

/* See header for documentation */
void free_symbol_table() {
    for (unsigned i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        if (shadow_symbol_table[i] != NULL) {
            entry_t *current_entry = shadow_symbol_table[i];
            entry_t *next_entry;
            while (current_entry != NULL) {
                free_entry_content(current_entry);
                next_entry = current_entry->next;
                free(current_entry);
                current_entry = next_entry;
            }
        }
    }
}

/**
 * \brief                               Calculate hash value of key
 * \param[in]                           key: Key as string
 * \return                              Hash value of input key
 */
unsigned hash(const char *key) {
    if (key == NULL) {
        return -1;
    }
    char first_char = key[0];
    unsigned hash_value = 0;
    while (*key != '\0') {
        hash_value += *key;
        ++key;
    }
    hash_value += first_char % 11 + (first_char << 3) - first_char;
    return hash_value % SYMBOL_TABLE_SIZE;
}

/* See header for documentation */
entry_t *insert(const char *name, unsigned length, unsigned line_num, bool declaration,
                char error_msg[ERROR_MSG_LENGTH]) {
    unsigned hash_value = hash(name);
    entry_t *entry = symbol_table[hash_value];
    bool first_value = true;
    while ((entry != NULL) && (strcmp(name, entry->name) != 0)) {
        first_value = false;
        entry = entry->next;
    }
    if (entry == NULL) {
        if (declaration == false) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Undeclared identifier %s at line %u\n", name, line_num);
            free_symbol_table();
            return NULL;
        }
        entry = malloc(sizeof (entry_t));
        if (entry == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for symbol table entry for %s failed", name);
            free_symbol_table();
            return NULL;
        }

        strncpy(entry->name, name, length);
        entry->scope = cur_scope;
        entry->lines = malloc(sizeof (ref_list_t));
        if (entry->lines == NULL) {
            snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for reference list for %s failed", name);
            free_symbol_table();
            return NULL;
        }

        entry->lines->line_num = line_num;
        entry->lines->next = NULL;
        entry->qualifier = NONE_T;
        entry->type = VOID_T;
        entry->next = symbol_table[hash_value];
        symbol_table[hash_value] = entry;
        if (first_value) {
            shadow_symbol_table[hash_value] = entry;
        }
    } else {
        if (declaration == true) {
            if (entry->scope == cur_scope) {
                snprintf(error_msg, ERROR_MSG_LENGTH,
                         "Multiple declaration of identifier %s at line %u (previous declaration in line %u)\n",
                         name, line_num, entry->lines->line_num);
                free_symbol_table();
                return NULL;
            } else {
                entry = malloc(sizeof (entry_t));
                if (entry == NULL) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for symbol table entry for %s failed",
                             name);
                    free_symbol_table();
                    return NULL;
                }

                strncpy(entry->name, name, length);
                entry->scope = cur_scope;
                entry->lines = malloc(sizeof (ref_list_t));
                if (entry->lines == NULL) {
                    snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for reference list for %s failed", name);
                    free_symbol_table();
                    return NULL;
                }

                entry->lines->line_num = line_num;
                entry->lines->next = NULL;
                entry->qualifier = NONE_T;
                entry->type = VOID_T;
                entry->next = symbol_table[hash_value];
                symbol_table[hash_value] = entry;
            }
        } else {
            ref_list_t *references = entry->lines;
            while (references->next != NULL) {
                references = references->next;
            }
            references->next = malloc(sizeof (ref_list_t));
            if (references->next == NULL) {
                snprintf(error_msg, ERROR_MSG_LENGTH, "Allocating memory for reference list for %s failed", name);
                free_symbol_table();
                return NULL;
            }

            references->next->line_num = line_num;
            references->next->next = NULL;
        }
    }
    return entry;
}

/* See header for documentation */
void hide_scope() {
    for (unsigned i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        if (symbol_table[i] != NULL) {
            entry_t *entry = symbol_table[i];
            while (entry != NULL && entry->scope == cur_scope) {
                entry = entry->next;
            }
            symbol_table[i] = entry;
        }
    }
    if (cur_scope > 0) {
        --cur_scope;
    }
}

/* See header for documentation */
void incr_scope() {
    ++cur_scope;
}

/* See header for documentation */
void set_type_info(entry_t *entry, qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
                   unsigned depth) {
    entry->qualifier = qualifier;
    entry->type = type;
    memcpy(entry->sizes, sizes, depth * sizeof (unsigned));
    entry->depth = depth;
    unsigned length = 1;
    for (unsigned i = 0; i < depth; ++i) {
        entry->sizes[i] = sizes[i];
        length *= sizes[i];
    }
    entry->length = length;
    if (qualifier == CONST_T) {
        entry->values = calloc(length, sizeof (value_t));
    }
}

/* See header for documentation */
void set_func_info(entry_t *entry, bool is_unitary, bool is_quantizable, type_info_t *pars_type_info,
                   unsigned num_of_pars) {
    entry->is_function = true;
    entry->is_unitary = is_unitary;
    entry->is_quantizable = is_quantizable;
    entry->pars_type_info = pars_type_info;
    entry->num_of_pars = num_of_pars;
}

/* See header for documentation */
void fprint_symbol_table(FILE * output_file){
    for (unsigned i = 0; i < MAX_TOKEN_LENGTH; ++i) {
        fputc('-', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "---------- ");
    for (unsigned i = 0; i < 11 + 2 * MAX_ARRAY_DEPTH; ++i) {
        fputc('-', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "------ -------------\n");

    fprintf(output_file, "Name");
    for (unsigned i = 0; i < MAX_TOKEN_LENGTH + 1 - sizeof ("Name"); ++i) {
        fputc(' ', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "Qualifier  Type");
    for (unsigned i = 0; i < 12 + 2 * MAX_ARRAY_DEPTH - sizeof ("Type"); ++i) {
        fputc(' ', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "Scope  Line Numbers \n");

    for (unsigned i = 0; i < MAX_TOKEN_LENGTH; ++i) {
        fputc('-', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "---------- ");
    for (unsigned i = 0; i < 11 + 2 * MAX_ARRAY_DEPTH; ++i) {
        fputc('-', output_file);
    }
    fputc(' ', output_file);
    fprintf(output_file, "------ -------------\n");
    for (unsigned i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
        if (shadow_symbol_table[i] != NULL) {
            entry_t *l = shadow_symbol_table[i];
            while (l != NULL) { 
                ref_list_t *t = l->lines;
                fprintf(output_file, "%-*s", MAX_TOKEN_LENGTH + 1, l->name);
                switch (l->qualifier) {
                    case NONE_T: {
                        fprintf(output_file, "%-11s", "");
                        break;
                    }
                    case CONST_T: {
                        fprintf(output_file, "%-11s", "const");
                        break;
                    }
                    case QUANTUM_T: {
                        fprintf(output_file, "%-11s", "quantum");
                        break;
                    }
                }
                unsigned type_str_length = 0;
                if (l->is_function) {
                    fprintf(output_file, "-> ");
                    type_str_length += 3;
                }
                switch (l->type) {
                    case BOOL_T: {
                        fprintf(output_file, "bool");
                        type_str_length += 4;
                        break;
                    }
                    case INT_T: {
                        fprintf(output_file, "int");
                        type_str_length += 3;
                        break;
                    }
                    case UNSIGNED_T: {
                        fprintf(output_file, "unsigned");
                        type_str_length += 8;
                        break;
                    }
                    case VOID_T: {
                        fprintf(output_file, "void");
                        type_str_length += 4;
                        break;
                    }
                }
                for (unsigned j = 0; j < l->depth; ++j) {
                    fprintf(output_file, "[]");
                    type_str_length += 2;
                }
                for (unsigned k = type_str_length; k < 12 + MAX_ARRAY_DEPTH * 2; ++k) {
                    fprintf(output_file, " ");
                }
                fprintf(output_file, "%-7u", l->scope);
                while (t != NULL) {
                    fprintf(output_file, "%-4u ", t->line_num);
                    t = t->next;
                }
                fprintf(output_file, "\n");
                l = l->next;
	        }
        }
    }
}
