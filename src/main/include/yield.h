/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* yield.h
 *
 * Declarations for two cooperative threads
 *
 */

#ifndef __YIELD_H__
#define __YIELD_H__

#include <stdlib.h>

/* ######################################################### */
/* #       YIELD CONTROL TO SECONDARY THREAD               # */
/* ######################################################### */

extern void *YIELDVEC;

// Call the secondary thread
void __fastcall__ yield(void);

// Register a function to be the seconday thread
// func should be address of a function that takes no params and returns void
void __fastcall__ setYieldFunc(void *func);


#endif

