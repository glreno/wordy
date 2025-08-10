/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "ds_screendriver.h"
#include "vw_base.h"
#include <conio.h>
#include <string.h>
#include <atari.h>
#include <_antic.h>
#include <_atarios.h>

/*
* C portion of the implementation of wordl tile grid view
*/

#pragma code-name (push,"APP")

//segment APP
void vw_drawTwoTiles(vw_base *this, char col, char row, char left, char right)
{
    //char x = this->x+col*3+1;
    //char y = this->y+row*2+1;
    char x = this->xoff[col];
    char y = row*this->ysz+this->yoff+this->y;
    ds_put2c(right,left,x,y);
}

//segment APP
/** Write a letter to a tile. */
void vw_writeTile(vw_base *this, char col, char row, char color, char letter)
{
    char right;
    char left=letter;

    // If the color is GREEN, then we need to find the right
    // char for this fontmanager. This might have to allocate one!
    if ( color == VW_GREEN )
    {
        // This will return a char from '0' to '9'.
        left = vfm_getGreenLetter(this->fontmanager,letter);
    }

    // If the color is GREY or YELLOW, then letter is an ASCII letter.
    // We need to convert it to the Atari Internal code, by subtracting 32.
    // And if it's yellow, add 128 to invert it.
    // But if it's GREEN, then it's an ASCII number from 0-9; and the
    // right half will be an ATASCII control code. And cputcxy() will NOT
    // display ctrl-J or ctrl-M because CRLF. So we MUST print in internal codes.
    // But the rule for numbers is still to subtract 32.
    left -= 0x20;
    if ( color == VW_YELLOW )
    {
        left |=  0x80;
    }

    // Right-side char is the lower case.
    // In Atari Internal code, you get lower case by adding 0x40.
    // That works for the numbers I'm using for green, too.
    right = left + 0x40;
    vw_drawTwoTiles(this,col,row,left,right);
}

//segment APP
/** Remove the letter from a tile. Used for backspace */
void vw_clearTile(vw_base *this, char col, char row)
{
    vw_drawTwoTiles(this,col,row,0x60,0x40);
}

#pragma code-name (pop)

