; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   XEGS Test cartridge
;
; Just an ID number at 0x8004 in each bank.

    .segment "CARTBANK0ID"
    .byte "Bank",0,"Zero",0

    .segment "CARTBANK1ID"
    .byte "Bank",1,"One",0

    .segment "CARTBANK2ID"
    .byte "Bank",2,"Two",0

    .segment "CARTBANK3ID"
    .byte "Bank",3,"Three",0

; Custom boot code

.CODE

.export         mycartinit, _BANKSWITCH

mycartinit:
    LDA #2
    STA _BANKSWITCH
    RTS

_BANKSWITCH = $D500
PCOLR0 = $02C6
DOSVEC = $A
COLDST = $244

.import copydata, start

.segment "CARTBANK2"

mycartstart:
    LDA #1
    STA COLDST ; force coldstart on reset
    LDA #$A0
    STA PCOLR0
    JSR copydata
    JSR start ; run program
    JMP (DOSVEC)

; copied from carthdr.s

.import         __CARTSIZE__, __CARTFLAGS__

.segment        "MYCARTHDR"

                .word   mycartstart       ; start routine
                .byte   0               ; must be zero
                .byte   <__CARTFLAGS__
                .word   mycartinit        ; init routine



