/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/


/* Random number generator test */
#include <stdlib.h>
#include <conio.h>
#include <string.h>

char buf[5];
int main(void)
{
    char x,y;
    int r;
    // Not seeding the random number generator will ALWAYS
    // generate the same thing: 359B...48E6

    // Calling _randomize() depends on VCOUNT and RTCLOCK,
    // so if you call them first thing in main() the always
    // generate the same thing: 417B...6C01
    //_randomize();

    // Calling srand() with the Atari random number generator
    // at D20A RANDOM after an immediate wsync always
    // generate the same thing: 4889...7115
    // ANTIC.wsync=0;

    // Calling srand() with the Atari random number generator
    // at D20A RANDOM after a loop that takes a few jiffies
    // still generates the same number all the time but the number
    // is different based on the length of the loop.
    // x<255 y<64 generates 5B02...64E5
    // x<254 y<64 generates 4949...30B9
    for(x=0;x<254;++x)
        for(y=0;y<64;++y)
            strcpy(buf,"xxxx");

    // Calling srand() with the Atari random number generator
    // at D20A RANDOM  with no delay at all will always
    // generate the same thing: 3F88...0CC8
    // (These two reads get 0x16 and 0xFE)
    x=POKEY_READ.random;
    y=POKEY_READ.random;
    srand(x<<8+y);

    // With no delay at all, generate and print a pile
    // of random numbers.
    gotoxy(0,0);
    for(y=0;y<22;++y)
    {
        for(x=0;x<8;++x)
        {
            r=rand();
            itoa(r,buf,16);
            if ( r<0x1000 ) cputs(" ");
            if ( r<0x100 ) cputs(" ");
            if ( r<0x10 ) cputs(" ");
            cputs(buf);
            cputs(" ");
        }
    }
    for(;;) ;
}
