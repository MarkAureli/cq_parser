/**
 * \file                                rules.h
 * \brief                               Rules include file
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

#ifndef RULES_H
#define RULES_H


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
 *                                                macros
 * =====================================================================================================================
 */

#define MAXTOKENLEN 40
#define MAXARRAYDEPTH 3
#define ERRORMSGLENGTH 256
#define HASHTABLESIZE 211


/*
 * =====================================================================================================================
 *                                                closing C++ check & header guard
 * =====================================================================================================================
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // RULES_H
