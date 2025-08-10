/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* bankswitch.h
 *
 * Declarations XEGS cart bankswitch code
 *
 */

#ifndef __BANKSWITCH_H__
#define __BANKSWITCH_H__

#include <stdlib.h>

/* ######################################################### */
/* #                   BANKSWITCH ADDRESS                  # */
/* ######################################################### */

// Defined to be D500 in title.asm
extern char BANKSWITCH;

// switch to any cartridge bank (asm routine in title.asm)
void __fastcall__ bankswitch(unsigned char bankid);

// Switch to bank 6 (probably), convenience calls for the
// banks that are almost certainly there. These are all the same function.
void __fastcall__ bankswitchTitle();
void __fastcall__ bankswitchStartup();
void __fastcall__ bankswitchFontdata();
void __fastcall__ bankswitchGameInit();


#endif

