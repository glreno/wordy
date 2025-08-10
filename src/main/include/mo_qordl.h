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

#ifndef __MO_QORDL_H__
#define __MO_QORDL_H__

#include "mq_puz.h"
#include "md_dict.h"


/* ######################################################### */
/* #               FOUR PUZZLE OPPONENT                    # */
/* ######################################################### */

typedef struct moq_struct {
    mq *gameModel;
    md_dict *dict;
    void *vu_letters;
    char commandModeFlag;
} moq;

void __fastcall__ moq_initialize(void *usedLetters, md_dict *dict, mq *gameModel, moq *this);
void __fastcall__ moq_gameDriver(const char *title, moq *this);

#endif
