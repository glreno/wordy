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
#include "title.h"

//                       0 12345 6789 01 23456 7 8 9
const char testmsg[] = "\0\0this\0was\0a\0test\0\0\0";
const int testdl[] = {
    0x7070,0x4770, (int)testmsg, 0x4170,(int)testdl
};

int main(void)
{
    int k;
    char shown = 0;

    // Do some stuff
    for(k=0;k<2048;++k)
    {
        memcpy((char*)0x0600,(char*)0xa000,256);
        // If someone presses L, show the license screen.
        if ( !shown && title_show_license_on_L() )
        {
            shown=1;
        }
    }
    // ok, loading is done
    // Clear the key buffer and show the press a key message
    title_show_press_a_key();

    // Wait for a keypress. It might be an L,
    // which means show the license -- unless you
    // already showed the license
    title_wait_for_key(shown);

    // Restore the original screen, and hang
    OS.sdmctl = 0;
    OS.sdlst = (void*)testdl;
    OS.sdmctl = 0x22;
    for(;;)
        ;
}
