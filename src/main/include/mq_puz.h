/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* mq_puz.h
 *
 * Declarations for four-puzzle qordl game
 *
 */

#ifndef __MQ_PUZ_H__
#define __MQ_PUZ_H__

#include <stdlib.h>
#include "mw_puz.h"

/* ######################################################### */
/* #               FOUR PUZZLE MODEL                       # */
/* ######################################################### */

#define WORDLS_IN_QORDL 4
typedef struct mq_struct
{
    char stillInPlay[WORDLS_IN_QORDL]; // set to 0 when a puzzle should no longer accept keypresses, i.e., isWon.
    mw puzzles[WORDLS_IN_QORDL];
} mq;

/* Create wordl models and views. Advance to row 0. */
void __fastcall__ mq_initialize(vfm_fontmanager *fm1, vfm_fontmanager *fm2, mq *model);

/* Accept a keypress, update view, and decide what should happen next.
 * Do nothing if isWon already.
 *
 * Possible returns:
 *  0:Letter/backspace accepted
 *  1:ENTER was pressed, and all five letters have been entered - controller should look up the word
 *  2:"E" was pressed, and tiles contain "PLEAS" -- controller should go to command mode
 *  3:Invalid keypress -- controller should beep or something
 */
char __fastcall__ mq_keypress(mq *model,char key);

/* Check the guess against the solution.
 * Results are stored in currColors, use mq_checkLetter() to read them.
 */
void __fastcall__ mq_scoreRow(mq *model);

/* Check the correctness of a tile.
 * This ONLY returns the result, does not update the view.
 * The controller will have to call mw_colorLetter() to do that.
 * This is NOT valid until mw_scoreRow() is called!
 * Returns one of VW_GREY, VW_YELLOW, VW_GREEN
 */
char __fastcall__ mq_checkLetter(mq *model, char puzzleidx, char col);

/* Recolor a tile with the correct score. Call mq_checkLetter() if you need to know WHAT color it will be.
 * This is NOT valid until mq_scoreRow() is called!
 * This is done one column at a time for animation purposes.
 */
void __fastcall__ mq_colorLetter(mq *model, char puzzleidx, char col);

/* Did I win? Returns number of puzzles that are won */
char mq_isWon(mq *model);

/* Advance to next row.
 * scoring is done, prepare for next guess
 * do not advance if isWon already
 * advance to next row, first tile
 * copy currGuess to lastGuess, clear currGuess and currColors
 * return number of rows remaining, -1 means the game is ended (won or last row complete)
 */
char __fastcall__ mq_nextRow(mq *model);

/* Calculate the screen coordinates of a given tile (assumes a nice 40 column text mode)
 * Result is two 8-bit numbers in an int; MSB is x, LSB is Y
 */
unsigned int __fastcall__ mq_getTileCoords(char puzzleidx, char col, mq *model);

#endif
