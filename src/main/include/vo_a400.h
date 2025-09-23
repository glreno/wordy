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
 * Declarations for a view that shows a little computer
 *
 */

//#include "vo_base.h"
//#include "ds_screendriver.h"

#ifndef __VO_A400_H__
#define __VO_A400_H__

/* ######################################################### */
/* #                      VO_A400                          # */
/* ######################################################### */

typedef struct vo_a400_struct {
    unsigned char *font;
} vo_a400;




// Called by vot_initialize to set up bank switching
//void __fastcall__ vot_preinit(void);

void __fastcall__ vo4_initialize(unsigned char *font1, unsigned char *font2);


void __fastcall__ vo4_animateTimerCallback();

#endif
