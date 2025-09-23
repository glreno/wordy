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
    .define title20char " aqordl - robot mode"

    ; Two lines of 20-char text for game description
    .define description1 " a word puzzle game "
    .define description2 "with a dancing robot"
    ; Two lines of 40-char text for sub description
    .define description3 "                                        "
    .define description4 "          but not social media          "
    ; Two lines of 20-char text for press C/L
    .define description5    "                    "
    .define description6    "  130XE DISK VERSION"
;                            01234567890123456789
    .define descriptionLoad "LOADING ROBOT       "
    .define      creditLoad "LOADING DICTIONARY. "
    .define     licenseLoad "  LOADING DICTIONARY..  "



    ; Two lines of 20-char text for game instructions
    .define instruction1 "  nine attempts to  "
    .define instruction2 "  guess four words  "
    ; Two lines of 40-char text for detailed instructions
;                         01234567890123456789
;                                             01234567890123456789
    .define instruction3 "GREEN letters are correct.              "
    .define instruction4 "YELLOW letters are in the wrong place.  "
    ; Two lines of 20-char text for press C/L
    .define instruction5 "PRESS C FOR CREDITS "
    .define instruction6 "PRESS L FOR LICENSE "
    .define instructionLoad "LOADING GAME...     "

;
; Title screen template package
;
    .include "include/title_template_disk.inc"


; All the file header definitions

; The rule is that you need to know where
; the FIRST and LAST segments are in the chunk.
; since the header is start and end ADDRESS.
; There's defined symbols for the memory chunk,
; UNLESS there is BSS in there, which doesn't get written to disk.
; Also note that the loader basically follows the order
; that the segments are in the .cfg file, but always
; loads the chunk in the order RO - RW - BSS

    INITAD = $02E2

    .segment "TITLE_HDR"
    .import __TITLE_START__,__TITLE_LAST__
    .word __TITLE_START__
    .word __TITLE_LAST__ -1

    .segment "TITLE_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_title_screen

    .segment "TITLEC_HDR"
    .import __TITLECRED_START__,__TITLECRED_LAST__
    .word __TITLECRED_START__
    .word __TITLECRED_LAST__ -1

    .segment "TITLEC_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_credits_screen

    .segment "TITLEI_HDR"
    .import __TITLEINST_START__,__TITLEINST_LAST__
    .word __TITLEINST_START__
    .word __TITLEINST_LAST__ -1

    .segment "TITLEI_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_instruction_screen

    .segment "TITLEL_HDR"
    .import __TITLELIC_START__,__TITLELIC_LAST__
    .word __TITLELIC_START__
    .word __TITLELIC_LAST__ -1

    .segment "TITLEL_TRL"
    .word INITAD
    .word INITAD+1
    .word _title_show_license_screen

    .segment "BIG1HDR"
    .import __BIG1_START__,__BIG1_LAST__
    .word __BIG1_START__
    .word __BIG1_LAST__ -1

    .segment "BIG2HDR"
    .import __BIG2_START__,__BIG2_LAST__
    .word __BIG2_START__
    .word __BIG2_LAST__ -1

    .segment "XEBANK0_HDR"
    .import __DICT0_START__,__DICT0_LAST__
    .word __DICT0_START__
    .word __DICT0_LAST__ -1

    .segment "XEBANK1_HDR"
    .import __DICT1_START__,__DICT1_LAST__
    .word __DICT1_START__
    .word __DICT1_LAST__ -1

    .segment "XEBANK2_HDR"
    .import __VOR_START__,__VOR_LAST__
    .word __VOR_START__
    .word __VOR_LAST__ -1

    .segment "TRAMPHDR"
    .import __TRAMP_START__,__TRAMP_LAST__
    .word __TRAMP_START__
    .word __TRAMP_LAST__ -1

    .segment "FONTSHDR"
    .import __FONTS_START__,__FONTS_LAST__
    .word __FONTS_START__
    .word __FONTS_LAST__ -1

