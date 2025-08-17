/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* w_wordl.h
 *
 * Declarations for single wordl game
 *
 */

#ifndef __MD_PICK_H__
#define __MD_PICK_H__

#include "md_dict.h"

/* Choose the correct dictionary.
 * Pass in 'e','n',h', and pointers to the title text.
 * It will check PAL itself.
 * *locale will be set to S or K
 * *level will be set to E/N/H
 * Returns the address of the chosen dictionary.
 */
md_dict* __fastcall__ md_pickDictionary(char* locale, char *level, unsigned char choice);


#endif
