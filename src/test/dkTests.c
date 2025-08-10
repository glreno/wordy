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
#include "yield.h"
#include "dk_kbdriver.h"

unsigned char *SCREENRAM;
ds_page *BANK[2];
char currbank=0;
char delay=1;
char pmcolor;
char mode=0;

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

const unsigned char PLAYER0[7] =
{
    0b00011100,
    0b00111110,
    0b01111111,
    0b01011101,
    0b01111111,
    0b01111111,
    0b01010101,
};
const unsigned char PLAYER1[7] =
{
    0b00011100,
    0b00111110,
    0b01111111,
    0b01011101,
    0b01111111,
    0b01111111,
    0b01001001,
};

void movePlayer(void)
{
    unsigned char x;
    --delay;
    if ( delay > 0 )
        return;

    // Note that this delay is number of yeilds, and NOT frames!
    // delay=1 is "animate on every yield"
    delay=(mode?1:100);
    if (currbank==0)
    {
        ++currbank;
    }
    else
    {
        --currbank;
        x=BANK[0]->colors.hposp1;
        if ( x > 0xD7 ) x=0x18;
        ++x;
        BANK[0]->colors.hposp1 = x;
        ++x;
        BANK[1]->colors.hposp1 = x;
    }
    ds_flipToPageNextVBI(BANK[currbank]);
}

int main(void)
{
    char buf[6];
    int *p;
    char c;

    OS.coldst=1; // force cold start on warm reset

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    SCREENRAM = (unsigned char *) OS.appmhi+1; // 9420 on 48K machine, 3420 on 16K machine, 1420 on 8K
    BANK[0] = (ds_page *) (OS.appmhi+0x3E1); // 2K screen page // 9800 on 48K machine, 3800 on 16K machine, 1800 on 8K
    BANK[1] = (ds_page *) (OS.appmhi+0x3E1+0x800); // 2K screen page // 9800 on 48K machine, 3800 on 16K machine, 1800 on 8K

    // Initialize the keyboard driver
    dk_initialize();

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 40*24);

    // Create the screen pages
    ds_init_page(BANK[0]);
    ds_init_page(BANK[1]);

    // Create a display list in all pages
    mkDlist(&(BANK[0]->page));
    mkDlist(&(BANK[1]->page));

    // initialize the fonts
    ds_setFont(BANK[0],0xE0,0,-3,34);
    ds_setFont(BANK[1],0xE0,0,-3,34);

    // initialize the player data
    memcpy(BANK[0]->pm.player1+200,PLAYER0,7);
    memcpy(BANK[1]->pm.player1+200,PLAYER1,7);
    pmcolor = mode?77:99;
    BANK[0]->colors.hposp1 = 100;
    BANK[0]->colors.pcolr1 = pmcolor;
    BANK[1]->colors.hposp1 = 101;
    BANK[1]->colors.pcolr1 = pmcolor;

    // Load the BANK
    ds_init(BANK[currbank]);

    // Initialize the keyboard driver
    // dk_init();

    // load the yield handler
    setYieldFunc(&movePlayer);

    p=(int*)(0x79);
    cputs("dkTest 79,7A=");
    itoa(*p,buf,16);
    cputs(buf);
    if ( *p == 0xfefe )
    {
        cputs(" OSB Mode");
    }
    else if ( *p == 0 )
    {
        cputs(" Compatibility Mode");
    }
    else
    {
        cputs(" XL Mode");
    }
    gotoxy(2,2);
    for(;;)
    {
        if ( mode )
        {
            // pink ghost
            c=cgetc();
            yield();
        }
        else
        {
            // purple ghost
            c=dk_getc();
            yield();
        }
        if ( c == 0x9B )
        {
            // RETURN
            c=29;
        }
        if ( c == 0x7E || c == 0x9C )
        {
            // backspace or delete line
            mode = !mode;
            // change PM color -- pugnacious purple is mode 0, testing the dk_getc
            pmcolor = (mode?77:99);
            BANK[0]->colors.pcolr1 = pmcolor;
            BANK[1]->colors.pcolr1 = pmcolor;
            delay=1;
            yield();
        }
        cputc(c);
    }
    return 0;
}
