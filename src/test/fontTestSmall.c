/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Unit tests for the vw_small.h font functions,
 * showing a single font. */

#include "ds_screendriver.h"
#include "vw_small.h"
#include "vfm_fontmanager.h"
#include "awordlFont.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

extern unsigned char DS_NEXTBANK;
extern unsigned char DS_ATCBV;
unsigned char *SCREENRAM;

/* Locations in the 2K __RESERVED_MEMORY__ which ends at A000 */

/* BANK0 is a completely blank screen -- the font points to the empty
* player area at $9C00 */

ds_page *BANK;
unsigned char *FONTLIST0[1];
unsigned char *FONTLIST1[1];
unsigned char *FONTLIST2[1];
vfm_fontmanager fontmanager0;
vfm_fontmanager fontmanager1;
vfm_fontmanager fontmanager2;

vw_base grey;
vw_base yellow;
vw_base green1;
vw_base green2;
vw_base green3;
vw_base zed;

void mkDlist0(ds_pageHeader *b)
{
    int i;
    unsigned int *p;
    b->displaylist[0]=0x70;
    b->displaylist[1]=0x70;
    b->displaylist[2]=0xF0; // dli starts running in first blank row!
    b->displaylist[3]=0xC2; // mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    for(i=6;i<29;++i)
    {
        b->displaylist[i]=0x82;
    }
    b->displaylist[29]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[30]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

void mkDlist1(ds_pageHeader *b)
{
    int i;
    unsigned int *p;
    b->displaylist[0]=0x70;
    b->displaylist[1]=0x70;
    b->displaylist[2]=0xF0; // dli starts running in first blank row!
    b->displaylist[3]=0xC2; // mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    for(i=6;i<29;++i)
    {
        b->displaylist[i]=0x84;
    }
    b->displaylist[29]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[30]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

int main(void)
{
    int x,y;
    char fontids[3];

    if ( OS.ramtop < 0x40 ) { cputs("Min 16K"); for(;;); } // 3C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    SCREENRAM = (unsigned char *) OS.appmhi+1; // 9420 on 48K machine, 3420 on 16K machine, 1420 on 8K
    BANK = (ds_page *) (OS.appmhi+0x3E1); // 2K screen page // 9800 on 48K machine, 3800 on 16K machine, 1800 on 8K
    FONTLIST0[0] = (unsigned char*) BANK+2048;
    FONTLIST1[0] = (unsigned char*) BANK+3072;
    FONTLIST2[0] = (unsigned char*) BANK+4096;

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);
    vfm_initialize(&fontmanager0,&AWORDLFONTDEF,1,FONTLIST0);
    vfm_initialize(&fontmanager1,&AWORDLFONTDEF,1,FONTLIST1);
    vfm_initialize(&fontmanager2,&AWORDLFONTDEF,1,FONTLIST2);
    // These will all allocate font 0
    fontids[0]=vfm_allocateFont(&fontmanager0);
    fontids[1]=vfm_allocateFont(&fontmanager1);
    fontids[2]=vfm_allocateFont(&fontmanager2);

    // Create the screen pages
    ds_init_page(BANK);
    BANK->page.myAddress = BANK;

    // Create a display list in all pages
    mkDlist1(&(BANK->page));

    // initialize tile page
    BANK->colors.color0 = 0x04;
    BANK->colors.color1 = 0xD4;
    BANK->colors.color2 = 0x08;
    BANK->colors.color3 = 0xE8;
    BANK->colors.color4 = 0;

    // Load the BANK
    ds_init(BANK);

    // Start font manager and build fonts
    // (this has to be after ds_init for this test since we're putting the font in the PM area)
    ds_setFont(BANK,0xE0,0,-3,0);
    ds_setFont(BANK,vfm_getFontBase(fontids[0],&fontmanager0),-3,1,15);
    ds_setFont(BANK,vfm_getFontBase(fontids[1],&fontmanager1),-3,16,19);
    ds_setFont(BANK,vfm_getFontBase(fontids[2],&fontmanager2),-3,20,34);

    cputs("This is a test");
    // Initialize six vw_small views
    // Yellow and Grey are both 5x5, so are a letter short.
    // So there is also a very small one-row vw_small for z
    // And also three for Green, since each fontmanager can only handle 10 green characters.
    vw_initialize(&fontmanager0,5,1,1,&grey);
    vws_initialize(&grey);
    vw_initialize(&fontmanager0,5,1,22,&yellow);
    vws_initialize(&yellow);
    vw_initialize(&fontmanager0,1,13,1,&green1);
    vws_initialize(&green1);
    vw_initialize(&fontmanager1,2,15,1,&green2);
    vws_initialize(&green2);
    vw_initialize(&fontmanager2,2,19,1,&green3);
    vws_initialize(&green3);
    vw_initialize(&fontmanager0,1,13,22,&zed);
    vws_initialize(&zed);

    // Write the alphabet in grey,yellow,green
    for(y=0;y<5;y++)
    {
        for(x=0;x<5;x++)
        {
            vw_writeTile(&grey,x,y,VW_GREY,'A'+(y*5)+x);
            vw_writeTile(&yellow,x,y,VW_YELLOW,'A'+(y*5)+x);
        }
    }
    vw_writeTile(&zed,0,0,VW_GREY,'Z');
    vw_writeTile(&zed,2,0,VW_YELLOW,'Z');

    // test cleartile by writing something then clearing it
    vw_writeTile(&zed,3,0,VW_YELLOW,'Z');
    vw_clearTile(&zed,3,0);

    // page flip to page, and build the font
    ds_flipToPageNextVBIandWait(BANK);

    for(x=0;x<5;x++)
    {
        vw_writeTile(&green1,x,0,VW_GREEN,'A'+x);

        vw_writeTile(&green2,x,0,VW_GREEN,'F'+x);

        vw_writeTile(&green2,x,1,VW_GREEN,'K'+x);

        vw_writeTile(&green3,x,0,VW_GREEN,'P'+x);

        vw_writeTile(&green3,x,1,VW_GREEN,'U'+x);
    }
    vw_writeTile(&zed,4,0,VW_GREEN,'Z');

    for(;;)
    {
    }
    return 0;
}
