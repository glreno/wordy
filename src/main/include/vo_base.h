/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* vo_base.h
 *
 * Declarations for opponent view communications
 *
 */

#ifndef __VO_BASE_H__
#define __VO_BASE_H__

/* ######################################################### */
/* #            STANDARD MESSAGE DEFINITIONS               # */
/* ######################################################### */

#define NO_MESSAGE_QUEUED       0x00
#define MSG_WELCOME             0x01
#define MSG_PLAYAGAIN           0x02
#define MSG_RESTART             0x03
#define MSG_PLEASE              0x04
#define MSG_COMMAND             0x05
#define MSG_PALETTE_CHANGE      0x06

#define MSG_LOOKING_UP_WORD     0x10
#define MSG_WORD_NOT_FOUND      0x11
#define MSG_CHECKING_LETTER     0x12
#define MSG_LETTER_IS_GREY      0x13
#define MSG_LETTER_IS_YELLOW    0x14
#define MSG_LETTER_IS_GREEN     0x15
#define MSG_WORD_ALL_GREEN      0x16
#define MSG_GREEN_WORD_COUNT    0x17
#define MSG_ROWS_LEFT           0x18
#define MSG_WINNER              0x19
#define MSG_LOSER               0x1A
#define MSG_LOSER1              0x1B

#define MSG_WAIT_FOR_BLOCK      0x20
#define MSG_WAIT_FOR_KEY        0x21
#define MSG_KEY_PRESSED         0x22
#define MSG_BAD_KEY_PRESSED     0x23
#define MSG_KEY_ACCEPTED        0x24


/* ######################################################### */
/* #                      VO_BASE                          # */
/* ######################################################### */

typedef struct vo_base_struct {
    char nextMessageId; // 0 means no message, or that the message has been received
    char nextMessageN; // offset 1
    const char *nextMessageP; // offset 2,3
} vo_base;

extern vo_base *vo_singleton;
#pragma zpsym ("vo_singleton");  // flags this as zeropage

void __fastcall__ vo_initialize(vo_base *this);

// Register a function to be the seconday thread
// func should be address of a function that takes no params and returns void
void __fastcall__ vo_setYieldFunc(void *func);

// Store msgid in nextMessage, args in nextMessageArgs
// Note that this args pointer will be copied and used later, so it can't be a local buffer!
// The yield() handler will display the message, and set msgid==0.
void __fastcall__ vo_messagep(char msgid, char argi, const char *argp);
void __cdecl__ vo_message(char msgid, char argi);

// Subclass should call this after it's done reading
// everything from the nextMesssage fields, allowing
// the other thread to queue up another one.
void __fastcall__ vo_markMessageReceived();

// Return true if msgid==0
char __fastcall__ vo_isMessageReceived();

// Convenience routine for those times when you have nothing left to do
void __fastcall__ vo_waitUntilMessageReceived();


#endif
