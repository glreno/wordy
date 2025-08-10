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

;    .segment "DA"
; moved to RODATA to make room in the main cart bank
    .RODATA


;
; ONETWENTYEIGHTH NOTES
;

_AE_128_DRUM:
 AE_128_DRUM:
    ; Attack, Decay: 8; Sustain:0 Release:15   = 25, a 128th note
    .byte $22,1, $24,1, $26,1, $28,1,    $2A,1, $2C,1, $2E,1, $2A,1
    .byte $28,2, $27,2, $26,2, $25,2,    $24,2, $23,2, $21,3
    .byte $A0,0, 0,0
.export _AE_128_DRUM
.export  AE_128_DRUM

;
; SIXTEENTH NOTES
;

; works as a simple drum at pitch 32
; works as a very low snare drum at pitch 32
_AE_16_SNARE:
 AE_16_SNARE:
    ; Attack, Decay: 50; Sustain:0 Release:148   = 198, a 16th note
    .byte $42,5,  $43,5,  $44,5,  $45,5,     $46,5,  $47,5,  $48,5,  $49,5,  $4A,5
    .byte $49,9 , $48,9 , $47,10, $46,10,    $45,11, $44,13, $43,16, $42,20, $41,50
    .byte $A0,0, 0,0
.export _AE_16_SNARE
.export  AE_16_SNARE

;
; EIGHTH NOTES
;

; snare-with-brush at pitch 1
_AE_8_SNAREBRUSH:
 AE_8_SNAREBRUSH:
    ; Attack, Decay: 50; Sustain:0 Release:350   = 400, a 8th note
    .byte $02,5,  $03,5,  $04,5,  $05,5,     $06,5,  $07,5,  $08,5,  $09,5,  $0A,5
    .byte $09,18, $08,18, $07,20, $06,21,    $05,23, $04,27, $03,32, $02,41, $01,100
    .byte $A0,0, 0,0
.export _AE_8_SNAREBRUSH
.export  AE_8_SNAREBRUSH


