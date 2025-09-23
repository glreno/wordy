; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Check that this machine has 48K RAM.
;   If not, show an error screen, and hang
;

    .include "atari.inc"
    .macpack atari

    .export memCheck48k


    .segment "MEMCHK"

    RTS     ; for older DOSes which unconditionally run the first load chunk

memCheck48k:

    LDA RAMTOP
    CMP #$C0
    BMI fail

    ; Memory OK!
    RTS

    ; Alas, not enough, so we just hang.
fail:

    ; disable screen
    LDA #0
    STA $22F

    ; load the error page display list
    LDA #<error_display_list
    STA $230
    LDA #>error_display_list
    STA $231

    ; enable narrow width screen
    LDA #33
    STA $22F

hang:
    JMP hang

error_screen_text:
    scrcode "  48K required  "
;   scrcode "0123456789012345"

error_display_list:

; Leading blank rows
    .byte $70,$70,$70
; one line of big text, and address of screen RAM
    .byte $47
    .word error_screen_text
; end of screen
    .byte $41
    .word error_display_list

; Disk headers

    .segment "MEMCHKHDR"
    .import __MEMCHK_LOAD__,__MEMCHK_SIZE__
    .word __MEMCHK_LOAD__
    .word __MEMCHK_LOAD__+__MEMCHK_SIZE__-1

    .segment "MEMCHKTRL"
    .word INITAD
    .word INITAD+1
    .word memCheck48k

