; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver envelope patterns
;   Definitions are in ae_trailing.asm

; Each envelope is a series of byte pairs, ending in 0,0
; The first byte of each pair is the AUDC byte. The high nybble $A0 is a clear tone.
; The low nybble is the volume.
; The second byte is duration, in B5 cycles.
; A sixteenth note is 200 cycles.

    .RODATA

;
; SIXTEENTH NOTES
;

.import _AE_16REST_16_TRAILING
.import  AE_16REST_16_TRAILING
.import _AE_32REST_16_TRAILING
.import  AE_32REST_16_TRAILING
.import _AE_16_TRAILING
.import  AE_16_TRAILING

;
; HALF NOTES
;
.import _AE_2_TRAILING
.import  AE_2_TRAILING

