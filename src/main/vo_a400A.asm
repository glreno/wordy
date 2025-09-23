; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Base class for animated opponent views

    .include "include/ds_screendriverA.inc"
    .include "include/trampoline.inc"
    .include "zeropage.inc"
    .import _vo4_singleton
    .import _gameModel
    .importzp _DS_ZTMPI
    .importzp _DS_ZTMPIL
    .importzp _DS_ZTMPIH

    .DATA

vor_row_counter:
    .byte 0
vor_quarter_counter_tb:
    .byte 0
vor_quarter_counter_lr:
    .byte 0

    .CODE
   ;.segment "LOWCODE"


_vo4_animateTimerCallback:
    .export _vo4_animateTimerCallback
    .proc vo4_animateTimerCallback


    ; Read the singleton object:
    ; Offset 0 of that pointer is the font to update;
    ; move that pointer into ztmpi
    LDA _vo4_singleton
    LDX _vo4_singleton+1
    STA _DS_ZTMPIL
    STX _DS_ZTMPIH

    ;
    ; update the font
    ;
    ; The screen characters are at offsets $20 and $30
    ;

    ; The row counter runs from 0..3
    LDX vor_row_counter
    CLC
    ; The quarter counter runs from 0..3 but it's
    ; stored as top-bottom 0..1 and left-right 0..1
    LDA vor_quarter_counter_tb
    ASL A
    ADC vor_quarter_counter_lr
    TAY

    JSR drawQuarterScreen

    ; This is the implementation of:
    ;   for tb=0..1
    ;       for row=0..3
    ;           for lr=0..1
    ; but this being assembler we do everything in the other order
    ; and we're only doing one iteration per call! So here we're
    ; just incrementing those counters.
    ; Yes there are faster ways to do this but I'm going for a visual effect
    ; where it updates these two chars like a raster scan

    ; X is still the row counter
    LDY vor_quarter_counter_lr
    INY
    CPY #2
    BNE lr_not_done
        ; both quarters are done; advance to next row
        INX
        CPX #4
        BNE x_not_done
            ; four rows done - advance to next half
            LDX #0
            LDY vor_quarter_counter_tb
            INY
            CPY #2
            BNE tb_not_done
                ; bottom half done
                LDY #0
tb_not_done:
            STY vor_quarter_counter_tb
x_not_done:
        STX vor_row_counter
        ; and start in the first quarter again
        LDY #0
lr_not_done:
    STY vor_quarter_counter_lr


    RTS

drawQuarterScreen:
    ; row offset is in X
    ; quarter ID is in Y
    ; is the puzzle done? Stash the result
    LDA _gameModel,Y
    PHA
    ; calculate the address of this line
    TXA
    CLC
    ADC fontOffsets,Y
    TAY
    PLA
    ; X is the line of the char to draw
    ; Y is the char's offset in the font
    ; A is 0 if the puzzle is complete
    ; is the puzzle done?

    BNE randomize
    LDA circleBmp,X
    JMP draw
randomize:
    ; odd or even line?
    TXA
    AND #1
    BEQ even
odd:
;   LDA #$66  ; turn on random green/grey pixels  0110 0110
    LDA #%01000100
    JMP both
even:
;   LDA #$99  ; turn on random green/grey pixels  1001 1001
    LDA #%00010001
both:
    AND $D20A ; random number generator
draw:
    STA (_DS_ZTMPI),Y
    RTS

fontOffsets:
    .byte $20,$30,$24,$34

circleBmp:
    .byte %00000000
    .byte %00101000
    .byte %00101000
    .byte %00000000

    .endproc
