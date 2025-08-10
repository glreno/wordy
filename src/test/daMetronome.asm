; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver tester - a metronome made from the key click,
;   that lets you play some other sound every four beats.

.include "atari.inc"
.include "../main/include/ae_simple.inc"


.import _da_keyClick
.import _da_playNote
.import pusha
.import AE_KEYCLICK
.import AE_SHUT_UP

; Application locations in Page Zero

    .ZEROPAGE
    CountdownToClick: .word 0
    .exportzp CountdownToClick
    Beat: .byte 0
    .exportzp Beat
    status: .word 0
    .exportzp status

    .RODATA

    ; Controls for what gets played on the fourth beat

    ; VTIMR1 is the daAudioDriverI interrupt timer, running at P=31 (B5)
    ; VTIMR2 is the daMetronome test interrupt timer, also running at P=31
    ; -- so any sounds you play on voices 1 or two must be at that frequency!
    ; (That limitation is for this application, because of VTIMR2)
    ; Some triad chords that use B:   G#dim   G#+B+D    38,32,27 (remember to subtract 1!)
    ;                                 Bdim    B+D+F     32,27,23
    ;                                 Emaj    E+G#+B    48,38,32
    ;                                 Gmaj    G+B+D     41,32,27
    ;                                 Bmaj    B+D#+F#   32,26,22
    ;                                 Emin    E+G+B     48,41,32
    ;                                 G#min   G#+B+D#   38,32,26
    ;                                 Bmin    B+D+F#    32,27,22
    ;                                 Ebaug   Eb+G+B    51,41,32
    ;                                 Gaug    G+B+D#    41,32,26
    ;                                 Baug    B+D#+F##  32,26,20

    SAMPLE1ENABLE = 1
    SAMPLE2ENABLE = 1
    SAMPLE3PITCH = 26-1 ; D#
    SAMPLE4PITCH = 22-1 ; F#
    SAMPLE1 = AE_16_SIMPLE
    SAMPLE2 = AE_KEYCLICK
    SAMPLE3 = AE_16_SIMPLE
    SAMPLE4 = AE_16_SIMPLE

    .segment "ONCE"

;
;
; INTERRUPT HANDLERS
;
; These all go in LOWCODE, which is guaranteed to never be swapped out
;
;

_METRONOME:
    .proc METRONOME
    .export _METRONOME
    ; OS wwill have pushed A onto the stack
    ; if we need to prodect X and Y do it now

    ; Check to see if it's time to start the next click
    LDA CountdownToClick
    ORA CountdownToClick+1
    BEQ startClick
    LDA CountdownToClick
    SEC
    SBC #1
    STA CountdownToClick
    BCS metronomedone
    DEC CountdownToClick+1
    JMP metronomedone
startClick:
    ; On beat 0, start the other three sounds
    LDA Beat
    BEQ startOtherSounds
    DEC Beat
    ; On all other beats, do a keyboard click
    ; preserve X, it will be used as a param
    TXA
    PHA
    JSR _da_keyClick
    PLA
    TAX
restarttimer:
    ; start the timer to start the next sound after one quarter note ( 1000(0x3E8) 998Hz cycles)
    ; (That works out to 120 bpm, on the low end of allegro)
    ; (whole=3000, half=2000, 8th=500 16th=250 32d=125)
    LDA #$E8
    STA CountdownToClick
    LDA #$3
    STA CountdownToClick+1

metronomedone:
    PLA
    CLI
    RTI

startOtherSounds:
    ; reset the beat counter
    LDA #3
    STA Beat
    TXA
    PHA
play1: ; play sample1 on voice 1
    LDA #SAMPLE1ENABLE
    BEQ play2
    LDA #1
    JSR pusha
    LDA #<SAMPLE1
    LDX #>SAMPLE1
    JSR _da_playNote
play2: ; play sample2 on voice 2
    LDA #SAMPLE2ENABLE
    BEQ play3
    LDA #2
    JSR pusha
    LDA #<SAMPLE2
    LDX #>SAMPLE2
    JSR _da_playNote
play3: ; play sample3 on voice 3
    LDA #SAMPLE3PITCH
    BEQ play4
    STA AUDF3
    LDA #3
    JSR pusha
    LDA #<SAMPLE3
    LDX #>SAMPLE3
    JSR _da_playNote
play4: ; play sample4 on voice 4
    LDA #SAMPLE4PITCH
    BEQ doneOtherSounds
    STA AUDF4
    LDA #4
    JSR pusha
    LDA #<SAMPLE4
    LDX #>SAMPLE4
    JSR _da_playNote
doneOtherSounds:
    PLA
    TAX
    JMP restarttimer

    .endproc

    .CODE

_main:
    .proc main
    .export _main

    ; initialize the metronome to a long time in the future
    LDA #$80
    STA CountdownToClick
    STA CountdownToClick+1

    LDA #<_METRONOME
    STA VTIMR2
    LDA #>_METRONOME
    STA VTIMR2+1

    ; Start timer at the same frequency as the main one
    LDA #31
    STA AUDF2
    LDA #$A0
    STA AUDC2

    ; Start the metronome
    LDA #1
    STA CountdownToClick
    LDA #0
    STA CountdownToClick+1

    ; Enable POKEY interrupt
    LDA #$43
    STA IRQEN ; D20E enable key interrupts (not break key though) and Timers 1 and 2
    STA POKMSK ; shadow of IRQEN

forever:
    CLC
    LDA status
    ADC #1
    STA status
    BCS forever
    INC status+1
    JMP forever
    .endproc
