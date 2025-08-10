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

;
; SIXTEENTH NOTES
;

_AE_16REST_16_TRAILING:
 AE_16REST_16_TRAILING:
    .byte $A0,100
_AE_32REST_16_TRAILING:
 AE_32REST_16_TRAILING:
    .byte $A0,100
_AE_16_TRAILING:
 AE_16_TRAILING:
    ; Attack, Decay, Sustain = 200 = 1/16 note (half an animation frame); Release=400 (1/8)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,189 ; 16th note is 200 cycles; A+D is 11; 200-11=189
    .byte $A7,50, $A6,50, $A5,50, $A4,50, $A3,50, $A2,50, $A1,50, $A0,50
    .byte $A0,1, 0,0
.export _AE_16_TRAILING
.export  AE_16_TRAILING
.export _AE_32REST_16_TRAILING
.export  AE_32REST_16_TRAILING
.export _AE_16REST_16_TRAILING
.export  AE_16REST_16_TRAILING


;
; HALF NOTES
;
    .RODATA ; TODO DA bank is full
_AE_2_TRAILING:
 AE_2_TRAILING:
    ; Attack, Decay, Sustain = 1600 = 1/2 note (four animation frames); Release=400 (1/8)
    .byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1, $A8,1, $A9,1, $AA,1
    .byte $A9,1
    .byte $A8,189 ; half note is 1600 cycles; A+D is 11; 1600-11=1589
    .byte $A8,200, $A8,200, $A8,200
    .byte $A8,200, $A8,200, $A8,200, $A8,200
    .byte $A7,50, $A6,50, $A5,50, $A4,50, $A3,50, $A2,50, $A1,50, $A0,50
    .byte $A0,1, 0,0
.export _AE_2_TRAILING
.export  AE_2_TRAILING

