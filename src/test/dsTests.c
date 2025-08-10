/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Unit tests for the ds_screendriver.h functions */

#include "ds_screendriver.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

extern unsigned char DS_NEXTPAGE;
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

ds_page *const BANK0 = (ds_page *) 0x8000; // 2K screen page - always empty
ds_page *const BANK1 = (ds_page *) 0x8800; // 2K screen page
ds_page *const BANK2 = (ds_page *) 0x9000; // 2K screen page
ds_page *const BANK3 = (ds_page *) 0x9800; // 2K screen page
unsigned char *const FONT1 = (unsigned char*) 0x7000; // 1K font
unsigned char *const FONT2 = (unsigned char*) 0x7400; // 1k font
unsigned char *const FONT3 = (unsigned char*) 0x7800; // 1K font
unsigned char *const FONT4 = (unsigned char*) 0x7C00; // 1K font

// Increment 2C8 (COLOR4 shadow) and return
//const unsigned char FlipCB[4] = { 0xee, 0xc8, 0x02, 0x60 }; // INC $2C8 (COLOR4) RTS
// Copy some number into 2C8 (COLOR4 shadow) and return
const unsigned char FlipCB[11] = { 0xe6, 0xcc, 0xa5, 0xcc, 0x8d, 0xc8, 0x02, 0x8d, 0x1a, 0xd0, 0x60 }; // INC ZTMP LDA ZTMP STA $2C8 (COLOR4) RTS

// store VCOUNT in D01A (COLBK register)
const unsigned char DLICB[5] = { 0x98, 0x8d, 0x1a, 0xd0, 0x60 }; // TYA (it's VCOUNT) STA $D01A (COLBK) RTS

// Test 4 anim callback:
// Schedule flip to BANK1, then disable the anim callback
unsigned char TCB4[11] = { 0xA9, 0x88, 0x8d, 0xd4, 0x20, 0xA9, 0x60, 0x8d, 0xE1,0x20, 0x60 }; // LDA #$88 (BANK1) STA 0x20D4 or so (DS_NEXTPAGE); LDA #$60, STA 0x20E1 or so (DS_ATCBV) RTS

/* Flip-Flop Jogger animation, from Dr. C. Wacko's Miracle Guide
* To Designing And Programming You Own Atari Computer Arcade Games
* by David Heller, John F. Johnson, and Robert Kurcina (c) 1983
* page 72.
*/
const unsigned char JOGGER[3][8] = {  { 24,56,62,10, 8,56,20, 2 },
                                { 24,56,62,10, 8,24,38,32 },
                                { 24,24,12,28, 8, 8,12, 8 } };


void mkDlist(ds_pageHeader *b)
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

void cloneInternalFont(unsigned char *dest)
{
    memcpy(dest,(unsigned char*)0xe000,1024);
}

void copyInternalFontChar(unsigned char * const dest,int dstidx, int srcidx,int xorwith)
{
    int i;
    unsigned char* internal = (unsigned char *)0xe000;
    for(i=0;i<8;i++)
    {
        dest[dstidx*8+i] = (internal[srcidx*8+i] ^ xorwith );
    }
}

void fillFontChar(unsigned char *dest,int dstidx, int n)
{
    memset(dest+dstidx*8,n,8);
}

void test1(void)
{
    char k;
    int y;

    copyInternalFontChar(FONT1,3,17,0xff);
    copyInternalFontChar(FONT2,3,18,0xff);
    copyInternalFontChar(FONT3,3,19,0xff);
    copyInternalFontChar(FONT4,3,20,0xff);
    fillFontChar(FONT1,0,0xaa);
    fillFontChar(FONT2,0,0x55);
    fillFontChar(FONT3,0,0x55);
    fillFontChar(FONT4,0,0xaa);

    // Load the fonts into the screen pages
    ds_setFont(BANK1,0xE0,0,-3,8);
    ds_setFont(BANK1,FONT1BASE,0,9,19);
    ds_setFont(BANK1,FONT2BASE,0,20,34);
    ds_setFont(BANK2,0xE0,0,-3,8);
    ds_setFont(BANK2,FONT3BASE,0,9,19);
    ds_setFont(BANK2,FONT4BASE,0,20,34);

    // modify PAGE2 displaylist for test
    BANK2->page.displaylist[9]=0x4;

    // Create some PM graphics in both pages
    memcpy(BANK1->pm.player1+96, (char*)0xE088, 8); // char '1' from font
    memcpy(BANK2->pm.player1+96, (char*)0xE090, 8); // char '2' from font
    BANK1->colors.hposp1 = 100;
    BANK1->colors.pcolr1 = 99;
    BANK2->colors.hposp1 = 108;
    BANK2->colors.pcolr1 = 73;
    BANK3->colors.hposp1 = 100;
    BANK3->colors.pcolr1 = 99;

    // Register a pageflip callback that increments BAK


    // Now print some stuff
    // BANK0 is loaded, so this will be invisible until the flip.
    printf("Test 1: Press space to page flip");
    for(y=0;y<24;++y) {
        ds_putc(3,1,y);
        ds_putc(y,39,y);
    }
    // ds_puts prints internal codes, so A becomes the graphics char ctrl-a
    ds_puts("QWE",3,5,10);
    ds_puts("ASD",3,5,11);
    ds_puts("ZXC",3,5,12);

    cputsxy(4,1,"Press D when done");


    for(;;)
    {
        // load BANK2
        ds_flipToPageNextVBI(BANK2);
        k = cgetc();
        if ( k == 'd' )
        {
            BANK2->page.displaylist[9]=BANK2->page.displaylist[10];
            return;
        }
        // load BANK1
        ds_flipToPageNextVBI(BANK1);
        k = cgetc();
        if ( k == 'd' )
        {
            BANK2->page.displaylist[9]=BANK2->page.displaylist[10];
            return;
        }
    }
}

void test2(void)
{
    int k;
    // Clear screen, print instructions
    ds_flipToPageNextVBIandWait(BANK0);
    ds_initScreenRam(SCREENRAM, 40*24);
    OS.rowcrs = 1;
    OS.colcrs = 1;
    printf("Test 2: DLI callback function\n");
    printf("Press S to start test\n");
    printf("Press C to continue\n");

    ds_flipToPageNextVBI(BANK3);
    for(;;)
    {
        k = cgetc();
        if ( k == 'c' )
        {
            ds_setDliCallback(0);
            return;
        }
        if ( k == 's' )
        {
            //printf("\n\nThe fuzzy white line is because the DLI\nis called during a blank line, and the\ncolor change takes place immediately.\nnever change a color WHILE\nit is being drawn.");
            ds_setDliCallback(&DLICB);
        }
    }
}

void test3(void)
{
    int k;
    // Clear screen, print instructions
    ds_flipToPageNextVBIandWait(BANK0);
    ds_initScreenRam(SCREENRAM, 40*24);
    OS.rowcrs = 1;
    OS.colcrs = 1;
    printf("Test 3: Flip CB function\n");
    printf("Press space to page flip\n");
    printf("Press D when done\n");
    // FlipCB was supposed to increment color 4, but that gets wiped out by the callback!
    // FlipCB increments 0600 and adds stores it in color 4
    ds_setFlipCallback(&FlipCB);

    for(;;)
    {
        // load BANK2
        ds_flipToPageNextVBI(BANK2);
        k = cgetc();
        if ( k == 'd' )
        {
            ds_setFlipCallback(0);
            return;
        }

        // load BANK1
        ds_flipToPageNextVBI(BANK1);
        k = cgetc();
        if ( k == 'd' )
        {
            ds_setFlipCallback(0);
            return;
        }
    }
}

void test4(void)
{
    int* addr;
    int k;
    // Clear screen, print instructions
    ds_flipToPageNextVBIandWait(BANK0);
    ds_initScreenRam(SCREENRAM, 40*24);
    OS.rowcrs = 1;
    OS.colcrs = 0;
    printf("Test 4: Useless Box (timer test)\n\n");
    printf("Press U to test\n\n");
    printf("The U key is useless\n\n");
    printf("Press C to continue\n");
    // load default font through all of pages 1 and 2
    // Load blank font into line 5 of page 2
    ds_setFont(BANK1,0xE0,0,-3,4);
    ds_setFont(BANK1,FONT0BASE,0,5,5);
    ds_setFont(BANK1,0xE0,0,6,34);

    ds_setFont(BANK2,0xE0,0,-3,2);
    ds_setFont(BANK2,FONT0BASE,0,3,3);
    ds_setFont(BANK2,0xE0,0,4,34);

    // Never do it this way. Create a .asm file.
    // Put the correct addresses into the callback
    // TCB4[11] = LDA #$88 (BANK1) STA 0x20D4 or so (DS_NEXTPAGE); LDA #$60, STA 0x20E1 or so (DS_ATCBV) RTS
    //             0   1            2   4 3                         5   6     7    9 8                   10
    TCB4[1] = BANK1->id[1];
    addr = (int*) (TCB4+3);
    *addr = (int) &DS_NEXTPAGE;
    addr = (int*) (TCB4+8);
    *addr = (int) &DS_ATCBV;
    // load BANK1
    ds_flipToPageNextVBI(BANK1);
    for(;;)
    {
        k = cgetc();
        if ( k == 'c' )
        {
            ds_setAnimTimerCallback(0, NULL);
            return;
        }
        if ( k == 'u' )
        {
            // load BANK2, with a 1 second timer to flip back to page 1
            //ds_setAnimTimerCallback(3*60,&DS_NOCB); // ASM external symbol, needs &
            ds_setAnimTimerCallback(60,&TCB4); // C array, no & needed, except to convert to const ptr
            ds_flipToPageNextVBI(BANK2);
        }
    }
}

static char pageidx;
static int test5timer;
ds_page *pages[9];
void test5handler(void)
{
    --pageidx;
    ds_flipToPageNextVBI(pages[pageidx]);
    if ( pageidx == 0 )
    {
        pageidx=3;
    }
}

void test5(void)
{
    int k;
    int j;
    // Clear screen, print instructions
    ds_flipToPageNextVBIandWait(BANK0);
    ds_initScreenRam(SCREENRAM, 40*24);
    OS.rowcrs = 0;
    OS.colcrs = 1;
    printf("Test 5: Jogger\n");
    printf("Press + - to change speed\n");
    printf("Press D when done\n");

    // Load the jogger into P1 of pages 1-3
    memcpy(BANK1->pm.player1+96, JOGGER[0], 8);
    memcpy(BANK2->pm.player1+96, JOGGER[1], 8);
    memcpy(BANK3->pm.player1+96, JOGGER[2], 8);

    // Clean up the space characters in the fonts
    fillFontChar(FONT1,0,0);
    fillFontChar(FONT2,0,0);
    fillFontChar(FONT3,0,0);
    fillFontChar(FONT4,0,0);
    // Load the jogger into the players and font char !
    memcpy(BANK1->pm.player1+96, JOGGER[0], 8);
    memcpy(BANK2->pm.player1+96, JOGGER[1], 8);
    memcpy(BANK3->pm.player1+96, JOGGER[2], 8);
    memcpy(FONT1+8, JOGGER[0], 8);
    memcpy(FONT2+8, JOGGER[1], 8);
    memcpy(FONT3+8, JOGGER[2], 8);
    BANK2->colors.hposp1 = 100;
    BANK2->colors.pcolr1 = 99;

    // List of pages to animate through
    pageidx=3;
    pages[0]=BANK3;
    pages[1]=BANK2;
    pages[2]=BANK1;
    // same thing just to make setting up the fonts eaiser
    pages[3]=BANK3;
    pages[4]=BANK2;
    pages[5]=BANK1;
    pages[6]=BANK3;
    pages[7]=BANK2;
    pages[8]=BANK1;

    // Draw a line of ! down the side of the screen,
    // and set the fonts on each line
    j=0;
    for(k=0;k<24;k++)
    {
        ds_putc(1,36+k%3,k);
        ds_putc(k,k,21);
        ds_setFont(pages[j+0],FONT3BASE,0,k,k);
        ds_setFont(pages[j+1],FONT2BASE,0,k,k);
        ds_setFont(pages[j+2],FONT1BASE,0,k,k);
        ++j;
        if ( j > 2 )
        {
            j=0;
        }
    }

    test5timer=15;
    ds_setAnimTimerCallback(test5timer,test5handler);
    ds_flipToPageNextVBI(BANK0); // kick things off by pageflipping to anything

    for(;;)
    {
        k = cgetc();
        if ( k == 'd' )
        {
            ds_setAnimTimerCallback(0, NULL);
            return;
        }
        else if ( k == '-' || k == '_' )
        {
            if ( test5timer > 1)
            {
                // decrement the animation speed
                // - test5timer is the start value of the timer
                // - to change the timer, you need to reregister the callback
                --test5timer;
                ds_setAnimTimerCallback(test5timer,test5handler);
                gotoxy(31,1);
                printf("%d  ",test5timer);
            }
        }
        else if ( k == '=' || k == '+' )
        {
            // increment the animation speed
            ++test5timer;
            ds_setAnimTimerCallback(test5timer,test5handler);
            gotoxy(31,1);
            printf("%d  ",test5timer);
        }
    }
}

int main(void)
{
    if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset
    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);

    // Create some raw fonts
    cloneInternalFont(FONT1);
    cloneInternalFont(FONT2);
    cloneInternalFont(FONT3);
    cloneInternalFont(FONT4);

    // Create the screen pages
    ds_init_page(BANK0);
    ds_init_page(BANK1);
    ds_init_page(BANK2);
    ds_init_page(BANK3);

    // Create a display list in all pages
    mkDlist(&(BANK0->page));
    mkDlist(&(BANK1->page));
    mkDlist(&(BANK2->page));
    mkDlist(&(BANK3->page));

    // initialize the completely blank page
    ds_setFont(BANK0,FONT0BASE,0,-3,34);
    // and a plaintext page
    ds_setFont(BANK3,0xE0,0,-3,34);

    // Load the BANK0
    ds_init(BANK0);

    // First set of tests: swap two pages
    test1();
    // Second set of tests: test DLI callback
    test2();

    // Third set of tests: test Flip callback
    test3();

    // Fourth set of tests: the useless box
    test4();

    // Fifth set of tests: the running man
    test5();

    // Should never get here
    ds_flipToPageNextVBIandWait(BANK0);
    ds_initScreenRam(SCREENRAM, 40*24);
    cputsxy(4,1,"ALL DONE!");
    ds_flipToPageNextVBI(BANK3);
    for(;;)
        ;
    return 0;
}
