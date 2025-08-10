; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Screen Driver machine language parts - interrupt context
;   NONE of this is in the CODE segment

.include "atari.inc"
.include "include/ds_screendriverA.inc"

; OS LOCATIONS
    DLSTL  = $D402
    DLSTH  = $D403

; Application locations

    .export _DS_DLI
    .export _DS_CURRPAGE
    .export _DS_NEXTPAGE
    .export _DS_DLICBV
    .export _DS_ATCBV
    .export _DS_PFCBV
    .export _DS_DLICB
    .export _DS_ATCB
    .export _DS_PFCB
    .export _DS_NOCB
    .export _DS_ATTIME
    .export _DS_TIMERHANDLER
    .exportzp _DS_ZTMPI
    .exportzp _DS_ZTMPIL
    .exportzp _DS_ZTMPIH
    .export _ds_restartAnimTimer
    .constructor _DS_CTOR
    .interruptor _DS_PAGEFLIP

; Application locations in Page Zero

    .ZEROPAGE

    ; Address of the font list in the current page.
    ; _DS_CURRPAGE is the MSB of the font list AND the current page;
    ; it is set during the VBI after _DS_NEXTPAGE is set.
    DS_PAGEFONT: .word 0
    _DS_CURRPAGE = DS_PAGEFONT+1

    ; a couple zero-page bytes for storing addresses
    ; this is used during the page flip, so do not use in application,
    ; but it is available for use during the PF and AT and DLI callbacks.
    ; DO NOT USE IN THE MAIN THREAD. The interrupt WILL overwrite it.
    _DS_ZTMPI: .word 0
    _DS_ZTMPIL = _DS_ZTMPI
    _DS_ZTMPIH = _DS_ZTMPI+1


; Unitialized application data

    .BSS
    _DS_NEXTPAGE:   .byte 0
    _DS_DLICBV:     .byte 0,0,0 ; JMP and an address
    _DS_DLICB = _DS_DLICBV+1
    _DS_PFCBV:      .byte 0,0,0 ; JMP and an address
    _DS_PFCB = _DS_PFCBV+1
    _DS_ATCBV:      .byte 0,0,0 ; JMP and an address
    _DS_ATCB = _DS_ATCBV+1
    _DS_ATTIME:     .word 0 ; duration of animation timer

    .segment "ONCE"

; CONSTRUCTOR - intialize the callback flags

_DS_CTOR:
    ; disable callbacks
    LDA #$60 ; opcode for RTS
    STA _DS_DLICBV
    STA _DS_PFCBV
    STA _DS_ATCBV
    ; clear the NEXTPAGE so the first VBI doesn't page flip to garbage
    LDA #0
    STA _DS_NEXTPAGE
    ; initialize the timer jump address CDTMA2 to our handler
    LDA # < _DS_TIMERHANDLER
    STA CDTMA1
    LDA # > _DS_TIMERHANDLER
    STA CDTMA1+1
    RTS

;
;
; INTERRUPT HANDLERS
;
; These all go in LOWCODE, which is guaranteed to never be swapped out
;
;

    .segment "LOWCODE"

; DLI routine - copy address of next font into CHBASE $D409
; 
; The next font is in a table in the screen page.
; You can find the current screen page by checking PMBASE $D407,
; and the font list is two bytes past that.


_DS_DLI:
    ; DLI PHASE ONE -- about 15+ cycles in 40 byte modes, 55+ in blank lines, while it draws the last scanline of the line that has the interrupt bit set
                            ; 17 cycles
    ; save registers
    PHA                     ; 3
    TYA                     ; 2
    PHA                     ; 3

    ; copy font[vcount] to chbase
    LDY VCOUNT              ; 2
    LDA (DS_PAGEFONT),Y     ; 5
    ; we just hit 15 cycles exactly. SOMETIMES if you do the STA CHBASE,
    ; it will affect the last char of the line.
    ; So do nothing for a couple cycles instead of STA WSYNC
    ORA #0                  ; 2 Racing the beam!

    ; DLI PHASE TWO -- during the horizontal blank. 17+ cycles allowed
                            ; 11 cycles if not calling CB
                            ; 18 if calling the callback
    STA CHBASE              ; 4

    ; Call callback if enabled. (if it's disabled, you are JSRing to an RTS)
    ; When CB is called, A contains the new CHBASE, Y contains VCOUNT
    ; Phase 2 will be over; drawing WILL be occurring while it is being called
    JSR _DS_DLICBV          ; 6
                            ; 6 more for the JSR, plus whatever the CB itself does

    ; DLI PHASE THREE -- drawing the first scanline of the next line

    ; restore registers
DLI_DONE:
    PLA                     ; 4
    TAY                     ; 2
    PLA                     ; 4
    RTI                     ; 6

; VBI ROUTINE
;
; Load the _DS_NEXTPAGE -- set SDLSTL and PMBASE,
; and set the font pointer for the DLI
; Afterwards, call the Page Flip Callback _DS_PFCB if enabled
; and maybe restart the animation timer in CDTMV1

_DS_PAGEFLIP:
    ; skip if next page is 0
    LDA _DS_NEXTPAGE
    BEQ PAGEFLIP_DONE

    ; Update DISPLAY LIST pointers first!
    ; You do not want any chance of a delay here.
    ; (We are in the Vertical Blank, so we have to write to
    ; the ANTIC registers directly, since the OS has ALREADY
    ; copied the shadows to the registers. This is fine;
    ; if we weren't in the VB writing to the register would
    ; cause a flicker)

    ; copy page MSB in _DS_NEXTPAGE to S/DLSTH and PMBASE
    STA SDLSTH
    STA DLSTH
    STA PMBASE

    ; copy dlist LSB to S/DLSTL
    ; display list is at page+$C0 (ds_page.page.displaylist)
    LDA #ds_pageHeader_displaylist
    STA SDLSTL
    STA DLSTL


    ; copy colors to GTIA and its shadows
    ; colors are at page+$1B (ds_page.page.colors)
    LDA _DS_NEXTPAGE
    STA _DS_ZTMPIH
    LDA #ds_pageHeader_pcolors
    STA _DS_ZTMPIL
    LDY #8
@L1: LDA (_DS_ZTMPI),Y
    STA $2C0,Y
    STA $D012,Y
    DEY
    BPL @L1
    ; copy PM location & size to GTIA
    ; pm stuff is at page+$0E (ds_page.page.pmpos)
    LDA #ds_pageHeader_pmpos
    STA _DS_ZTMPIL
    LDY #12
@L2: LDA (_DS_ZTMPI),Y
    STA $D000,Y
    DEY
    BPL @L2

    ; copy page LSB to font pointers
    ; font list is at page+$24 (ds_page.page.fonts)
    LDA #ds_pageHeader_fonts
    STA DS_PAGEFONT ; zero page location for addr of font list

    ; Set the state of the NEXT PAGE to ON SCREEN (0)
    ; The state was set to 1 when it was put there by ds_flipToPageNextVBI
    ; (_DS_ZTMPIH is still pointing at _DS_NEXTPAGE)
    LDY #ds_pageHeader_screenState ; 4
    LDA #DS_SCREENSTATE_ONSCREEN
    STA _DS_ZTMPIL
    STA (_DS_ZTMPI),Y


    ; _DS_CURRPAGE is set last, because someone may be waiting for that
    LDX _DS_CURRPAGE ; to pass to the PFCB
    STX _DS_ZTMPIH   ; to the tmp pointer so we can update the state
    LDA _DS_NEXTPAGE
    STA _DS_CURRPAGE ; ID of current page, also DS_PAGEFONT+1
    ; hint: if you busywait on _DS_CURRPAGE, it is set BEFORE the callback


    ; Set the state of the CURR PAGE to OFF SCREEN (-1)
    ; _DS_ZTMPIL is still 0 and _DS_ZTMPIH was set before _DS_CURRPAGE changed
    LDY #ds_pageHeader_screenState
    LDA #DS_SCREENSTATE_OFFSCREEN
    STA (_DS_ZTMPI),Y
    ; Those SHOULD be atomic DEC but it has no indirect addressing mode!
    ; Since we're loading a constant the STA should be sufficiently atomic

    ; Call callback if enabled; if disabled, you are JSRing to an RTS
    ; when called, A contains the new page address MSB
    ; and X contains the old page address MSB
    LDA _DS_CURRPAGE
    JSR _DS_PFCBV

PAGEFLIPCB_DONE:
    ; clear _DS_NEXTPAGE
    LDA #0
    STA _DS_NEXTPAGE
    ; hint: if you busywait on _DS_NEXTPAGE, it is cleared AFTER the callback

    ; Since the screen changed, reset the screensaver
    STA ATRACT

    ; and we are done
PAGEFLIP_DONE:
    RTS

; ANIMATION TIMER HANDLER
;
; Call _DS_ATCB if enabled
;
; To call this, set system timer CDTMV1
; (which is done by _ds_restartAnimTimer and _ds_setAnimTimerCallback)
_DS_TIMERHANDLER:
    JSR _DS_ATCBV
    ; proceed to restart the timer

; ANIMATION TIMER STARTER
;
; Usually this is called by the VBI.
; But the function is available here in case
; anyone needs to restart it.
; (say if the next frame is not available)
_ds_restartAnimTimer:
    LDA _DS_ATTIME+1
    STA CDTMV1+1
    LDA _DS_ATTIME
    STA CDTMV1
_DS_NOCB: ; a convenient RTS here
    RTS

