/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Animation queuing code for vo_robot etc. */

#include "ds_screendriver.h"
#include "vo_anim.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>


//
// RUN-ONCE INITIALIZATION CODE
//
#pragma code-name (push,"ONCE")
#pragma rodata-name (push,"ONCE")
#pragma local-strings (on)

//segment ONCE
void __fastcall__ voa_initPages()
{
    char pageid;
    ds_page *p;
    vo_anim *this=voa_singleton;
    for(pageid=0;pageid<VOA_NPAGES;++pageid)
    {
        //cleanPage(pageid);
        //cleanPage is in a different bank, so just copy the code here
        p=this->pages[pageid];
        p->page.next=NOPAGE; // mark page as available for use
        p->page.screenState=DS_SCREENSTATE_INITIAL; // 3 INITIAL STATE
        p->page.queueState=VOA_QUEUESTATE_CLEANED; // 4 CLEANED
        p->page.spare[0]=0;
        p->page.spare[1]=0;
    }
}

//segment ONCE
void __fastcall__ voa_loadFirstPage(ds_page *firstpage)
{
    firstpage->page.queueState=VOA_QUEUESTATE_READY; // 1 LOADED AND READY TO QUEUE
    ds_init(firstpage);
    voa_currentPage=firstpage;
    // page flip to page
    // ds_flipToPageNextVBIandWait(PAGES[2]); REDUNDANT
}



#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

#pragma code-name (push,"VOR_VIEW")

//segment VOR_VIEW but is fine in VIEW
// Find a clean page if there is one.
// Return -1 if there isn't one.
// If there are any pages that are OFFSCREEN, then clean them.
int __fastcall__ voa_checkForCleanPage()
{
    vo_anim *this=voa_singleton;
    int pageid;
    int i;
    pageid=-1;
        // scan for screenState==OFFSCREEN, and set state to CLEANED
        for(i=0;i<VOA_NPAGES;++i)
        {
            // check page to see if it is OFF SCREEN (-1)
            if ( this->pages[i]->page.screenState==DS_SCREENSTATE_OFFSCREEN )
            {
                // should check to see if this page should be cached.
                // do some cleanup in the page, and set
                // screenState to 3 INITIAL and queueState to 4 CLEANED
                voa_cleanPage(i);
            }
        }
        // Look for a page that is in the CLEANED state
        for(i=0;pageid==-1 && i<VOA_NPAGES;++i)
        {
            if ( this->pages[i]->page.queueState==VOA_QUEUESTATE_CLEANED ) // 4 CLEANED
            {
                pageid=i;
            }
        }
    return pageid;
}

unsigned int animExitTo=0xffff;

//segment VOR_VIEW very very small - FIVE bytes, no need to port
// Return true if there is an animation loop queued in animExitTo
// (works fine in VIEW)
char __fastcall__ voa_animLoopQueued()
{
    return (animExitTo!=0xffff);
}

// TODO ran out of space in the VIEW segment. And in RAM.
#pragma code-name (pop)
#pragma code-name (push,"VOR_VIEW")

//segment VOR_VIEW TODO move to VIEW
char __fastcall__ voa_renderFrameAndEnqueuePage(unsigned char picid,unsigned char soundid)
{
    vo_anim *this=voa_singleton;
    int pageid;
    // Find the next available unused page
    // This may busy wait for quite a while
    pageid = voa_checkForCleanPage();
    if ( pageid >=0 )
    {

        this->pages[pageid]->page.queueState=VOA_QUEUESTATE_INITIALIZED; // mark page as being drawn - queueState 3 INITIALIZED
        this->pages[pageid]->page.spare[0]=picid; // Which pic is being drawn? Will be used by voa_renderFrameCB
        this->pages[pageid]->page.spare[1]=soundid; // Which sound should be played? 0 is none.
        // MAYBE that should be a frame index (which includes a pointer to the pic, and other info)
        voa_renderFrame(pageid);

        this->pages[pageid]->page.queueState=VOA_QUEUESTATE_DRAWN; // mark page as drawn - queueState 2 DRAWN

        // Queue up the page
        this->pages[pageid]->page.queueState=VOA_QUEUESTATE_READY; // queueState 1 LOADED AND READY TO QUEUE
        this->lastPageDrawn->page.next = this->pages[pageid];
        this->lastPageDrawn->page.queueState=VOA_QUEUESTATE_NEXTSET; // queueState 0 NEXT IS SET
        this->pages[pageid]->page.screenState=DS_SCREENSTATE_QUEUED; // screenState QUEUED
        // and prepare to draw the NEXT page by 
        // saving a pointer to the one we just drew
        this->lastPageDrawn = this->pages[pageid];
        return 1;
    }
    return 0;
}


#pragma code-name (pop)
#pragma code-name (push,"VOR_VIEW")
 
// src/main/vo_anim.c:184: Warning: Incomplete array 'voa_animFrameQueue[]' assumed to have one element
vo_anim_frame_node *voa_animFrameQueue[];
unsigned int voa_qcurr;

//segment VOR_VIEW but works fine in VIEW
vo_anim_frame_node *voa_frameQueuePeek()
{
    return &((*voa_animFrameQueue)[voa_qcurr]);
}

unsigned char animCountdown;
unsigned int animCountdownDest;
char *animCountdownCompleteFlag=NULL;

//segment VOR_VIEW but works fine in VIEW
void voa_setLoopCountdown(unsigned char loops, int dest,char *flag)
{
    animCountdownDest=dest;
    animCountdown=loops;
    animCountdownCompleteFlag=flag;
}

//segment VOR_VIEW but works fine in VIEW
void voa_frameQueueAdvance()
{
    // This needs to be smarter -- look at nextFrame, and see if
    // is a simple advance or a branch to a new loop.
    unsigned char flag = (*voa_animFrameQueue)[voa_qcurr].flag;
    unsigned int next;

    if ( flag & ANIM_FLAG_COUNTDOWN )
    {
        --animCountdown;
        if ( animCountdown==0 )
        {
            animExitTo = animCountdownDest;
            flag|=ANIM_FLAG_EXIT;
            if (animCountdownCompleteFlag!=NULL)
            {
                *animCountdownCompleteFlag=0;
            }
        }
    }

    if ( (flag & ANIM_FLAG_EXIT)  && animExitTo != 0xffff )
    {
        next=animExitTo;
        animExitTo=0xffff;
    }
    else
    {
        next = (*voa_animFrameQueue)[voa_qcurr].next;
    }
    voa_qcurr=next;
}

//segment VOR_VIEW but works fine in VIEW
// Process the next item in the animation feeder queue if possible
void __fastcall__ voa_animate()
{
    unsigned char nextFrame;
    unsigned char nextSound;
    vo_anim_frame_node *peek;
    unsigned char sfxflag;
    // Draw the next frame of animation, if possible
    // Peek at the animation queue, and attempt to render the next frame
    // (very likely to fail, when there is not an empty page)
    peek = voa_frameQueuePeek();
    sfxflag = ( ( (*voa_animFrameQueue)[voa_qcurr].flag ) & ANIM_FLAG_SFX_MASK );
    nextFrame = peek->picid;
    nextSound = peek->soundid;
    if ( SFX_LEVEL <= sfxflag )
    {
        // Thresholds:
        //      sfxflag         SFX_LEVEL (set by options menu)
        //      0 Music         1 Music Only
        //      1 Prompts       2 Key Click off
        //      2 Normal FX     3 Normal
        //      3 Annoying      4 Annoying

        // This frame's sfxflag is above the allowed threshold,
        // (i.e., user set 2 to disable normal fx, but this is an annoying sound)
        // and 2<3 so mute the sound
        // Default is normal SFX_LEVEL=3, but the annoying sound is level 3,
        // and 3==3 so mute the sound
        nextSound = 0;
    }
    if ( voa_renderFrameAndEnqueuePage(nextFrame,nextSound) )
    {
        // frame rendered successfully - advance the animation queue
        voa_frameQueueAdvance();
    }
}

#pragma code-name (pop)
