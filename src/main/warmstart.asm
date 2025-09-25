; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Bankswitch code for 130XE
;

    .import xlDisableBasic
    .import start

; OS LOCATIONS

; Location of 130XE bankswitch control byte, PORTB

    COLDST = $0244
    DOSINI = $C

    .segment "BASICCHK"

_enableWarmStart:
    .export _enableWarmStart
    LDA #0
    STA COLDST
    LDA #1
    STA 9 ; BOOT? was in to DOS
    LDA #<warmstart
    STA DOSINI
    LDA #>warmstart
    STA DOSINI+1
    RTS

    .import start
    .import _title_show_instruction_screen
warmstart:
    .export warmstart
    JSR xlDisableBasic
    JSR _title_show_instruction_screen
    JMP start



