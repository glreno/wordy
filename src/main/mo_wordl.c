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
#include "mo_wordl.h"
#include "vo_base.h"
#include "yield.h"
#include "dk_kbdriver.h"
#include "da_audioDriver.h"
#include "vu_letters.h"
#include "bankswitch.h"

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"GAMEINIT")
#pragma rodata-name (push,"GAMEINIT")
#pragma local-strings (on)

//segment GAMEINIT
void __fastcall__ mow_initialize(mow *this, mw *gameModel, md_dict *dict, void *usedLetterView)
{
    this->gameModel = gameModel;
    this->dict = dict;
    this->vu_letters = usedLetterView;
    this->commandModeFlag = 0;
    vu_initialize(this->vu_letters);
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code
#pragma code-name (push,"APP")

//segment APP
void __fastcall__ mow_messageLoser(mw *puzzle)
{
    vo_messagep( MSG_LOSER1,0,(char*)(puzzle->solution));
    vo_waitUntilMessageReceived();
}

//segment APP
void __fastcall__ mow_messageKey(char i,mow *this)
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
void __fastcall__ mow_checkAndColorLetter(mow *this, char p,char i)
{
    char k;
    mw *gm = this->gameModel;
    char c = gm->currGuess[i];
    vo_messagep( MSG_CHECKING_LETTER,c,(void*)mw_getTileCoords(i,gm));
    vo_waitUntilMessageReceived();
    k=mw_checkLetter((*gm),i);
    if ( k == VW_GREEN )
    {
        vo_message( MSG_LETTER_IS_GREEN,c);
        vu_green(c,this->vu_letters);
    }
    else if ( k == VW_YELLOW )
    {
        vo_message( MSG_LETTER_IS_YELLOW,c);
        vu_green(c,this->vu_letters);
    }
    else vo_message( MSG_LETTER_IS_GREY,c);
    vo_waitUntilMessageReceived();
    mw_colorLetter(gm,i);
}

//segment APP
void __fastcall__ mow_getKey(mow *this)
{
    int i=0;
    mw *gm = this->gameModel;
    while ( i != 1 )
    {
        vo_message( MSG_WAIT_FOR_KEY, 0);
        // dk_getc always returns an ascii character
        // (it waits instead of returning -1)
        i = dk_getc();
        da_keyClick();
        vo_message( MSG_KEY_PRESSED, i );
        vo_waitUntilMessageReceived();
        i = mw_keypress(gm,i);
        // 1==enter 2==please 3==bad key
        mow_messageKey(i,this);
    }
}

//segment APP
void __fastcall__ mow_handleCommand(char *guess,mow *this)
{
    this->commandModeFlag=0;
    vo_messagep( MSG_COMMAND,0,guess);
    vo_waitUntilMessageReceived();
}

//segment APP
void __fastcall__ mow_gameDriver(mow *this)
{
    char i,p;
    int k;
    char y=0;
    char *guess;
    mw *gm = this->gameModel;

    vo_messagep( MSG_WELCOME, 0, "AWordl (6 row version)");
    vo_waitUntilMessageReceived();
    for(;;)
    {
        mow_getKey(this);

        // Find the current guess.
        guess = gm->currGuess;
        if ( this->commandModeFlag )
        {
            mow_handleCommand(guess,this);
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
            mw_scoreRow(gm);
                    for(i=0;i<5;++i)
                    {
                        mow_checkAndColorLetter(this,p,i);
                    }
                    if ( mw_isWon(gm) )
                    {
                        vo_messagep(MSG_WORD_ALL_GREEN,p,guess);
                        vo_waitUntilMessageReceived();
                    }

            // marked the used letters as used.
            for(i=0;i<5;++i)
            {
                vu_grey(guess[i],this->vu_letters);
            }

            k = mw_isWon(gm);
            vo_message( MSG_GREEN_WORD_COUNT,k);
            vo_waitUntilMessageReceived();
            if ( k == 1 )
            {
                vo_message( MSG_WINNER,0); //"We have a winner!");
                vo_waitUntilMessageReceived();
                return;
            }

            bankswitchStartup();
            k = mw_nextRow(gm);
            ++y;
            if ( k < 1 )
            {
                mow_messageLoser(gm);
                return;
            }
            vo_message( MSG_ROWS_LEFT,k);
            vo_waitUntilMessageReceived();
        }
    }
}

#pragma code-name (pop)
