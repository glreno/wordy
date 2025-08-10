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
#include "aqordlFont.h"
#include "mo_qordl.h"
#include "vo_text.h"
#include "dk_kbdriver.h"


/* VIDEO DATA */

// SCREENRAM and PAGE are only globals for debugging purposes
unsigned char *SCREENRAM;
ds_page *PAGE;

unsigned char *FONTLIST1[1], *FONTLIST2[1];
vfm_fontmanager fontmanager1, fontmanager2;

/* The dictionaries */
extern md_dict DICT_ES_US;
extern md_dict DICT_ES_UK;
extern md_dict DICT_EA;

/* Game code (these don't need to be global, really) */
mq gameModel;
vo_text opponentView;
moq opponentModel;
void *vu_letters;

const unsigned char SPOTLIGHT[] = {
    0b01111100,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b11111110,
    0b01111100
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
    b->displaylist[3]=0xC2; // 0x04+0x40+0x80 mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    for(i=6;i<26;++i)
    {
        b->displaylist[i]=0x84;
    }
    b->displaylist[26]=0x70;
    for(i=27;i<32;++i)
    {
        b->displaylist[i]=0x0e;//five lines of mode e at the end of things
    }
    b->displaylist[32]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[33]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

void initializeQordl()
{
    char shown = 0;
    unsigned char *FONT1, *FONT2;
    unsigned char FONTBASE1,FONTBASE2;

    //
    // INITIALIZATION CODE AND LICENSE SCREEN
    //

    //title_erase_loading_msg();
    title_show_instruction_screen();

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

    SCREENRAM = (unsigned char*) (OS.appmhi+1); // 26*40=1040 (0x410) bytes of screen RAM
    FONT1 = (unsigned char*) SCREENRAM+0x410; // 1K font
    FONT2 = (unsigned char*) (FONT1+1024); // 1K font after the other font
    PAGE = (ds_page*) (FONT2+1024); // 2K page after the fonts

    // Addresses:   8K      16K     32K+
    // SCREENRAM    0BF0    2BF0    6BF0
    // FONT1        1000    3000    7000
    // FONT2        1400    3400    7400
    // PAGE         1800    3800    7800

    FONTBASE1 = ((unsigned int)FONT1)>>8;
    FONTBASE2 = ((unsigned int)FONT2)>>8;
    FONTLIST1[0]=FONT1;
    FONTLIST2[0]=FONT2;

    // Create a screen
    ds_initScreenRam(SCREENRAM, 40*26); // this sets SAVMSC and zeroes out the given amount of space
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
    vfm_initialize(&fontmanager1,&AQORDLFONTDEF,1,FONTLIST1);
    vfm_initialize(&fontmanager2,&AQORDLFONTDEF,1,FONTLIST2);
    ds_setFont(PAGE,0xe0,0,-3,0);
    ds_setFont(PAGE,FONTBASE1,0,1,10);
    ds_setFont(PAGE,FONTBASE2,0,11,24);
    ds_setFont(PAGE,0xE0,0,25,34);

    shown = shown || title_show_license_on_L();

    // Initialize the used letter display
    vu_letters=SCREENRAM+21*40;

    shown = shown || title_show_license_on_L();

    // Initialize the game model
    mq_initialize(&fontmanager1,&fontmanager2,&gameModel);
    moq_initialize(vu_letters,&DICT_EA,&gameModel, &opponentModel);

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
    int i;
    md_word w1;
    char buf[6];
    for(i=0;i<4;++i)
    {
        if ( (GTIA_READ.pal & 0xE) == 0xE )
        {
            md_pickRandomWord(&DICT_ES_US,&w1);
        }
        else
        {
            md_pickRandomWord(&DICT_ES_UK,&w1);
        }
        md_wordToString(buf,&w1);
        buf[5]='\0';
        mw_setSolution(buf,&(gameModel.puzzles[i]));
    }
}

#pragma code-name (pop)

//segment CODE - run by OS
int main()
{
    initializeQordl();
    for(;;)
    {
        pickWord();
        moq_gameDriver("AQordl (text mode)",&opponentModel);
        dk_getc();
        // Need to bankswitch to where the initialize code is!
        bankswitchStartup();
        mq_initialize(&fontmanager1,&fontmanager2,&gameModel);
        vfm_clearGreenLetters(&fontmanager1);
        vfm_clearGreenLetters(&fontmanager2);
        moq_initialize(vu_letters,&DICT_EA,&gameModel,&opponentModel);
    }
}
