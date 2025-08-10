/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* title.h
 *
 * Declarations for title-screen functions
 *
 * The title screen is hardcoded in the cartridge in bank 6.
 * (this is also where the DATA segment is)
 * It will be automatically shown when on cartridge boot.
 * The first thing main() should do is all of the application
 * setup -- and while it's doing that occasionally call 
 * title_show_license_on_L(void). Once loading is done, call
 * title_show_press_a_key(void) and then cgetc().
 * Once a key is pressed, randomize, and page-flip to a new screen.
 *
 * These functions are ONLY available when the correct bank is loaded!
 */

#ifndef __TITLE_H__
#define __TITLE_H__

#include <stdlib.h>

/* ######################################################### */
/* #                  TITLE SCREEN                         # */
/* ######################################################### */

extern char COPYRIGHT_NOTICE[];

// Automatically called on cart boot. You never need this.
// This page-flips the hardcoded screen into place. The previous
// display list is stored in the previous display RAM at *OS.savmsc,
// you can draw in the display RAM and then restore the screen
// afterwards.
void __fastcall__ title_show_title_screen(void);

// Show the license screen.
void __fastcall__ title_show_license_screen(void);

// Show the credits screen.
void __fastcall__ title_show_credits_screen(void);

// Show the instructions screen.
void __fastcall__ title_show_instruction_screen(void);

// Show the license screen if the L key has been pressed.
// Call this occasionally while loading stuff.
char __fastcall__ title_show_license_on_L(void);

// Enable the "Press a key to begin" message on the title & license screens.
void __fastcall__ title_show_press_a_key(void);

// Wait for a keypress. Show the license if L is pressed (and wait again)
// (don't show license again if it's already on screen)
void __fastcall__ title_wait_for_key(char shown);

// For the disk version, you can wipe out the 'LOADING...' line since it's in RAM
void __fastcall__ title_erase_loading_msg(void);

#endif

