; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
; Title definitions
;
    ; 7 char string, used for copyright messages in ROM
    .define title7char  "Aqordl "

    ; 10 char string, used for copyright messages in Gr.0 text
    .define title10char "   Aqordl "

    ; 20 char string, used for title messages in Gr.2 text
    ; so do not mix case!
    .define title20char  "       aqordl       "

    ; Two lines of 20-char text for game description
    .define description1 " a word puzzle game "
    .define description2 "                    "
    ; Two lines of 40-char text for sub description
    .define description3 "                                        "
    .define description4 "      with a very small dictionary      "
    .define description5 "                                        "
    .define description6 "       48K Disk version - Text Only Mode"
    .define descriptionLoad "LOADING GAME        "
    .define      creditLoad "LOADING DICTIONARY. "
    .define     licenseLoad "  LOADING GAME...       "

    ; Two lines of 20-char text for game instructions
    .define instruction1 "  nine attempts to  "
    .define instruction2 "  guess four words  "
    ; Two lines of 40-char text for detailed instructions
;                         01234567890123456789
;                                             01234567890123456789
    .define instruction3 "GREEN letters are correct.              "
    .define instruction4 "YELLOW letters are in the wrong place.  "
    .define instruction5 "To change palette, press 1-9 (0 for b/w)"
    .define instruction6 "To mute, guess 'PLEASE QUIET'           "
    .define instructionLoad "STARTING GAME...    "
;
; Title screen template package
;
    .include "include/title_template_disk.inc"


; All the file header definitions

; The rule is that you need to know where
; the FIRST and LAST segments are in the chunk.
; since the header is start and end ADDRESS.
    INITAD = $02E2

    .segment "TITLE_HDR"
    .import __TITLE_PAGE_LOAD__,__TITLE_PAGE_SIZE__
    .word __TITLE_PAGE_LOAD__
    .word __TITLE_PAGE_LOAD__+__TITLE_PAGE_SIZE__-1

    .segment "TITLE_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_title_screen

    .segment "TITLEC_HDR"
    .import __TITLEC_PAGE_LOAD__,__TITLEC_PAGE_SIZE__
    .word __TITLEC_PAGE_LOAD__
    .word __TITLEC_PAGE_LOAD__+__TITLEC_PAGE_SIZE__-1

    .segment "TITLEC_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_credits_screen

    .segment "TITLEI_HDR"
    .import __TITLEI_PAGE_LOAD__,__TITLEI_PAGE_SIZE__
    .word __TITLEI_PAGE_LOAD__
    .word __TITLEI_PAGE_LOAD__+__TITLEI_PAGE_SIZE__-1

;   .segment "TITLEI_TRL"
;   .word INITAD
;   .word INITAD+1
;   .word _title_show_instruction_screen

    .segment "BIGHDR"
    .import __CODE_LOAD__
    .import __COPYRIGHT_ROM_LOAD__,__COPYRIGHT_ROM_SIZE__
    .word __CODE_LOAD__
    .word __COPYRIGHT_ROM_LOAD__+__COPYRIGHT_ROM_SIZE__-1

    .segment "TITLEL_HDR"
    .import __TITLEL_PAGE_LOAD__,__TITLEL_PAGE_SIZE__
    .word __TITLEL_PAGE_LOAD__
    .word __TITLEL_PAGE_LOAD__+__TITLEL_PAGE_SIZE__-1

    .segment "TITLEL_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_license_screen

    .segment "DICTHDR"
    .import __DICT_BANK_1_LOAD__
    .import __DICT_BANK_4_LOAD__,__DICT_BANK_4_SIZE__
    .word __DICT_BANK_1_LOAD__
    .word __DICT_BANK_4_LOAD__+__DICT_BANK_4_SIZE__-1

    .segment "RESTHDR"
    .import __TRAMPOLINES_LOAD__
    .import __COPYRIGHT_LOAD__,__COPYRIGHT_SIZE__
    .word __TRAMPOLINES_LOAD__
    .word __COPYRIGHT_LOAD__+__COPYRIGHT_SIZE__-1


