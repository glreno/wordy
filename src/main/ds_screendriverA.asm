; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Screen Driver machine language parts - application context
;   This is all stuff that can be called directly from your application.
;   It is mostly in the DS segment, and a lot of it uses cc65 stack and zeropage locations
;   A couple things are in the ONCE segment.

.include "atari.inc"
.include "zeropage.inc"
.include "include/ds_screendriverA.inc"

.import ldax0sp
.import incsp3
.import incsp2
.import incsp1
.import pusha
.import umul8x8r16
.import umul8x8r16m
.import _ds_restartAnimTimer


; Application locations

;   .import _DS_CURRPAGE
    .import _DS_NEXTPAGE
    .import _DS_DLICBV
    .import _DS_ATCBV
    .import _DS_PFCBV
    .import _DS_DLICB
    .import _DS_ATCB
    .import _DS_PFCB
    .import _DS_ATTIME
    .exportzp _DS_ZTMPM
    .exportzp _DS_ZTMPML
    .exportzp _DS_ZTMPMH
;   .import _ds_restartAnimTimer
    .export _ds_flipToPageNextVBI
    .export _ds_flipToPageNextVBIandWait
    .export _ds_setDliCallback
    .export _ds_setFlipCallback
    .export _ds_setAnimTimerCallback
    .export _ds_calcScrLoc
    .export _ds_putc
    .export _ds_put2c
    .export _ds_puts



; Application locations in Page Zero

    .ZEROPAGE

    ; a couple zero-page bytes for storing addresses
    ; reserved for functions that MAY OR MAY NOT
    ; be called in the interrupt.
    _DS_ZTMPM: .word 0
    _DS_ZTMPML = _DS_ZTMPM
    _DS_ZTMPMH = _DS_ZTMPM+1

;
;
; SUPPORT FUNCTIONS
;
; This is stuff that could be in C if you really wanted to
;
;

    .segment "DS"

; ADD PAGE TO PAGE FLIP QUEUE
;
; Set the state of the new page to QUEUED(1)
; Store the page MSB in _DS_NEXTPAGE
; C function header:
; void __fastcall__ ds_flipToPageNextVBI(ds_page *page)
; The param page will be in A(LSB) and X(MSB)
;
; This uses _DS_ZTMPM since _DS_ZTMP is exclusively reserved
; for the interrupts. You can call this function from
; an interrupt or from the main thread,
; BUT NOT BOTH IN THE SAME APPLICATION!
_ds_flipToPageNextVBI:
    .proc ds_flipToPageNextVBI
    STA _DS_ZTMPML
    STX _DS_ZTMPMH
    LDA #DS_SCREENSTATE_ISNEXT ; store 1 in screenState; 1=next 0=onscreen -1=done
    LDY #ds_pageHeader_screenState ; 4
    STA (_DS_ZTMPML),Y
    ; address MSB is in X
    STX _DS_NEXTPAGE
    RTS
    ; The page flip will occur on the next VBI,
    ; and the Page Flip Callback _DS_PFCBV will be called.
    .endproc

    .segment "DS"
; The page flip will occur in the VBI, and a callback will be called.
; Busy-waits until the page actually changes.
; This is a good thing to call before major screen redraws.
; In DS segment because busywaiting
; void __fastcall__ ds_flipToPageNextVBIandWait(ds_page *page);
_ds_flipToPageNextVBIandWait:
    .proc ds_flipToPageNextVBIandWait
    JSR _ds_flipToPageNextVBI
busywait:
    LDA _DS_NEXTPAGE
    BNE busywait
    RTS
    .endproc

    .segment "ONCE"

; REGISTER THE DLI CALLBACK
; void ds_setDliCallback(void *const cb)
;
; This is main-thread code, so it can use the standard cc65 zeropage addresses
_ds_setDliCallback:
    .proc ds_setDliCallback
    ; Address is in AX
    ; stash the LSB in page zero, we need A
    STA tmp1
    ; Disable the callback by setting the vector to RTS
    lda #$60 ; RTS
	sta _DS_DLICBV
    ; Check address for null - if so, we're done.
    TXA
    ORA tmp1
    beq done
    ; Store the address in the vector
    LDA tmp1
	sta _DS_DLICB
	stx _DS_DLICB+1
    ; Enable the callback by setting the vector to JMP
	lda #$4C ; JMP
	sta _DS_DLICBV
done:
    RTS
    .endproc

    .segment "ONCE"

; REGISTER THE FLIP CALLBACK
; void ds_setFlipCallback(void *const cb)
;
; This is main-thread code, so it can use the standard cc65 zeropage addresses
_ds_setFlipCallback:
    .proc ds_setFlipCallback
    ; Address is in AX
    ; stash the LSB in page zero, we need A
    STA tmp1
    ; Disable the callback by setting the vector to RTS
    lda #$60 ; RTS
	sta _DS_PFCBV
    ; Check address for null - if so, we're done.
    TXA
    ORA tmp1
    beq done
    ; Store the address in the vector
    LDA tmp1
	sta _DS_PFCB
	stx _DS_PFCB+1
    ; Enable the callback by setting the vector to JMP
	lda #$4C ; JMP
	sta _DS_PFCBV
done:
    RTS
    .endproc

    .segment "ONCE"

; REGISTER THE ANIMATION TIMER CALLBACK
; void ds_setAnimTimerCallback(unsigned int jiffies,void *const cb)
;
; This is main-thread code, so it can use the standard cc65 zeropage addresses
_ds_setAnimTimerCallback:
    .proc ds_setAnimTimerCallback
    ; Address is in AX, jiffies is on the stack
    ; stash the LSB in page zero, we need A
    STA tmp1
    ; Disable the callback by setting the vector to RTS
    lda #$60 ; RTS
	sta _DS_ATCBV
    ; Check address for null - if so, we're done.
    TXA
    ORA tmp1
    beq done
    ; Store the address in the vector
    LDA tmp1
	sta _DS_ATCB
	stx _DS_ATCB+1
    ; Copy the jiffies param from the stack to DS_ATTIME
	jsr     ldax0sp
	sta     _DS_ATTIME
	stx     _DS_ATTIME+1
    ; Enable the callback by setting the vector to JMP
	lda #$4C ; JMP
	sta _DS_ATCBV
done:
    jsr incsp2 ; pop the param off the stack
    JMP _ds_restartAnimTimer
    .endproc

; DS segment because used by ds_putc and ds_puts
    .segment "DS"

; Calculate the address of a character on the screen
; unsigned char *ds_calcScrLoc(char cx,char cy)
;    return OS.savmsc + cy*40 + cx;
; cx is on the stack, cy is in the A register. X and Y registers not used.
_ds_calcScrLoc:
    .proc ds_calcScrLoc
    ; Store cy in ptr1, then multiply by 40
    STA ptr1
    LDA #0
    STA ptr1+1
    LDA #40 ; screen width
    JSR umul8x8r16
    ; umul8x8r16: multiply two unsigned 8 bit numbers, ptr1*A -> ptr1
    ; Add SAVMSC (start of screen ram) to ptr1
    CLC
    LDA SAVMSC
    ADC ptr1
    STA ptr1
    LDA SAVMSC+1
    ADC ptr1+1
    STA ptr1+1
    ; add cx, which is on the stack
    CLC
    LDY #0
    LDA (sp),Y
    ADC ptr1
    STA ptr1
    BCC nc
    INC ptr1+1
nc: jmp incsp1 ; pop the param off the stack
    .endproc

; DS segment because used by ONCE code and APP code
    .segment "DS"

; Print a an internal-code at the given coordinate
; void ds_putc(char c,char cx,char cy)
;  scrloc=ds_calcScrLoc(cx,cy)
;  *scrloc = c
; Params c and cx are on the stack
_ds_putc:
    .proc ds_putc
    JSR _ds_calcScrLoc
    ; that pops the cx param off the stack
    ; location is stored in ptr1
    LDY #0
    LDA (sp),Y
    STA (ptr1),Y
    jmp incsp1 ; pop the param c off the stack
    .endproc

; DS segment because used by ONCE code and APP code
    .segment "DS"

_ds_put2c:
    .proc ds_put2c
    jsr _ds_putc ; this will remove left,cx from stack, only right is still there.
    LDY #0
    LDA (sp),Y
    INY
    STA (ptr1),Y
    jmp incsp1 ; pop the param c off the stack
    .endproc

; DS segment because it probably accesses a string in another cart bank
    .segment "DS"

; Print a an internal-code string at the given coordinate
; void ds_puts(const char *const src,char len,char x,char y)
;   unsigned char *scrloc = ds_calcScrLoc(x,y);
;   memcpy(scrloc,s,len);
; params s,len,cx are on the stack
_ds_puts:
    .proc ds_puts
    JSR _ds_calcScrLoc
    ; that pops the cx param off the stack
    ; dest is stored in ptr1
    ; stack is now: len src
    ; Store len in Y and src in ptr2
    LDY #2
    LDA (sp),Y
    STA ptr2+1
    DEY
    LDA (sp),Y
    STA ptr2
    DEY
    LDA (sp),Y
    TAY
    DEY ; we'll be counting backwards, so Y is too high
    ; Now copy it!
loop:
    LDA (ptr2),Y
    STA (ptr1),Y
    DEY
    BPL loop

    jmp incsp3 ; pop the param src and len off the stack
    .endproc

; ONCE segment because it's used setDefaultPalette, otherwise it would be trampolined
    .segment "ONCE"

; Selects the color palette from a hardcoded list of ten.
; void __fastcall__ ds_setStandardPalette(char p,ds_page *page)
; memcpy(&(page->colors.color0),&ds_defaultColors+p*5,5);
_ds_setStandardPalette:
    .export _ds_setStandardPalette
    .proc ds_setStandardPalette
    ; Store dest in ptr2 -- MSB is in X, but A is 0; replace with correct offset
    LDA #ds_pageHeader_colors
    STA ptr2
    STX ptr2+1
    ; umul8x8r16: multiply two unsigned 8 bit numbers, ptr1*A -> ptr1
    LDY #0
    LDA (sp),Y
    STA ptr1
    LDA #5 ; 5 colors in the colormap
    JSR umul8x8r16

    ; Store source in ptr1 -- offset is already there, need to add it
    LDA #>_ds_defaultColors
    STA ptr1+1
    LDA #<_ds_defaultColors
    CLC
    ADC ptr1
    STA ptr1
    BCC nocarry
    INC ptr1+1
nocarry:

    ; Copy five bytes
    LDY #4 ; indexes 0..4
loop:
    LDA (ptr1),Y
    STA (ptr2),Y
    DEY
    BPL loop

    ; all done
    JMP incsp1
    .endproc

; ONCE segment because it's used to initialize screen pages
    .segment "ONCE"

; Selects the color palette for PAL or NTSC
; void __fastcall__ ds_setDefaultPalette(ds_page *page)
; Check NTSC or PAL, and call _ds_setStandardPalette(1|6,page)
_ds_setDefaultPalette:
    .export _ds_setDefaultPalette
    .proc ds_setDefaultPalette
    ; Just need to load a couple memory addresses;
    ; target address is in AX, store the A in Y for now
    ; but it's a ds_page, so A is zero. No need to stash it!
    ; LDA $62 ; XL PAL flag, doesn't work on 400/800
    LDA $D014 ; 400/800 PAL flag, probably works on XL, it does on Altirra
    AND #$E
    BEQ pal
ntsc:
    LDA #1
    JMP pushit
pal:
    LDA #7
pushit:
    JSR pusha
    LDA #0
    JMP _ds_setStandardPalette
    .endproc


    .RODATA
; Standard color palettes
; The names are those in Altirra's Adjust Colors dialog
_ds_defaultColors:
;palette0_Atari_Standard:
;    .byte 40,202,148,70,0

palatte0_monochrome:
    .byte $04,$B2,$08,$FA, 0

    ; NTSC COLORMAPS

    ;   grey, green, white, yellow, black

palette1_Altirra_Default_NTSC_XL:       ; B4F8
    .byte $04, $B4, $08, $F8, $00

palette2_Altirra_NTSC_XE:               ; D6FA bright
    .byte $04, $D6, $0A, $FA, $00

palette3_Altirra_NTSC_Contemporary_XL:  ; B418
    .byte $04, $B4, $08, $18, $00

palette4_Altirra_NTSC_XL_with_1702:     ; D41A bright
    .byte $04, $D4, $0A, $1A, $00

palette5_Altirra_NTSC_800:              ; D4FA bright
    .byte $04, $D4, $0A, $FA, $00

    ; PAL COLORMAPS

palette6_Altirra_320_PAL:               ; A2E8
    .byte $04, $A2, $08, $E8, $00

palette7_Altirra_Default_PAL:           ; A4EA bright
    .byte $04, $A4, $0A, $EA, $00

palette8_Altirra_OlivierPAL:            ; B4EA
    .byte $04, $B4, $08, $EA, $00

    ; blue-orange colormap

palette9_contrast:                      ; 942A
    .byte $04, $94, $08, $2A, $00

    .export _ds_defaultColors

