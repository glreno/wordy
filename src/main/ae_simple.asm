; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver envelope patterns

; Each envelope is a series of byte pairs, ending in 0,0
; The first byte of each pair is the AUDC byte. The high nybble $A0 is a clear tone.
; The low nybble is the volume.
; The second byte is duration, in B5 cycles.
; A sixteenth note is 200 cycles, which is also one animation frame at 10fps

    .segment "DA"

; SIMPLE notes
; Not a complicated waveform. Attack and Decay are 11 cycles combined,
; and release is 32 cycles.
; Sustain is duration minus AD_R (43) cycles
; so this uses the entire duration of the note.

;
;   THIRTYSECONDTH NOTES
;

_AE_32_SIMPLE:
 AE_32_SIMPLE:
    ; Attack, Decay, Sustain, Release = 100 = 1/32 note (half an animation frame)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,57 ; 32th note is 100 cycles; 100-43=57
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_32_SIMPLE
.export  AE_32_SIMPLE

;
; SIXTEENTH NOTES
;

_AE_32REST_16_SIMPLE:
 AE_32REST_16_SIMPLE:
    .byte $A0,100
_AE_16_SIMPLE:
 AE_16_SIMPLE:
    ; Attack, Decay, Sustain = 200 = 1/16 note (half an animation frame); Release=32
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,157 ; 16th note is 200 cycles; 200-43=157
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_16_SIMPLE
.export  AE_16_SIMPLE
.export _AE_32REST_16_SIMPLE
.export  AE_32REST_16_SIMPLE


;
; EIGHTH NOTES
;

_AE_8_SIMPLE:
 AE_8_SIMPLE:
    ; Attack, Decay, Sustain, Release = 400 = 1/8 note (one animation frame)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,157 ; 8th note is 400 cycles; 400-43 = 357
    .byte $A8,200
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_8_SIMPLE
.export  AE_8_SIMPLE

;
; QUARTER NOTES
;

_AE_4_SIMPLE:
 AE_4_SIMPLE:
    ; Attack, Decay, Sustain, Release = 800 = 1/4 note (two animation frames)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,157 ; quarter note is 800 cycles; 800-43 = 757
    .byte $A8,200, $A8,200, $A8,200
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_4_SIMPLE
.export  AE_4_SIMPLE

