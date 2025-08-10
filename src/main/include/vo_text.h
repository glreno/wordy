/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vo_text.h
 *
 * Declarations for the ridiculously simple text-only opponent view.
 *
 */

#include "vo_base.h"
#include "ds_screendriver.h"

#ifndef __VO_TEXT_H__
#define __VO_TEXT_H__

/* ######################################################### */
/* #                      VO_TEXT                          # */
/* ######################################################### */

typedef struct vo_text_struct {
    vo_base base;
    // vo_t specific stuff
    char queue[40];
    ds_page *page;
    unsigned char *screenram;
    char x,y;
    char spotlightY;
    const unsigned char *spotlightBmp;
    char spotlightSize;
} vo_text;




// Called by vot_initialize to set up bank switching
void __fastcall__ vot_preinit(void);

void __fastcall__ vot_initialize(vo_text *this, char y, ds_page *page, char spotlightSize, const unsigned char *spotlightBmp, unsigned char*screenram);

// Select the palette, because the colors I picked suck
void __cdecl__ vot_setPalette(char p);

#endif
