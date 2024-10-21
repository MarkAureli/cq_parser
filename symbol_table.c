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
static struct entry **symbol_table;

/**
 * \brief                               Whether symbol table entries are hidden upon leaving the scope
 */
static bool hide;

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

/* See header for documentation */
void init_symbol_table(bool dump_mode) {
    symbol_table = calloc(SYMBOL_TABLE_SIZE, sizeof(entry_t*));
    hide = !dump_mode;
    cur_scope = 0;
}

/* See header for documentation */
void free_symbol_table() {
    // TODO: free symbol table
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
entry_t *insert(const char *name, unsigned length, unsigned line_num, bool declaration) {
    unsigned hash_value = hash(name);
    entry_t *l = symbol_table[hash_value];
	
    while ((l != NULL) && (strcmp(name,l->name) != 0)) {
        l = l->next;
    }

    if (l == NULL) {
        if (declaration == false) {
            fprintf(stderr, "Undeclared identifier %s at line %u\n", name, line_num);
            exit(1);
        }
        l = (entry_t*) calloc(1, sizeof (entry_t));
        strncpy(l->name, name, length);
        l->scope = cur_scope;
        l->lines = calloc(1, sizeof (ref_list_t));
        l->lines->line_num = line_num;
        l->lines->next = NULL;
        l->qualifier = NONE_T;
        l->type = VOID_T;
        l->next = symbol_table[hash_value];
        symbol_table[hash_value] = l;
    } else {
        if (declaration == true) {
            if (l->scope == cur_scope) {
                fprintf(stderr, "Multiple declaration of identifier %s at line %u (previous declaration in line %u)\n",
                        name, line_num, l->lines->line_num);
                exit(1);
            } else {
                l = calloc(1, sizeof (entry_t));
                strncpy(l->name, name, length);
                l->scope = cur_scope;
                l->lines = calloc(1, sizeof (ref_list_t));
                l->lines->line_num = line_num;
                l->lines->next = NULL;
                l->qualifier = NONE_T;
                l->type = VOID_T;
                l->next = symbol_table[hash_value];
                symbol_table[hash_value] = l;
            }
        } else {
            ref_list_t *t = l->lines;
            while (t->next != NULL) {
                t = t->next;
            }
            t->next = calloc(1, sizeof (ref_list_t));
            t->next->line_num = line_num;
            t->next->next = NULL;
        }
    }
    return l;
}

/* See header for documentation */
void hide_scope() {
    if (hide) {
        for (unsigned i = 0; i < SYMBOL_TABLE_SIZE; ++i) {
            if (symbol_table[i] != NULL) {
                entry_t *l = symbol_table[i];
                while (l != NULL && l->scope == cur_scope) {
                    l = l->next;
                }
                symbol_table[i] = l;
            }
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
void dump_symbol_table(FILE * output_file){
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
        if (symbol_table[i] != NULL) {
            entry_t *l = symbol_table[i];
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
