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
#include "vo_robot.h"
#include "dk_kbdriver.h"


/* VIDEO DATA */

// SCREENRAM and PAGES are only globals for debugging purposes
unsigned char *SCREENRAM;
ds_page *PAGES[3];

unsigned char *FONTLIST1[3], *FONTLIST2[3];
vfm_fontmanager fontManagerTop, fontManagerBot;

/* The dictionaries */
extern md_dict DICT_HS_US;
extern md_dict DICT_HS_UK;
extern md_dict DICT_HA;

/* Game code (these don't need to be global, really) */
mq gameModel;
vo_robot opponentView;
moq opponentModel;
void *vu_letters;

const unsigned char LASER[] = {
    0b00011000,
    0b00111100,
    0b00111100,
    0b00011000
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
    unsigned int *p;
    // This would be smaller as a memcpy but we're in the ONCE segment so size doesn't matter as much as readability.
    // Note that the DLI needs to be triggered in the row that needs the new font, AND the row before it!
    b->displaylist[0]=0x70;
    b->displaylist[1]=0x70;
    b->displaylist[2]=0xF0; // 0x80+0x70 (8 blank rows) dli starts running in first blank row!
    b->displaylist[3]=0xC2; // 0x04+0x40+0x80 mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    memset(b->displaylist+6,0x04,20);
    // Set the DLI on a couple of those
    b->displaylist[6]=0x84;
    b->displaylist[15]=0x84;
    b->displaylist[16]=0x84;
    // And a blank line after the main screen
    b->displaylist[26]=0x70;
    memset(b->displaylist+27,0x0e,5);
    b->displaylist[32]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[33]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

//segment ONCE
void initializeQordl()
{
    char shown = 0;
    int i,a,b;

    //
    // INITIALIZATION CODE AND LICENSE SCREEN
    //

    // The cartridge will have bankswitched to this bank already,
    // but the zerobss will wipe out the LAST_BANKSWITCH field before calling main.
    // So do it again.
    bankswitchTitle();

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
    FONTLIST1[0] = (unsigned char*) SCREENRAM+0x410; // 1K font
    FONTLIST1[1] = (unsigned char*) (FONTLIST1[0]+1024);
    FONTLIST1[2] = (unsigned char*) (FONTLIST1[1]+1024);
    FONTLIST2[0] = (unsigned char*) (FONTLIST1[2]+1024);
    FONTLIST2[1] = (unsigned char*) (FONTLIST2[0]+1024);
    FONTLIST2[2] = (unsigned char*) (FONTLIST2[1]+1024);
    PAGES[0] = (ds_page*) (FONTLIST2[2]+1024); // 2K page after the fonts
    PAGES[1] = (ds_page*) (FONTLIST2[2]+1024+2048);
    PAGES[2] = (ds_page*) (FONTLIST2[2]+1024+4096);

    // Addresses:   16K     32K+
    // SCREENRAM    0BF0    4BF0
    // FONTLIST1[0] 1000    5000
    // FONTLIST1[1] 1400    5400
    // FONTLIST1[2] 1800    5800
    // FONTLIST2[0] 1C00    5C00
    // FONTLIST2[1] 2000    6000
    // FONTLIST2[2] 2400    6400
    // PAGES[0]     2800    7800
    // PAGES[1]     3000    7000
    // PAGES[2]     3800    7800

    // Create a screen
    ds_initScreenRam(SCREENRAM, 40*26); // this sets SAVMSC and zeroes out the given amount of space
    vor_initialize(&opponentView, 0, PAGES, 4, LASER, SCREENRAM);

    shown = shown || title_show_license_on_L();

    // Create the screen pages
    for(i=0;i<3;++i)
    {
        ds_init_page(PAGES[i]);
        shown = shown || title_show_license_on_L();

        // Create a display list in all pages
        mkDlist(&(PAGES[i]->page));

        // initialize tile page
        ds_setDefaultPalette(PAGES[i]);
        shown = shown || title_show_license_on_L();
    }

    // Start font managers and build fonts
    vfm_initialize(&fontManagerTop,&AQORDLFONTDEF,3,FONTLIST1);
    vfm_initialize(&fontManagerBot,&AQORDLFONTDEF,3,FONTLIST2);
    for(i=0;i<3;++i)
    {
        ds_setFont(PAGES[i],0xE0,0,-3,0);

        a=vfm_allocateFont(&fontManagerTop);
        PAGES[i]->page.fonts[0]=a;
        a=vfm_getFontBase(a,&fontManagerTop);
        ds_setFont(PAGES[i],a,0,1,10);

        b=vfm_allocateFont(&fontManagerBot);
        PAGES[i]->page.fonts[1]=b;
        b=vfm_getFontBase(b,&fontManagerBot);
        ds_setFont(PAGES[i],b,0,11,24);

        ds_setFont(PAGES[i],0xE0,0,25,34);
    }

    shown = shown || title_show_license_on_L();

    // Initialize the used letter display
    vu_letters=SCREENRAM+21*40;

    shown = shown || title_show_license_on_L();

    // Initialize the game model
    mq_initialize(&fontManagerTop,&fontManagerBot,&gameModel);
    moq_initialize(vu_letters,&DICT_HA,&gameModel,&opponentModel);

    shown = shown || title_show_license_on_L();

    // ok, loading is done
    // Clear the key buffer and show the press a key message
    title_show_press_a_key();

    // Wait for a keypress. It might be an L,
    // which means show the license -- unless you
    // already showed the license
    title_wait_for_key(shown);

    voa_loadFirstPage(PAGES[2]);
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
            md_pickRandomWord(&DICT_HS_US,&w1);
        }
        else
        {
            md_pickRandomWord(&DICT_HS_UK,&w1);
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
        moq_gameDriver("AQordl (robot mode)", &opponentModel);
        dk_getc();
        // Need to bankswitch to where the initialize code is!
        bankswitchStartup();
        mq_initialize(&fontManagerTop,&fontManagerBot,&gameModel);
        vfm_clearGreenLetters(&fontManagerTop);
        vfm_clearGreenLetters(&fontManagerBot);
        moq_initialize(vu_letters,&DICT_HA,&gameModel,&opponentModel);
    }
}

