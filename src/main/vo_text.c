/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "vo_text.h"
#include "byteunion.h"
#include "yield.h"
#include "da_audioDriver.h"
#include <_atarios.h>
#include <conio.h>
#include <string.h>

//vo_text *vot_singleton; // Same zero page ptr as vo_singleton. This should probably be in the .h file
#define vot_singleton ((vo_text *)vo_singleton)

// A *VERY* simple opponent view -- it just writes text to a line in the screen.

void vot_handleYield(void);

//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"ONCE")
#pragma rodata-name (push,"ONCE")
#pragma local-strings (on)

//segment ONCE
void __fastcall__ vot_initialize(vo_text *this, char y, ds_page *page, char spotlightSize,const unsigned char *spotlightBmp,unsigned char *screenram)
{
    vot_preinit();
    vo_initialize((vo_base*)this);

    bzero(this->queue,40);
    this->page = page;
    this->screenram = screenram;
    this->y = y;
    this->x = 0;
    this->spotlightY=0;
    this->spotlightSize=spotlightSize;
    this->spotlightBmp=spotlightBmp;
    vo_setYieldFunc(&vot_handleYield);
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

//
// STUFF THAT GOES IN THE MAIN BANKS
//

// temporarily removing the const so this goes into DATA not RODATA
/* const */ char tada[] = {
    0xA1,1, 0xA2,1, 0xA3,1, 0xA4,1, 0xA5,1, 0xA6,1, 0xA7,1,
    0xA6,243, // 250=1/16note
    0xA0,125, // rest 1/32
    0xA4,1, 0xA8,1,
    0xAA,248, 0xAA,250, // 250*2 = eigth
    0xA9,100, 0xA8,100, 0xA7,100, 0xA6,100, 0xA5,100, 0xA4,100, 0xA3,100, 0xA2,100, 0xA1,100,
    0xA0,1, 0,0
};

//
// BANK-SWITCHED OPPONENT CODE
//

#pragma code-name (push,"VOT_BANK")
#pragma rodata-name (push,"VOT_BANK")
#pragma local-strings (on)

//segment VOT_BANK
// Draw a spotlight on the letter being scored.
void __fastcall__ vot_drawSpotlight(unsigned int argp)
{
    vo_text *this=vot_singleton;
    byteunion bu;
    bu.w=argp;
    bzero(this->page->pm.player2+this->spotlightY,this->spotlightSize);
    this->spotlightY = 32+8*bu.msb;
    memcpy(this->page->pm.player2+this->spotlightY,this->spotlightBmp,this->spotlightSize);
    this->page->colors.pcolr2=0x72;
    this->page->colors.hposp2=48+4*bu.lsb;
    //the ds_screendriver PM changes only work if you page flip;
    //so flip to the current page
    ds_flipToPageNextVBI(this->page);
}

//segment VOT_BANK
// Remove the spotlight (clear the PM graphics)
void __fastcall__ vot_clearSpotlight()
{
    vo_text *this=vot_singleton;
    bzero(this->page->pm.player2+this->spotlightY,this->spotlightSize);
    this->page->colors.hposp2=0;
    ds_flipToPageNextVBI(this->page);
}

//segment VOT_BANK
void __fastcall__ vot_handleCommand(const char *guess)
{
    // TODO there are no opponent commands yet, but there surely will be
}

//segment VOT_BANK
char vot_queueMessage()
{
    vo_text *this=vot_singleton;
    vo_base *base=vo_singleton;
    char argi = base->nextMessageN;
    const char *argp = base->nextMessageP;
    // ok, just going to print it really
    switch(base->nextMessageId)
    {
        case MSG_WELCOME: // 01
            strcpy(this->queue,"Welcome to ");
            strcat(this->queue,(char*)argp);
            return 1;
        case MSG_PLEASE: // 04
            strcpy(this->queue,"Please what?");
            return 1;
        case MSG_COMMAND: // 05
            vot_handleCommand(argp);
            return 0;
        case MSG_PALETTE_CHANGE: // 06
            vot_setPalette(argi);
            return 0;
        case MSG_LOOKING_UP_WORD: // 10
            strcpy(this->queue,"Looking up: ");
            strcat(this->queue,(char*)argp);
            return 1;
        case MSG_WORD_NOT_FOUND: // 11
            POKEY_WRITE.audf2 = 193; // E3
            POKEY_WRITE.audf3 = 243; // C3
            da_checkAndPlayNote(1,2,AE_32_SIMPLE);
            da_checkAndPlayNote(1,3,AE_16REST_16_TRAILING);
            strcpy(this->queue,"Word not found.");
            return 1;
        case MSG_ROWS_LEFT: // 18
            strcpy(this->queue,"Rows left: .");
            this->queue[11]='0'+(char)argi;
            return 1;
        case MSG_WINNER: // 19
            strcpy(this->queue,"Well done!");
            // fall through -- same sound as all green
        case MSG_WORD_ALL_GREEN: // 16
            POKEY_WRITE.audf2 = 64; // B4
            POKEY_WRITE.audf3 = 50; // D#5
            POKEY_WRITE.audf4 = 43; // F#5
            da_checkAndPlayNote(1,2,tada);
            da_checkAndPlayNote(1,3,tada);
            da_checkAndPlayNote(1,4,tada);
            return (base->nextMessageId == MSG_WINNER);
        case MSG_LOSER: // 1A - you lost QORDL
            // 28 0x1c uparrow, 29 0x1d downarrow
            strcpy(this->queue," Alas! Answers: ");
            strcat(this->queue,((char**)argp)[0]);
            strcat(this->queue," ");
            strcat(this->queue,((char**)argp)[1]);
            strcat(this->queue," ");
            strcat(this->queue,((char**)argp)[2]);
            strcat(this->queue," ");
            strcat(this->queue,((char**)argp)[3]);
            POKEY_WRITE.audf2 = 255; // B2
            POKEY_WRITE.audf3 = 217; // D3
            POKEY_WRITE.audf4 = 172; // F#3
            da_checkAndPlayNote(1,2,tada);
            da_checkAndPlayNote(1,3,tada);
            da_checkAndPlayNote(1,4,tada);
            return 1;
        case MSG_LOSER1: // 1B - you lost WORDL
            strcpy(this->queue,"Sorry, the answer is: ");
            strcat(this->queue,(char*)argp);
            return 1;
        case MSG_WAIT_FOR_KEY: // 21
            return 0;
        case MSG_CHECKING_LETTER: // 12
            vot_drawSpotlight((unsigned int)argp);
            return 0;
        case MSG_PLAYAGAIN: // 02
        case MSG_RESTART: // 03
            return 0;
        case MSG_LETTER_IS_GREY: // 13
            POKEY_WRITE.audf3 = 128;
            da_checkAndPlayNote(4,3,AE_32_SIMPLE);
            return 0;
        case MSG_LETTER_IS_YELLOW: // 14
            POKEY_WRITE.audf3 = 64;
            da_checkAndPlayNote(2,3,AE_16_TRAILING);
            return 0;
        case MSG_LETTER_IS_GREEN: // 15
            POKEY_WRITE.audf3 = 25;
            da_checkAndPlayNote(2,1,AE_32_ABRUPT);
            da_checkAndPlayNote(2,3,AE_32REST_16_TRAILING);
            return 0;
        case MSG_BAD_KEY_PRESSED: // 23
            POKEY_WRITE.audf2 = 255; // B2
            da_checkAndPlayNote(1,2,AE_32_SIMPLE);
            return 0;
        case MSG_GREEN_WORD_COUNT: // 17
            vot_clearSpotlight();
        case MSG_KEY_PRESSED: // 22
        case MSG_KEY_ACCEPTED: // 24
        default:
            // Unrecognised message
//          strcpy(this->queue,"MSG: ...");
//          this->queue[5]='0'+(((base->nextMessageId)&0xf0)>>4);
//          this->queue[6]='0'+((base->nextMessageId)&0xf);
            return 0;
    }
}

//segment VOT_BANK
void vot_startProcessingMessage()
{
    char printing;
    vo_text *this=vot_singleton;
    bzero(this->queue,40);
    // convert nextMessageId,N,P into a text string in queue
    printing = vot_queueMessage();
    if (printing)
    {
        this->x = 0;
        bzero(this->screenram+this->y*40,40);
        gotoxy(0,this->y);
    }
}

//segment VOT_BANK
void vot_handleYield()
{
    vo_text *this=vot_singleton;
    // First, see if there is work to do on the current message
    char c = this->queue[this->x];
    if ( c != (char)0 )
    {
        //while ( c != (char)0 )
        // Print one letter!
        cputc(c);
        ++(this->x);

        // Make a key click and return
        return;
    }

    // If voice 3 is busy, we want it to finish
    if ( da_isVoiceBusy(3) )
    {
        return;
    }

    // If there is not more work to be done on the current message,
    // start processing the next one
    if ( this->base.nextMessageId != 0 )
    {
        vot_startProcessingMessage();
        vo_markMessageReceived();
        // Note that this sets nextMessage=0 when the new message STARTS PROCESSING.
        // It is not necessarily DONE.
    }
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
