/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Unit tests for the mw typing and scoring functions (with no dictionary)
 * -- enter a word, then guess it and see how you score */

#include "ds_screendriver.h"
#include "vfm_fontmanager.h"
#include "awordlFont.h"
#include "mw_puz.h"
#include "vw_small.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

extern unsigned char DS_NEXTBANK;
extern unsigned char DS_ATCBV;
unsigned char *SCREENRAM;

/* Locations in the 2K __RESERVED_MEMORY__ which ends at A000 */

/* BANK0 is a completely blank screen -- the font points to the empty
* player area at $9C00 */

unsigned char FONTBASE;
ds_page *BANK;
unsigned char *FONT;
unsigned char *FONTLIST[1];
vfm_fontmanager fontmanager;

mw solutionModel;
mw testModel;

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
    for(i=15;i<25;++i)
    {
        b->displaylist[i]=0x82;
    }
    b->displaylist[29]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[30]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

int main(void)
{
    int i,k;
    int x;
    OS.coldst=1; // force cold start on warm reset

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    SCREENRAM = (unsigned char *) OS.appmhi+1; // 9420 on 48K machine, 3420 on 16K machine, 1420 on 8K
    BANK = (ds_page *) (OS.appmhi+0x3E1); // 2K screen page // 9800 on 48K machine, 3800 on 16K machine, 1800 on 8K
    FONT = (unsigned char*) (BANK->pm.player0); // 1K font in the PM data space
    FONTBASE = ((int)FONT)/256;
    FONTLIST[0]=FONT;

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);

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
    // (this has to be after ds_init_page for this test since we're putting the font in the PM area)
    vfm_initialize(&fontmanager,&AWORDLFONTDEF,1,FONTLIST);
    ds_setFont(BANK,0xE0,0,-3,0);
    ds_setFont(BANK,FONTBASE,0,1,9);
    ds_setFont(BANK,0xE0,0,10,34);

    // Initialize a model to enter the solution with.
    vws_initialize(mw_initialize(&fontmanager,1,2,1,&solutionModel));
    for(x=0;x<5;x++)
    {
        vw_clearTile(&(solutionModel.view),x,0);
    }

    // page flip to page
    ds_flipToPageNextVBIandWait(BANK);


    k=0;
    while ( k != 1 && k != 2 )
    {
        k = cgetc();
        gotoxy(3,0);
        printf("  %d   ",k);
        k = mw_keypress(&solutionModel,k);
        gotoxy(20,0);
        printf("  %d  %d   ",solutionModel.cur_x,k);
    }
    gotoxy(0,0);
    printf("%d Accepted %s",k,solutionModel.currGuess);

    // Create another model using the accepted word
    vws_initialize(mw_initialize(&fontmanager, 1,5,1,&testModel));
    mw_setSolution(solutionModel.currGuess,&testModel);
    for(x=0;x<5;x++)
    {
        vw_clearTile(&(testModel.view),x,0);
    }

    k=0;
    while ( k != 1 && k != 2 )
    {
        k = cgetc();
        gotoxy(3,0);
        printf("  %d   ",k);
        k = mw_keypress(&testModel,k);
        gotoxy(20,0);
        printf("  %d  %d   ",testModel.cur_x,k);
    }
    gotoxy(0,10);
    printf("%d Scoring %s\n",k,testModel.currGuess);

    mw_scoreRow(&testModel);
    for(i=0;i<5;i++)
    {
        k = mw_checkLetter(testModel,i);
        printf("%d %c : %c -> %d\n",i,testModel.solution[i],testModel.currGuess[i],k);
        mw_colorLetter(&testModel,i);
    }
    k = mw_isWon(&testModel);
    printf("Winner? %d",k);

    for(;;)
    {
    }
    return 0;
}
