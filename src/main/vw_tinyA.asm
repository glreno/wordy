; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

; ASM portion of the implementation of qordl tiny-tile view

    .include "zeropage.inc"
    .import popa
    .import pusha
    .import pushax
    .import incsp3
    .import _ds_puts
    .import staxptr1

    .segment "GAMEINIT"

; internal codes for a row of blank tiles.
; Not using a string because it's full of \\ and \0

; const char BLANKLINE[16]
_BLANKLINE:
    .export _BLANKLINE

    .byte $3B, $60, $40
    .byte $3C, $60, $40
    .byte $3C, $60, $40
    .byte $3C, $60, $40
    .byte $3C, $60, $40
    .byte $3D

_GUTTER:
    .export _GUTTER
    .byte $BE,$BF,$BE

; Initialize data structure, and draw grid of blank tiles
; - so called at the start of every game
; You are expected to have already called vws_initialize

; void vwt_initialize( vw_base *this)
_vwt_initialize:
    .export _vwt_initialize
    .proc vwt_initialize

    ; this->ysz=1; // offset 5
    ; this->yoff=0; // offset 6
    JSR staxptr1
    LDY #5
    LDA #1
    STA (ptr1),Y
    INY
    LDA #0
    STA (ptr1),Y

    ; Precalulate X and Y coordinate
    ; A=this->x+1; // offset 0
    ; for(i=0;i<5;++i)
    ; {
    ;     this->xoff[i]=A; // offsets 7-B
    ;     A+=3;
    ; }
    CLC
    LDY #0
    LDA (ptr1),Y
    ADC #1
    LDY #7
    LDX #5
xoff_loop:
    STA (ptr1),Y
    ADC #3
    INY
    DEX
    BNE xoff_loop

    ; Draw the tiles
    ; char nrows=this->nrows; // offset 2
    ; char cx=this->x; // offset 0
    ; char cy=this->y; // offset 1
    ; // all the pointer math is done at this point
    ; for(i=0;i<nrows;++i) // nrows is offset 2
    ; {
    ;     // can't use cputsxy, because this string contains \0
    ;     ds_puts(BLANKLINE,16,cx,cy); // uses ptr1 and ptr2
    ;     ++cy;
    ; }

    ; Push CX,CY,NROWS onto the stack
    LDY #0          ; X offset 0
    LDA (ptr1),Y
    JSR pusha
    LDY #1          ; Y offset 1
    LDA (ptr1),Y
    JSR pusha
    LDY #2          ; NROWS offset 2
    LDA (ptr1),Y
    JSR pusha
    ; Stack: 0:NROWS 1:CY 2:CX
printloop:
    LDY #0
    ; LDA #0
    ; CMP (sp),Y
    LDA (sp),Y
    BEQ printdone
    ; ds_puts params are: (from bottom of stack) s,nchars,cx; and cy in A
    LDA #<_BLANKLINE
    LDX #>_BLANKLINE
    JSR pushax  ; Stack: blankline, nrows, cy, cx
    LDA #$10
    JSR pusha   ; Stack: 0:nchars, 1:blankline, 3:nrows, 4:cy, 5:cx
    LDY #$05    ; CX
    LDA (sp),Y
    JSR pusha   ; Stack: 0:cx 1:nchars, 2:blankline, 4:nrows, 5:cy, 6:cx
    LDY #$05    ; CY
    LDA (sp),Y
    JSR _ds_puts
    ; Stack is now again: 0:NROWS 1:CY 2:CX
    ; Increment CY
    CLC
    LDY #1
    LDA (sp),Y
    ADC #1
    STA (sp),Y
    ; Decrement NROWS
    LDY #0
    LDA (sp),Y
    SEC
    SBC #1
    STA (sp),Y
    JMP printloop
printdone:
    JMP incsp3
    .endproc

_vwt_initialize_gutter:
    .export _vwt_initialize_gutter
    .proc vwt_initialize_gutter

    LDA #14
printloop2:
    JSR pusha   ; stack 0:x
    ; ds_puts params are: (from bottom of stack) s,nchars,cx; and cy in A
    LDA #<_GUTTER
    LDX #>_GUTTER
    JSR pushax  ; 0,1:gutter 2:x
    LDA #2
    JSR pusha   ; 0:nchar 1,2:gutter 3:x
    LDY #3
    LDA (sp),Y
    JSR pusha   ; 0:x 1:nchar 2,3:gutter 4:x
    LDA #11
    JSR _ds_puts

    ; now do it again at x+24
    LDA #<(_GUTTER+1)
    LDX #>(_GUTTER+1)
    JSR pushax  ; 0,1:gutter 2:x
    LDA #2
    JSR pusha   ; 0:nchar 1,2:gutter 3:x
    LDY #3
    LDA (sp),Y
    CLC
    ADC #24
    JSR pusha   ; 0:x 1:nchar 2,3:gutter 4:x
    LDA #11
    JSR _ds_puts

    JSR popa
    SEC
    SBC #2
    
    BPL printloop2
    
    RTS


    .endproc

