; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver machine language parts - rarely used

.include "atari.inc"
.importzp sp

; Application locations in Page Zero

    .importzp _DA_VOICE1_ENV
    .import _playNote

; DA segment because it is busywaiting, and could be here a while
    .segment "DA"

; void __fastcall__ da_playNoteAndWait(char voice, const char envelope[]);
; envelope ptr is in ax, voice ID is on the stack
_da_playNoteAndWait:
    .proc da_playNoteAndWait
    .export _da_playNoteAndWait
    jsr _da_playNote
loop:
    LDA _DA_VOICE1_ENV+1
    BNE loop
    RTS

    .endproc

