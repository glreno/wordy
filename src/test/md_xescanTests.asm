; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    INITAD = $02E2
    .import _bankswitch

; This is support code for running md_scanTests using 130XE bank swapping instead of a cartridge.

_title_show_instruction_screen:
    .export _title_show_instruction_screen
    RTS

; All the file header definitions

; The rule is that you need to know where
; the FIRST and LAST segments are in the chunk.
; since the header is start and end ADDRESS.


    .segment "LOWCODE_HDR"
    .import __LOWCODEBANK_START__,__LOWCODEBANK_LAST__
    .word __LOWCODEBANK_START__
    .word __LOWCODEBANK_LAST__ -1

    .segment "XEBANK0_HDR"
    .import __XEBANK0_START__,__XEBANK0_LAST__
    .word __XEBANK0_START__
    .word __XEBANK0_LAST__ -1

    .segment "XEBANK1_HDR"
    .import __XEBANK1_START__,__XEBANK1_LAST__
    .word __XEBANK1_START__
    .word __XEBANK1_LAST__ -1

    .segment "XEBANK2_HDR"
    .import __XEBANK2_START__,__XEBANK2_LAST__
    .word __XEBANK2_START__
    .word __XEBANK2_LAST__ -1

    .segment "XEBANK3_HDR"
    .import __XEBANK3_START__,__XEBANK3_LAST__
    .word __XEBANK3_START__
    .word __XEBANK3_LAST__ -1
