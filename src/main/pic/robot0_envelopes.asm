; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver envelope patterns
;   Definitions are in da_envelopes.asm

; Each envelope is a series of byte pairs, ending in 0,0
; The first byte of each pair is the AUDC byte. The high nybble $A0 is a clear tone.
; The low nybble is the volume.
; The second byte is duration, in B5 cycles.
; A sixteenth note is 200 cycles, which is also one animation frame at 10fps

; AUDC value for a rest
rest = $A0

    .RODATA

.export _env_robots_of_death_keys
.export env_robots_of_death_keys
_env_robots_of_death_keys:
 env_robots_of_death_keys:
.byte $A8,8,   $A0,0,0,0

.export _env_tadaa
.export env_tadaa
_env_tadaa:
 env_tadaa:
; This is a 16th note, followed by a 32d rest, then an eighth note
    ; Attack:7 Delay:1 Sustain:192 Release:0 = 200
.byte $A1,1, $A2,1, $A3,1, $A4,1, $A5,1, $A6,1, $A7,1
.byte $A6,193 ; 200=1/16note

    ; rest for a thirtysecond
.byte rest,100

    ; Attack:2 Delay:0 Sustain:498
.byte $A4,1, $A8,1
.byte $AA,198, $AA,200 ; 200*2 = eigth

    ; Release: 900
.byte $A9,100, $A8,100, $A7,100, $A6,100, $A5,100, $A4,100, $A3,100, $A2,100, $A1,100
.byte rest,0, 0,0

