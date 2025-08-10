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
#include <string.h>
#include <atari.h>
#include <_antic.h>
#include <_atarios.h>

/*
* C portion of the implementation of screen driver.
*/
extern void DS_DLI(void);
extern unsigned char DS_NEXTPAGE;

// Callbacks -- always disable the callback before changing it!
// This is why there is a setDli/FlipCallback() function
extern unsigned char DS_DLICBV;
extern unsigned char DS_PFCBV;
extern unsigned char DS_ATCBV;
extern unsigned int DS_ATTIME;
extern void *DS_DLICB;
extern void *DS_PFCB;
extern void *DS_ATCB;

extern unsigned char DS_CURRPAGE;
#pragma zpsym ("DS_CURRPAGE");  /* DS_CURRPAGE is in the zeropage */

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"ONCE")
#pragma rodata-name (push,"ONCE")
#pragma local-strings (on)

//segment ONCE
void ds_initScreenRam(unsigned char *const location, size_t length)
{
    memset(location, 0, length);
    /* In Gr.0, TXTMSC-SAVMC = 800 bytes, which  is 20x40 chars per line */
    OS.savmsc = location;       /* out of box, 9C40 */
    OS.txtmsc = location+800;   /* out of box, 9F60 */
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

#pragma code-name (push,"DS")

//segment DS
void ds_disableScreen()
{
    OS.sdmctl = 0; // disable screen
    // You probably need to call ds_init next!
}

//segment DS
void ds_init(ds_page *page)
{
    // Enable PM graphics
    GTIA_WRITE.gractl = 3;
    OS.gprior = 32;

    // Enable DLI

    //ANTIC.nmien = 0; // Turn off interrupts (not needed since vdslst is not already set)
    OS.vdslst = &DS_DLI;
    ANTIC.nmien = 192; // 128(DLI)+64(VBI) (Why not 32(reset button)?)
    // (VBI is automatically registered by cc65 runtime)

    // Page flip!
    ds_flipToPageNextVBIandWait(page);

    // Enable the screen
    OS.sdmctl = DMACTL_PLAYFIELD_NORMAL | DMACTL_DMA_MISSILES | DMACTL_DMA_PLAYERS | DMACTL_PMG_SINGLELINE | DMACTL_DMA_FETCH;
}

//segment DS
void ds_init_page(ds_page *page)
{
    bzero(page->page.pmpos, 22);
    memset(page->page.fonts,0xE0,156);
    bzero(page->pm.missiles, 256*5);
    page->page.myAddress = page;
    // Load the system default colors into the page.
    // Note that this was just zeroed out with pmpos, rather than
    // zeroing out the PM colors separately.
    ds_setStandardPalette(0,page);
}

//segment DS
/*
* Load a font into a page.
* Update a range in the fontlist to point to a specific font.
* Range is given in text rows, but the data structure stores in VCOUNT rows -- 4 per text row.
* offset_vcount should be zero, but you may need to add a line or two to match your display list.
*/
void ds_setFont(ds_page *page, unsigned char fontbase, signed char offset_vcount, char start, char end)
{
    char i;

    // First VCOUNT the DLI hits is 15, so call that 0.
    // Min is -3: 15 + 4*-3 = 3
    // Zero: 15+4*0 = 15
    char s = 15+4*start;

    // Max is 34: 15+4*34+3 = 154, which is the size of the font list -1
    char e = 15+4*end+3;

    for(i=s;i<=e;++i)
    {
        page->page.fonts[i] = fontbase;
    }
}


#pragma code-name (pop)

