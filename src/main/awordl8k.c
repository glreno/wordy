/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include <_atarios.h>
#include <atari.h>
#include <conio.h>
#include <string.h>
#include "bankswitch.h"
#include "title.h"
#include "ds_screendriver.h"
#include "vfm_fontmanager.h"
#include "awordlFont.h"
#include "mw_puz.h"
#include "vw_small.h"
#include "md_dict.h"
#include "mo_wordl.h"
#include "vo_text.h"
#include "yield.h"
#include "dk_kbdriver.h"

/* VIDEO DATA */

// SCREENRAM and PAGE are only globals for debugging purposes
unsigned char *SCREENRAM;
ds_page *PAGE;

unsigned char *FONTLIST[1];
vfm_fontmanager fontmanager;

/* The dictionaries */
extern md_dict DICT_HS;
extern md_dict DICT_HA;

/* Game code (these don't need to be global, really) */
mw gameModel;
vo_text opponentView;
mow opponentModel;
void *vu_letters;

const unsigned char SPOTLIGHT[] = {
    0b01111110,
    0b11111111,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11000011,
    0b11111111,
    0b01111110
};

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"ONCE")
#pragma rodata-name (push,"ONCE")
#pragma local-strings (on)

//segment ONCE
void mkDlist(ds_pageHeader *b)
{
    char i;
    unsigned int *p;
    // This would be smaller as a memcpy but we're in the ONCE segment so size doesn't matter as much as readability.
    b->displaylist[0]=0x70;
    b->displaylist[1]=0x70;
    b->displaylist[2]=0xF0; // 0x80+0x70 (8 blank rows) dli starts running in first blank row!
    b->displaylist[3]=0xC2; // mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    for(i=6;i<25;++i)
    {
        b->displaylist[i]=0x84;
    }
    b->displaylist[25]=0x70;
    for(i=26;i<31;++i)
    {
        b->displaylist[i]=0x0e;//five lines of mode e at the end of things
    }
    b->displaylist[31]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[32]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

//segment ONCE
void initializeAWordl()
{
    char shown = 0;
    unsigned char *FONT;
    unsigned char FONTBASE;

    //
    // INITIALIZATION CODE AND LICENSE SCREEN
    //

    // You may not bankswitch while the Title/License is on screen!
    // Before bankswitching, you must:
    // occasionaly check title_show_licence_on_L()
    // eventually title_show_press_a_key() and title_wait_for_key(shown);
    // build a display page, and page flip

    // If someone presses L, show the license screen.
    // This is something you call periodically while loading
    shown = shown || title_show_license_on_L();

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    // Remember that PAGE is a pmbase value, so must be on a 2K boundary.
    // and that SCREENRAM needs dlist help if it crosses a 4K boundary.
    SCREENRAM = (unsigned char*) (OS.appmhi+1); // 25*40=1000 (0x3E8) bytes of screen RAM
    FONT = (unsigned char*) (SCREENRAM + 0x3E8); // 1K font
    PAGE = (ds_page*) (FONT+1024); // 2K of screen page AFTER the font

    // Addresses:   8K      16K     32K+
    // SCREENRAM    1018    3018    7018
    // FONT         1400    3400    7400
    // PAGE         1800    3800    7800


    FONTBASE = ((unsigned int)FONT)>>8;
    FONTLIST[0]=FONT;

    // Create a screen
    ds_initScreenRam(SCREENRAM, 40*25); // this sets SAVMSC and zeroes out the given amount of space
    vot_initialize(&opponentView, 0, PAGE, 8, SPOTLIGHT, SCREENRAM);
    // Create the screen pages
    ds_init_page(PAGE);

    shown = shown || title_show_license_on_L();

    // Create a display list in all pages
    mkDlist(&(PAGE->page));

    // initialize tile page
    ds_setDefaultPalette(PAGE);

    shown = shown || title_show_license_on_L();

    // Start font manager and build fonts
    // (this has to be after ds_init_page for this test since we're putting the font in the PM area)
    vfm_initialize(&fontmanager,&AWORDLFONTDEF,1,FONTLIST);
    ds_setFont(PAGE,0xE0,0,-3,0);
    ds_setFont(PAGE,FONTBASE,0,1,23);
    ds_setFont(PAGE,0xE0,0,24,34);

    shown = shown || title_show_license_on_L();

    // Initialize the used letter display
    vu_letters=SCREENRAM+20*40;

    shown = shown || title_show_license_on_L();

    // Initialize the game model
    vws_initialize(mw_initialize(&fontmanager,6,2,1,&gameModel));
    mow_initialize(&opponentModel,&gameModel,&DICT_HA,vu_letters);

    shown = shown || title_show_license_on_L();

    // ok, loading is done
    // Clear the key buffer and show the press a key message
    title_show_press_a_key();

    // Wait for a keypress. It might be an L,
    // which means show the license -- unless you
    // already showed the license
    title_wait_for_key(shown);

    // Load the PAGE
    ds_init(PAGE);
    // page flip to page
    ds_flipToPageNextVBIandWait(PAGE);
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

#pragma code-name (push,"APP")

//segment APP - accesses dictionary
void pickWord()
{
    md_word w1;
    char buf[6];
    md_pickRandomWord(&DICT_HS,&w1);
    md_wordToString(buf,&w1);
    buf[5]='\0';
    mw_setSolution(buf,&gameModel);
}

#pragma code-name (pop)

//segment CODE - run by OS
int main()
{
    initializeAWordl();
    for(;;)
    {
        pickWord();
        mow_gameDriver(&opponentModel);
        dk_getc();
        // Need to bankswitch to where the initialize code is!
        bankswitchStartup();
        vws_initialize(mw_initialize(&fontmanager,6,2,1,&gameModel));
        vfm_clearGreenLetters(&fontmanager);
        mow_initialize(&opponentModel,&gameModel,&DICT_HA,vu_letters);
    }
}
