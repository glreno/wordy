/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

//#define TESTMODE

#include "mw_puz.h"
#include <conio.h>
#include <string.h>
#include <atari.h>
#include <ctype.h>
#include <_antic.h>
#include <_atarios.h>
#ifdef TESTMODE
#include <stdio.h>
#endif

/*
* C portion of the implementation of single-puzzle wordl model
*/


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
char mw_keypress(mw *model,char key)
{
    if ( model->cur_x >= 5 )
    {
        if ( key == 155 )
        {
            return 1; // enter
        }
        if ( (key == 'E' || key == 'e') && !strcmp("PLEAS",model->currGuess))
        {
            // Command mode entered.
            // Erase the current word.
            while (model->cur_x>0)
            {
                --(model->cur_x);
                vw_clearTile(&(model->view),model->cur_x,model->cur_y);
            }
            return 2; // command mode
        }
        if ( key != 126 ) // backspace - treat as regular key
        {
            return 3; // exceeded puzzle size!
        }
    }
    if ( key == 126 && model->cur_x > 0 )
    {
        // backspace
        --(model->cur_x);
        vw_clearTile(&(model->view),model->cur_x,model->cur_y);
        return 0;
    }
    if ( isalpha(key) )
    {
        key = (key & 0x5f); // convert to uppercase and not-inverse by removing bits 7 and 5
        model->currGuess[model->cur_x] = key;
        vw_writeTile(&(model->view),model->cur_x,model->cur_y,VW_GREY,key);
        ++(model->cur_x);
        return 0;
    }
    if ( isdigit(key) )
    {
        // If it's a number, just return it.
        // The controller will recognize it as a palette-change code.
        return key;
    }
    return 3; // Not a recognized key
}

//segment APP
void mw_scoreRow(mw *model)
{
    unsigned char i,k;
    char found;
    // First pass: mark all the GREEN letters
    for (i=0;i<5;++i)
    {
        if ( model->solution[i]==model->currGuess[i] )
        {
            model->currColors[i] = VW_GREEN;
        }
    }
#ifdef TESTMODE
    printf("Pass 1 done: %d %d %d %d %d\n",
        model->currColors[0],
        model->currColors[1],
        model->currColors[2],
        model->currColors[3],
        model->currColors[4] );
    cgetc();
        gotoxy(0,11);
        printf("                              \n");
        gotoxy(0,11);
#endif
    // Second passes: mark all the YELLOW letters
    // For each letter that is not green,
    // scan the guess for the same letter.
    // If you find that letter, and it is still unscored, make it yellow
    for (i=0;i<5;++i)
    {
#ifdef TESTMODE
        printf("Y check of %d - %c\n",i,model->solution[i]);
#endif
        if ( model->currColors[i] != VW_GREEN )
        {
            found=0;
            for (k=0;k<5 && !found ;++k)
            {
#ifdef TESTMODE
                printf("Y check of %d - %c %c ",k,model->solution[i],model->currGuess[k]);
#endif
                // (if i==k, currColor will be green already)
                if ( model->currColors[k] == 0xff )
                {
#ifdef TESTMODE
                    printf("CHECK ");
#endif
                    if ( model->solution[i]==model->currGuess[k] )
                    {
#ifdef TESTMODE
                        printf("MATCH");
#endif
                        found=1;
                        model->currColors[k] = VW_YELLOW;
                    }
                }
#ifdef TESTMODE
                printf("\n");
#endif
            }
        }
#ifdef TESTMODE
        else
        {
            printf("Skip - color == %d\n",model->currColors[i]);
        }
        cgetc();
        gotoxy(0,11);
        printf("                              \n");
        printf("                              \n");
        printf("                              \n");
        printf("                              \n");
        printf("                              \n");
        printf("                              \n");
        gotoxy(0,11);
#endif
    }
#ifdef TESTMODE
    printf("Pass 2 done\n");
#endif

    // Third pass: anything unassigned is grey
    for (i=0;i<5;++i)
    {
        if ( model->currColors[i] == 0xff )
        {
            model->currColors[i] = VW_GREY;
        }
    }
}

//segment APP
/* Calculate the coordinates of a given tile.
 * Result is two 8-bit numbers in an int; MSB is x, LSB is Y
 */
unsigned int __fastcall__ mw_getTileCoords(char col, mw *mw)
{
    vw_base *vw = & (mw->view);
    char row = mw->cur_y;
    return vw_getTileCoords(row,col,vw);
}

#pragma code-name (pop)

