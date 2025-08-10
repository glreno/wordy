/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vw_base.h
 *
 * Declarations for wordl tile grid view
 *
 */

#ifndef __VW_BASE_H__
#define __VW_BASE_H__

#include <stdlib.h>
#include "vfm_fontmanager.h"

/* ######################################################### */
/* #                   BASE TILE VIEW                     # */
/* ######################################################### */

typedef struct vw_base_struct
{
    char x; //0
    char y; //1
    char nrows; //2
    vfm_fontmanager *fontmanager; //3,4
    char ysz,yoff; //5,6
    char xoff[5]; // 7,8,9,A,B
} vw_base;

enum { VW_GREY=0, VW_YELLOW, VW_GREEN };

/** Initialize data structure, and draw grid of blank tiles */
void __fastcall__ vw_initialize( vfm_fontmanager *fontmanager, char nrows, char y, char x, vw_base *this);

/** Write a letter to a tile.
 * This writes the left and half chars, so for letter "X"
 * it will actually write "Xx" to the screen.
 * Color is one of VW_GREY, VW_YELLOW, VW_GREEN
 * Letter is the ascii letter.
 * For green letters, the letter will be a digit from 0-9,
 * and the lowercase equivalent will be ctrl-P through ctrl-Y.
 * If you're using a complete green alphabet, then the letter is
 * from ! to ;
 */
void __fastcall__ vw_writeTile(vw_base *this, char col, char row, char color, char letter);

/** Remove the letter from a tile. Used for backspace,
 *  basically the same as vw_writeTile(row,col,VW_GREY,' ')
 */
void __fastcall__ vw_clearTile(vw_base *this, char col, char row);

/* Calculate the coordinates of a given tile.
 * Result is two 8-bit numbers in an int; MSB is x, LSB is Y
 */
unsigned int __fastcall__ vw_getTileCoords(char row, char col, vw_base *this);

#endif
