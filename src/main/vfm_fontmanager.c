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
#include "vfm_fontmanager.h"
#include "bankswitch.h"
#include <conio.h>
#include <string.h>
#include <atari.h>
#include <_antic.h>
#include <_atarios.h>

/*
* C portion of the implementation of wordl small-tile font manager
*/

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"FONTDATA")
#pragma rodata-name (push,"FONTDATA")
#pragma local-strings (on)

//segment FONTDATA (same cart bank as ONCE, used in init!)
/** Initialize the font manager, and build all the managed fonts.
 */
void vfm_initialize(vfm_fontmanager *vfm, const vfm_fontdef *fontdef, int nfonts, unsigned char *fonts[])
{
    char i;

    vfm->nfonts = nfonts;
    vfm->fontdef = fontdef;
    vfm_clearGreenLetters(vfm);
    bzero(vfm->isFontAllocated,nfonts);
    vfm->managedFonts[0]=((unsigned int) fonts[0]) >> 8;
    vfm_buildFont(fonts[0],fontdef);
    for(i=1;i<nfonts;++i)
    {
        vfm->managedFonts[i]=((unsigned int) fonts[i])>>8;
        memcpy(fonts[i],fonts[0],1024);
    }
}

// end of initialization code

//segment FONTDATA (same cart bank as ONCE, used in init!)
/** Write a char to a font */
void vfm_setChar(unsigned char *const font, int idx, const unsigned char *const data)
{
    memcpy(font+idx*8,data,8);
}

/*
const unsigned char CHECKERBOARD[8] = {
    0b01100110,
    0b10011001,
    0b01100110,
    0b10011001,
    0b01100110,
    0b10011001,
    0b01100110,
    0b10011001
};
*/

//segment FONTDATA (same cart bank as ONCE, used in init!)
/** Fill in the tileset entries in a font */
void vfm_buildFont(unsigned char *const font, const vfm_fontdef *const src)
{
    unsigned char buf[8];
    char i;

    bzero(font,1024);
    // fill in the font with a checkerboard pattern
    // ...you probably want this commented out
/*
    for(i=0;i<128;++i)
    {
        vfm_setChar(font,i,CHECKERBOARD);
    }
*/

    // Hardcoded: space (atascii 32, internal 0)
    //vfm_setChar(font,0,FC_SPACE);
    bzero(font,8);

    // Hardcoded: LEFT blank tile (atascii 96 control-dot diamond, internal 96 0x60, ascii backquote)
    vfm_setChar(font,0x60,src->yellowBkgdLeft);
    // Hardcoded: RIGHT blank tile (atascii 0 control-comma heart, internal 64 0x40)
    vfm_setChar(font,0x40,src->yellowBkgdRight);

    if ( src->hasLRBoxArt )
    {
    // Hardcoded: LEFT OF BOX (atascii 91, internal 59 0x3b [ )
    vfm_setChar(font,0x3b,src->boxLeft);
    // Hardcoded: RIGHT OF BOX (atascii 93, internal 61 0x3d ] )
    vfm_setChar(font,0x3d,src->boxRight);
    // Hardcoded: LEFT AND RIGHT OF BOX (atascii 92, internal 60 0x3c \ )
    // Add two arrays together!
    for(i=0;i<8;++i)
    {
        buf[i] = src->boxLeft[i]+src->boxRight[i];
    }
    vfm_setChar(font,0x3c,buf);
    }
    if ( src->hasTBBoxArt )
    {
        // Hardcoded: TOP OF BOX (atascii 94, internal 62 0x3e ^ )
        vfm_setChar(font,0x3e,src->boxTop);
        // Hardcoded: BOTTOM OF BOX (atascii 95, internal 63 0x3f _ )
        vfm_setChar(font,0x3f,src->boxBot);

        if ( src->hasTBBoxArt != 2)
        {
            // Hardcoded: TOP AND BOTTOM OF BOX (atascii 64, internal 32 0x20 @ )
            // Add two arrays together!
            for(i=0;i<8;++i)
            {
                buf[i] = src->boxTop[i]+src->boxBot[i];
            }
            vfm_setChar(font,0x20,buf);
        }
    }
    
    // And now the letters. For each letter,
    // find the letter in original Atari font at 0xE000
    // and split it in to left & right halves.
    // For each half, double it from X-X- to XX--XX--
    // and then mask it from the yellow backgrounds,
    // and store it in the appropriate entry.
    for(i=0;i<26;++i)
    {
        vfm_createYellowLetter(font,src,i);
    }
}

static const unsigned char inverter[16] = {
    0b11111111, // 0000
    0b11111100, // 0001
    0b11110011, // 0010
    0b11110000, // 0011

    0b11001111, // 0100
    0b11001100, // 0101
    0b11000011, // 0110
    0b11000000, // 0111

    0b00111111, // 1000
    0b00111100, // 1001
    0b00110011, // 1010
    0b00110000, // 1011

    0b00001111, // 1100
    0b00001100, // 1101
    0b00000011, // 1110
    0b00000000, // 1111
};

//segment FONTDATA (same cart bank as ONCE, used in init!)
void vfm_createYellowLetter(unsigned char *const font, const vfm_fontdef *const src, int letter)
{
    // letter is a number from 0-25
    char i;
    unsigned char c;
    unsigned char *orig=(unsigned char *)(src->sourceFontA+(letter*8));
    unsigned char *fleft = (unsigned char*) (font+(0x21+letter)*8);
    unsigned char *fright = (unsigned char*) (fleft + 8*0x40);

    for(i=0;i<8;++i)
    {
        c=orig[i];
        fleft[i] =  src->yellowBkgdLeft[i] & inverter[(c&0xf0)>>4];
        fright[i] = src->yellowBkgdRight[i] & inverter[(c&0x0f)];
    }
}

//segment FONTDATA (same cart bank as ONCE, used in init!)
void vfm_createGreenLetter(unsigned char *const font, const vfm_fontdef *const src, int letter,int dest)
{
    // letter is a number from 0-25
    // dest is a number from 0-9, but this does not matter to this function
    char i;
    unsigned char c,left,right;
    unsigned char *orig=(unsigned char *)(src->sourceFontA+(letter*8));
    // note dest might be negative, don't care
    unsigned char *fleft = (unsigned char*) (font+(0x10+dest)*8);
    unsigned char *fright = (unsigned char*) (fleft + 8*0x40);

    for(i=0;i<8;++i)
    {
        c=orig[i];
        left = inverter[(c&0xf0)>>4];
        right = inverter[c&0x0f];
        fleft[i] =
                  ( src->greenBkgdLeft[i] & left )
                | ( 0xff & ~left ) ; // change letter bits from 00 to 11
        fright[i] =
                  ( src->greenBkgdRight[i] & right )
                | ( 0xff & ~right ) ; // change letter bits from 00 to 11
    }
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)

#pragma code-name (push,"DS")

//segment DS
/*
 * returns ascii location of the given letter ('0' through '9')
 * if not allocated, allocate it, and create it in all managed fonts
 * Ex:
 * vfm_getGreenLetter(&fontmananger,0,'C') returns '0'
 * vfm_getGreenLetter(&fontmananger,0,'Q') returns '1'
 * vfm_getGreenLetter(&fontmananger,0,'C') returns '0'
 */
char vfm_getGreenLetter(vfm_fontmanager *vfm, char letter)
{
    char i,j;
    // First, look for letter in the allocated green letters
    for(i=0;i<10;++i)
    {
        if ( vfm->greenLetters[i]==letter )
        {
            return '0'+i;
        }
    }
    // OK, didn't find it, look for an unallocated one
    for(i=0;i<10;++i)
    {
        if ( vfm->greenLetters[i]=='\0' )
        {
            bankswitchFontdata();
            vfm->greenLetters[i]=letter;
            // vfm_createGreenLetter uses 'A'=0, not ascii or internal code, for the letter
            // and then '0'=0, not ascii or internal code, for the destination.
            for(j=0;j<vfm->nfonts;++j)
            {
                vfm_createGreenLetter( vfm_getFontAddress(j,vfm), vfm->fontdef, letter-'A',i);
            }
            return '0'+i;
        }
    }
    return '0'; // out of space!
}

#pragma code-name (pop)
