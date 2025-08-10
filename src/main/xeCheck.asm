; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Check to see if this is a 130XE.
;   If not, show an error screen, and hang
;

    .include "atari.inc"
    .macpack atari

; Standard bank IDs. These will all be the same value

    .export xeCheck


; The bits of PORTB are:
; 7 $80 1=enable RAM $5000-$57FF        0=enable selftest ROM
; 6 $40 ???
; 5 $20 VBE 1=ANTIC accesses main RAM   0=ANTIC access extended RAM
; 4 $10 VBE 1=CPU accesses main RAM     0=CPU access extended RAM
; 3 $08 Extended RAM bank MSB   00=Bank 0
; 2 $04 Extended RAM bank LSB   11=Bank 3
; 1 $02 1=enable RAM $A000-$BFFF        0=enable BASIC
; 0 $01 1=enable OS ROM                 0=enable RAM $C000-$CFFF and $D800-$FFFF

; On boot, this should be $FF (RAM,everything in main RAM, basic disabled, ROM enabled)
; To bank switch to Bank 3: 1110 1111 = $EF
; And for main RAM:         1111 0011 = $F3
    bank3 = $EF
    mainram = $F3

; The goal of this module is to write a value to $4000 in main RAM
; and a different value to a 130XE high-memory bank.
; If you get the main RAM value after reading $4000,
; then this isn't a 130XE and can just stop loading now.

    .segment "XECHK"

    RTS     ; for older DOSes which unconditionally run the first load chunk

xeCheck:

    dest = $4000

    ; Write a value to the high mem FIRST
    ; if there is no bank-switching, this will get overwritten

    LDA #bank3
    STA PORTB

    LDA #$42
    STA dest

    ; Bankswitch to main RAM, and write a different value
    LDA #mainram
    STA PORTB

    LDA #123
    STA dest

    ; Bankswitch back to high mem, and see what we see
    LDA #bank3
    STA PORTB

    LDA dest
    CMP #$42
    BNE fail    ; This is not a 130 XE!

    ; this is a 130XE. Bankswitch back to main ram, and return to the loader.
    LDA #mainram
    STA PORTB

    RTS

    ; Alas, not a 130XE, so we just hang.
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
    scrcode " 130XE required "
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

    .segment "XECHKHDR"
    .import __XECHK_LOAD__,__XECHK_SIZE__
    .word __XECHK_LOAD__
    .word __XECHK_LOAD__+__XECHK_SIZE__-1

    .segment "XECHKTRL"
    .word INITAD
    .word INITAD+1
    .word xeCheck
