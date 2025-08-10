; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Wordl Model machine language parts

.include "zeropage.inc"
.include "include/trampoline.inc"

    .import incsp1
    .import incsp4
    .import pusha
    .import pushax
    .import pushc0
    .import popa
    .import popax
    .import popptr1
    .import staxptr1
    .import _vw_initialize


; Did I win? Return 1 if all five letters are green.
; char mw_isWon(mw *model)
;   for(i=0;i<5;++i)
;       if ( model->currColors[i] != VW_GREEN )
;           return 0;
;   return 1;

    .segment "APP"
trampoline _mw_isWon,_mw_isWon_tramp
        .proc mw_isWon
        ; store mw address in zeropage pointer
        stx ptr1+1  ; STAXPTR1 and INCPTR1 by 18
        clc
        adc #18 ; to LSB of addr, add struct offset to currcolors
        sta ptr1
        bcc nocarry
        inc ptr1+1
nocarry:
        ldx #0 ; we won't need X, other to return MSB 0
        ldy #4
        lda #2 ; VW_GREEN
loop:
        cmp (ptr1),y
        bne fail
        dey
        bpl loop
        lda #1 ; Everything in the loop was green!
        rts
fail:
        lda #0
        rts
        .endproc

    .segment "GAMEINIT"
; segment GAMEINIT - called by mw_nextRow(runtime) and mw_initialize(initialization) so it probably SHOULD be in APP
; void mw_resetForNextRow(char y,mw *model)
;   bzero(model->lastGuess,6); // offset 6
;   bzero(model->currGuess,6); // offset 12 (these two are next to each other)
;   memset(model->currColors,0xff,5); // offset 18
;   model->cur_y=y; // offset 24
;   model->cur_x=0;  // offset 25
_mw_resetForNextRow:
    .export _mw_resetForNextRow
    .proc mw_resetForNextRow

    JSR staxptr1
    JSR popa
    PHA ; move that param to the other stack

;   model->cur_x=0;  // offset 25
    LDY #25
    LDA #0
    STA (ptr1),Y
;   model->cur_y=y; // offset 24
    DEY
    PLA
    STA (ptr1),Y

;   memset(model->currColors,0xff,5); // offset 18
    LDX #5-1
    LDY #22
    LDA #$FF
loop1:
    STA (ptr1),Y
    DEY
    DEX
    BPL loop1

;   bzero(model->lastGuess,6); // offset 6
;   bzero(model->currGuess,6); // offset 12
; (these two are next to each other, and right before currColors)

    LDX #12-1
    LDA #0
loop2:
    STA (ptr1),Y
    DEY
    DEX
    BPL loop2

    RTS

    .endproc

    .segment "GAMEINIT"
; segment GAMEINIT - only so it can call mw_resetForNextRow - move that, and this can be trampolined
;  Advance to next row.
; scoring is done, prepare for next guess
; do not advance if isWon already
; advance to next row, first tile
; copy currGuess to lastGuess, clear currGuess and currColors
; return number of rows remaining, -1 means the game is won, 0 means you just lost.
; char mw_nextRow(mw *model)
_mw_nextRow:
    .export _mw_nextRow
    .proc mw_nextRow

    ; mw_isWon uses the same AX param, but we need it twice later.
    JSR pushax ; ptr for reading cur_y
    JSR pushax ; ptr for the return

    ; model pointer is still in AX, so call mw_isWon
    JSR _mw_isWon
    ; This is a very rare case of calling a trampoline from segment gameinit
;   if ( mw_isWon(model) )
;       return -1;
    BEQ notwon ; it's irritating that C 'false' is BEQ
    LDA #$FF
    TAX
    JMP incsp4 ; didn't need that pointer after all
notwon:

;   y = model->cur_y; // offset 24
;   ++y;

    JSR popptr1 ; that pointer we pushed earlier
    LDY #24 ; cur_y
    LDA (ptr1),Y
    CLC
    ADC #1
    PHA ; cur_y+1 for the return
    JSR pusha ; cur_y for param

;   mw_resetForNextRow(model,y);
    LDA ptr1    ; model ptr for param
    LDX ptr1+1
    JSR _mw_resetForNextRow

;   return model->nrows - y; // nrows is offset 23
    PLA ; that cur_y+1 we pushed earlier
    STA tmp1

    ; stack: model
    JSR popptr1 ; that pointer we pushed earlier
    LDY #23 ; nrows
    LDA (ptr1),Y
    SEC
    SBC tmp1
    LDX #0
    RTS
    .endproc

    .segment "GAMEINIT"
; Create (and return) a wordl view. Advance to row 0.
; Note that this calls vw_initialize, but NOT vws_initialize or vwt_initialize! You have to do that!
;vw_base *mw_initialize(vfm_fontmanager *fontmanager,char nrows,char y,char x,mw *model)
;   model->nrows=nrows;
;   vw_initialize(fontmanager,nrows,y,x,&(model->view));
;   mw_resetForNextRow(0,model);
;   return &(model->view);
_mw_initialize:
    .export _mw_initialize
    .proc mw_initialize

    JSR staxptr1
    STA ptr2
    STX ptr2+1

;   model->nrows=nrows; byte 3 on the stack, byte 23 in model
    LDY #2
    LDA (sp),Y
    LDY #23
    STA (ptr1),Y

;   find model->view; it's offset 26 in model, NOT a pointer
    CLC
    LDA #26
    ADC ptr2
    STA ptr2
    LDA #0
    ADC ptr2+1
    STA ptr2+1

    ; ptr1 is model, ptr2 is model->view
    ; preserve them on the system stack
    ; and load model->view into AX for the call to vw_initialize
    ; ptr2 (model->view) for the return
    LDA ptr2
    PHA
    LDA ptr2+1
    PHA
    TAX
    ; ptr1 (model) for mw_resetForNextRow
    LDA ptr1
    PHA
    LDA ptr1+1
    PHA
    ; ptr2 for the call to vw_initialize
    LDA ptr2
    ;LDX ptr2+1

;   vw_initialize(fontmanager,nrows,y,x,&(model->view));
    JSR _vw_initialize
    ; That will have wiped out all the the params from the stack
    JSR pushc0
    PLA
    TAX
    PLA
;   mw_resetForNextRow(0,model);
    JSR mw_resetForNextRow

;   return &(model->view);
    PLA
    TAX
    PLA
    RTS

    .endproc
