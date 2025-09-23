/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "vo_robot.h"
#include "yield.h"
#include "vfm_fontmanager.h"
#include "robot0_anim.h"
#include <atari.h>
#include <conio.h>
#include <string.h>

// initialized by vor_initialize
char vor_blockAllMessages;

// Initialized with first message
unsigned char vor_boredTimer;
unsigned char vor_amBored;
extern void vor_bored();
unsigned char lastprogress;


// A dancing robot with laser eyes?

void vor_handleYield(void);


//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"ONCE")
#pragma rodata-name (push,"ONCE")
#pragma local-strings (on)

char lpf; // lasers-per-frame 5 for pal, 6 for ntsc

//segment ONCE
void __fastcall__ vor_initialize(vo_robot *this, char liney, ds_page **pages, unsigned char *screenram)
{
    unsigned char x,y,i,c;
    vor_preinit();
    vo_initialize((vo_base*)this);
    vor_blockAllMessages = 0; // must clear this on warmstart!

    bzero(this->queue,40);
    this->anim.pages[0] = pages[0];
    this->anim.pages[1] = pages[1];
    this->anim.pages[2] = pages[2];
    this->anim.lastPageDrawn = pages[2];
    this->screenram = screenram;
    this->y = liney;
    this->x = 0;
    this->laserTargetCoords.w=0;
    vo_setYieldFunc(&vor_handleYield);

    // Fill in the opponent-animation area
    i=0;
    for(x=0;x<6;++x)
    {
        for(y=0;y<8;++y)
        {
            // We are using 48 codepoints, so can't use boxtops in the view!
            c=VFM_UNRESERVED_CODEPOINTS[i];
            if ( y > 0 )
            {
                ds_putc(c,17+x,3+y);
            }
            ds_putc(c,17+x,11+y);
            ++i;
        }
    }

    // Initialize all the pages. This is a separate function in vo_anim.c
    voa_initPages();
    lastprogress=0;
    if ( GTIA_READ.pal & 0xE == 0 )
    {
        // PAL system
        lpf = 5;
    }
    else
    {
        lpf = 6;
    }
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

//
// STUFF THAT GOES IN THE MAIN BANKS
//


//
// BANK-SWITCHED OPPONENT CODE
//

#pragma code-name (push,"VOR_CODE")
#pragma rodata-name (push,"VOR_IDX")
#pragma local-strings (on)

void __fastcall__ vor_doDrawLaser(unsigned char pageid);

//segment VOR_CODE
// Draw a laser on the letter being scored.
// Progress runs from 0 to 60, it is NOT a percent.
void __fastcall__ vor_drawLaser(unsigned char pageid, int sx, int sy, unsigned char progress);
void __fastcall__ vor_drawLaser(unsigned char pageid, int sx, int sy, unsigned char progress)
{
    vo_robot *this=vor_singleton;
    vo_anim *thisA=voa_singleton;
    int ex;
    int ey;
    int dx;
    int dy;
    int chunksize;
    int chunk;
    int i;
    byteunion bu;
    bu.w=this->laserTargetCoords.w;
    if ( bu.w == 0 )
    {
        // nothing was in the queue
        return;
    }
    if ( lastprogress>59 )
    {
        lastprogress=0;
    }
    ex=48+4*bu.lsb;
    ey=32+8*bu.msb;

    // The laser is moving from (sx,sy) to (ex,ey)
    // Divide the range from lastprogress(exclusive) to progress (inclusive) into 5 or 6 chunks,
    // so that you have 5 or 6 numbers that differ by chunksize.
    chunksize=(lastprogress-progress)/lpf;
    for(i=0;i<lpf;++i)
    {
        // note that those 60s are NOT related to pal or ntsc, just progress
        chunk = progress+chunksize*(i+1);
        dx = chunk*(ex-sx)/60;
        dy = chunk*(ey-sy)/60;

        // Store the laser coordinates in the page.freespace
        // so that the VBI can draw the laser there
        thisA->pages[pageid]->page.freespace[0x000+i]=((sx+dx)&0xff);
        thisA->pages[pageid]->page.freespace[0x100+i]=((sy+dy)&0xff);
    }
    thisA->pages[pageid]->page.freespace[0x3D]=lpf-1;

    lastprogress=progress;
}

//segment VOR_CODE
// Remove the laser (clear the PM graphics)
void __fastcall__ vor_clearLaser(unsigned char pageid);
void __fastcall__ vor_clearLaser(unsigned char pageid)
{
    vo_robot *this=vor_singleton;
    vo_anim *thisA=voa_singleton;
    bzero(thisA->pages[pageid]->pm.player2,512);
    thisA->pages[pageid]->colors.hposp2=0;
}

//segment VOR_CODE
// Done drawing laser
void __fastcall__ vor_finishLaser();
void __fastcall__ vor_finishLaser()
{
    vo_robot *this=vor_singleton;
    this->laserTargetCoords.w = 0;
    vor_blockAllMessages=0;
}

//segment VOR_CODE
// Queue up the next laser
void __fastcall__ vor_queueLaser();
void __fastcall__ vor_queueLaser(unsigned int coords)
{
    vo_robot *this=vor_singleton;
    this->laserTargetCoords.w = coords;
}

extern vfm_fontmanager fontManagerTop, fontManagerBot;



//segment VOR_CODE
// Draw an animation frame into a Page.
// The ID for the animation is stored in page.spare[0]
void __fastcall__ vor_renderFrame(unsigned char pageid)
{
    vo_anim *this=voa_singleton;
    unsigned char picid=this->pages[pageid]->page.spare[0];
    unsigned char fontbaseTop = vfm_getFontBase(this->pages[pageid]->page.fonts[0],&fontManagerTop);
    unsigned char fontbaseBot = vfm_getFontBase(this->pages[pageid]->page.fonts[1],&fontManagerBot);
    int ret,laserstartx;
    voa_playCue(); // this yield() is taking a long time, check the audio queue
    ds_clearFontPic(fontbaseTop,fontbaseBot);
    vor_clearLaser( pageid );
    voa_playCue();

    ret = voa_drawPic(fontbaseBot, fontbaseTop, picid);

    voa_playCue(); // This plays the already queued chord, not the one we just loaded.

    if ( ret & 0x8000 )
    {
        // Laser parameter is set:
        // we only want the top 8 bits for this.
        ret = ((ret>>8)&0xff);
            // Bit 7 set -- It's a laser
            if ( ret & 0x40 )
            {
                // Bit 6 set -- it's a RIGHT laser
                laserstartx = 126;
            }
            else
            {
                // it's a LEFT laser
                laserstartx = 122;
            }
            // Read the progress bits - this is a number from 0-60
            ret = (ret & 0x3f);
            vor_drawLaser( pageid, laserstartx, 74, ret );
            if ( ret >= 60 )
            {
                vor_finishLaser();
            }
    }
    else
    {
        // No laser! Clear the laser X coordinate so it doesn't get drawn
        picid=this->pages[pageid]->page.freespace[0]=0;     // X coord of first laser; 0 means do not draw
        picid=this->pages[pageid]->page.freespace[0x3D]=0;  // index of laser coord to draw
    }
    ds_copyBelt(fontbaseTop,fontbaseBot);
    voa_playCue();
}

//segment VOR_CODE
void __fastcall__ vor_handleCommand(const char *guess)
{
    if(!strcmp("DANCE",guess) )
    {
        voa_setLoopCountdown(2,vor_anim_alldone_wristcheck,&vor_blockAllMessages);
        vor_blockAllMessages=1;
        voa_startAnimationLoop( vor_anim_victory );
    }
    if(!strcmp("SADLY",guess) )
    {
        voa_startAnimationLoop( vor_anim_loss );
    }
    if(!strcmp("ANGRY",guess) )
    {
        voa_startAnimationLoop( vor_anim_angry_badkey );
    }
    if(!strcmp("BORED",guess) )
    {
        voa_startAnimationLoop( vor_anim_bored );
    }
    if(!strcmp("WRIST",guess) )
    {
        voa_startAnimationLoop( vor_anim_wristcheck );
    }
    if(!strcmp("JUMPY",guess) )
    {
        voa_startAnimationLoop( vor_anim_fistpump );
    }
}

//segment VOR_CODE
extern char* vor_laser_anim_list[];
void vor_startLaserAnimation(unsigned char yellow);
// Look up the correct animation in void_laser_anim_list for the this->laserTargetCoords.
// Set yellow=1 for yellow, 0 for green.
// The green and yellow lists are in the same array; yellow is offset by 1.
// So for x==7, the green is at 7 but the yellow is at 6.
// This only works because the coordinates are known, and always two apart.
void vor_startLaserAnimation(unsigned char yellow)
{
    vo_robot *this=vor_singleton;
    unsigned char tilex;
    unsigned char centerx;
    unsigned char right;
    int ex,ey;
    int dx,dy,distsq;
    byteunion bu;
    int frames=0;

    bu.w=this->laserTargetCoords.w;
    if ( bu.w == 0 )
    {
        // nothing was in the queue
        return;
    }
    tilex=bu.lsb;
    if ( tilex > 0x12 )
    {
        right=8; // multiplier for array index
        centerx=126;
    }
    else
    {
        right=0;
        centerx=122;
    }
    ex=48+4*tilex;
    ey=32+8*bu.msb;
    dx=centerx-ex;
    dy=74-ey;
    distsq=(dx*dx)+(dy*dy);

    // distsq will be a number from 125 to 18824.
    // No need to actually take the square root,
    // we just need to know to take 1,2,3, or 4 frames.
    if ( distsq > 1225 ) ++frames;
    if ( distsq > 4761 ) ++frames;
    if ( distsq > 10609 ) ++frames;
    // that's actually going to be 0..3 but that's what
    // we need for an index anyway.

    // The array index is:
    //  0..3    green left
    //  4..7    yellow left
    //  8..11   green right
    // 12..15   yellow right
    yellow=yellow*4;

    voa_startAnimationLoop( *vor_laser_anim_list[ frames+yellow+right ]);
}

// qcpy and qcat should move to vo_baseA.asm
// and be used in vo_text.c as well
void qcpy(char *s)
{
    vo_robot *this=vor_singleton;
    strcpy(this->queue,s);
}

void qcat(char *s)
{
    vo_robot *this=vor_singleton;
    strcat(this->queue,s);
}


//segment VOR_CODE
char vor_queueMessage()
{
    vo_robot *this=vor_singleton;
    vo_base *base=vo_singleton;
    char argi = base->nextMessageN;
    const char *argp = base->nextMessageP;
    // ok, just going to print it really
    switch(base->nextMessageId)
    {
        case MSG_WELCOME: // 01
            qcpy("Welcome to ");
            qcat((char*)argp);
            // Startup animation: tap wrist, then go to standby
            voa_setLoopCountdown(5,vor_anim_alldone_wristcheck,&vor_blockAllMessages);
            voa_startAnimationLoop( vor_anim_laserwait );
            vor_blockAllMessages=1;
            return 1;
        case MSG_RESTART: // 03
            qcpy("Shall we play a game?");
            return 1;
        case MSG_PLEASE: // 04
            qcpy("Please what?");
            return 1;
        case MSG_COMMAND: // 05
            vor_handleCommand(argp);
            return 0;
        case MSG_PALETTE_CHANGE: // 06
            voa_setPalette(argi,VOA_NPAGES);
            return 0;
        case MSG_LOOKING_UP_WORD: // 10
            qcpy("Looking up: ");
            qcat((char*)argp);
            voa_startAnimationLoop( vor_anim_lookup_wristcheck ); // look up word on wrist computer
            return 1;
        case MSG_WORD_NOT_FOUND: // 11
            voa_startAnimationLoop( vor_anim_angry_notfound ); // angry
            qcpy("Word not found.");
            return 1;
        case MSG_ROWS_LEFT: // 18
            qcpy("Rows left: .");
            this->queue[11]='0'+(char)argi;
            voa_startAnimationLoop( vor_anim_alldone_wristcheck ); // tap wrist computer when done
            return 1;
        case MSG_WINNER: // 19
            qcpy("Well done!");
            voa_setLoopCountdown(3,vor_anim_alldone_wristcheck,NULL);
            voa_startAnimationLoop( vor_anim_victory );
            vor_amBored=200; // don't get bored while playing the song!
            return 1;
        case MSG_WORD_ALL_GREEN: // 16
            voa_startAnimationLoop( vor_anim_fistpump );
            return 0;
        case MSG_LOSER: // 1A - you lost QORDL
            // 28 0x1c uparrow, 29 0x1d downarrow
            qcpy(" Alas! Answers: ");
            qcat(((char**)argp)[0]);
            qcat(" ");
            qcat(((char**)argp)[1]);
            qcat(" ");
            qcat(((char**)argp)[2]);
            qcat(" ");
            qcat(((char**)argp)[3]);
            voa_startAnimationLoop( vor_anim_loss );
            return 1;
        case MSG_LOSER1: // 1B - you lost WORDL
            qcpy("Sorry, the answer is: ");
            qcat((char*)argp);
            return 1;
        case MSG_WAIT_FOR_KEY: // 21
            return 0;
        case MSG_CHECKING_LETTER: // 12
            vor_queueLaser((unsigned int)argp);
            return 0;
        case MSG_WAIT_FOR_BLOCK:
            // Do nothing, this is handled elsewhere
            return 0;
        case MSG_PLAYAGAIN: // 02
        case MSG_LETTER_IS_GREY: // 13
            vor_queueLaser(0); // dequeue the laser
            return 0;
        case MSG_LETTER_IS_YELLOW: // 14
            vor_blockAllMessages=1;
            vor_startLaserAnimation(1); // 1 means yellow
            return 0;
        case MSG_LETTER_IS_GREEN: // 15
            vor_blockAllMessages=1;
            vor_startLaserAnimation(0); // 0 means green
            return 0;
        case MSG_BAD_KEY_PRESSED: // 23
            voa_startAnimationLoop( vor_anim_angry_badkey ); // angry
            return 0;
        case MSG_GREEN_WORD_COUNT: // 17
            //voa_startAnimationLoop( vor_anim_standby ); // standby
            return 0;
        case MSG_KEY_ACCEPTED: // 24
            // finish whatever animation and return to standby
            voa_startAnimationLoop( vor_anim_standby ); // standby
            return 0;
        case MSG_KEY_PRESSED: // 22
        default:
            // Unrecognised message
//          strcpy(this->queue,"MSG: ...");
//          this->queue[5]='0'+(((base->nextMessageId)&0xf0)>>4);
//          this->queue[6]='0'+((base->nextMessageId)&0xf);
            return 0;
    }
}

//segment VOR_CODE
void vor_startProcessingMessage()
{
    char printing;
    vo_robot *this=vor_singleton;
    bzero(this->queue,40);
    // convert nextMessageId,N,P into a text string in queue
    printing = vor_queueMessage();
    if (printing)
    {
        this->x = 0;
        bzero(this->screenram+this->y*40,40);
        gotoxy(0,this->y);
    }
}

//segment VOR_CODE
void vor_handleYield()
{
    vo_robot *this=vor_singleton;
    char c = this->queue[this->x];

    // First, play a sound if there is one queued up
    // (can't play sounds in an interrupt context, so yield() has to do it!)
    // This REALLY needs to move to the interrupt context, or we need to call it a LOT more often.
    voa_playCue();

    // First, see if there is work to do on the current message
    // (this should be timed to print 1 letter per jiffy, instead of per yield)
    // (In fact, this shouldn't be in here at all, it will interfere with animation)
    if ( c != (char)0 )
    {
        //while ( c != (char)0 )
        // Print one letter!
        cputc(c);
        ++(this->x);
    }

    // If there is an animation queued, we want to wait for it to be started.
    if ( vor_blockAllMessages || voa_animLoopQueued() )
    {
        // Advance the animation queue
        voa_animate();
        return;
    }

    if ( c != (char)0 )
    {
        return;
    }

    // If there is not more work to be done on the current message,
    // start processing the next one
    if ( this->anim.base.nextMessageId != 0 )
    {
        vor_amBored=vor_boredTimer=0;

        vor_startProcessingMessage();
        vo_markMessageReceived();
        // Note that this sets nextMessage=0 when the new message STARTS PROCESSING.
        // It is not necessarily DONE.

        // Things that startProcessingMessage can have done:
        // 1. Queue a new text message in this->queue and this->x
        // 2. Shift animation to a new sequence
        // 3. Start a sound (this will move to the animation code later)
        // 4. Execute a command
    }
    else
    {
        vor_bored();
    }
    // Advance the animation queue
    voa_animate();
    voa_playCue();
}

void vor_bored()
{
    // vor_boredTimer is incremented every yield() that doesn't have a message,
    // and reset to 0 whenever there IS a message.
    // 255 is ABOUT two seconds.
    // So every twoseconds, increment the vor_amBored state.
    // States:
    // 0..4 (eleven seconds) not bored yet.
    // 5..7 (six seconds) tap foot
    // 8..12 (eleven seconds) do nothing
    // 13..14 (four seconds) tap wrist
    // 15..19 (eleven seconds) do nothing
    // 20..22 (six seconds) tap foot
    // then back to zero

    ++vor_boredTimer;
    if ( vor_boredTimer >= 254 )
    {
        vor_boredTimer=0;
        ++ vor_amBored;
        switch ( vor_amBored )
        {
            case 6: case 20:
                // tap foot
                voa_startAnimationLoop( vor_anim_bored );
                break;
            case 14:
                // tap wrist
                voa_startAnimationLoop( vor_anim_bored_wristcheck );
                break;
            case 23:
                vor_amBored=0;
            case 0: case 8: case 15:
                voa_startAnimationLoop( vor_anim_standby );
                break;
        }
    }
}

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
