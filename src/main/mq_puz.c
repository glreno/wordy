/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "mq_puz.h"
#include "vw_tiny.h"
#include "byteunion.h"
#include "bankswitch.h"
#include <string.h>

/*
* C portion of the implementation of four-puzzle qordl model
*/


//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"GAMEINIT")
#pragma rodata-name (push,"GAMEINIT")
#pragma local-strings (on)

#ifdef USE_C_MQ_INITIALIZE
//segment GAMEINIT
/* Create wordl models and views. Advance to row 0. */
void mq_initialize(vfm_fontmanager *fm1, vfm_fontmanager *fm2, mq *model)
{
    memset(model,1,WORDLS_IN_QORDL); // clear all the stillInPlay flags
    // TODO the 24 and 12 need to be parameterized. And the 9.
    vwt_initialize(mw_initialize(fm1,9,2,0,&(model->puzzles[0])));
    vwt_initialize(mw_initialize(fm1,9,2,24,&(model->puzzles[1])));
    vwt_initialize(mw_initialize(fm2,9,12,0,&(model->puzzles[2])));
    vwt_initialize(mw_initialize(fm2,9,12,24,&(model->puzzles[3])));
}
#endif

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code
#pragma code-name (push,"APP")

//segment APP
/* Accept a keypress, update view, and decide what should happen next.
 * Do nothing if isWon already.
 *
 * Possible returns:
 *  0:Letter/backspace accepted
 *  1:ENTER was pressed, and all five letters have been entered - controller should look up the word
 *  2:"E" was pressed, and tiles contain "PLEAS" -- controller should go to command mode
 *  3:Invalid keypress -- controller should beep or something
 */
char mq_keypress(mq *model,char key)
{
    char p;
    char ret;
    for(p=0;p<WORDLS_IN_QORDL;++p)
    {
        if (model->stillInPlay[p])
        {
            ret=mw_keypress( &(model->puzzles[p]),key);
        }
    }
    // just return the result from the last model,
    // they all should return the same thing,
    // but they all need to know about it.
    return ret;
}

//segment APP
void mq_scoreRow(mq *model)
{
    char p;
    for(p=0;p<WORDLS_IN_QORDL;++p)
    {
        if (model->stillInPlay[p])
        {
            mw_scoreRow( &(model->puzzles[p]));
        }
    }
}

//segment APP
char mq_checkLetter(mq *model,char puzzleidx,char col)
{
    return mw_checkLetter( model->puzzles[puzzleidx], col);
}

//segment APP
void mq_colorLetter(mq *model,char puzzleidx,char col)
{
    if (model->stillInPlay[puzzleidx])
    {
        mw_colorLetter( &(model->puzzles[puzzleidx]), col);
    }
}

//segment APP
char mq_isWon(mq *model)
{
    char p;
    char n=0;
    for(p=0;p<WORDLS_IN_QORDL;++p)
    {
        n+=mw_isWon( &(model->puzzles[p]));
    }
    return n;
}

//segment APP
/* Advance to next row.
 * scoring is done, prepare for next guess
 * do not advance if isWon already
 * advance to next row, first tile
 * copy currGuess to lastGuess, clear currGuess and currColors
 */
char mq_nextRow(mq *model)
{
    char p;
    signed char r, ret=-1;
    bankswitchStartup(); // for mw_nextRow
    for(p=0;p<WORDLS_IN_QORDL;++p)
    {
        if (mw_isWon( &(model->puzzles[p])) )
        {
            model->stillInPlay[p]=0;
        }
        r=mw_nextRow( &(model->puzzles[p]));
        if ( r>=0 )
        {
            ret=r;
        }
    }
    return ret;
}


//segment APP
/* Calculate the coordinates of a given tile.
 * Result is two 8-bit numbers in an int; MSB is x, LSB is Y
 */
unsigned int __fastcall__ mq_getTileCoords(char puzzleidx, char col, mq *model)
{
    mw *mw = (& (model->puzzles[puzzleidx]));
    return mw_getTileCoords(col,mw);
}

#pragma code-name (pop)
