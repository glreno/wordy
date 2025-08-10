/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vo_anim.h
 *
 * Declarations for opponent view animation
 *
 */

#include "vo_base.h"
#include "ds_screendriver.h"
#include "da_audioDriver.h"

#ifndef __VO_ANIM_H__
#define __VO_ANIM_H__

/* ######################################################### */
/* #         ANIMATION QUEUE STATE DEFINITIONS             # */
/* ######################################################### */

#define VOA_QUEUESTATE_CLEANED 4
#define VOA_QUEUESTATE_INITIALIZED 3
#define VOA_QUEUESTATE_DRAWN 2
#define VOA_QUEUESTATE_READY 1
#define VOA_QUEUESTATE_NEXTSET 0


/* ######################################################### */
/* #                      VO_ANIM                          # */
/* ######################################################### */

// Assmbly code timer handler, and stuff it needs
#define NOPAGE ((void *)0xffff)

// Current page on screen; probably should be in the struct?
extern ds_page *voa_currentPage;

#define VOA_NPAGES 3

typedef struct vo_anim_struct {
    vo_base base;               // offset 4,5,6,7 (four bytes)
    ds_page *lastPageDrawn;     // offset 8,9 (two bytes)
    ds_page *pages[VOA_NPAGES]; // offset A,B,C,D,E,F (npages*2 bytes)
} vo_anim;

typedef struct vo_anim_frame_node_struct {
    unsigned int next; // probably an index into a huge list of these things
    unsigned char flag; // 1=exit allowed here
    unsigned char picid; // ID of the pic to draw, maybe should be an int
    unsigned char soundid; // ID of the sound to play
} vo_anim_frame_node;

typedef struct vo_anim_pic_struct {
    void *topPic1;  // offset 0
    void *topPic2;  // offset 2
    void *botPic1;  // offset 4
    void *botPic2;  // offset 6
    char cue;     // offset 8     - cue to play, if not 0
    char param;     // offset 9     - value to return from voa_drawPic()
} vo_anim_pic;

//vo_anim *voa_singleton; // Same zero page ptr as vo_singleton.
#define voa_singleton ((vo_anim *)vo_singleton)

void __fastcall__ voa_loadFirstPage(ds_page *firstpage);

void __fastcall__ voa_initPages();


char __fastcall__ voa_renderFrameAndEnqueuePage(unsigned char picid,unsigned char soundid);

// Draw an animation frame into a Page.
// The ID for the animation is stored in page.spare[0]
void __fastcall__ voa_renderFrame(char pageid);

// Select the palette, because the colors I picked suck
void __cdecl__ voa_setPalette(char p,char npages);

// Reset a page to the INTIAL/CLEAN state
void __fastcall__ voa_cleanPage(char pageid);

// Find a clean page if there is one.
// Return -1 if there isn't one.
// If there are any pages that are OFFSCREEN, then clean them.
int __fastcall__ voa_checkForCleanPage();

// Request that the animation move to a new loop at the next exit
void __fastcall__ voa_startAnimationLoop(unsigned int nextLoop);

// Process the next item in the animation feeder queue if possible
void __fastcall__ voa_animate();

// Process the next item in the audio cue queue, if there.
// This calls da_playCue(&cues[voa_cueQueue]).
// Call this frequently! It needs to be called as soon as possible
// after the VBI completes. You can't predict that, so
// just call it a LOT. Cooperative multitasking FTW!
void __fastcall__ voa_playCue();

// This is the hardcoded table of cues to be played.
// You need to link in a .o that contains cueball.
extern da_cue cueball[];

// This is the one-item queue that contains the index to play.
extern unsigned char voa_cueQueue;



// Return true if there is an animation loop queued in animExitTo
char __fastcall__ voa_animLoopQueued();

// Draw a vo_anim_pic to the given pair of fonts
int __cdecl__ voa_drawPic(unsigned char fontbaseBot, unsigned char fontbaseTop,unsigned char picid);

// Set a loop counter and destination for "exit":"counter" loops
// If flag is not null, *flag will be set to zero after exit.
void voa_setLoopCountdown(unsigned char loops, int dest, char* flag);

#endif
