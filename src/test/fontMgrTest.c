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

/* Locations in the 24K __RESERVED_MEMORY__ which ends at A000 */
#define NPAGES 3
#define NFONTS 3
ds_page *PAGE[NPAGES];
unsigned char *FONTLIST[NFONTS];


vfm_fontmanager fontmanager;
vw_base grey;
vw_base yellow;

// Assmbly code timer handler, and stuff it needs
#define NOPAGE ((void *)0xffff)
extern ds_page *currentPage;
void timerHandler(void);

void mkDlist1(ds_pageHeader *b,int id)
{
    int i;
    unsigned int *p;
    b->displaylist[0]=0x70;
    b->displaylist[1]=0x70;
    b->displaylist[2]=0xF0; // dli starts running in first blank row!
    b->displaylist[3]=0xC2; // mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[4]));
    *p = (unsigned int)SCREENRAM;
    for(i=6;i<26;++i)
    {
        b->displaylist[i]=0x84;
    }
    b->displaylist[26]=0xC6; // mode 6+address+dli
    p=(unsigned int *) (&(b->displaylist[27]));
    *p = (unsigned int) (&(b->displaylist[33]));
    b->displaylist[29]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[30]));
    *p = (unsigned int ) (&(b->displaylist[0]));
    // some text
    for(i=33;i<53;++i)
    {
        b->displaylist[i]=0;
    }
    b->displaylist[36]=36; // D
    b->displaylist[37]=44; // L
    b->displaylist[38]=41; // I
    b->displaylist[39]=51; // S
    b->displaylist[40]=52; // T
    b->displaylist[42]=16+id; // number
}

void putDotInFont(unsigned char *font,int internalcode, int row, unsigned char bitmap)
{
    unsigned char buf[8];
    // put a dot in a font
    memset(buf,0,8);
    // char 1 line 2 bitmap c0
    buf[row-1]=bitmap;
    buf[row]=bitmap;
    vfm_setChar(font,internalcode,buf);
}

void cleanPage(int pageid)
{
    int fontid;
    ds_page *p;
    p=PAGE[pageid];
    fontid = p->page.spare[0];
    p->page.spare[0]=-1;
    p->page.next=NOPAGE; // mark page as available for use
    vfm_deallocateFont(fontid,&fontmanager);
    p->page.screenState=DS_SCREENSTATE_INITIAL; // INITIAL STATE
    p->page.queueState=4; // CLEANED
}

int waitForCleanPage()
{
    int pageid;
    int i;
    pageid=-1;
    while (pageid==-1)
    {
        // scan for screenState==OFFSCREEN and deallocate the font etc, then set state to CLEANED
        for(i=0;i<NPAGES;++i)
        {
            // check page to see if it is OFF SCREEN (-1)
            if ( PAGE[i]->page.screenState==DS_SCREENSTATE_OFFSCREEN )
            {
                // do some cleanup in the page, and set
                // screenState=INITIAL and queueState to CLEANED
                cleanPage(i);
            }
        }
        // Look for a page that is in the CLEANED state
        for(i=0;pageid==-1 && i<NPAGES;++i)
        {
            if ( PAGE[i]->page.queueState==4 ) // CLEANED
            {
                pageid=i;
            }
        }
    }
    return pageid;
}

const unsigned char clock[48] = {
    // idx line bits
       1,   2,   0xC0, // 12:00
       2,   4,   0x30, //  1:00
       3,   7,   0xC0, //  2:00
       4,   4,   0x0C, //  3:00
       5,   2,   0xC0, //  4:00
       6,   4,   0x30, //  5:00
       7,   6,   0xC0, //  6:00
       8,   4,   0x03, //  7:00
       8,   2,   0xC0, //  8:00
       9,   4,   0x0C, //  9:00
       10,  7,   0xC0, // 10:00
       10,  4,   0x03  // 11:00
};
ds_page *lastPageDrawn;
void drawAndEnqueuePage(int animid)
{
    char fontid;
    int pageid;
    // Find the next available unused page
    // This may busy wait for quite a while
    fontid = 0xff;
    while ( fontid==0xff)
    {
        pageid=waitForCleanPage();
        // OK, we have a clean page. Can we get a font?
        fontid=vfm_allocateFont(&fontmanager);
        // If not, try again. Page is still not marked as in use, so check that again too.
        // (If we needed more than one font, we would have
        // to try to allocate all of them here, and DEallocate them if we didn't get enough.
        // Why? Because we need waitForCleanPage to clean up more pages)
    }

        PAGE[pageid]->page.queueState=3; // mark page as being drawn - queueState INITIALIZED
        // Allocate a font
        PAGE[pageid]->page.spare[0]=fontid;
        PAGE[pageid]->page.spare[1]=animid;
        PAGE[pageid]->page.spare[2]=0xeeee;

        // Load the font into the page
        ds_setFont(PAGE[pageid],0xE0,0,-3,34);
        ds_setFont(PAGE[pageid],vfm_getFontBase(fontid,&fontmanager),0,1,19);

        // Clear out chars 0-10 in the font
        memset(FONTLIST[fontid],0,88);
        // Draw the animation in the font
        putDotInFont(FONTLIST[fontid],clock[0*3],clock[0*3+1],clock[0*3+2]);
        putDotInFont(FONTLIST[fontid],clock[animid*3],clock[animid*3+1],clock[animid*3+2]);
        PAGE[pageid]->page.queueState=2; // mark page as drawn - queueState DRAWN

        // Queue up the page
    PAGE[pageid]->page.queueState=1; // queueState LOADED AND READY TO QUEUE
    lastPageDrawn->page.next = PAGE[pageid];
    lastPageDrawn->page.queueState=0; // queueState NEXT IS SET
    PAGE[pageid]->page.screenState=DS_SCREENSTATE_QUEUED; // screenState QUEUED
    // and prepare to draw the NEXT page by 
    // saving a pointer to the one we just drew
    lastPageDrawn = PAGE[pageid];
}

void initializePages()
{
    int f;
    int numInFont;
    // Create the screen pages
    for(f=0;f<NPAGES;++f)
    {
        ds_init_page(PAGE[f]); // initialize (and zero-out) the page ram
        PAGE[f]->page.screenState=DS_SCREENSTATE_INITIAL; // INITIAL STATE
        PAGE[f]->page.queueState=5; // INITIAL STATE
        PAGE[f]->page.id=f;

        // Create a display list in all pages
        mkDlist1(&(PAGE[f]->page),f);

        // initialize colors
        PAGE[f]->colors.color0 = 0x04;
        PAGE[f]->colors.color1 = 0xD4;
        PAGE[f]->colors.color2 = 0x08;
        PAGE[f]->colors.color3 = 0xE8;
        PAGE[f]->colors.color4 = 0;
        PAGE[f]->colors.pcolr0 = 99;
        PAGE[f]->colors.hposp0 = 104;
        PAGE[f]->colors.sizep0 = 0;

        // In each page, write the page number in Player 0
        memset(&(PAGE[f]->pm.player0),0,256);
        numInFont=0xE080+(f*8);
        memcpy(&(PAGE[f]->pm.player0[176]),(unsigned char*)numInFont,8);
        PAGE[f]->page.queueState=4; // CLEAN
    }
}

int main(void)
{
    int f;

    if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    SCREENRAM = (unsigned char *) OS.appmhi+1; // 9420 on 48K machine, 3420 on 16K machine, 1420 on 8K
    PAGE[0] = (ds_page *) (OS.appmhi+0x3E1); // 2K screen page at 0x1000, PM at 0x1400
    for(f=1;f<NPAGES;++f)
    {
        PAGE[f] = (ds_page*) ( ((char*)PAGE[f-1]) + 2048);
    }
    FONTLIST[0] = (unsigned char*) ( ((char*)PAGE[NPAGES-1])+2048); // 7000
    for(f=1;f<NFONTS;++f)
    {
        FONTLIST[f] = (unsigned char *) (FONTLIST[f-1]+1024);
    }

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);
    // force it to move thc cursor out of the OLD screen RAM
    gotoxy(0,0);
    //cputs("");

    for(f=0;f<NFONTS;++f)
    {
        memset(FONTLIST[f],0,1024);
    }
    vfm_initialize(&fontmanager,&AWORDLFONTDEF,NFONTS,FONTLIST);
    initializePages();

    // Initialize two vw_small views
    vw_initialize(&fontmanager,1,17,1,&grey);
    vws_initialize(&grey);
    vw_initialize(&fontmanager,1,17,22,&yellow);
    vws_initialize(&yellow);

    // In each font, replace the letter "B" with the font number
    for(f=0;f<NFONTS;++f)
    {
        vfm_createGreenLetter(FONTLIST[f],&AWORDLFONTDEF,f-17,18); // -16='1', 18='B', in the wierd numbering used by createGreenLetter
    }

    // Left side: PAGE#
    vw_writeTile(&grey,0,0,VW_GREY,'P');
    vw_writeTile(&grey,1,0,VW_GREY,'A');
    vw_writeTile(&grey,2,0,VW_GREY,'G');
    vw_writeTile(&grey,3,0,VW_GREY,'E');
    // Right side: FONT# (the number will be in the letter B)
    vw_writeTile(&yellow,0,0,VW_YELLOW,'B');
    vw_writeTile(&yellow,1,0,VW_YELLOW,'F');
    vw_writeTile(&yellow,2,0,VW_YELLOW,'O');
    vw_writeTile(&yellow,3,0,VW_YELLOW,'N');
    vw_writeTile(&yellow,4,0,VW_YELLOW,'T');

    // Print the characters for the graphics
    gotoxy(16,10);
    cputs(" * !\"#");
    gotoxy(16,11);
    cputs(")    $");
    gotoxy(16,12);
    cputs(" ( '&%");

    // Load the first page and start the graphics system up
    // bootstrap by taking PAGE[0] and marking it as on screen
    lastPageDrawn=PAGE[NPAGES-1]; // so that drawAndEnqueuePage can say PAGE[0].next = the new page
        f=vfm_allocateFont(&fontmanager);
        PAGE[NPAGES-1]->page.spare[0]=f;
    // Now draw a page; it will mark PAGE[0]->next as this new page
    drawAndEnqueuePage(11);
    // Now throw PAGE[0] back in to the pool
    PAGE[NPAGES-1]->page.screenState=DS_SCREENSTATE_OFFSCREEN; // change to 1 TAKEN OFF SCREEN (normally the screen driver would do this)
    // and load the page we actually just drew
    ds_setAnimTimerCallback(15,timerHandler);
    currentPage=lastPageDrawn; // bootstrap the timerHandler
    ds_init(lastPageDrawn);

    for(;;)
    {
        for(f=0;f<12;++f)
        {
            drawAndEnqueuePage(f);
        }
    }
    return 0;
}

