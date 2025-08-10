; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Font Manager machine language parts

.include "zeropage.inc"


    .import incsp1
    .import staxptr1

    .segment "FONTDATA"
; List of all characters (internal ids) that are NOT reserved
; for grey/yellow letters, green letters, or box art
; There are 47 here, 9x5+2 (or 50, if the boxtops are available)

_VFM_UNRESERVED_CODEPOINTS:
    .export _VFM_UNRESERVED_CODEPOINTS

    ; 0 = SPACE
    .byte 1,2,3,4,5         ; !"#$%
    .byte 6,7,8,9,10        ; &'()*
    .byte 11,12,13,14,15    ; +,-./
    ; 16-25 L GREEN LETTERS ascii 48-57 '0'-'9'
    .byte 26,27,28,29,30    ; :;<=>
    .byte 31                ; ?
    ; 32    BOX BT              ascii 64 @
    ; 33-58 L YELLOW LETTERS    ascii 65-90 A-Z
    ; 59    LEFT BOX SIDE       ascii 91 [
    ; 60    LR BOX SIDES        ascii 92 \
    ; 61    RIGHT BOX SIDE      ascii 93 ]
    ; 62    BOX TOP             ascii 94 ^
    ; 63    BOX BOTTOM          ascii 95 _
    ; 64    RIGHT BLANK TILE    atascii 0 heart
    .byte 65,66,67,68,69    ; ^A-^E
    .byte 70,71,72,73,74    ; ^F-^J
    .byte 75,76,77,78,79    ; ^K-^O
    ; 80-89 R GREEN LETTERS ascii 16-25 ^P-^Y
    .byte 90,91,92,93,94    ; ^Z esc up down left
    .byte 95                ; right
    ; 96 LEFT BLANK TILE atascii 96 diamond
    ; 97-122 R YELLOW LETTERS ascii 97-122 a-z
    .byte 123,124,125,126,127 ; spade pipe clear delete tab
    .byte 32,62,63 ; available if you're not using boxtops

; DS segment because used by ONCE and APP code
    .segment "DS"

; Get the base address (MSB only) of the specified font
; unsigned char vfm_getFontBase(char fontid,vfm_fontmanager *vfm)
;   return (vfm->managedFonts[fontid]);
_vfm_getFontBase:
    .export _vfm_getFontBase
        JSR staxptr1
        lda #$0C    ; offset of managedFonts
        ldy #0      ; param is top of stack
        clc
        adc (sp),y
        tay
        ldx #0
        lda (ptr1),y
        jmp incsp1  ; pop the 1 byte param off the stack


    .segment "ONCE"

; aqordl only allocates fonts once; some of the 8K test carts
; re-allocate them at runtime. If a real application needs to
; do that, then vfm_allocateFont needs to move to segment DS.

; selects one of the unallocated managed fonts, marks it allocated
; returns ID of allocated font, 0xff on error
; char vfm_allocateFont(vfm_fontmanager *vfm)
_vfm_allocateFont:
    .export _vfm_allocateFont
    .proc vfm_allocateFont
    ; store pointer to vfm, and get nfonts
    JSR staxptr1
    LDY #10
    LDA (ptr1),Y
    TAX
    ; store pointer to isFontAllocated
    LDA #$1C
    CLC
    ADC ptr1
    STA ptr1
    LDA #0
    ADC ptr1+1
    STA ptr1+1
    ; step through the array, look for a zero
    ; X is number of fonts
    LDY #0
loop:
    LDA (ptr1),Y
    BEQ foundone
    INY
    DEX
    BPL loop
    ; none found
    LDA #$FF
    TAY
    RTS
foundone:
    ; array index is in Y
    LDA #1
    STA (ptr1),Y
    LDX #0
    TYA
    RTS
    .endproc

