/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* w_wordl.h
 *
 * Declarations for single wordl game
 *
 */

#ifndef __MW_PUZ_H__
#define __MW_PUZ_H__

#include <stdlib.h>
#include "vw_base.h"

/* ######################################################### */
/* #             SINGLE PUZZLE MODEL                       # */
/* ######################################################### */

typedef struct mw_struct
{
    char solution[6];
    char lastGuess[6]; // needed to track 'was it green'    - OFFSET=6
    char currGuess[6]; // updated on each keystroke         - OFFSET=12
    char currColors[5];// updated in mw_checkLetter(0)      - OFFSET=18
    char nrows;         // number of rows of the puzzle     - OFFSET=23
    char cur_y;         // Row that is currently being typed in
    char cur_x;         // Column that next typed letter will be in, 5==ENTER expected
    vw_base view;     // not a pointer!                    - OFFSET=26
} mw;

/* Create (and return) wordl view. Advance to row 0. */
vw_base * __fastcall__ mw_initialize(vfm_fontmanager *fontmanager,char nrows,char y,char x, mw *model);
void __fastcall__ mw_resetForNextRow(char y,mw *model);

/* Set the solution to the puzzle */
//void mw_setSolution(const char *solution,mw *model);
#define mw_setSolution(sol,model) strcpy((model)->solution,(sol))

/* Accept a keypress, update view, and decide what should happen next.
 * Do nothing if isWon already.
 *
 * Possible returns:
 *  0:Letter/backspace accepted
 *  1:ENTER was pressed, and all five letters have been entered - controller should look up the word
 *  2:"E" was pressed, and tiles contain "PLEAS" -- controller should go to command mode
 *  3:Invalid keypress -- controller should beep or something
 */
char __fastcall__ mw_keypress(mw *model,char key);

/* Check the guess against the solution.
 * Results are stored in currColors, use mw_checkLetter() to read them.
 */
void __fastcall__ mw_scoreRow(mw *model);

/* Check the correctness of a tile.
 * This ONLY returns the result, does not update the view.
 * The controller will have to call mw_colorLetter() to do that.
 * This is NOT valid until mw_scoreRow() is called!
 * Returns one of VW_GREY, VW_YELLOW, VW_GREEN
 */
//char mw_checkLetter(mw &model, char col);
#define mw_checkLetter(model,col) model.currColors[col]

/* Recolor a tile with the correct score. Call mw_checkLetter() if you need to know WHAT color it will be.
 * This is NOT valid until mw_scoreRow() is called!
 * This is done one column at a time for animation purposes.
 */
//void mw_colorLetter(mw *model, char col);
#define mw_colorLetter(model, col) vw_writeTile(&((model)->view),(col),(model)->cur_y,(model)->currColors[(col)],(model)->currGuess[(col)])

/* Did I win? Return 1 if all five letters are green. */
char __fastcall__ mw_isWon(mw *model);

/* Advance to next row.
 * scoring is done, prepare for next guess
 * do not advance if isWon already
 * advance to next row, first tile
 * copy currGuess to lastGuess, clear currGuess and currColors
 * return number of rows remaining, -1 means the game is ended (won or last row complete)
 */
char __fastcall__ mw_nextRow(mw *model);

/* Calculate the coordinates of a given tile.
 * Result is two 8-bit numbers in an int; MSB is x, LSB is Y
 */
unsigned int __fastcall__ mw_getTileCoords(char col, mw *mw);


#endif
