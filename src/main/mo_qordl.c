/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include <_atarios.h>
#include <atari.h>
#include <ctype.h>
#include <string.h>
#include "mo_qordl.h"
#include "vo_base.h"
#include "yield.h"
#include "dk_kbdriver.h"
#include "da_audioDriver.h"
#include "vu_letters.h"

#pragma code-name (push,"APP")

char *solutions[4];

//segment APP
void __fastcall__ moq_messageLoser(mw *puzzles)
{
    solutions[0]=puzzles[0].solution;
    solutions[1]=puzzles[1].solution;
    solutions[2]=puzzles[2].solution;
    solutions[3]=puzzles[3].solution;
    vo_messagep( MSG_LOSER,0,(char*)solutions);
    vo_waitUntilMessageReceived();
}

//segment APP
void __fastcall__ moq_messageKey(char i,moq *this)
{
    char msg=MSG_KEY_ACCEPTED;
    if ( i==3 )
    {
        msg = MSG_BAD_KEY_PRESSED;
    }
    else if ( i==2 )
    {
        msg = MSG_PLEASE;
        this->commandModeFlag=1;
    }
    else if ( isdigit(i) )
    {
        msg = MSG_PALETTE_CHANGE;
        i=i-'0';
    }
    vo_message( msg,i);
    vo_waitUntilMessageReceived();
}

//segment APP
void __fastcall__ moq_checkAndColorLetter(moq *this, char p,char i)
{
    char k;
    mq *gm = this->gameModel;
    char c = gm->puzzles[p].currGuess[i];
    vo_messagep( MSG_CHECKING_LETTER,c,(char*)mq_getTileCoords(p,i,gm));
    vo_waitUntilMessageReceived();
    k=mq_checkLetter(gm,p,i);
    if ( k == VW_GREEN )
    {
        vo_message( MSG_LETTER_IS_GREEN,c);
    }
    else if ( k == VW_YELLOW )
    {
        vo_message( MSG_LETTER_IS_YELLOW,c);
    }
    else vo_message( MSG_LETTER_IS_GREY,c);
    vo_waitUntilMessageReceived();
    vo_message( MSG_WAIT_FOR_BLOCK,0); // wait for animation to complete
    vo_waitUntilMessageReceived();
    mq_colorLetter(gm,p,i);
    if ( k == VW_GREEN || k == VW_YELLOW )
    {
        vu_green(c,this->vu_letters);
    }
}

//segment APP
void __fastcall__ moq_getKey(moq *this)
{
    mq *gm = this->gameModel;
    int i=0;
    while ( i != 1 )
    {
        vo_message( MSG_WAIT_FOR_KEY, 0);
        // dk_getc always returns an ascii character
        // (it waits instead of returning -1)
        i = dk_getc();
        da_keyClick();
        vo_message( MSG_KEY_PRESSED, i );
        vo_waitUntilMessageReceived();
        i = mq_keypress(gm,i);
        // 1==enter 2==please 3==bad key
        moq_messageKey(i,this);
    }
}

//segment APP
char __cdecl__ moq_findActivePuzzle(mw *puzzles,char y)
{
    // If a puzzle's cur_y < y then it is
    // already won. We need ANY other puzzle.
    signed char i;
    for(i=3;i>=0;--i)
    {
        if ( puzzles[i].cur_y==y )
        {
            return i;
        }
    }
    return 0xff;
}

//segment APP
void __fastcall__ moq_handleCommand(char *guess,moq *this)
{
    this->commandModeFlag=0;
    vo_messagep( MSG_COMMAND,0,guess);
    vo_waitUntilMessageReceived();
}

//segment APP
void __fastcall__ moq_gameDriver(const char *title,moq *this)
{
    char i,p;
    int k;
    char y=0;
    char *guess;
    mq *gm = this->gameModel;
    mw *puzzles = gm->puzzles;

    vo_messagep( MSG_WELCOME, 0, title);
    vo_waitUntilMessageReceived();
    vo_message( MSG_RESTART, 0 );
    vo_waitUntilMessageReceived();
    for(;;)
    {
        moq_getKey(this);

        // Find the current guess.
        p = moq_findActivePuzzle(puzzles,y);
        guess = puzzles[p].currGuess;
        if ( this->commandModeFlag )
        {
            moq_handleCommand(guess,this);
        }

        vo_messagep( MSG_LOOKING_UP_WORD,p,guess);
        vo_waitUntilMessageReceived();
        k = md_findWord(this->dict,(md_word*) guess,NULL);

        if ( k == -1 )
        {
            vo_messagep( MSG_WORD_NOT_FOUND,0,guess);
            vo_waitUntilMessageReceived();
        }
        else
        {
            mq_scoreRow(gm);
            for(p=0;p<4;++p)
            {
                if (gm->stillInPlay[p])
                {
                    for(i=0;i<5;++i)
                    {
                        moq_checkAndColorLetter(this,p,i);
                    }
                    if ( mw_isWon(&(puzzles[p])) )
                    {
                        vo_messagep(MSG_WORD_ALL_GREEN,p,guess);
                        vo_waitUntilMessageReceived();
                    }
                }
            }

            // marked the used letters as used.
            for(i=0;i<5;++i)
            {
                vu_grey(guess[i],this->vu_letters);
            }

            k = mq_isWon(gm);
            vo_message( MSG_GREEN_WORD_COUNT,k);
            vo_waitUntilMessageReceived();
            if ( k == 4 )
            {
                vo_message( MSG_WINNER,0); //"We have a winner!");
                vo_waitUntilMessageReceived();
                return;
            }

            k = mq_nextRow(gm);
            ++y;
            if ( k < 1 )
            {
                moq_messageLoser(puzzles);
                return;
            }
            vo_message( MSG_ROWS_LEFT,k);
            vo_waitUntilMessageReceived();
        }
    }
}

#pragma code-name (pop)
