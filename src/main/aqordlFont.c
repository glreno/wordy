/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/
#include "aqordlFont.h"

extern unsigned char INTERACT_FONT;

#pragma code-name (push,"FONTDATA")
#pragma rodata-name (push,"FONTDATA")
#pragma local-strings (on)

const vfm_fontdef AQORDLFONTDEF = {
    &INTERACT_FONT,
    {
    // yellow left - yellow and light grey are 11
    0,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111
    },{
    // yellow right
    0,
    0b11111100,
    0b11111101,
    0b11111101,
    0b11111101,
    0b11111101,
    0b11111101,
    0b11111100
    },

    // green left - green is 10
    {
    0,
    0b01010101,
    0b01101010,
    0b10101010,

    0b10101010,
    0b10101010,
    0b01101010,
    0b01010101
    },{
    // green right
    0,
    0b01010100,
    0b10101001,
    0b10101001,

    0b10101001,
    0b10101001,
    0b10101001,
    0b01010100
    },

    1, // flags indicating there is LR box art
    2, // flags indicating there is LR box art but to NOT combine them
       // (Box top & bottom are being used for a decorative border)

    // box left - dark grey is 01
    { 0, 0x0, 0x01, 0x01,   0x01, 0x01, 0x01, 0x0},
    // box right
    { 0,0,0,0,0,0,0,0 },
    //{ 0x40, 0x40, 0x40, 0x40,   0x40, 0x40, 0x40, 0x40}

    // box top
    {
    0,0,0,
    0b01000001,
    0b11010111,
    0b01000001,
    0,0,
    },
    // box bottom
    {
    0,0,0,
    0b00010100,
    0b01000001,
    0b00010100,
    0,0,
    }
};

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)

