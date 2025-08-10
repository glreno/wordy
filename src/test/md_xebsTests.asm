; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    INITAD = $02E2
    .import _bankswitch

; This is support code for running md_bsTests using 130XE bank swapping instead of a cartridge.

_title_show_title_screen:
    .export _title_show_title_screen
    RTS

; All the file header definitions

; The rule is that you need to know where
; the FIRST and LAST segments are in the chunk.
; since the header is start and end ADDRESS.

    .segment "LOWCODE_HDR"
    .import __LOWCODE_LOAD__
    .import __LOWCODE_LOAD__,__LOWCODE_SIZE__
    .word __LOWCODE_LOAD__
    .word __LOWCODE_LOAD__+__LOWCODE_SIZE__-1

    .segment "XEBANK0_HDR"
    .import __CARTBANK0_LOAD__
    .import __CARTBANK0_LOAD__,__CARTBANK0_SIZE__
    .word __CARTBANK0_LOAD__
    .word __CARTBANK0_LOAD__+__CARTBANK0_SIZE__-1

    .segment "XEBANK1_HDR"
    .import __CARTBANK1_LOAD__
    .import __CARTBANK1_LOAD__,__CARTBANK1_SIZE__
    .word __CARTBANK1_LOAD__
    .word __CARTBANK1_LOAD__+__CARTBANK1_SIZE__-1

    .segment "XEBANK2_HDR"
    .import __CARTBANK2_LOAD__
    .import __CARTBANK2_LOAD__,__CARTBANK2_SIZE__
    .word __CARTBANK2_LOAD__
    .word __CARTBANK2_LOAD__+__CARTBANK2_SIZE__-1

