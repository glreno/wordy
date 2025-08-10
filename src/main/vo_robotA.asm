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
    .importzp _DS_ZTMPI
    .importzp _DS_ZTMPIL
    .importzp _DS_ZTMPIH

    .import _vor_renderFrame
    .import _voa_renderFrameCB
    .import _voa_animFrameQueue
    .import _vor_animFrameQueue
    .import _voa_preinit

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

    RTS
    .endproc

.segment "VOR_IDX"

; Yes, this is a bit wasteful, but it's better than a switch statement

    .import _vor_anim_laserleft1_yellow
    .import _vor_anim_laserleft2_yellow
    .import _vor_anim_laserleft3_yellow
    .import _vor_anim_laserleft4_yellow
    .import _vor_anim_laserleft5_yellow
    .import _vor_anim_laserright1_yellow
    .import _vor_anim_laserright2_yellow
    .import _vor_anim_laserright3_yellow
    .import _vor_anim_laserright4_yellow
    .import _vor_anim_laserright5_yellow

    .import _vor_anim_laserleft1_green
    .import _vor_anim_laserleft2_green
    .import _vor_anim_laserleft3_green
    .import _vor_anim_laserleft4_green
    .import _vor_anim_laserleft5_green
    .import _vor_anim_laserright1_green
    .import _vor_anim_laserright2_green
    .import _vor_anim_laserright3_green
    .import _vor_anim_laserright4_green
    .import _vor_anim_laserright5_green

_vor_laser_anim_list:
.export _vor_laser_anim_list
    .word _vor_anim_laserleft1_yellow ;  01
    .word _vor_anim_laserleft1_green ;  01
    .word 0 ;  03
    .word _vor_anim_laserleft2_yellow ;  04
    .word _vor_anim_laserleft2_green ;  04
    .word 0 ;  06
    .word _vor_anim_laserleft3_yellow ;  07
    .word _vor_anim_laserleft3_green ;  07
    .word 0 ;  09
    .word _vor_anim_laserleft4_yellow ;  0A
    .word _vor_anim_laserleft4_green ;  0A
    .word 0 ;  0C
    .word _vor_anim_laserleft5_yellow ;  0D
    .word _vor_anim_laserleft5_green ;  0D
    .word 0 ;  0F
    .word 0 ;  10
    .word 0 ;  11
    .word 0 ;  12
    .word 0 ;  13
    .word 0 ;  14
    .word 0 ;  15
    .word 0 ;  16
    .word 0 ;  17
    .word 0 ;  18
    .word _vor_anim_laserright1_yellow ;  19
    .word _vor_anim_laserright1_green ;  19
    .word 0 ;  1B
    .word _vor_anim_laserright2_yellow ;  1C
    .word _vor_anim_laserright2_green ;  1C
    .word 0 ;  1E
    .word _vor_anim_laserright3_yellow ;  1F
    .word _vor_anim_laserright3_green ;  1F
    .word 0 ;  21
    .word _vor_anim_laserright4_yellow ;  22
    .word _vor_anim_laserright4_green ;  22
    .word 0 ;  24
    .word _vor_anim_laserright5_yellow ;  25
    .word _vor_anim_laserright5_green ;  25

