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
 * and also a font sampler.
 * This does NOT use the vfm_fontmanager, since this test is all about comparing
 * completely different fonts, not fast-swapping very similar fonts.
 */

#include "ds_screendriver.h"
#include "vw_small.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

extern unsigned char DS_NEXTBANK;
extern unsigned char DS_ATCBV;
unsigned char *const SCREENRAM = (unsigned char *) 0x6c20;

/* Locations in the __RESERVED_MEMORY__ which ends at A000 */

/* BANK0 is a completely blank screen -- the font points to the empty
* player area at $8400 */

const unsigned char FONT0BASE = 0x84;
const unsigned char FONT1BASE = 0x70;
const unsigned char FONT2BASE = 0x74;
const unsigned char FONT3BASE = 0x78;
const unsigned char FONT4BASE = 0x7c;

ds_page *const BANK0 = (ds_page *) 0x8000; // 2K screen page - plain gr.0
ds_page *const BANK1 = (ds_page *) 0x8800; // 2K screen page - mode 4, with one line of gr.0 at top
ds_page *const BANK2 = (ds_page *) 0x9000; // 2K screen page
ds_page *const BANK3 = (ds_page *) 0x9800; // 2K screen page
unsigned char *const FONT1 = (unsigned char*) 0x7000; // 1K font
unsigned char *const FONT2 = (unsigned char*) 0x7400; // 1k font
unsigned char *const FONT3 = (unsigned char*) 0x7800; // 1K font
unsigned char *const FONT4 = (unsigned char*) 0x7C00; // 1K font

vw_base grey;
vw_base yellow;
vw_base green;
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

const char FONTDEF1[68] = {
    0x08, 0xE1, // 0xE108
    // yellow left - yellow and light grey are 11
    0b00111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b00111111,
    // yellow right
    0b11111100,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111100,

    // green left - green is 10
    0b00101010,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b00101010,
    // green right
    0b10101000,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b10101000,

    0,0 // NO BOX ART
};

// Take the black dots out of the grey/yellow tile corners
const char FONTDEF2[68] = {
    0x08, 0xE1, // 0xE108
    // yellow left - yellow and light grey are 11
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111,
    // yellow right
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111,

    // green left - green is 10
    0b01101010,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b01101010,
    // green right
    0b10101001,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b10101001,

    1,1,
    // box left - drak grey is 01
    0x01, 0x01, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,
    // box right
    0x40, 0x40, 0x40, 0x40,   0x40, 0x40, 0x40, 0x40,
    // box top
    0, 0, 0, 0,    0, 0, 0, 0x55,
    // box bottom
    0x55, 0, 0, 0,    0, 0, 0, 0,
};

// Take the black dots out of the grey/yellow tile corners
// and add a highlight in the left-side corners
const char FONTDEF3[68] = {
    0x08, 0xE1, // 0xE108
    // yellow left - yellow and light grey are 11
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111,
    // yellow right
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111,

    // green left - green is 10
    0b11011010,
    0b01101010,
    0xaa, 0xaa,   0xaa, 0xaa,
    0b01101010,
    0b11011010,
    // green right
    0b10101001,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b10101001,

    1,1,
    // box left - drak grey is 01
    0x01, 0x01, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,
    // box right
    0x40, 0x40, 0x40, 0x40,   0x40, 0x40, 0x40, 0x40,
    // box top
    0, 0, 0, 0,    0, 0, 0, 0x55,
    // box bottom
    0x55, 0, 0, 0,    0, 0, 0, 0,
};


// Failed font -- tried to add another column & row to make the tiles bigger
/*
const char FONTDEF_fail[64] = {
    // yellow left - yellow and light grey are 11
    0b00111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b00111111,
    // yellow right
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111,

    // green left - green is 10
    0b00101010,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b00101010,
    // green right
    0b10101010,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b10101010,

    // box left - drak grey is 01
    0x01, 0x01, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01,
    // box right
    // 11 01 = d; 00 01 = 1 ; 01 01 = 5
    0xd0, 0xd0, 0xd0, 0xd0,   0xd0, 0xd0, 0xd0, 0x50,
    // box top
    0, 0, 0, 0,    0, 0, 0, 0x55,
    // box bottom
    0xff, 0x55, 0, 0,    0, 0, 0, 0,
};
*/

int main(void)
{
    int x,y,first;

    if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);

    // Create the screen pages
    ds_init_page(BANK0);
    ds_init_page(BANK1);
    ds_init_page(BANK2);
    ds_init_page(BANK3);
    BANK0->page.myAddress = BANK0;
    BANK1->page.myAddress = BANK1;
    BANK2->page.myAddress = BANK2;
    BANK3->page.myAddress = BANK3;

    // Create a display list in all pages
    mkDlist0(&(BANK0->page));
    mkDlist1(&(BANK1->page));
    mkDlist1(&(BANK2->page));
    mkDlist1(&(BANK3->page));

    // initialize tile page 1
    ds_setFont(BANK1,0xE0,0,-3,0);
    ds_setFont(BANK1,FONT1BASE,-3,1,34);
    BANK1->colors.color0 = 0x04;
    BANK1->colors.color1 = 0xD4;
    BANK1->colors.color2 = 0x08;
    BANK1->colors.color3 = 0xE8;
    BANK1->colors.color4 = 0;

    // initialize tile page 2
    ds_setFont(BANK2,0xE0,0,-3,0);
    ds_setFont(BANK2,FONT2BASE,-3,1,34);
    BANK2->colors.color0 = 0x04;
    BANK2->colors.color1 = 0xD4;
    BANK2->colors.color2 = 0x08;
    BANK2->colors.color3 = 0xE8;
    BANK2->colors.color4 = 0;

    // initialize tile page 3
    ds_setFont(BANK3,0xE0,0,-3,0);
    ds_setFont(BANK3,FONT3BASE,-3,1,34);
    BANK3->colors.color0 = 0x04;
    BANK3->colors.color1 = 0xD4;
    BANK3->colors.color2 = 0x08;
    BANK3->colors.color3 = 0xE8;
    BANK3->colors.color4 = 0;

    // Load the BANK0
    ds_init(BANK0);
    cputsxy(7,0,"Press a key to continue");
    cgetc();

    // Initialize three vw_small views
    // These are all 5x5, so are a letter short.
    // So there is also a very small vw_small for z
    vw_initialize(NULL,5,1,1,&grey);
    vws_initialize(&grey);
    vw_initialize(NULL,5,1,22,&yellow);
    vws_initialize(&yellow);
    vw_initialize(NULL,5,13,1,&green);
    vws_initialize(&green);
    vw_initialize(NULL,1,13,22,&zed);
    vws_initialize(&zed);

    // Write the alphabet in grey,yellow,green
    for(y=0;y<5;y++)
    {
        for(x=0;x<5;x++)
        {
            vw_writeTile(&grey,x,y,VW_GREY,'A'+(y*5)+x);
            // VW_YELLOW will invert the chars
            vw_writeTile(&yellow,x,y,VW_YELLOW,'A'+(y*5)+x);
            // since we allocate the green letters manually in this test,
            // just tell it to print them in GREY, which doesn't mangle the character.
            // vw_writeTile(VW_GREEN) allocates a new green character.
            vw_writeTile(&green,x,y,VW_GREY/*GREEN*/,'!'+(y*5)+x);
        }
    }
    vw_writeTile(&zed,0,0,VW_GREY,'Z');
    vw_writeTile(&zed,2,0,VW_YELLOW,'Z');
    vw_writeTile(&zed,4,0,VW_GREY/*GREEN*/,':');

    // test cleartile by writing something then clearing it
    vw_writeTile(&zed,3,0,VW_YELLOW,'Z');
    vw_clearTile(&zed,3,0);

    first = 1;
    for(;;)
    {
        // pause for effect
        cgetc();

        // page flip to page 1, and build the font 1
        ds_flipToPageNextVBIandWait(BANK1);
        if ( first )
        {
            vfm_buildFont(FONT1,(vfm_fontdef*)FONTDEF1);
            for(x=0;x<26;x++)
            {
                vfm_createGreenLetter(FONT1,(vfm_fontdef*)FONTDEF1,x,x-15);
            }
        }

        // pause for effect
        cgetc();

        // page flip to page 2, and build the font 2
        ds_flipToPageNextVBIandWait(BANK2);
        if ( first )
        {
            vfm_buildFont(FONT2,(vfm_fontdef*)FONTDEF2);
            for(x=0;x<26;x++)
            {
                vfm_createGreenLetter(FONT2,(vfm_fontdef*)FONTDEF2,x,x-15);
            }
        }

        // pause for effect
        cgetc();

        // page flip to page 3, and build the font 3
        ds_flipToPageNextVBIandWait(BANK3);
        if ( first )
        {
            vfm_buildFont(FONT3,(vfm_fontdef*)FONTDEF3);
            for(x=0;x<26;x++)
            {
                vfm_createGreenLetter(FONT3,(vfm_fontdef*)FONTDEF3,x,x-15);
            }
        }

        first=0;
    }
    return 0;
}
