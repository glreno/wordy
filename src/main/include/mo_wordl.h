/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* mo_qordl.h
 *
 * Declarations for the opponent that drives a qordl game
 *
 */

#ifndef __MO_WORDL_H__
#define __MO_WORDL_H__

#include "mw_puz.h"
#include "md_dict.h"


/* ######################################################### */
/* #               FOUR PUZZLE OPPONENT                    # */
/* ######################################################### */

typedef struct mow_struct {
    mw *gameModel;
    md_dict *dict;
    void *vu_letters;
    char commandModeFlag;
} mow;

void __fastcall__ mow_initialize(mow *this,mw *gameModel, md_dict *dict, void *usedLetterView);
void __fastcall__ mow_gameDriver(mow *this);

#endif
