; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Cueball macros
;
; Use these to simplify transcribing music into a cueball file.

; Before each measure, set its ID with: measure(7)
; Each note (or set of simultaneous notes) can be composed thus:
; note 8,drum, C5,DA_8th, 0,0, 0,0, "YOU"
;
;      ^ ^     ^  ^       ^ ^  ^ ^  ^
;      | |     |  |       | |  | |  |
;      | |     |  |       | |  | |  +- Lyric, this will just appear as a comment
;      | |     |  |       | |  | |  |
;      | |     |  |       | |  | +---- Voice 4 Envelope
;      | |     |  |       | |  +------ Voice 4 Note
;      | |     |  |       | |
;      | |     |  |       | +--------- Voice 3 Envelope
;      | |     |  |       +----------- Voice 3 Note
;      | |     |  |
;      | |     |  +------------------- Voice 2 Envelope
;      | |     +---------------------- Voice 2 Note
;      | |
;      | +---------------------------- Voice 1 Envelope
;      +------------------------------ Duration before the next note
;
; The note is a single byte; use a macro from da_notes.inc
; The envelope is a pointer to an envelope; see da_envelopes.inc

;
; There is no Voice 1 note, it is hardcoded to B5.
; The duration should probably be the length of the shortest note.
; 

; In the assembled code, that macro will expand to:

;       ; measure07_0 "YOU"
;       .byte C5,0,0
;       .word drum,DA_8th,0,0

; The cueballtoinc.groovy script will read the same macros when generating
; its list of indices. Since measure07_0 is a note, the script generates
; (assuming the current index is 327):
;       .export measure07_0 = 327
;       .export measure07_1 = 0
; The next note will have index 328, and symbol measure07_2
;
; The number of zero parameters is based on the duration parameter;
; the number is (16/duration)-1 so 8 generates one, 4 generates 3.
;
; To use this index in an anim file, set "measure":"measure07".
; The animtoasm script will set "sound":"measure07_0" through
; "sound":"mySongMeasure01_F" on that frame, and the subsequent ones
; up through the calculated duration.

.include "../include/da_notes.inc"

measurecounter .set 0
measureid .set 0

.macro measure id
    ; name not used by the assembler
    measurecounter .set 0
    measureid .set id
.endmacro

.macro note duration, v1e, v2p,v2e,  v3p,v3e,  v4p,v4e, lyric
    ; lyric and duration not used by assembler
    .byte v2p,v3p,v4p
    .word v1e,v2e,v3e,v4e
.endmacro
