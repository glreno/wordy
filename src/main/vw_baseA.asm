; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Wordl View machine language parts

.include "zeropage.inc"
.include "include/trampoline.inc"

    .import incsp2
    .import incsp5
    .import ldaxysp
    .import popa
    .import popptr1
    .import pusha
    .import pushax
    .import staxptr1
    .import umul8x8r16m
    .import _ds_put2c
    .import _vfm_getGreenLetter

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


; Draw both halves of a tile
; void vw_drawTwoHalfTiles(char right,char left, char row, char col, vw_base *this)
trampoline _vw_drawTwoHalfTiles,_vw_drawTwoHalfTiles_tramp
    .proc vw_drawTwoHalfTiles

;   int ret = vw_getTileCoords(row,col,this);
;   byteunion *bu = &ret;
;   ds_put2c(right,left,bu->lsb,bu->msb);


    ; AX=this   Stack: right left row col <- TOP

    ; Call vw_getTileCoords(row,col,this)
    ; don't need row,col,this afterwards, so stack already set up for call
    JSR _vw_getTileCoords
    ; that returns x,y in registers A,X
    ; AX=x,y    STACK= right,left <- TOP

    ; call ds_put2c(right,left,x,y)
    JSR pusha
    TXA
    ; AX=y,y    STACK= right,left,x <- TOP
    LDX #0
    JSR _ds_put2c
    ; stack is now empty

    RTS

    .endproc

; Remove the letter from a tile. Used for backspace.
; void vw_clearTile(vw_base *this, char col, char row)
;   vw_drawTwoHalfTiles(0x40,0x60,row,col,this);

trampoline _vw_clearTile,_vw_clearTile_tramp
    .proc vw_clearTile

    ; this is just a matter of rearranging the stack a bit
    ; Now:   A:row   Stack: this,col
    ; Goal: AX:this  Stack: $40 $60 row col this
    TAX         ;X=row
    JSR popa
    PHA         ; systemstack: col
    TXA         ;A=row
    PHA
    ; Now:   Stack: this    systemstack: col row <-TOP
    JSR popptr1
    ; Stack is clear, this is in ptr1
    LDA #$60
    LDX #$40
    JSR pushax
    ;   Stack: $40 $60
    PLA
    TAX ; X=row
    PLA ; A=col
    JSR pushax
    ;   Stack: $40 $60 col row
    LDA ptr1
    LDX ptr1+1
    ; AX is this

    JMP _vw_drawTwoHalfTiles_tramp

    .endproc


; void vw_writeTilePart2(vw_base *this, char row, char col, char color, char left);
; A:left stack: this row col color <- TOP

trampoline _vw_writeTilePart2,_vw_writeTilePart2_tramp
    .proc vw_writeTilePart2

;   // If the color is GREY or YELLOW, then letter is an ASCII letter.
;   // We need to convert it to the Atari Internal code, by subtracting 32.
;   // And if it's yellow, add 128 to invert it.
;   // But if it's GREEN, then it's an ASCII number from 0-9; and the
;   // right half will be an ATASCII control code. And cputcxy() will NOT
;   // display ctrl-J or ctrl-M because CRLF. So we MUST print in internal codes.
;   // But the rule for numbers is still to subtract 32.

;   left -= 0x20;
;   if ( color == VW_YELLOW )
;   {
;       left |=  0x80;
;   }

    SEC
    SBC #$20
    PHA
    ; left-$20 on the system stack

    JSR popa
    CMP #1 ; VW_YELLOW
    BNE notyellow

yellow:
    PLA
    ORA #$80
    PHA

notyellow:
    ; At this point, we have calculated LEFT and it is on the system stack.
    ; Stack: this row col <- TOP

;   // Right-side char is the lower case.
;   // In Atari Internal code, you get lower case by adding 0x40.
;   // That works for the numbers I'm using for green, too.
;   right = left + 0x40;
;   vw_drawTwoHalfTiles(right,left,row,col,this);

    ; First thing we're going to do is read 'this' from the stack
    ; and stash it in ptr1
    LDY #3
    JSR ldaxysp
    JSR staxptr1

    ; There are two copies of this on the stack, but one of them is
    ; a convenient pair of bytes we can overwrite to be 'right' and 'left'
    ; ptr1:this Stack: this row col <- TOP   systemstack:left

    PLA
    LDY #2
    STA (sp),Y

    INY
    CLC
    ADC #$40
    STA (sp),Y

    ; fetch this from ptr1, need in AX for the call
    LDA ptr1
    LDX ptr1+1

    JMP _vw_drawTwoHalfTiles_tramp

    .endproc


    .segment "APP"
    ; Segment APP because vfm_getGreenLetter will bankswitch

; Write a letter to a tile.
;
; void vw_writeTile(vw_base *this, char row, char col, char color, char letter)
;
_vw_writeTile:
    .export _vw_writeTile
    .proc vw_writeTile
;   // If the color is GREEN, then we need to find the right
;   // char for this fontmanager. This might have to allocate one!
;   // If it does, it will bankswitch to the font
;   if ( color == VW_GREEN )
;       // This will return a char from '0' to '9'.
;       left = vfm_getGreenLetter(this->fontmanager,letter);

    ; not MUCH different from the C code.
    ; Check color; if it is green, make the call
    PHA ; stash letter, which is in A
    LDY #0
    LDA (sp),Y
    CMP #2 ; VW_GREEN
    BNE notgreen

green:

;       left = vfm_getGreenLetter(this->fontmanager,letter);
    ; Get this from the stack, (offsets 3&4),
    ; store it in ptr1, and get this->fontmgr (offset 3,4)
    LDY #4
    JSR ldaxysp
    JSR staxptr1
    LDY #4
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y

    ; push fontmgr
    JSR pushax

    PLA ; get the letter from the system stack
    ; make the call
    JSR _vfm_getGreenLetter

    ; return is in A, push it on the system stack very temporarily
    PHA

notgreen:

    PLA ; get the letter from the system stack

;   The stack is already set up -- params are in the same order
;   vw_writeTilePart2(this,row,col,color,left);

    JMP _vw_writeTilePart2

    .endproc
