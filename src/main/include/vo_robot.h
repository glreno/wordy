/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vo_robot.h
 *
 * Declarations for an animated robot opponent
 *
 */

#include "vo_anim.h"
#include "byteunion.h"

#ifndef __VO_ROBOT_H__
#define __VO_ROBOT_H__

/* ######################################################### */
/* #                      VO_ROBOT                         # */
/* ######################################################### */


typedef struct vo_robot_struct {
    vo_anim anim;           // offsets 0-B
    // vo_t specific stuff
    char queue[40];         // offsets B-0x33
    unsigned char *screenram; // offsets 0x34,35
    char x,y;               // offsets 0x36,37
    const unsigned char *spotlightBmp;   // offsets 0x38,39
    char spotlightSize;     // offset 0x3A
    byteunion laserTargetCoords; // offsets 0x3b,3c; 0 if not drawing laser
} vo_robot;

//vo_robot *vor_singleton; // Same zero page ptr as vo_singleton.
#define vor_singleton ((vo_robot *)vo_singleton)



// Called by vor_initialize to set up bank switching
void __fastcall__ vor_preinit(void);

void __fastcall__ vor_initialize(vo_robot *this, char y, ds_page **pages, char spotlightSize, const unsigned char *spotlightBmp, unsigned char*screenram);


#endif
