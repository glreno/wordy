; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Font Manager machine language parts
;   (part 2 - stuff not needed by qordl itself these days)

.include "zeropage.inc"


    .import incsp1
    .import staxptr1

    .segment "DS"

; marks the specified font as unallocated, call when done with an animation frame
; void vfm_deallocateFont(char fontid,vfm_fontmanager *vfm)
;    vfm->isFontAllocated[fontid] = 0;
_vfm_deallocateFont:
    .export _vfm_deallocateFont
        JSR staxptr1
        lda #$1C    ; offset of isFontAllocated
        ldy #0      ; param is top of stack
        clc
        adc (sp),y
        tay
        lda #0  ; not-allocated flag
        sta (ptr1),y
        jmp incsp1  ; pop the 1 byte param off the stack

