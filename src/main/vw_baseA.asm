; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Wordl View machine language parts

.include "zeropage.inc"

    .import incsp2
    .import incsp5
    .import staxptr1
    .import umul8x8r16m

    .segment "GAMEINIT"
; Initialize the data structure
; void vw_initialize( vfm_fontmanager *fontmanager, char nrows, char y, char x, vw_base *this)
;   this->x=x; // offset 0
;   this->y=y; // offset 1
;   this->nrows=nrows; // offset 2
;   this->fontmanager = fontmanager; // offset 5,6
_vw_initialize:
    .export _vw_initialize
    .proc vw_initialize
    JSR staxptr1
    ; Copying the params to the data structure is a lot easier
    ; since they are in the same order
    LDY #5
    JSR _copyStackToPtr1
    JMP incsp5
    .endproc

; This is still in GAMEINIT but should move to CODE
; if anyone needs it for anything BUT initialization.
_copyStackToPtr1:
    .export _copyStackToPtr1
    .proc copyStackToPtr1
loop:
    LDA (sp),Y
    STA (ptr1),Y
    DEY
    BPL loop
    RTS
    .endproc


; APP segment because called by ONCE and GAMEINIT and VO to spotlight tiles
    .segment "APP"

; Find the Gr.0 x,y coordinates of a given tile
; unsigned int __fastcall__ vw_getTileCoords(char row, char col, vw_base *this);
; 'this' is in AX. row and col are on the stack.
; Return is X coord in A, Y coord in X, just to be confusing.
_vw_getTileCoords:
    .proc vw_getTileCoords
    .export _vw_getTileCoords

    ; USING ptr2 BECAUSE umul8x8 USES ptr1 AND ptr3 !
    STA ptr2
    STX ptr2+1

    ; C code:
;    byteunion bu;
;    char thisy=this->y;        // offset 1
;    char thisyoff=this->yoff;  // offset 6
;    char thisysz=this->ysz;    // offset 5
;    char *thisxoff=this->xoff; // offset 7
;    bu.lsb = thisxoff[col];    // offset 7+col, store in A to return
;    bu.msb = thisy+thisyoff+thisysz*row;
;    return bu.w;

    ; Start with the X coordinate - col is first item on the stack
    LDY #0
    LDA (sp),Y
    CLC
    ADC #7
    TAY
    LDA (ptr2),Y
    PHA ; sitting on the system stack, waiting for PLA for return

    ; The Y coord is just a bunch of ptr2 reads -- except for
    ; the ysz*row part. Row is (sp),1 and ysz is (ptr2),5
    ; Multiply that, keep just the LSB in A, and then it's just
    ; a matter of adding things.

    ; umul8x8r16m: multiply two unsigned 8 bit numbers, ptr1*ptr3 -> ptr1
    LDY #1
    LDA (sp),Y          ; param col
    STA ptr1
    LDY #5
    LDA (ptr2),Y        ; this->ysz
    STA ptr3
    JSR umul8x8r16m
    LDY #6
    CLC
    ADC (ptr2),Y        ; this->yoff
    LDY #1
    CLC
    ADC (ptr2),Y        ; this->y
    ; Result is in A, need to return it in X
    TAX

    PLA ; stashed this earlier
    JMP incsp2
    .endproc

