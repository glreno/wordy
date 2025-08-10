/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/
#include "awordlFont.h"

#pragma code-name (push,"FONTDATA")
#pragma rodata-name (push,"FONTDATA")
#pragma local-strings (on)

const vfm_fontdef AWORDLFONTDEF = {
    (unsigned char*)0xE108,
    {
    // yellow left - yellow and light grey are 11
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111
    }, {
    // yellow right
    0b11111111,
    0xff, 0xff, 0xff,   0xff, 0xff, 0xff,
    0b11111111
    },

    // green left - green is 10
    {
    0b01011010,
    0b01101010,
    0xaa, 0xaa,   0xaa, 0xaa,
    0b01101010,
    0b01011010
    },{
    // green right
    0b10101001,
    0xaa, 0xaa, 0xaa,   0xaa, 0xaa, 0xaa,
    0b10101001
    },

    1,1, // flags indicating there is box art
    // box left - dark grey is 01
    { 0x01, 0x01, 0x01, 0x01,   0x01, 0x01, 0x01, 0x01},
    // box right
    { 0x40, 0x40, 0x40, 0x40,   0x40, 0x40, 0x40, 0x40},
    // box top
    { 0, 0, 0, 0,    0, 0, 0, 0x55},
    // box bottom
    { 0x55, 0, 0, 0,    0, 0, 0, 0}
};

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
