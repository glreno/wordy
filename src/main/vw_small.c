/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "ds_screendriver.h"
#include "vw_small.h"
#include <conio.h>
#include <string.h>
#include <atari.h>
#include <_antic.h>
#include <_atarios.h>

/*
* C portion of the implementation of wordl small-tile view
*/

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"GAMEINIT")
#pragma rodata-name (push,"GAMEINIT")
#pragma local-strings (on)

// internal codes for a row of blank tiles.
// Not using a string because it's full of \\ and \0
const char BLANKLINE[16] = {
    0x3B, 0x60, 0x40,
    0x3C, 0x60, 0x40,
    0x3C, 0x60, 0x40,
    0x3C, 0x60, 0x40,
    0x3C, 0x60, 0x40,
    0x3D
};

//segment GAMEINIT
/** Initialize data structure, and draw grid of blank tiles */
void vws_initialize( vw_base *this)
{
    char i;
    char x;
    char y;
    char nrows=this->nrows;
    // Precalulate X and Y coordinate
    x=this->x+1;
    for(i=0;i<5;++i)
    {
        this->xoff[i]=x;
        x+=3;
    }
    this->ysz=2;
    this->yoff=1;
    // You are expected to have already called vws_initialize

    // Draw the tiles
    x=this->x;
    y=this->y;
              // 0123456789ABCDE
    cputsxy(x,y," ^^ ^^ ^^ ^^ ^^");
    ++y;
    for(i=0;i<nrows;++i)
    {
        // can't use cputsxy, because this string contains \0
        ds_puts(BLANKLINE,16,x,y);
        ++y;
        if ( i<nrows-1 )
        {
            cputsxy(x,y," @@ @@ @@ @@ @@");
            ++y;
        }
    }
    cputsxy(x,y," __ __ __ __ __");
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code



