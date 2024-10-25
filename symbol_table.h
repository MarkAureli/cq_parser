/**
 * \file                                symbol_table.h
 * \brief                               Symbol table include file
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
 *                                                header guard
 * =====================================================================================================================
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


/*
 * =====================================================================================================================
 *                                                includes
 * =====================================================================================================================
 */

#include <stdbool.h>
#include <stdio.h>
#include "rules.h"


/*
 * =====================================================================================================================
 *                                                C++ check
 * =====================================================================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
 * =====================================================================================================================
 *                                                type definitions
 * =====================================================================================================================
 */

/**
 * \brief                               Qualifier enumeration
 */
typedef enum qualifier {
    NONE_T,                                 /*!< No qualifier */
    CONST_T,                                /*!< Constant qualifier */
    QUANTUM_T,                              /*!< Quantum qualifier */
} qualifier_t;

/**
 * \brief                               Type enumeration
 */
typedef enum type {
    VOID_T,                                 /*!< Void type */
    BOOL_T,                                 /*!< Bool type */
    INT_T,                                  /*!< Integer type */
    UNSIGNED_T,                             /*!< Unsigned integer type */
} type_t;

/**
 * \brief                               Value union
 */
typedef union value {
    bool b_val;                             /*!< Bool value */
    int i_val;                              /*!< Integer */
    unsigned u_val;                         /*!< Unsigned integer value */
} value_t;

/**
 * \brief                               Qualified type struct
 * \note                                This structure defines the qualified type (qualifier + type) of a variable
 */
typedef struct q_type {
    qualifier_t qualifier;                  /*!< Qualifier of qualified type */
    type_t type;                            /*!< Type of qualified type */
} q_type_t;

/**
 * \brief                               Type info struct
 * \note                                This structure defines the full type information of a variable
 */
typedef struct type_info {
    qualifier_t qualifier;                  /*!< Qualifier of type information */
    type_t type;                            /*!< Type of type information */
    unsigned sizes[MAX_ARRAY_DEPTH];        /*!< Sizes of type information */
    unsigned depth;                         /*!< Depth of type information */
} type_info_t;

/**
 * \brief                               Reference entry struct
 * \note                                This structure defines a linked entry of references (line numbers) to a variable
 */
typedef struct ref_list { 
    unsigned line_num;                      /*!< Line number */
    struct ref_list *next;                  /*!< Next reference list entry */
} ref_list_t;

/**
 * \brief                               Symbol table entry struct
 * \note                                This structure defines the symbol table as a linked list of entries
 */
typedef struct entry {
    char name[MAX_TOKEN_LENGTH];            /*!< Name of entry */
    unsigned scope;                         /*!< Scope of entry */
    ref_list_t *lines;                      /*!< List of references of entry */
    qualifier_t qualifier;                  /*!< Qualifier of entry */
    type_t type;                            /*!< Type of entry */
    unsigned sizes[MAX_ARRAY_DEPTH];        /*!< Sizes of entry */
    unsigned depth;                         /*!< Depth of entry */
    unsigned length;                        /*!< Length of flattened array of entry's values */
    bool is_function;                       /*!< Whether entry is a function */
    union {
        value_t *values;                    /*!< Array of entry's values */
        struct {
            bool is_unitary;                /*!< Whether function is unitary */
            bool is_quantizable;            /*!< Whether function can be used for creating a superposition */
            type_info_t *pars_type_info;    /*!< Type information of function parameters */
            unsigned num_of_pars;           /*!< Number of function parameters */
        };
    };
    struct entry *next;                     /*!< Next symbol table entry */
} entry_t;


/*
 * =====================================================================================================================
 *                                                function declarations
 * =====================================================================================================================
 */

/**
 * \brief                               Convert qualifier to printable string
 * \param[in]                           qualifier: Qualifier
 * \return                              String representing input qualifier
 */
char *qualifier_to_str(qualifier_t qualifier);

/**
 * \brief                               Convert type to printable string
 * \param[in]                           type: Type
 * \return                              String representing input type
 */
char *type_to_str(type_t type);

/**
 * \brief                               Initialize symbol table
 */
void init_symbol_table();

/**
 * \brief                               Free symbol table
 */
void free_symbol_table();

/**
 * \brief                               Insert entry in symbol table and return pointer to that entry
 * \param[in]                           name: Name of entry
 * \param[in]                           length: Length of name string
 * \param[in]                           line_num: Line number of appearance
 * \param[in]                           declaration: Whether appearance is declaration
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Pointer to new symbol table entry
 */
entry_t *insert(const char *name, unsigned length, unsigned line_num, bool declaration,
                char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Hide all symbol table entries of current scope and decrease scope counter
 */
void hide_scope();

/**
 * \brief                               Increase scope counter
 */
void incr_scope();

/**
 * \brief                               Set type information of symbol table entry
 * \param[in]                           entry: Symbol table entry
 * \param[in]                           qualifier: Qualifier to be set
 * \param[in]                           type: Type to be set
 * \param[in]                           sizes: Sizes to be set
 * \param[in]                           depth: Depth to be set
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting the type information was successful
 */
bool set_type_info(entry_t *entry, qualifier_t qualifier, type_t type, const unsigned sizes[MAX_ARRAY_DEPTH],
                   unsigned depth, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Set function information of symbol table entry
 * \note                                Also sets \ref is_function of entry to true
 * \param[in]                           entry: Symbol table entry
 * \param[in]                           is_unitary: Whether function is unitary
 * \param[in]                           is_quantizable: Whether function can be used for creating a superposition
 * \param[in]                           pars_type_info: Type information of function parameters
 * \param[in]                           num_of_pars: Number of function parameters
 * \param[out]                          error_msg: Message to be written in case of an error
 * \return                              Whether setting the function information was successful
 */
bool set_func_info(entry_t *entry, bool is_unitary, bool is_quantizable, type_info_t *pars_type_info,
                   unsigned num_of_pars, char error_msg[ERROR_MSG_LENGTH]);

/**
 * \brief                               Write symbol table content to output file
 * \param[out]                          output_file: Output file for symbol table
 */
void fprint_symbol_table(FILE *output_file);


/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SYMBOL_TABLE_H */
