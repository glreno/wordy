; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Options screen


    .include "zeropage.inc"
    .macpack atari
    .import SFX_LEVEL
    .import _cgetc
    .import pushax
    .import popax
    .import _memcpy
    .import OPTIONS_BANK_ID
    .import _bankswitch
    .import _bankswitchTitle

    .segment "OPTIONS_BANK"

; The screen is 6 lines of 40 bytes of RAM:
;   ptr1+0      LEVEL
;   ptr1+40     a Easy      0 mute all
;   ptr1+80     b Normal    1 key click off
;   ptr1+120    c Hard      2 Normal
;   ptr1+160    d SuperHard 3 Annoying
;   ptr1+200    Press a key
;   ptr1+240    display list

; Screen offsets of the 18-char highlight bars
audoffsets:
    .byte 61,101,141,181
lvloffsets:
    .byte 41,81,121,161
audvalues:
    .byte 0,2,3,4
lvlvalues:
    .byte "enht"

; Screen data, this will be copied to RAM
; Each of these strings is preceded by the screen RAM offset to print at, and length
optionscreentext:
    .export optionscreentext
    scrcode "   LEVEL    WORDS      AUDIO            "
    scrcode "  A  Easy     766     0  Mute           "
    scrcode "  B  Normal  1372     1  Key Click Off  "
    scrcode "  C  Hard    4707     2  Normal         "
    scrcode "  D  Tough   4974     3  Annoying       "
          ;  0123456789012345678901234567890123456789
; Those dictionary sizes are the US numbers; UK are little smaller
; Display list. The start of the display list is copied from the title page.
dlistoffset = * - optionscreentext
optionscreendl:
; Leading blank rows
    .byte $70,$70,$70
; first line (big text) and address of screen RAM
    .byte $47
    .import options_screen_name
    .word options_screen_name
    .byte $70,$70,$70,$70
; real text
    .byte $42
menuoffset = * - optionscreendl
    .word 0 ; screen ram address goes here
    .byte $30
    .byte 2   ; E 0
    .byte 2   ; N 1
    .byte 2   ; H 2
    .byte 2   ; T 3
    .byte $70,$70
    ; need to put an address here!
    .byte $42 ; palette
descoffset = * - optionscreendl
    .word desc_palette ; instruction text address goes here
    .byte 2,2 ; palette
    .byte $70,$70
    .byte $42   ; Press a key
    .word desc_pressakey
    .byte $41
addroffset = * - optionscreendl
; and that will be followed by the dl address
bytestocopy = * - optionscreentext

; swappable screen text:

desc_pressakey:
    scrcode "          Press ENTER to start          "
desc_palette:
    scrcode "To change palette during game, press    "
    scrcode "any digit.  9:high-contrast 0:monochrome"
    scrcode "                                        "
desc_easy:
    scrcode "EASY: The most common words in English  "
    scrcode "                                        "
    scrcode "                                        "
desc_normal:
    scrcode "NORMAL: The most common words in English"
    scrcode "including derived forms.                "
    scrcode "The Easy words are more likely to occur."
desc_hard:
    scrcode "HARD: Almost all 5-letter words, except "
    scrcode "for naughty, rare, archaic, and slang.  "
    scrcode "Easy and Normal words are more likely.  "
desc_tough:
    scrcode "TOUGH: Rare,archaic,slang,naughty words,"
    scrcode "with equal probability. Not safe for    "
    scrcode "live streaming unless you like dead air."
desc_map:
    .word desc_easy, desc_normal, desc_hard, desc_tough
    .BSS
_selectedDictionary:
    .byte 0
    .export _selectedDictionary

    .segment "TRAMPOLINES"

; Show the options screen

; show_options_screen(void *screenram)
_show_options_screen:
    .export _show_options_screen
    .proc show_options_screen

    ; Copy the entire screen text and dlist from ROM to RAM
    ; AX contains dest, and I'll need that later. Push it.
    JSR pushax
    ; And then push it again, because memcpy needs it
    JSR pushax

    ; disable screen before trampolining
    LDY #0
    STY $22F

    ; bankswitch to a place that has room to store this screen
    LDA #<OPTIONS_BANK_ID
    JSR _bankswitch
    JMP show_options_screen_tramp

    .segment "OPTIONS_BANK"

show_options_screen_tramp:
    .export show_options_screen_tramp
    ; Source
    LDA #<optionscreentext
    LDX #>optionscreentext
    JSR pushax
    ; Length
    LDA #<bytestocopy
    LDX #>bytestocopy
    ; Copy
    JSR _memcpy

    ;
    ; Put addresses in the display list
    ;

    JSR popax

    ; AX is now the screen RAM. Store that in ptr1
    ; then add the dlist offset ($C8) to it
    ; (which is easy because A is zero) and store that in ptr2
    STX ptr1+1  ; ptr1 = screen ram
    STA ptr1
    STX ptr2+1  ; ptr2 = dlist
    LDA #dlistoffset
    STA ptr2

;   LDY #menuoffset
;   LDA ptr1        it's a zero so skip copying it
;   STA (ptr2),Y
;   INY
    LDY #menuoffset+1
    LDA ptr1+1
    STA (ptr2),Y
    INY

    ; and finally the address of the dlist
    LDY #addroffset
    LDA ptr2
    STA (ptr2),Y
    INY
    LDA ptr2+1
    STA (ptr2),Y

    ;
    ; Flip to the new dlist
    ;

    LDA ptr2
    STA $230
    LDA ptr2+1
    STA $231
    ; enable normal width screen
    LDA #34
    STA $22F

    ; prepare the selector by storing 'previous' values
    LDY #0   ; both will try to deselect Y=0 and cancel each other out
    STY tmp2 ; tmp2=previous aud highlight y
    STY tmp3 ; tmp3=previous lvl highlight y

    ; Select the default options
    LDA #$62 ; 'b' normal
    JSR doSelect

    LDA #$32 ; '2' normal (NOTE: screen value, the thing we store in SFX_LEVEL is different!!!)
    JSR doSelect

    ; Show the palette key help instead of the Normal dictionary description
    LDY #descoffset
    LDA #<desc_palette
    STA (ptr2),Y
    LDA #>desc_palette
    INY
    STA (ptr2),Y

optionKeyLoop:
    ; now a little loop:
    ; Wait for a key.
    ; If it's a valid letter, change the selected dictionary.
    ; If it's a valid number, change the selected audio level.
    ; If it's enter, return from the function so the game can start

    ; Wait for a key.
    JSR _cgetc
    CMP #155
    BEQ done
    ; If it's a valid letter, change the selected dictionary.
    ; If it's a valid number, change the selected audio level.
    ; If it's enter, return from the function so the game can start
    JSR doSelect
    JMP optionKeyLoop
done:
    JMP _bankswitchTitle
    ; Don't JSR there! This code is in the bank!
    ; implicit return; end of _show_options_screen

doSelect:
    ; highlight the row that is in A
    ; A will be one of: 0123 (audio) or enhs (level)

    ; Convert A into an index in the audoffsets/lvloffsets array.
    ; This means leaving 0123 alone, and turning enhs to 4567
    ; These are all ascii, of course, and the letters could be upper or lower.
    ; The high nybble of an ascii character actually does MEAN something!
    ; 0000 and 0001 = control
    ; 0010          = misc
    ; 0011          = number
    ; 0100 and 0101 = upper case letter
    ; 0110 and 0111 = lower case letter

    STA tmp1

    ; Check bits $30 to see if we have a number here
    LDA #$20
    ; BIT is weird. If ANY of the specified bits are not set, it does NOT set the Z flag.
    ; Which means it can only test for 1 bit, and BNE if that bit is set.
    ; And it sets the V flag to whateever is in bit 6 of whatever is at tmp1
    ; (NOT bit 6 of A!)
    ; Remember the ASCII codes for 'a' and '1' are:
    ;               '1' = $31 = 0011 0001
    ;               'A' = $41 = 0100 0001
    ;               'a' = $61 = 0110 0001
    ; which means bit 5 ($20) indicates "lowercase or digit" and bit 6 ($40) indicates "letter"
    ; and the magic V flag behaviour of BIT is a letter detector.
    BIT tmp1
    BVS isletter
    BNE isdigit

isdigit:
    ; tmp1 contains an ascii digit
    LDA tmp1
    AND #$03 ; and now it contains a number from 0-3
    ; look up the value to store
    TAY
    LDA audvalues,Y
    ; store the selected value
    STA SFX_LEVEL
    ; level 0 is mute, so store a 1 in the mute-all register
    ; actually I probably need to store 0 there otherwise

    ; and update the screen

    ; clear the previous highlight, and
    ; store the new highlight as "previous highlight"
    LDA tmp2 ; tmp2=previous aud highlight Y
    STY tmp2
    ; and highlight
    TAY
    JSR doHighlight
    LDY tmp2
    JMP doHighlight
isletter:
    ; tmp1 contains an ascii letter, which is effectively a number from 1-26.
    ; I want a number from 0-3, so subtract 1 and then mask off the rest
    DEC tmp1
    LDA tmp1
    AND #$03 ; and now it contains a number from 0-3
    STA tmp1 ; keep it around, we need to select the dictionary, update the menu, and update the description
    ORA #4   ; make that 4-7
    ; look up the value to store
    TAY
    LDA audvalues,Y
    ; store the selected value
    STA _selectedDictionary
    ; Y is also the index we need to highlight the menu item, so keep that around

    ; clear the previous highlight, and
    ; store the new highlight as "previous highlight"
    LDA tmp3 ; tmp3=previous highlight, Y=newhighlight
    STY tmp3
    ; and highlight
    TAY
    JSR doHighlight

    ; Load the description
    ; tmp1 is the number from 0..3
    ; Double it to 0..6 and use that as an index into
    ; the table of dictionary descriptions.
    ; Store the address there in the dlist at (ptr2),descoffset

    LDY #descoffset
    LDA tmp1
    ASL A
    TAX
    LDA desc_map,X
    STA (ptr2),Y
    INY
    LDA desc_map+1,X
    STA (ptr2),Y

    ; Get the Y register back from tmp3, and highlight the new row
    ; and we can return after that
    LDY tmp3

; Highlight the block of text starting at audoffsets,Y
doHighlight:
    ; Y contains the offset index
    LDA audoffsets,Y
    TAY
    ; Y is now the offset into screen RAM to start at
    ; screen RAM is in ptr1
    LDX #18
LH1:
    LDA (ptr1),Y
    EOR #$80
    STA (ptr1),Y
    INY
    DEX
    BNE LH1

    RTS ; end of doHighlight and also doSelect

    .endproc
