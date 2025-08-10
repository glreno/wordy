/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Unit tests for the vw_tiny.h font functions,
 * showing a single font. */

#include "ds_screendriver.h"
#include "vu_letters.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>

unsigned char *SCREENRAM;

ds_page *BANK;

unsigned int lines[11];

char *testnames[] = {
        "Test 0: Do nothing",
        "Test 1: Grey all letters",
        "Test 2: Green all letters",
        "Test 3: Alternate unused/grey",
        "Test 4: Same but other letters",
        "Test 5: Alternate unsued/green",
        "Test 6: Same but other letters",
        "Test 7: Alternate grey/green",
        "Test 8: Same but other letters",
        "Test 9: All green, attempt to turn grey",
        "Test A: All grey, attempt to turn green",
        "And that was the tests"
};

void mkDlist1(ds_pageHeader *b)
{
    char n;
    int i=0;
    unsigned int *p;
    int m=4;
    b->displaylist[i++]=0x70;
    b->displaylist[i++]=0x70;
    b->displaylist[i++]=0x70;
    b->displaylist[i++]=0xC0+m; // mode 2+address+dli
    p=(unsigned int *) (&(b->displaylist[i++]));
    *p = (unsigned int)SCREENRAM;
    i++;

    for(n=0;n<2;++n)
    {
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=m;
    }

    b->displaylist[i++]=0x0e;
    b->displaylist[i++]=0x0e;
    b->displaylist[i++]=0x0e;

    // need to include a screen address again because of a 4K boundary after four lines
    b->displaylist[i++]=0xCE;
    p=(unsigned int *) (&(b->displaylist[i++]));
    *p = (unsigned int)(SCREENRAM+(40*16));
    i++;
    b->displaylist[i++]=0x0e;
    // Alternate mode 2 with 5 lines of mode E
    for(n=1;n<9;++n)
    {
        b->displaylist[i++]=m;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
        b->displaylist[i++]=0x0e;
    }
    b->displaylist[i++]=m;
    b->displaylist[i++]=0x41; // wait for VB + jump to address
    p=(unsigned int *) (&(b->displaylist[i]));
    *p = (unsigned int ) (&(b->displaylist[0]));
}

int main(void)
{
    int a,b;

    if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset

    // memory allocation based on APPMHI,
    // which is MEMTOP minus a gr.0 screen minus __RESERVED_MEMORY__
    SCREENRAM = (unsigned char *) OS.appmhi+1; // (11*5+12)*40=2680 bytes
    // Screen RAM reservation in __RESERVED_MEMORY__ includes an extra 8 bytes so that the screen doesn't cross a 4K boundary in the middle of a scan line.
    // Those 8 bytes are between the end of the screen RAM and the beginning of the bank.
    BANK = (ds_page *) (OS.appmhi+2681+8); // 2K screen page

    // Create a screen
    ds_disableScreen();
    ds_initScreenRam(SCREENRAM, 2680);

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

/*
    // a diagonal line of 'U'
    for(a=0;a<40;++a)
    {
        SCREENRAM[a*40+a]=53;
    }
    for(a=0;a<15;++a)
    {
        SCREENRAM[1638+a*40-a]=35;
    }
*/

    // label each row
    for(a=0;a<12;++a)
    {
        gotoxy(0,a*6);
//      cputs(testnames[a]);
    }

    // Set up the test objects
    lines[0]=40+(unsigned int)SCREENRAM; // skip one line
    for(a=1;a<11;++a)
    {
        lines[a]=lines[a-1]+6*40;
    }

    vu_initialize((void*)(lines[0]));
    vu_initialize((void*)(lines[1]));
    for(a='A';a<='Z';++a)
    {
        // Test 0: Do nothing
        // Test 1: Grey all letters
        vu_grey(a,((void*)lines[1]));
    }
    vu_initialize((void*)(lines[2]));
    for(a='A';a<='Z';++a)
    {
        // Test 2: Green all letters
        vu_green(a,((void*)lines[2]));
    }
    vu_initialize((void*)(lines[3]));
    for(a='A';a<='Z';++a)
    {
        // Test 3: Alternate unused/grey
        b= (a&1);
        if (b) vu_grey(a,((void*)lines[3]));
    }
    vu_initialize((void*)(lines[4]));
    for(a='A';a<='Z';++a)
    {
        // Test 4: Same but other letters
        b= (a&1);
        if (!b) vu_grey(a,((void*)lines[4]));
    }
    vu_initialize((void*)(lines[5]));
    for(a='A';a<='Z';++a)
    {
        // Test 5: Alternate unsued/green
        b= (a&1);
        if (b) vu_green(a,((void*)lines[5]));
    }
    vu_initialize((void*)(lines[6]));
    for(a='A';a<='Z';++a)
    {
        // Test 6: Same but other letters
        b= (a&1);
        if (!b) vu_green(a,((void*)lines[6]));
    }
    vu_initialize((void*)(lines[7]));
    for(a='A';a<='Z';++a)
    {
        // Test 7: Alternate grey/green
        b= (a&1);
        if (b) vu_grey(a,((void*)lines[7]));
        else vu_green(a,((void*)lines[7]));
    }
    vu_initialize((void*)(lines[8]));
    for(a='A';a<='Z';++a)
    {
        // Test 8: Same but other letters
        b= (a&1);
        if (b) vu_green(a,((void*)lines[8]));
        else vu_grey(a,((void*)lines[8]));
    }
    vu_initialize((void*)(lines[9]));
    for(a='A';a<='Z';++a)
    {
        // Test 9: All green, attempt to turn grey
        vu_green(a,((void*)lines[9]));
        vu_grey(a,((void*)lines[9]));
    }
    vu_initialize((void*)(lines[10]));
    for(a='A';a<='Z';++a)
    {
        // Test 10: All grey, attempt to turn green
        vu_grey(a,((void*)lines[10]));
        vu_green(a,((void*)lines[10]));
    }


    for(;;)
    {
    }
    return 0;
}

