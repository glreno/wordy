/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* da_audioDriver.h
 *
 * Declarations for the audio driver
 *
 */

#ifndef __DA_AUDIODRIVER_H__
#define __DA_AUDIODRIVER_H__

#include <stdlib.h>

/* ######################################################### */
/* #             STANDARD SOUND ENVELOPES                  # */
/* ######################################################### */

// Always available
extern char AE_KEYCLICK[];
extern char AE_SHUT_UP[];

// da_envelopes.o:

//
// THIRTYSECONDTH NOTES
//

extern char AE_32_SIMPLE[];
extern char AE_32_ABRUPT[]; // 32d note that cuts off more abruptly


//
// SIXTEENTH NOTES
//

extern char AE_16_SIMPLE[];
extern char AE_32REST_16_SIMPLE[]; // same, with a 32dth rest before

extern char AE_16_TRAILING[]; // 16the note with quarter-note-long release
extern char AE_32REST_16_TRAILING[]; // same, with a 32dth rest before
extern char AE_16REST_16_TRAILING[]; // same, with a 32dth rest before


/* ######################################################### */
/* #                  AUDIO DRIVER                         # */
/* ######################################################### */

// Mute flag. Set to 1 to shut up.
extern char DA_QUIET;

// Play a note.
//
// Valid voices are 1,2,3,4 (to match what the OS calls them)
// envelope is a list of envelope commands, ending in 0,0
// A envelope command is two byte: and AUDC value, and a number of cycles to delay before the next command.
// The delay is a number of cycles of pitch 31 (B5).
// Delay 1 is 'next cycle'; Delay 0 is 255 cycles!
//  Whole Note:    3200
//  Half Note:     1600
//  Quarter Note:   800
//  Eighth Note:    400
//  Sixteenth Note: 200
// Use audf2,audf3,audf4 to change pitch.
// Voice 1 is a timer. Do NOT change audf1 from 31.

void __fastcall__ da_playNote(char voice, const char envelope[]);

// Play a note, and wait for it to finish
void __fastcall__ da_playNoteAndWait(char voice, const char envelope[]);

// Is the voice still doing something?
char __fastcall__ da_isVoiceBusy(char voice);

// Convenience call for da_playNote(1, DA_ENV_KEYCLICK)
void __fastcall__ da_keyClick(void);

typedef struct da_cueStruct {
    unsigned char freq[3]; // offsets   0,1,2
    const char *env[4];    // offsets 3,5,7,9
} da_cue;

// Play up to four notes at once
void __fastcall__ da_playCue( da_cue *cue );

#endif
