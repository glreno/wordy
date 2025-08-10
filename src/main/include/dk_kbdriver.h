/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* dk_kbdriver.h
 *
 * Declarations for the keyboard driver
 *
 */

#ifndef __DK_KBDRIVER_H__
#define __DK_KBDRIVER_H__

/* ######################################################### */
/* #                 KEYBOARD DRIVER                       # */
/* ######################################################### */

// Find the OS's keymap.
// If it's not already in KEYMAP ($79,$7A) store it there.
// In the 400/800 this is the addresses ROWINC and COLINC,
// which are used by the S: driver's DRAWTO.
// So DO NOT USE WITH DRAWTO!!!
void __fastcall__ dk_initialize(void);

// just like cgetc but it keeps calling yield().
// And it doesn't check for the BREAK key.
// Or support the ESC key or caps lock or Atari.
char __fastcall__ dk_getc(void);

#endif
