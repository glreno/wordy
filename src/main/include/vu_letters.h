/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vu_letters.h
 *
 * Declarations for the Used Letters View
 *
 */

#ifndef __V_USED_LETTERS_H__
#define __V_USED_LETTERS_H__

#include <stdlib.h>

/* ######################################################### */
/* #                 USED LETTERS                          # */
/* ######################################################### */


// no data structure needed, just a screen address
// Address is to 5 lines of ANTIC mode E graphics
// Letters will be 5x5, each letter taking 6 pixels or 1.5 bytes; 26*1.5=39 bytes
// White(bits 11): Unused
// Grey (bits 01): Used, and not in solution
// Green (bit 10): Used, part of solution (green OR yellow in any puzzle)

// Initialize - draw all the letters on the screen
void __fastcall__ vu_initialize(void *address);

// Change the specified letter to grey(used)
// Letter should be an uppercase ascii letter
// No effect if letter is already grey
void __fastcall__ vu_grey(char letter, void *address);

// Change the specified letter to green(used)
// Letter should be an uppercase ascii letter
// No effect if letter is already green
void __fastcall__ vu_green(char letter, void *address);

#endif
