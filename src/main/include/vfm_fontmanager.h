/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vfm_fontmanager.h
 *
 * Declarations for wordl small-tile font
 *
 */

#ifndef __VFM_FONTMANAGER_H__
#define __VFM_FONTMANAGER_H__

#include <stdlib.h>

/* ######################################################### */
/* #                   SMALL FONT MANAGER                  # */
/* ######################################################### */

typedef struct vfm_fontdef_struct
{
    unsigned char *sourceFontA ; // address of A in the source font, 0xE108
    unsigned char yellowBkgdLeft[8];
    unsigned char yellowBkgdRight[8];
    unsigned char greenBkgdLeft[8];
    unsigned char greenBkgdRight[8];
    char hasLRBoxArt; // if 0, this struct can end after these flags
    char hasTBBoxArt; // if 0, this struct can end after boxRight
    unsigned char boxLeft[8];
    unsigned char boxRight[8];
    unsigned char boxTop[8];
    unsigned char boxBot[8];
} vfm_fontdef;

// Maximum number of fonts any fontmanager can manage
// TODO make this overridable and reduce the default
// This specifies array sizes, so make sure it's defined properly everywhere!
// In particular, it's used in struct offsets in vfm_fontmanagerA.asm
#define VFM_MANAGED_FONT_COUNT 16

// List of 47 undefined font codepoints (internal coding)
// (it's really an array of 47, this is the first one)
extern unsigned char VFM_UNRESERVED_CODEPOINTS[];

typedef struct vfm_fontmanager_struct
{
    // which letters are in which green-letter slot
    // char 0 means not allocated
    // (so if greenLetters[0]='B' then internal-font locations 16 and 80)
    char greenLetters[10];  // OFFSET 0

    // number of managed fonts, max VFM_MANAGED_FONT_COUNT
    int nfonts;             // OFFSET 10
    // Addresses of managed fonts
    unsigned char managedFonts[VFM_MANAGED_FONT_COUNT]; // OFFSET 12
    // 0 if font is not in use, 1 if font has been allocated
    char isFontAllocated[VFM_MANAGED_FONT_COUNT];       // OFFSET 12 + VFM_MANAGED_FONT_COUNT = 28

    // Font definition, needed at runtime to build green tiles
    const vfm_fontdef *fontdef;
} vfm_fontmanager;

/** Initialize the font manager, and build all the managed fonts.
 */
void __fastcall__ vfm_initialize(vfm_fontmanager *vfm, const vfm_fontdef *fontdef, int numFonts, unsigned char *fonts[]);


/** Fill in a single character in a font */
void __fastcall__ vfm_setChar(unsigned char *const font, int internalcode, const unsigned char *const data);

/** Fill in the tileset entries in a font.
 * The letters will be based on the Atari built-in font, and
 * the two 16-byte background characters.
 */
void __fastcall__ vfm_buildFont(unsigned char *const font, const vfm_fontdef *const src );

void __fastcall__ vfm_createYellowLetter(unsigned char *const font, const vfm_fontdef *const src, int letter);
void __fastcall__ vfm_createGreenLetter(unsigned char *const font, const vfm_fontdef *const src, int letter,int dest);

/*
 * returns ascii location of the given letter ('0' through '9')
 * if not allocated, allocate it, and create it in all managed fonts
 * Ex:
 * vfm_getGreenLetter(&fontmananger,0,'C') returns '0'
 * vfm_getGreenLetter(&fontmananger,0,'Q') returns '1'
 * vfm_getGreenLetter(&fontmananger,0,'C') returns '0'
 */
char __fastcall__ vfm_getGreenLetter(vfm_fontmanager *vfm, char letter);

/*
 * Reset the green letter cache
 * (doesn't change the font, but all the chars can now be reused)
 */
//void vfm_clearGreenLetters(vfm_fontmanager *vfm);
#define vfm_clearGreenLetters(vfm) bzero((vfm)->greenLetters,10)

/****** FONT ALLOCATION *****/

/**
 * selects one of the unallocated managed fonts, marks it allocated
 * returns ID of allocated font, 0xff on error
 */
char __fastcall__ vfm_allocateFont(vfm_fontmanager *vfm);

/**
 * marks the specified font as unallocated, call when done with an animation frame
 */
void __fastcall__ vfm_deallocateFont(char fontid,vfm_fontmanager *vfm);


/**
 * gets the address of the specified managed font
 * ....the macro is smaller than the assembler equivalent
 */
//unsigned char* vfm_getFontAddress(char fontid,vfm_fontmanager *vfm);
#define vfm_getFontAddress(fontid,vfm) ((unsigned char *) ( vfm_getFontBase((fontid),(vfm)) << 8 ))

/**
 * Get the base address (MSB only) of the specified font
 */
unsigned char __fastcall__ vfm_getFontBase(char fontid,vfm_fontmanager *vfm);

#endif
