; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Used Letter View

.include "atari.inc"
.include "zeropage.inc"
.include "include/trampoline.inc"

.import _INTERACT_FONT
.import _vu_doDrawLine
.import incaxy
.import incsp1
.import incsp2
.import popptr1
.import pushax
.import ldax0sp
.import staxptr1

    .BSS
_vu_flags:
    .export _vu_flags ; only exported while optimizing
    ; one byte for each letter. Set to one when it gets colored in
    .byte 0,0,0,0,0
    .byte 0,0,0,0,0
    .byte 0,0,0,0,0
    .byte 0,0,0,0,0
    .byte 0,0,0,0,0
    .byte 0

;
;
; INITIALIZATION CODE
;
;
; Draws a line of letters.


    .segment "GAMEINIT"

; no data structure needed, just a screen address
; Address is to 5 lines of ANTIC mode E graphics
; Letters will be 5x5, each letter taking 6 pixels or 1.5 bytes; 26*1.5=39 bytes
; White(bits 11): Unused
; Grey (bits 01): Used, and not in solution
; Green (bit 10): Used, part of solution (green OR yellow in any puzzle)

; Initialize - draw all the letters on the screen
; void __fastcall__ vu_initialize(address);
; GLOBAL USAGE: ptr1 is the dest address for each letter
; GLOBAL USAGE: tmp4 is the counter
_vu_initialize:
    .export _vu_initialize
    .proc vu_initialize
    ; we do need the pointer to be on the stack for future reference
    JSR pushax
    JSR staxptr1

    ; Zero out all five lines (200 bytes)
    ; this isn't needed since you already cleared screen RAM (right?)
    ; and a re-initialize will OR over the existing grey/green letters.
    LDA #0
;   LDY #199
;clearloop:
;   STA (ptr1),Y
;   DEY
;   BNE clearloop
;   STA (ptr1),Y

    ; Zero out all 26 _vu_flags
    LDY #25
flagloop:
    STA _vu_flags,Y
    DEY
    BPL flagloop

    ; For each letter A-Z, draw the letter based on the font at _INTERACT_FONT
    ; Difficulty: Each six pixels is one and a half bytes.

    ; Counter in tmp4 -- can move to stack later
    LDA #1
    STA tmp4
letterloop:
    LDA tmp4
    AND #1
    STA tmp3 ; set the 'odd' flag

    ; calculate the address of the letter, push onto the stack
    LDA tmp4
    JSR _vu_getByteOffset ; A is now the byte offset into the line
    ; add the offset to the address that is on the stack; store in ptr1
    LDY #0
    CLC
    ADC (sp),Y
    STA ptr1
    INY
    LDA #0 ; so we can add the carry bit
    ADC (sp),Y
    STA ptr1+1

    ; calculate the font offset, store address in ax
    ; The address is INTERACT_FONT + (tmp4-1) *8 + 2
    ; Note that 25*8=200, well within one byte; and tmp4 starts at 1
    LDA tmp4
    SEC
    SBC #1
    ASL
    ASL
    ASL
    CLC
    ADC #2 ; can't carry
    LDX #>_INTERACT_FONT
    LDY #<_INTERACT_FONT
    JSR incaxy

    ; first thing drawLetter is going to do is store ax in ptr2;
    ; no need to do it here since it's not going to be reused

    ; draw the letter - dest on stack, font in ax, even/odd flag in tmp3
    JSR _vu_drawLetter

    ; increment letter counter, and loop if < 27
    INC tmp4
    LDA #27
    CMP tmp4
    BNE letterloop

    JMP incsp2
    .endproc

; unsigned char __fastcall__ vu_getBitsC0(unsigned char n)
; return ((n>>6)&3);
_vu_getBitsC0:
    .export _vu_getBitsC0
    LSR
    LSR
; unsigned char __fastcall__ vu_getBits30(unsigned char n)
; return ((n>>4)&3);
_vu_getBits30:
    .export _vu_getBits30
    LSR
    LSR
; unsigned char __fastcall__ vu_getBits0C(unsigned char n)
; return ((n>>2)&3);
_vu_getBits0C:
    .export _vu_getBits0C
    LSR
    LSR
; unsigned char __fastcall__ vu_getBits03(unsigned char n)
; return ((n>>0)&3);
_vu_getBits03:
    .export _vu_getBits03
    AND #3
    RTS

_vu_doubler:
    .export _vu_doubler
.byte $0,$3,$C,$F


_vu_writeHighBits:
    .export _vu_writeHighBits
; void __fastcall__ vu_writeHighBits(char *dest,unsigned char c)
;    *dest|=(vu_doubler[c]<<4);
; c is in A, dest is in ptr1 (not the stack where C wants it)
; GLOBAL USAGE: ptr1 used as param dest
    TAY
    LDA _vu_doubler,Y
    ASL A
    ASL A
    ASL A
    ASL A
    JMP _vu_writeBitsCommon

_vu_writeLowBits:
    .export _vu_writeLowBits
; void __fastcall__ vu_writeLowBits(char *dest,unsigned char c)
;    *dest|=vu_doubler[c];
; c is in A, dest is in ptr1 (not the stack where C wants it)
; GLOBAL USAGE: ptr1 used as param dest
    TAY
    LDA _vu_doubler,Y
_vu_writeBitsCommon:
    .export _vu_writeBitsCommon ; only exported while optimizing
    LDY #0
    ORA (ptr1),Y
    STA (ptr1),Y
    RTS

; void vu_drawLeftSideOdd(char *dest, char f)
; C0 to Low
; f is in A; dest is on stack
; GLOBAL USAGE: ptr1 used as param dest
_vu_drawLeftSideOdd:
    .proc vu_drawLeftSideOdd
    .export _vu_drawLeftSideOdd
    JSR _vu_getBitsC0 ; pull bits C0 from A
    JMP _vu_writeLowBits ; dest in ptr1, value in A
    ; clears stack and returns
    .endproc

; void vu_drawRightSideOdd(char *dest, char f)
; 30 to High ; 0C to Low
; f is in A; dest is on stack
; GLOBAL USAGE: ptr1 used as param dest
_vu_drawRightSideOdd:
    .proc vu_drawRightSideOdd
    .export _vu_drawRightSideOdd
    PHA ; push A onto the stack, since there's two calls to make
    JSR _vu_getBits30 ; pull bits 30 from A
    JSR _vu_writeHighBits ; dest in ptr1, value in A
    PLA
    JSR _vu_getBits0C ; pull bits 0C from A
    JMP _vu_writeLowBits ; dest in ptr1, value in A
    ; clears stack and returns
    .endproc

; void vu_drawLeftSideEven(char *dest, char f)
; C0 to High ; 30 to Low
; f is in A; dest is on stack
; GLOBAL USAGE: ptr1 used as param dest
_vu_drawLeftSideEven:
    .proc vu_drawLeftSideEven
    .export _vu_drawLeftSideEven
    PHA ; dup A onto the stack, since there's two calls to make
    JSR _vu_getBitsC0 ; pull bits C0 from A
    JSR _vu_writeHighBits ; dest in ptr1, value in A
    PLA
    JSR _vu_getBits30 ; pull bits 30 from A
    JMP _vu_writeLowBits ; dest in ptr1, value in A
    ; clears stack and returns
    .endproc

; void vu_drawRightSideEven(char *dest, char f)
; 0C to High
; f is in A; dest is on stack
; GLOBAL USAGE: ptr1 used as param dest
_vu_drawRightSideEven:
    .proc vu_drawRightSideEven
    .export _vu_drawRightSideEven
    JSR _vu_getBits0C ; pull bits 0C from A
    JMP _vu_writeHighBits ; dest in ptr1, value in A
    ; clears stack and returns
    .endproc

; Draw a character.
; dest is where to draw it; it will write to dest[0,40,80,120,160]
; font is an array of five bytes to copy
; void __fastcall__ vu_drawLetter(char *dest, char *font, boolean odd)
; dest is in ptr1, font is in AX, odd flag is in tmp3
; GLOBAL USAGE: ptr1 is dest, passed to functions being called
; GLOBAL USAGE: ptr2 is font pointer
; GLOBAL USAGE: tmp2 is a counter from 0..4
; GLOBAL USAGE: tmp3 is 0 for odd, 1 for even
_vu_drawLetter:
    .proc vu_drawLetter
    .export _vu_drawLetter
    ; store font pointer in ptr2
    STX ptr2+1
    STA ptr2

    ; set up a counter in tmp2
    LDA #5
    STA tmp2
foreachFontLine:
    ; read the next line of the font
    SEC
    LDA #5
    SBC tmp2
    TAY
    LDA (ptr2),Y
    PHA

    ; Draw left side
    LDY tmp3
    BEQ leftEven
leftOdd:
    JSR _vu_drawLeftSideOdd
    JMP leftAfter
leftEven:
    JSR _vu_drawLeftSideEven
leftAfter:

    ; increment ptr1 to draw the next half
    INC ptr1
    BNE nocarryPtr1a
    INC ptr1+1
nocarryPtr1a:

    ; Draw right side
    PLA
    LDY tmp3
    BEQ rightEven
rightOdd:
    JSR _vu_drawRightSideOdd
    JMP rightAfter
rightEven:
    JSR _vu_drawRightSideEven
rightAfter:

    ; next line - inc ptr2, ptr1(by 39), and dec tmp2
    LDA ptr1
    CLC
    ADC #39
    STA ptr1
    BCC nocarryPtr1b
    INC ptr1+1
nocarryPtr1b:

    DEC tmp2
    BNE foreachFontLine

    RTS
    .endproc


;
;
; RUNTIME CODE
;
;
; Two major functions: _vu_grey and _vu_green that change the color of a letter
; and the support function _vu_getByteOffset

    .segment "APP"
    ; used by GAMEINIT and TRAMPOLINE code

; Calculate (n>>1)*3 and subtract 1 if n is odd
; Increments Y if odd, which is convenient if Y is already 0.
; unsigned char __fastcall__ vu_getByteOffset(unsigned char n);
; GLOBAL USAGE: tmp1
_vu_getByteOffset:
    .proc vu_getByteOffset
    .export _vu_getByteOffset
    CLC
    LSR A    ; A is n>>1
    STA tmp1 ; tmp1 is (n>>1)
    BCS even
    DEC tmp1 ; tmp1 is (n>>1)-1
    INY
even:
    ; multiply by three (maybe add 1)
    CLC
    ASL A       ; A is 2*n (2*(n-1) if n is odd)
    ADC tmp1    ; A is 2*n+n-1  or  2*(n-1)+n-1
    LDX #0
    RTS
    .endproc

; Change the specified letter to green(used)
; Letter should be an uppercase ascii letter
; No effect if letter is already green
; void __fastcall__ vu_green(char letter, void *address);
; GLOBAL USAGE: ptr1 to pass dest to vu_grey_common
; GLOBAL USAGE: tmp2 to pass mask to vu_grey_common
    .segment "APP"
trampoline _vu_green,_vu_green_tramp
    .proc vu_green
    JSR staxptr1
    LDA #$AA
    STA tmp2
    JMP _vu_grey_common
    .endproc

; Change the specified letter to grey(used)
; Letter should be an uppercase ascii letter
; No effect if letter is already grey
; void __fastcall__ vu_grey(char letter, void *address);
; GLOBAL USAGE: ptr1 to pass dest to vu_grey_common
; GLOBAL USAGE: tmp2 to pass mask to vu_grey_common
    .segment "APP"
trampoline _vu_grey,_vu_grey_tramp
    .proc vu_grey
    JSR staxptr1
    LDA #$55
    STA tmp2
    ; JMP _vu_grey_common
    .endproc

; STILL IN THE TRAMPOLINE SEGMENT

; Apply the mask in tmp2 to the screen char at ptr1
; GLOBAL USAGE: ptr1 and tmp2
_vu_grey_common:
    .proc vu_grey_common
    .export vu_grey_common ; only exported while optimizing

    ; Mask in in tmp2
    ; Letter is on the C stack (sp),0
    ; Address of first entry is in ptr1

    ; Load the ascii letter off the stack, and subtract $40, we want A to be 1
    LDY #0
    LDA (sp),Y
    SEC
    SBC #$40

    ; Check the _vu_flags
    TAY
    LDA _vu_flags-1,Y
    BEQ doDraw
    ; Flag already set -- return!
    JMP incsp1 ; clear stack and RTS
doDraw:
    LDA #1
    STA _vu_flags-1,Y
    TYA
    ; Calculate the offset to the letter on the line, and add it to ptr1
    ; Also will increment Y if the letter is odd
    LDY #0
    JSR _vu_getByteOffset
    CLC
    ADC ptr1
    STA ptr1

    ; Check to see if the letter is even or odd, and act accordingly.
    ; vu_getByteOffset will have incremented Y to 1 if odd.
    TYA
    BNE even
odd:
    INC ptr1
    JSR vu_domask

    DEC ptr1
    LDA #$F0 ; clear the left half of the mask -- 10101010 becomes 11111010
    JMP done
even:
    JSR vu_domask
    INC ptr1
    LDA #$0F ; clear the right half of the mask -- 10101010 becomes 10101111
done:
    ORA tmp2
    STA tmp2
    JSR vu_domask
    JMP incsp1 ; clear stack and RTS
    .endproc

; STILL IN THE TRAMPOLINE SEGMENT

; apply mask in tmp2 to address ptr1 and the next 4 40-char lines too
vu_domask:
    .proc _vu_domask
    .export vu_domask ; only exported while optimizing
    LDX #5
    LDA #0 ; this will TAY first in the loop
    CLC ; we will add 4*40 to 0, no more. It will never carry.
loop:
    TAY
    LDA tmp2
    AND (ptr1),Y
    STA (ptr1),Y
    ; DEX + BEQ moved to after the TAY+ADC+TYA
    ; to eliminate a JMP worth of space
    ; at the cost of an extra ADD worth of time
    TYA
    ADC #40
    ; TAY moved to start of loop for speed
    DEX
    BNE loop
done:
    RTS
    .endproc

