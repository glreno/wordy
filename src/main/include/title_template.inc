; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    .import         __CARTFLAGS__
    .import         _bankswitchTitle
    .import copydata, copyrodata
    .import start, _cgetc
    .macpack atari
    .include "include/version.inc"

    .define copyright_long .concat(title7char,aqordl_version," Copyright (c) ",year," Gerald Reno, Jr.")
    .define copyright_noname .concat(aqordl_version," Copyright (c) ",year," Gerald Reno, Jr.")
    .define copyright_short title7char,aqordl_version," (c)",year," Reno"

; OS LOCATIONS
    DOSVEC = $A
    COLDST = $244
    CH = $2FC
    RAMTOP = $6A

; This is the first thing in Bank 0 of the cartridge, and so the first thing in the ROM file
    .segment "COPYRIGHT"
_COPYRIGHT_NOTICE_BANK0:
    .asciiz copyright_long

; This is the first thing in the main of the cartridge, and so at $A000
    .segment "COPYRIGHT_ROM"
_COPYRIGHT_NOTICE:
    .export _COPYRIGHT_NOTICE
    .asciiz copyright_short

    .segment "TITLEPAGE"

; This is the first thing in the title page cartridge bank
    .byte copyright_short
_COPYRIGHT_NOTICE_INTERNAL_WIDE:
    scrcode title10char
_COPYRIGHT_NOTICE_INTERNAL:
    scrcode copyright_noname

; This is the code to launch the cartridge, switch banks, and display the title screen
; This is the LAST thing in the LAST bank of the cartridge

    .segment "TITLECARTHDR"

    .word   _title_cartstart       ; start routine
    .byte   0               ; must be zero
    .byte   <__CARTFLAGS__
    .word   _bankswitchTitle       ; init routine


    .segment "TITLEPAGE"

_title_cartstart:
    .export _title_cartstart
.proc title_cartstart
.ifdef ramtop_min
    LDA #11
    STA $54
    LDA #< ramfailmsg
    LDX #> ramfailmsg
    .import _cputs
    JSR _cputs
.endif
    LDA #1
    STA COLDST ; force coldstart on reset
.ifdef ramtop_min
; Check RAM size, we need 16K
    LDA RAMTOP
    CMP #ramtop_min
    BMI ramfail
.endif
    JSR _title_show_title_screen
    JSR copydata
.ifdef do_copyrodata
    JSR copyrodata
.endif
    JSR start ; run program
    JMP (DOSVEC) ; return to DOS if the program ever returns
.endproc

ramfail:
hang:
.ifdef ramtop_min
    JMP hang
ramfailmsg:
.export ramfailmsg
    .byte ramfailmsgtext
    ; trailing zero is in start of licence_screen
.endif

license_screen:
    ;        01234567890123456789                    01234567
    ;        XXX                 01234567890123456789      XX
    scrcode "   Licensed under the Apache License, Version   "
    scrcode "   2.0 (the 'License'); you may not use this    "
    scrcode "   file except in compliance with the License.  "
    scrcode "   You may obtain a copy of the License at      "
    scrcode "   http://www.apache.org/licenses/LICENSE-2.0   "
    scrcode "   Unless required by applicable law or agreed  "
    scrcode "   to in writing, software distributed under    "
    scrcode "   the License is distributed on an             "
    scrcode "        'AS IS' BASIS, WITHOUT WARRANTIES       "
    scrcode "            OR CONDITIONS OF ANY KIND,          "
    scrcode "   either express or implied.  See the License  "
    scrcode "   for the specific language governing          "
    scrcode "   permissions and limitations under the        "
    scrcode "   license.                                     "
    scrcode "      DEPENDENCIES      "
    scrcode "   Developed using the cc65 cross-compiler,     "
    scrcode "   which is licensed under the zlib License     "
    scrcode "   Dictionary partially dependent on Ubuntu     "
    scrcode "   dictionaries-common, which is under GPL-2    "

;
; Hardcoded title screen
;

    ;        01234567890123456789
    ;                            01234567890123456789
title_screen_name:
    scrcode title20char ; mode 6 text, lower case for high-bits 01, green
title_screen:
    scrcode instruction1 ; "  nine attempts to  "
    scrcode instruction2 ; "  guess four words  "
    scrcode "To change palette, press 1-9 (0 for b/w)"
    scrcode "To mute, guess 'PLEASE QUIET'           "
    scrcode "PRESS C FOR CREDITS " ; mode 6 text high-bits 00, orange
    scrcode "PRESS L FOR LICENSE " ; mode 6 text high-bits 00, orange
    ;        01234567890123456789
    ;                            01234567890123456789
credits_screen:
    scrcode "Wordle created by Josh Wardle 2021      "
    scrcode "Wordle is TM New York Times 2022        "
    scrcode "Quordle created by Freddie Meyer 2022   "
    scrcode "Quordle is a property of Merriam-Webster"
    scrcode "Developed using the cc65 cross compiler."
    scrcode " dictionary sources " ; mode 6 text high-bits 01, green
    scrcode "/usr/dict/words ('spell', McIlroy 1982) "
    scrcode "dictionaries-common 1.28.14(Ubuntu 2021)"
    scrcode "'1000 Most Common Words in English'     "
    scrcode "    gonaturalenglish.com                "
    scrcode "'Top 5000 Lemmas' wordfrequency.info    "
    scrcode "'An Incomplete List of Impolite Words'  "
    scrcode "    (Carlin on Campus, Carlin, G. 1984) "
    scrcode "'The Ten Hundred Words People Use Most' "
    scrcode "    (Thing Explainer, Munroe, R. 2015)  "
    ; "press a key to continue" in mode 6 text high-bits 11, pink
    ; "A" is ascii 65 $41, we need it to be 128+32+1 = 225 $E1 -> 'A'+160
press_a_key_to_begin_wide:
    .byte 0,0
press_a_key_to_begin:
    .byte 'P'+160
    .byte 'R'+160
    .byte 'E'+160
    .byte 'S'+160
    .byte 'S'+160
    .byte 0
    .byte 'A'+160
    .byte 0
    .byte 'K'+160
    .byte 'E'+160
    .byte 'Y'+160
    .byte 0
    .byte 'T'+160
    .byte 'O'+160
    .byte 0
    .byte 'B'+160
    .byte 'E'+160
    .byte 'G'+160
    .byte 'I'+160
    .byte 'N'+160
    .byte 0

;
; display list for hardcoded title screen
;

title_display_list:
; Leading blank rows
    .byte $70,$70,$70
; first line (big text) and address of screen RAM
    .byte $47
    .word title_screen_name
; copyright text
    .byte $70, $42
    .word _COPYRIGHT_NOTICE_INTERNAL
    .byte $70,$70,$70,$70
; real text
    .byte $46           ; nine attempts to
    .word title_screen
    .byte $40
    .byte 6             ; guess four words
    .byte $70,$70,$70,$70
; documentation
    .byte 2             ; To change palette....
    .byte 2             ; To mute....
; keypress text
    .byte $70
    .byte 6             ; Press L for license
    .byte 6             ; Press C for credits
    .byte $70
    .byte $46
    .word press_a_key_to_begin
; end of screen
    .byte $41
    .word title_display_list

credits_display_list:
; Leading blank rows
    .byte $70,$70,$70
; first line (big text) and address of screen RAM
    .byte $47
    .word title_screen_name
; copyright text
    .byte $70, $42
    .word _COPYRIGHT_NOTICE_INTERNAL
    .byte $70
; real text
    .byte $42
    .word credits_screen
    .byte 2,2,2,2
    .byte $70
    .byte 6,2
    .byte 2,2,2,2
    .byte 2,2,2,2
; keypress text
    .byte $70
    .byte $46
    .word press_a_key_to_begin
; end of screen
    .byte $41
    .word credits_display_list

license_display_list:
; Leading blank rows
    .byte $70,$70,$30
; first line and address of copyright text
    .byte $42
    .word _COPYRIGHT_NOTICE_INTERNAL_WIDE
    .byte $70
; first line and address of license text
    .byte $42
    .word license_screen
; real text
    .byte 2,2,2
    .byte $70,2,$70 ; the URL
    .byte 2,2,2,2
    .byte 2,2,2,2,2
    .byte $6
    .byte 2,2,2,2
    .byte $70
    .byte $46
    .word press_a_key_to_begin_wide
; end of screen
    .byte $41
    .word license_display_list

_title_show_title_screen:
    .export _title_show_title_screen
.proc   title_show_title_screen
    ; disable screen
    LDA #0
    STA $22F
    ; stash the location of the OLD display list in the screen RAM
    ; I don't need it in here, but you might want it after done showing the title
;   LDY #0
;   LDA $230
;   STA ($58),Y
;   INY
;   LDA $231
;   STA ($58),Y
    ; load the title page display list
    LDA #<title_display_list
    STA $230
    LDA #>title_display_list
    STA $231
    ; colors
    LDA #$b6
    STA $2C5 ; COLOR1 (gr.1 $CA green lowercase; gr.0 foreground)
    LDA #$06
    STA $2C4 ; COLOR0 (gr.1 $28 orange uppercase) ("PRESS L")
    LDA #$02
    STA $2C6 ; COLOR2 (gr.1 $94 blue inverse uppercase; gr.0 bkgd)
    STA $2C8 ; COLOR4 (gr.1 $00 black background)
    STA $2C7 ; COLOR3 (gr.1 $46 pink inverse lowercase) ("PRESS A")
    ; enable normal width screen
    LDA #34
    STA $22F
    RTS
.endproc

_title_show_license_on_L:
    .export _title_show_license_on_L
.proc title_show_license_on_L
    ; Check for the key
    LDA CH
    ; "L" key is zero, so BEQ means it was pressed
    BEQ _title_show_license_screen
    ; "C" is $12
    CMP #$12
    BEQ _title_show_credits_screen
    ; return zero indicating license not shown
    LDA #0
    LDX #0
    RTS
.endproc

_title_show_credits_screen:
    .export _title_show_credits_screen
    .proc title_show_credits_screen
    ; disable screen
    LDA #0
    STA $22F
    ; display list
    LDA #<credits_display_list
    STA $230
    LDA #>credits_display_list
    STA $231
    ; enable normal-width screen
    LDA #34
    STA $22F
    ; return 34 indicating credits shown
    RTS

.endproc
_title_show_license_screen:
    .export _title_show_license_screen
.proc title_show_license_screen
    ; disable screen
    LDA #0
    STA $22F
    ; display list
    LDA #<license_display_list
    STA $230
    LDA #>license_display_list
    STA $231
    ; enable wide screen
    LDA #35
    STA $22F
    ; return 35 indicating license shown
    RTS
.endproc

_title_show_press_a_key:
    .export _title_show_press_a_key
.proc title_show_press_a_key
    LDA #$FF
    STA CH
    LDA #$c6
    STA $2C7
    RTS
.endproc

; void title_wait_for_key(char shown);
_title_wait_for_key:
    .export _title_wait_for_key
.proc title_wait_for_key
    ; A reg contains "shown" (0 if not shown)
    PHA
waitforkey:
    JSR _cgetc
    ; Check the stack. If it's not a 0, just return
    TAX ; copy the key for future reference
    PLA
    BEQ readthekey
    ; we already showed it
    RTS
readthekey:
    TXA ; get the key from the stash
    ; If A reg contains 'l'
    ; then show the license page
    CMP #108 ; ascii lowercase L
    BEQ itsanL
    ; If A reg contains 'c'
    ; then show the credits page
    CMP #99 ; ascii lowercase C
    BEQ itsanC
    ; not an L or C, wwe're done here
    RTS
itsanL:
    JSR _title_show_license_screen
    JMP shown
itsanC:
    JSR _title_show_credits_screen
shown:
    LDA #1 ; we've shown the license now
    JMP _title_wait_for_key
.endproc


