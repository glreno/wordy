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
    .import __DICT_BANK_1_LOAD__
    .import __DICT_BANK_2_LOAD__,__DICT_BANK_2_SIZE__
    .word __DICT_BANK_1_LOAD__
    .word __DICT_BANK_2_LOAD__+__DICT_BANK_2_SIZE__-1

    .segment "XEBANK1_HDR"
    .import __DICT_BANK_3_LOAD__
    .import __DICT_BANK_4_LOAD__,__DICT_BANK_4_SIZE__
    .word __DICT_BANK_3_LOAD__
    .word __DICT_BANK_4_LOAD__+__DICT_BANK_4_SIZE__-1


