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

; ABRUPT NOTES
; These notes cut off 1/32 (100 cycles) BEFORE the allotted time,
; with a much truncated Release. A+D=11 and Release=8;
; Susteain is duration minus (11+8+100) = 119
; but otherwize match the SIMPLE notes.

;
;   THIRTYSECONDTH NOTES
;

_AE_32_ABRUPT:
 AE_32_ABRUPT:
    ; Attack, Decay, Sustain = 100 = 1/32 note (half an animation frame); Release=8
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,31 ; 32th note is 100 cycles; A+D is 11; 100-119 < 0 so use 100-69=31
    .byte $A7,1, $A6,1, $A5,1, $A4,1, $A3,1, $A2,1, $A1,1, $A0,1
    .byte $A0,1, 0,0
.export _AE_32_ABRUPT
.export  AE_32_ABRUPT


;
; SIXTEENTH NOTES
;


;
; EIGHTH NOTES
;

_AE_8_ABRUPT:
 AE_8_ABRUPT:
    ; Attack, Decay, Sustain, Release = 400 = 1/8 note (one animation frame)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,81 ; 8th note is 400 cycles; 400-119 = 281
    .byte $A8,200
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_8_ABRUPT
.export  AE_8_ABRUPT

;
; QUARTER NOTES
;

_AE_4_ABRUPT:
 AE_4_ABRUPT:
    ; Attack, Decay, Sustain, Release = 800 = 1/4 note (two animation frames)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,81 ; quarter note is 800 cycles; 800-69 = 681
    .byte $A8,200, $A8,200, $A8,200
    .byte $A7,4, $A6,4, $A5,4, $A4,4, $A3,4, $A2,4, $A1,4, $A0,4
    .byte $A0,1, 0,0
.export _AE_4_ABRUPT
.export  AE_4_ABRUPT

