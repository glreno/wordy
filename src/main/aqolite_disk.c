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
#include "md_pick.h"
#include "vo_a400.h"

/* VIDEO DATA */

// SCREENRAM and PAGE are only globals for debugging purposes
unsigned char *SCREENRAM;
ds_page *PAGE;

unsigned char *FONTLIST1[1], *FONTLIST2[1];
vfm_fontmanager fontManagerTop, fontManagerBot;

/* The dictionaries */
extern md_dict DICT_HA;

/* Game code (these don't need to be global, really) */
mq gameModel;
vo_text opponentView;
moq opponentModel;
void *vu_letters;
md_dict *dict;
                 // 012345678901234567890123456
char titleText[] = "AQolite (text)   US/n";
// titleText[18] should be S or K
// titleText[20] should be e/n/h


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
    int a;
    char shown = 0;

    //
    // INITIALIZATION CODE AND LICENSE SCREEN
    //

    // The cartridge will have bankswitched to this bank already,
    // but the zerobss will wipe out the LAST_BANKSWITCH field before calling main.
    // So do it again.
    bankswitchTitle();
    title_erase_loading_msg();
    //title_show_instruction_screen(); - already shown by file load

    // You may not bankswitch while the Title/License is on screen!
    // Before bankswitching, you must:
    // occasionaly check title_show_licence_on_L()
    // eventually title_show_press_a_key() and title_wait_for_key(shown);
    // build a display page, and page flip
    shown = shown || title_show_license_on_L();

    // 800XL Disk version - hand-allocated memory!
    // Can't touch anything below 0400 ever,
    // or anything below 2000 before we're done using DOS.
    // But we can put all the PAGE data in the DOS area no problem.
    // The fonts go in the two volatile areas where the OS puts the screen on warmstart.
    // Put the screen after the officially usable RAM
    // in the reserved area, and let the loader figure out the rest.
    // __RESERVED_RAM__ is set to 32K so that nothing gets
    // allocated into the 130XE bank-switch area.

    SCREENRAM = (unsigned char*)    0x2BF0; // 26*40=1040 (0x410) bytes of screen RAM ending at 3000
    FONTLIST1[0] = (unsigned char*) 0x9C00; // because ram from 9C20...9FFF gets wiped on warmstart (basic)
    FONTLIST2[0] = (unsigned char*) 0xBC00; // because ram from BC20...BFFF gets wiped on warmstart (no basic)
    PAGE = (ds_page*) 0x0800; // 0800-1000, 2KB down where DOS isn't any more

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
    vfm_initialize(&fontManagerTop,&AQORDLFONTDEF,1,FONTLIST1);
    vfm_initialize(&fontManagerBot,&AQORDLFONTDEF,1,FONTLIST2);
    ds_setFont(PAGE,0xe0,0,-3,0);
        a=vfm_allocateFont(&fontManagerTop);
        PAGE->page.fonts[0]=a;
        a=vfm_getFontBase(a,&fontManagerTop);
    ds_setFont(PAGE,a,0,1,10);
        a=vfm_allocateFont(&fontManagerBot);
        PAGE->page.fonts[1]=a;
        a=vfm_getFontBase(a,&fontManagerBot);
    ds_setFont(PAGE,a,0,11,24);
    ds_setFont(PAGE,0xE0,0,25,34);

    shown = shown || title_show_license_on_L();

    // Initialize the used letter display
    vu_letters=SCREENRAM+21*40;

    shown = shown || title_show_license_on_L();

    // Initialize the game model
    mq_initialize(&fontManagerTop,&fontManagerBot,&gameModel);
    moq_initialize(vu_letters,&DICT_HA,&gameModel, &opponentModel);

    shown = shown || title_show_license_on_L();

    vo4_initialize(FONTLIST1[0],FONTLIST2[0]);

    shown = shown || title_show_license_on_L();

    // ok, loading is done
    // Clear the key buffer and show the press a key message
    title_show_press_a_key();

    // Wait for a keypress. It might be an L,
    // which means show the license -- unless you
    // already showed the license
    title_wait_for_key(shown);

    show_options_screen(&(PAGE->pm.player0));
    dict = md_pickDictionary(&titleText[18],&titleText[20],selectedDictionary);

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
        md_pickRandomWord(dict,&w1);
        md_wordToString(buf,&w1);
        buf[5]='\0';
        mw_setSolution(buf,&(gameModel.puzzles[i]));
    }
}

#pragma code-name (pop)

void enableWarmStart(void);

//segment CODE - run by OS
int main()
{
    enableWarmStart();
    initializeQordl();
    for(;;)
    {
        md_bankswitchIdx(); // BANK SWITCH!
        pickWord();
        moq_gameDriver(titleText,&opponentModel);
        dk_getc();
        // Need to bankswitch to where the initialize code is!
        bankswitchStartup();
        mq_initialize(&fontManagerTop,&fontManagerBot,&gameModel);
        vfm_clearGreenLetters(&fontManagerTop);
        vfm_clearGreenLetters(&fontManagerBot);
        moq_initialize(vu_letters,&DICT_HA,&gameModel,&opponentModel);
    }
}
