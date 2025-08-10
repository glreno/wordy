; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

.include "../include/cueballmac.inc"
.include "../include/ae_abrupt.inc"
.include "../include/ae_simple.inc"
.include "../include/ae_trailing.inc"
.include "../include/ae_drum.inc"
.include "robot0_envelopes.inc"

; _cueball MUST be in a segment that is accessible to da_playCue, which is trampolined.
; "CUEBALL" needs to either be in the same bank as "TRAMPOLINED", or in
; a non-bankswitched bank along with LOWCODE or RODATA
    .segment "CUEBALL"

NULL = 0

; List of cues to play.

; The structure is da_cue in da_audioDriver.h

; Each da_cue is a list of three pitche values (for AUDF2 AUDF3 AUDF4)
; and pointers to four envelopes (which control AUDC1 AUDC2 AUDC3 AUDC4)

; The reason for the size difference is that AUDF1 is hardcoded to 32 (B5) as a clock control.
; You can still play sounds on AUDF1, you just can't change the pitch.

; _cueball MUST be in a segment that is accessible to da_playCue, which is trampolined.
.export _cueball
_cueball:

; cue cueRest
.byte 0,0,0
.word NULL,NULL,NULL,NULL

; cue cueWordNotFound
; 1 notfoundChord
;    Duration: 400 (two anim frames) + 400 release
.byte E3,C3,0
.word NULL,AE_32_SIMPLE,AE_16REST_16_TRAILING,NULL

; cue cueTadaa
;  2 tadaChord
;    Duration: 3.5 anim frames + 900 release
.byte B4,Ds5,Fs5
.word NULL, env_tadaa, env_tadaa, env_tadaa 

; cue cueSadTrombone
;  3 sadChord
;    Duration: 3.5 anim frames + 900 release
.byte B2,D3,Fs3
.word NULL, env_tadaa, env_tadaa, env_tadaa 

; cue cueLaserGreen
;  4 greenChord
;    Duration: 300 (1.5 anim frames) + 400 release
.byte 0,25,0 ;  31 B5 and 25 something like 26 D#6
.word AE_32_ABRUPT,NULL,AE_32REST_16_TRAILING,NULL 

; cue cueLaserYellow
;  5 yellowChord
;    Duration: 200 (2 anim frames) + 400 release
.byte 0,B4,0
.word NULL,NULL,AE_16_TRAILING,NULL

; cue cueBadKey
;  6 badkeyChord
;    Duration: 100 (1 anim frames) + 8 release
.byte B2,0,0
.word NULL,AE_32_SIMPLE,NULL,NULL

; cue cueRobotsOfDeath_LOW
;  7 Robots of Death Key Clicks - LOW
;    Duration: 8 (one key click)
.byte 0,0,0 ;  B5
.word env_robots_of_death_keys,NULL,NULL,NULL

; cue cueRobotsOfDeath_HIGH
;  8 Robots of Death Key Clicks - HIGH
;    Duration: 8 (one key click)
.byte Fs6,0,0 ;  F#6
.word NULL,env_robots_of_death_keys,NULL,NULL

.import AE_SHUT_UP
AE_8=AE_8_ABRUPT
AE_4=AE_4_ABRUPT
SE_8=AE_8_SIMPLE
SE_4=AE_4_SIMPLE
TE_2=AE_2_TRAILING

; Victory dance

measure 1
note 4,AE_128_DRUM, 0,0, 0,0, 0,0, ""
note 4,AE_128_DRUM, 0,0, 0,0, 0,0, ""
note 4,AE_16_SNARE, 0,0, 0,0, 0,0, ""
note 4,AE_128_DRUM, 0,0, 0,0, 0,0, ""

measure 2
note 4,AE_128_DRUM, 0,0, 0,0, C3,AE_8, "one"
note 4,AE_128_DRUM, 0,0, 0,0, C3,AE_8, "two"
note 4,AE_16_SNARE, 0,0, 0,0, C3,AE_8, "three"
note 4,AE_128_DRUM, 0,0, 0,0, C3,AE_8, "four"

measure 3
note 8,AE_128_DRUM, C4,AE_8, 0,0, C3,AE_8, "YOU"
note 4,AE_16_SNARE, C4,AE_4, 0,0, 0,0, "SAY"
note 8,AE_128_DRUM, A3,SE_8, 0,0, 0,0, "TO"
note 8,AE_128_DRUM, B3,SE_8, 0,0, E3,AE_8, "MAY"
note 4,AE_16_SNARE, B3,AE_4, 0,0, 0,0, "TOE"
note 8,AE_128_DRUM, A3,AE_8, 0,0, 0,0, "AND"

measure 4
note 8,AE_128_DRUM, C4,AE_8, 0,0, C3,AE_8, "I"
note 4,AE_16_SNARE, C4,AE_4, 0,0, 0,0, "SAY"
note 8,AE_128_DRUM, D4,SE_8, 0,0, 0,0, "TO"
note 8,AE_128_DRUM, E4,SE_8, 0,0, E3,AE_8, "MAH"
note 4,AE_16_SNARE, E4,AE_4, 0,0, 0,0, "TOE"
note 8,0,           0,0,     0,0, 0,0, ""
; dotted quarter note there but I'm ignoring it

measure 5
note 8,AE_128_DRUM, 0,0, C5,AE_8, C3,AE_8, "I"
note 4,AE_16_SNARE, 0,0, C5,AE_4, 0,0, "SAY"
note 8,AE_128_DRUM, 0,0, A4,SE_8, 0,0, "PO"
note 8,AE_128_DRUM, 0,0, B4,SE_8, B2,AE_8, "TAY"
note 4,AE_16_SNARE, 0,0, B4,AE_4, 0,0, "TOE"
note 8,AE_128_DRUM, 0,0, A4,AE_8, 0,0, "AND"

measure 6
note 8,AE_128_DRUM, 0,0, C5,AE_8, C3,AE_8, "YOU"
note 4,AE_16_SNARE, 0,0, C5,AE_4, 0,0, "SAY"
note 8,AE_128_DRUM, 0,0, D5,SE_8, 0,0, "PO"
note 8,AE_128_DRUM, 0,0, E5,SE_8, E3,AE_8, "TAH"
note 4,AE_16_SNARE, 0,0, E5,AE_4, 0,0, "TOE"
note 8,0,           E4,SE_8, 0,0, 0,0, "TO"

measure 7
note 8,AE_128_DRUM, G4,SE_8, 0,0, G3,AE_8, "MAY"
note 4,AE_16_SNARE, C4,AE_8, 0,0, 0,0, "TO"
note 8,AE_128_DRUM, 0,0, C5,SE_8, 0,0, "TO"
note 8,AE_128_DRUM, 0,0, E5,SE_8, E3,AE_8, "MAH"
note 4,AE_16_SNARE, 0,0, C5,AE_8, 0,0, "TOE"
note 8,0,           E4,SE_8, 0,0, 0,0, "PO"

measure 8
note 8,AE_128_DRUM, G4,SE_8, 0,0, G3,AE_8, "TAY"
note 4,AE_16_SNARE, C4,AE_8, 0,0, 0,0, "TO"
note 8,AE_128_DRUM, 0,0, C5,SE_8, 0,0, "PO"
note 8,AE_128_DRUM, 0,0, E5,SE_8, E3,AE_8, "TAH"
note 4,AE_16_SNARE, 0,0, C5,AE_8, 0,0, "TOE"
note 8,0,           0,0, 0,0, 0,0, ""

measure 9
note 8,AE_128_DRUM, G4,AE_4, G5,AE_4, 0,0, "LETS"
note 8,AE_16_SNARE, 0,0, 0,0, 0,0, ""
note 8,0,           F4,AE_8, F5,AE_8, 0,0, "CALL"
note 8,AE_128_DRUM, E4,AE_8, E5,SE_8, 0,0, "THE"
note 8,AE_128_DRUM, D4,AE_4, D5,SE_8, 0,0, "WHOLE"
note 8,AE_16_SNARE, 0,0, 0,0, 0,0, ""
note 4,0,           C4,AE_4, C5,AE_4, 0,0, "THING"

measure 10
note 4,AE_128_DRUM, D4,TE_2, D5,TE_2, 0,0, "OFF"
note 4,AE_128_DRUM, 0,0, 0,0, 0,0, ""
note 4,AE_16_SNARE, 0,0, 0,0, 0,0, ""
note 4,AE_128_DRUM, 0,0, 0,0, 0,0, ""

