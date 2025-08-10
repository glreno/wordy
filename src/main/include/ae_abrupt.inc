; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver envelope patterns
;   Definitions are in ae_abrupt.asm

; Each envelope is a series of byte pairs, ending in 0,0
; The first byte of each pair is the AUDC byte. The high nybble $A0 is a clear tone.
; The low nybble is the volume.
; The second byte is duration, in B5 cycles.
; A sixteenth note is 200 cycles.

    .RODATA

;
; THIRTYSECONDTH NOTES
;

.import _AE_32_ABRUPT
.import  AE_32_ABRUPT


;
; SIXTEENTH NOTES
;

;
; EIGHTH NOTES
;

.import _AE_8_ABRUPT
.import  AE_8_ABRUPT

;
; QUARTER NOTES
;

.import _AE_4_ABRUPT
.import  AE_4_ABRUPT

