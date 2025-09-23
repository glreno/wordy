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

#include <zlib.h>

#pragma rodata-name (push,"MORERODATA")
#pragma code-name (push,"MORECODE")
#pragma data-name (push,"MORECODE")
#pragma bss-name (push,"MOREBSS")

//one fish two fish red fish blue fish
// Source length: 36  Compressed: 25
const unsigned char compressed [] = {
0xcb, 0xcf, 0x4b, 0x55, 0x48, 0xcb, 0x2c, 0xce, 0x50, 0x28, 0x29, 0xcf, 0x87, 0x30, 0x8a, 0x52, 0x53, 0x20, 0x8c, 0xa4, 0x9c, 0x52, 0x88, 0x1c, 0x0,
};
const int compressedLen=25;

int ret;
unsigned int destLen=1024;
unsigned char dest[1024];

int main(void)
{
    ret=inflatemem(dest,compressed);
    while(1)
        ;
}
