; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Support code for copying pictures into a font

    .include "zeropage.inc"
    .import decsp1
    .import incsp1
    .import incsp5
    .import pusha
    .import popa
    .import umul8x8r16
    .import _bankswitch
    .include "include/trampoline.inc"

    .BSS

; Bank ID where the pics are stored
; Used by copyPicToFont, which will need to
; read the pic array, then bankswitch to where the data is, and back
_DS_PICIDX_BANKID:
    .byte 0
    .export _DS_PICIDX_BANKID

; DS segment because it probably accesses image data in other cart banks
    .segment "DS"


; void __fastcall__ copyPicStripe(unsigned char fontbase, int*picstripe)
; pointer to picstripe is in ax
; destination font MSB is on the stack
; The picstripe is a series of:
; word image address
; byte dest character (you will need to multiply by 8)
; byte count of characters to load (you will need to multiply by 8)
_copyPicStripe:
    .proc copyPicStripe
    .export _copyPicStripe
    ; Store picsrtipe address in ptr2
    ; Get the source address from (ptr2),0 and store it in ptr4
    ; inc ptr2 by 2 so it points to (destchar,charcount)
    ; if *ptr2 is 0, return
    ; Destination address is (fontbase<<8)+(destchar*8)
    ; Get the destchar, multiply by 8, store in ptr1
    ; Add the font base address to ptr1 MSB
    ; Get the char count, and multiply it by 8 (max allowed is 16; 16*8=128 bytes to copy)
    ; (realistically count will never be more than 9)
    ; Copy the data
    ; Repeat bact to incrementing ptr2

    ; Store picstripe address in ptr2
    STA ptr2
    STX ptr2+1

    ; Get the source address from (ptr2),0 and store it in ptr4
    LDY #0
    LDA (ptr2),Y
    STA ptr4
    INY
    LDA (ptr2),Y
    STA ptr4+1
    ; stored value is used for incrementing ptr4 later; first iteration will add 0
    LDX #0 ; was PHA - but using the stack is risky, and X is available

readline:
    ; Increment ptr2 (twice)
    CLC
    LDA #2
    ADC ptr2
    STA ptr2
    BCC nocarryA
    INC ptr2+1
nocarryA:

    ; Check (ptr2),0 (the dest char) for 0 (char 0 is space, can't use it)
    LDY #0
    LDA (ptr2),Y
    BNE keepgoing

    ; We're done! Clear stack and return
    JMP incsp1

keepgoing:
    ; add previous count*8 to ptr4
    TXA
    BEQ nocarryB ; this branch is here just to avoid adding zero on the first iteration
    CLC
    ADC ptr4
    STA ptr4
    BCC nocarryB
    INC ptr4+1
nocarryB:

    ; Get the destchar from (ptr2),0 , multiply by 8, store in ptr1
    ; (we just checked it for zero)
    LDA (ptr2),Y
    STA ptr1
    LDA #0
    STA ptr1+1
    LDA #8
    JSR umul8x8r16
    ; umul stores result in ptr1, uses ptr3 for scratch

    ; Add the font base address (from stack) to ptr1 MSB
    CLC
    LDY #0
    LDA (sp),Y
    ADC ptr1+1
    STA ptr1+1

    ; Get the char count from (ptr2),1 , and multiply it by 8
    LDY #1
    LDA (ptr2),Y
    ASL A
    ASL A
    ASL A
    TAX ; PHA ; store count * 8 so we can add it to ptr4 later
    TAY
    DEY

    ; Copy the data
loop:
    LDA (ptr4),Y
    ; from here to nomask: should probably NOT be here,
    ; since it's hardcoded to what the robot pics have.
    ; This should be a callback in vo_robot.
    CMP #$EA ; white left edge and solid green rest
    BEQ domaskleft
    CMP #$AB ; white right edge and solid green rest
    BNE nomask
domaskright:
    LDA #3
    BNE domask
domaskleft:
    LDA #$C0
domask:
    JSR _vor_copyPicStripeMask
nomask:
    STA (ptr1),Y
    DEY
    BPL loop

    JMP readline

    .endproc

_vor_copyPicStripeMask:
    .export _vor_copyPicStripeMask
    .proc vor_copyPicStripeMask

    ; Y is a number from 0 to 7 DO NOT MODIFY!
    ; A contains a mask of 11000000 or 00000011 - store that in tmp2
    STA tmp2

    ; odd or even line?
    TYA
    AND #1
    BEQ even
odd:
    LDA #$66  ; turn on random green/grey pixels
    BNE both
even:
    LDA #$99  ; turn on random green/grey pixels
both:
    AND $D20A ; random number generator
    ORA tmp2  ; white bar on the side
    RTS
    .endproc

; DS segment because it probably accesses image data in other cart banks
    .segment "DS"

; Copy a pic (see pngtoasmpic.groovy) into a font
;
; void __cdecl__ ds_copyPicToFont(void *pic[], unsigned char fontbase)
;
; for(int i=1;pic[i]!=0;++i) copyPicStripe(fontbase,pic[i]);
;
; all params are on the stack. Put i on the stack too; the
; offsets will be: (sp),0=i; 1=fontbase; 2 and 3 = pic
; The bank ID will be read from pic[0] and stored in tmp1
_ds_copyPicToFont:
    .proc ds_copyPicToFont
    .export _ds_copyPicToFont

    ; allocate i on the stack and set i=1
    LDA #1
    JSR pusha

    ; read the bank ID from pic[0]
    LDY #2
    LDA (sp),Y ; pic LSB
    STA ptr1
    INY
    LDA (sp),Y ; pic MSB
    STA ptr1+1
    LDY #0
    LDA (ptr1),Y
    STA tmp1

loop:
    LDY #1
    LDA (sp),Y
    JSR pusha ; Push fontbase onto the stack
    ; (sp),0=fontbase, (sp),1=i; 2=fontbase; 3 and 4 = pic

    ; add i (now in (sp),1 to pic (at (sp),3 and (sp),4)
    ; and store result (&pic[i]) in ptr1
    LDY #1
    LDA (sp),Y
    INY
    INY
    CLC
    ADC (sp),Y ; pic LSB
    STA ptr1
    INY
    LDA (sp),Y ; pic MSB
    ADC #0
    STA ptr1+1

    ; If pic[i] is null, return
    LDY #1
    LDA (ptr1),Y
    TAX ; store MSB in X instead of loading it later
    DEY
    ORA (ptr1),Y ; null check
    BNE keepgoing

    ; Clear stack and return
    JMP incsp5
keepgoing:

    ; call copyPicStripe
    LDA (ptr1),Y
    ; address of stripe is in AX
    ; bankswitch before calling _copyPicStripe
    PHA
    LDA tmp1
    JSR _bankswitch
    PLA
    JSR _copyPicStripe
    ; That will have popped a byte off the stack.
    ; (sp),0=i; 1=fontbase; 2 and 3 = pic
    LDA _DS_PICIDX_BANKID
    JSR _bankswitch

    ; increment i by two
    LDY #0
    LDA (sp),Y
    CLC
    ADC #2
    STA (sp),Y

    JMP loop

    .endproc

; Copy the first row of a bank from font1 to font2
; void __fastcall__ ds_copyBelt( unsigned char fontbase1, unsigned char fontbase2);
trampoline _ds_copyBelt,_ds_copyBelt_tramp
    .proc ds_copyBelt
    ; Load the MSB of the two font addresses into ptr1 and ptr2
    STA ptr1+1
    JSR popa
    STA ptr2+1

    ; Need to copy 8 bytes from each of six characters.
    ; The char indexes are the first in each stripe:
    ;   1   9  27  68  76  94
    ; or, multiplied by 8 and converted to hex:
    ; 008 048 0D8 220 260 2F0

    LDA #$08
    JSR copychar
    LDA #$48
    JSR copychar
    LDA #$D8
    JSR copychar
    INC ptr1+1
    INC ptr1+1
    INC ptr2+1
    INC ptr2+1
    LDA #$20
    JSR copychar
    LDA #$60
    JSR copychar
    LDA #$F0
    JSR copychar
    RTS

copychar:
    STA ptr1
    STA ptr2
    LDY #7
copycharloop:
    LDA (ptr1),Y
    STA (ptr2),Y
    DEY
    BPL copycharloop
    RTS

    .endproc

; Clear all the pic characters from TWO fonts
; void __fastcall__ ds_clearFontPic( unsigned char fontbase1, unsigned char fontbase2);
trampoline _ds_clearFontPic,_ds_clearFontPic_tramp
    .proc ds_clearFontPic
    ; Load the MSB of the two font addresses into ptr1 and ptr2
    STA ptr1+1
    JSR popa
    STA ptr2+1

    ; The areas we want to clear are (from the groovy script):
    ; stripeindexes = [
    ;   [1:8],    // start char:number of chars
    ;   [9:7,26:1],
    ;   [27:5,65:3],
    ;   [68:8],
    ;   [76:4,90:4],
    ;   [94:2,123:5,32:1]
    ; ];
    ; But you notice some of those are adjacent. The actual ranges are:
    ;   [1:15]
    ;   [26:6]
    ;   [65:15]
    ;   [90:6]
    ;   [123:5]
    ;   [32:1]
    ; Multiplying ALL those numbers by 8 we get the index & count to call out little bzero with:
    ;   [8:120]     (ends at 127:  $008..$07F)
    ;   [208:48]    (ends at 255:  $0D0..$0FF)
    ;   [256:8]     (ends at 263:  $100..$107)
    ;   [520:120]   (ends at 639:  $208..$27F)
    ;   [720:48]    (ends at 767:  $2D0..$2FF)
    ;   [984:40]    (ends at1023:  $3D8..$3FF)
    ; Note that we need to add to the MSB of those pointers, but each range is well within Y indexing.

    LDA #0

    ; page #0
    LDX #$08
    LDY #119
    JSR zeroRanges

    LDX #$D0
    LDY #47
    JSR zeroRanges

    ; page #1
    INC ptr1+1
    INC ptr2+1
    LDX #$00
    LDY #7
    JSR zeroRanges

    ; page #2
    INC ptr1+1
    INC ptr2+1
    LDX #$08
    LDY #119
    JSR zeroRanges

    LDX #$D0
    LDY #47
    JSR zeroRanges

    ; page #3
    INC ptr1+1
    INC ptr2+1
    LDX #$D8
    LDY #39
    JSR zeroRanges

    RTS

zeroRanges:
    STX ptr1
    STX ptr2
zeroRangesLoop:
    STA (ptr1),Y
    STA (ptr2),Y
    DEY
    BPL zeroRangesLoop
    RTS
    .endproc

