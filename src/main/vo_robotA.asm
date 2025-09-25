; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Assembler code for the ridiculously simple robot opponent view.


    .import VOR_CODE_BANK_ID
    .import VOR_IDX_BANK_ID
    .import _VO_YIELDBANKID
    .import _DS_PICIDX_BANKID
    .include "zeropage.inc"
    .include "include/ds_screendriverA.inc"
    .importzp _DS_CURRPAGE
    .importzp _DS_ZTMPI
    .importzp _DS_ZTMPIL
    .importzp _DS_ZTMPIH

    .import _vor_renderFrame
    .import _voa_renderFrameCB
    .import _voa_animFrameQueue
    .import _vor_animFrameQueue
    .import _voa_preinit
    .import _ds_setVbiCallback

    .segment "ONCE"

; Called by vor_initialize to set up bank switching
; (this has to be in assembler to get the VOR_BANK_ID from the linker)
; void vor_preinit(void);
_vor_preinit:
    .proc vor_preinit
    .export _vor_preinit

    JSR _voa_preinit

    LDA #<VOR_CODE_BANK_ID
    STA _VO_YIELDBANKID
    LDA #<VOR_IDX_BANK_ID
    STA _DS_PICIDX_BANKID

    ; Initialize the renderFrame callback.
    ; this could be done later in vor_initialize
    LDA #<_vor_renderFrame
    STA _voa_renderFrameCB
    LDA #>_vor_renderFrame
    STA _voa_renderFrameCB+1

    ; Register the animation queue
    ; this could be done later in vor_initialize
    LDA #<_vor_animFrameQueue
    STA _voa_animFrameQueue
    LDA #>_vor_animFrameQueue
    STA _voa_animFrameQueue+1

    ; register the VBI callback
    LDA #<_vor_vbiCallback
    LDX #>_vor_vbiCallback
    JSR _ds_setVbiCallback

    RTS
    .endproc

    .RODATA

_LASER:
    .export _LASER;
    .byte %00011000
    .byte %00111100
    .byte %00111100
    .byte %00011000


.segment "RODATA"
; this SHOULD go in LOWCODE but there's not room in the cartridge bank
; unless I can squeeze this down to ten bytes

; This is run in the interrupt context
no_laser_to_draw:
    RTS
_vor_vbiCallback:
    .export _vor_vbiCallback
    .proc vor_vbiCallback

    ; Draw the laser!
    ; There is a list of X coordinates at page.freespace[$0000]
    ; There is a list of Y coordinates at page.freespace[$0100]
    ; and the current index is at page.freespace[$3D] or so.

    ; X coordinate first. Don't bother with the Y if it's zero.
    ; Get the freespace into _DS_ZTMPI. MSB is _DS_CURRPAGE+1, LSB is 0
    LDA _DS_CURRPAGE
    BEQ no_laser_to_draw ; don't crash on startup

    CLC
    ADC #1
    STA _DS_ZTMPIH
    LDA #0
    STA _DS_ZTMPIL

    ; Get the current index from freespace[$3D]
    LDY #$3D
    LDA (_DS_ZTMPI),Y
    PHA
    ; decrement it and store that
    SEC
    SBC #1
    STA (_DS_ZTMPI),Y
    PLA
    TAY

    ; get x coordinate and move player2 and 3 in to place
    LDA (_DS_ZTMPI),Y ; x coordinate
HPOSP2=$D002
    STA HPOSP2
HPOSP3=$D003
    STA HPOSP3
    BEQ no_laser_to_draw
    LDA #$72
COLPM2=$D014
    STA COLPM2
    LDA #$44
COLPM3=$D015
    STA COLPM3


    ; Y coordinate. This means a copying the four bytes of SPOTLIGHT
    ; to the four bytes starting at the stored Y
    ; To get the Y coords, add $0100 to the pointer.
    INC _DS_ZTMPIH

    ; We actually need to get the previous Y so we can erase it too.
    LDA (_DS_ZTMPI),Y ; y coordinate
    PHA
    INY
    LDA (_DS_ZTMPI),Y ; previous y coordinate
    TAY

    ; Now that we have all the coordinates we need,
    ; load the pointer with the address of Player 2.
    ; That will be the page address plus $0600.
    LDA _DS_CURRPAGE
    CLC
    ADC #6
    STA _DS_ZTMPIH

    ; Y is currently the previous Y coordinate, just need to zero out the next four bytes
    LDA #0
    STA (_DS_ZTMPI),Y
    INY
    STA (_DS_ZTMPI),Y
    INY
    STA (_DS_ZTMPI),Y
    INY
    STA (_DS_ZTMPI),Y

    ; same thing for player 3
    INC _DS_ZTMPIH
    STA (_DS_ZTMPI),Y
    DEY
    STA (_DS_ZTMPI),Y
    DEY
    STA (_DS_ZTMPI),Y
    DEY
    STA (_DS_ZTMPI),Y

    ; Draw the laser. The new Y coordinate is on the stack.
    ; Store the new Y coord in the pointer's LSB
    ; so that src and dest have the same index
    PLA
    STA _DS_ZTMPIL

    LDY #0
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    INY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    INY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    INY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y

    ; Same for player 2
    DEC _DS_ZTMPIH
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    DEY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    DEY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    DEY
    LDA _LASER,Y
    AND $D20A ; random number generator
    STA (_DS_ZTMPI),Y
    

alldone:
    RTS
    .endproc

.segment "VOR_IDX"

; Yes, this is a bit wasteful, but it's better than a switch statement

    .import _vor_anim_laserleft1_yellow
    .import _vor_anim_laserleft2_yellow
    .import _vor_anim_laserleft3_yellow
    .import _vor_anim_laserleft4_yellow
    .import _vor_anim_laserright1_yellow
    .import _vor_anim_laserright2_yellow
    .import _vor_anim_laserright3_yellow
    .import _vor_anim_laserright4_yellow

    .import _vor_anim_laserleft1_green
    .import _vor_anim_laserleft2_green
    .import _vor_anim_laserleft3_green
    .import _vor_anim_laserleft4_green
    .import _vor_anim_laserright1_green
    .import _vor_anim_laserright2_green
    .import _vor_anim_laserright3_green
    .import _vor_anim_laserright4_green

_vor_laser_anim_list:
.export _vor_laser_anim_list

    ; The array is four sets of frame-counts, 0..3 are 1..4 frames.
    ;  0..3    green left
    ;  4..7    yellow left
    ;  8..11   green right
    ; 12..15   yellow right

    .word _vor_anim_laserleft1_green
    .word _vor_anim_laserleft2_green
    .word _vor_anim_laserleft3_green
    .word _vor_anim_laserleft4_green

    .word _vor_anim_laserleft1_yellow
    .word _vor_anim_laserleft2_yellow
    .word _vor_anim_laserleft3_yellow
    .word _vor_anim_laserleft4_yellow

    .word _vor_anim_laserright1_green
    .word _vor_anim_laserright2_green
    .word _vor_anim_laserright3_green
    .word _vor_anim_laserright4_green

    .word _vor_anim_laserright1_yellow
    .word _vor_anim_laserright2_yellow
    .word _vor_anim_laserright3_yellow
    .word _vor_anim_laserright4_yellow

