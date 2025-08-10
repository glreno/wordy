; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Base class for opponent views

    .include "../main/include/ds_screendriverA.inc"
    .include "include/trampoline.inc"
    .importzp sp
    .import booleq
    .import incsp2
    .import pushax
    .import _yield
    .import _setYieldFunc
    .import _bankswitch

;
; STRUCT OFFSETS
;
    VO_NEXTMESSAGEID = 0 ; byte
    VO_NEXTMESSAGEN = 1  ; byte
    VO_NEXTMESSAGEP = 2  ; char*

    _VO_YIELDBANKID = YIELDBANK+1
    .export _VO_YIELDBANKID
    _VO_YIELDVEC = YIELDJMP+1
    .export _VO_YIELDVEC

    .ZEROPAGE
_vo_singleton:  .word 0
    .EXPORTZP _vo_singleton

    .DATA

; Call the secondary thread
; This function is in .DATA so that the address can be changed
; void __fastcall__ vo_yield();
_vo_yield:
    .export _vo_yield
YIELDBANK:
    LDA #0
    JSR _bankswitch
YIELDJMP:
    JMP voyieldreturn ; _VO_YIELDVEC points to this address



    .SEGMENT "ONCE"

; Register a function to be the seconday thread
; void __fastcall__ setYieldFunc(void *func);
; func should be address of a function that takes no params and returns void

_vo_setYieldFunc:
    .export _vo_setYieldFunc
    STA _VO_YIELDVEC
    STX _VO_YIELDVEC+1
voyieldreturn:
    ; _VO_YIELDVEC points here by default
    RTS

    .SEGMENT "ONCE"

; void __fastcall__ vo_initialize(vo_base *this)
_vo_initialize:
    .proc vo_initialize
    .export _vo_initialize

    ; Load the singleton's global zeropage pointer
    STA _vo_singleton
    STX _vo_singleton+1

    ; Register the _vo_yield function
    LDA #<_vo_yield
    LDX #>_vo_yield
    JSR _setYieldFunc

    ; and continue into vo_markMessageReceived
    ; to clear the structure
    JMP _vo_markMessageReceived
    .endproc

; VIEW segment, probably called by VO bank, maybe could be trampolined
; (ten bytes long, trampoline wrapper is 9 bytes)
    .segment "VIEW"

; Subclass should call this after it's done reading
; everything from the nextMesssage fields, allowing
; the other thread to queue up another one.
; void __fastcall__ vo_markMessageReceived()
_vo_markMessageReceived:
    .proc vo_markMessageReceived
    .export _vo_markMessageReceived
    LDA #0
    LDY #3
loop:
    STA (_vo_singleton),Y
    DEY
    BPL loop
    RTS
    .endproc

; VIEW segment, too small to trampoline
    .segment "VIEW"

; Return true if msgid==0
; char __fastcall__ vo_isMessageReceived()
;   return (vo_singleton->nextMessageId == 0);
_vo_isMessageReceived:
    .proc vo_isMessageReceived
    .export _vo_isMessageReceived
    LDY #VO_NEXTMESSAGEID
    LDA (_vo_singleton),Y
    JMP booleq
    .endproc

; VIEW segment, busywaiting, too small to trampoline, calls _yield
    .segment "VIEW"

; Convenience routine for those times when you have nothing left to do
; void __fastcall__ vo_waitUntilMessageReceived()
_vo_waitUntilMessageReceived:
    .proc vo_waitUntilMessageReceived
    .export _vo_waitUntilMessageReceived
loop:
    JSR _yield
    LDY #VO_NEXTMESSAGEID
    LDA (_vo_singleton),Y
    ; if that was a zero, return true
    BNE loop
    JMP booleq
    .endproc

; Store msgid in nextMessage, args in nextMessageArgs
; Note that this args pointer will be copied and used later, so it can't be a local buffer!
; The yield() handler will display the message, and set msgid==0.
; void __fastcall__ vo_messagep(char msgid, char argi, const char *argp);
; void __cdecl__ vo_message(char msgid, char argi);
; argp will be in AX; then the stack contains argi and msgid
; vo_message is cdecl to keep those two args on the stack
trampoline _vo_messagep,_vo_messagep_tramp
    LDY #2 ; 2 = VO_NEXTMESSAGEP
    STA (_vo_singleton),Y
    INY
    TXA
    STA (_vo_singleton),Y
    JMP _vo_message_tramp

trampoline _vo_message,_vo_message_tramp
    LDY #1 ; msgid
    LDA (sp),Y
    DEY ; 0 = VO_NEXTMESSAGEID
    STA (_vo_singleton),Y

    ; Y is 0, where argi is
    LDA (sp),Y
    INY ; 1 = VO_NEXTMESSAGEN
    STA (_vo_singleton),Y

    JMP incsp2

