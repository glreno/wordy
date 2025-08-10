; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Bankswitch code for XEGS-32 and XEGS-64 carts
;

; Standard bank IDs. These will all be the same value
    .import         TITLE_BANK_ID
    .import         STARTUP_BANK_ID
    .import         FONTDATA_BANK_ID
    TRAMPOLINE_BANK_ID = 0

; OS LOCATIONS

; Location of XEGS cartridge control byte.

    .export _BANKSWITCH = $D500

    .BSS
_LAST_BANKSWITCH: .byte 0

    ; LOWCODE is where you put stuff that must NEVER be bankswitched
    .segment "LOWCODE"

; A couple common bankswitches. These are all
; here because they ALL switch to the same bank
; No params, no return. This only works because
; it's the function before _bankswitch.
; void __fastcall__ bankswitchTitle();
; void __fastcall__ bankswitchStartup();
; void __fastcall__ bankswitchFontdata();

_bankswitchTitle:
    .export _bankswitchTitle
_bankswitchStartup:
    .export _bankswitchStartup
_bankswitchGameInit:
    .export _bankswitchGameInit
_bankswitchFontdata:
    .export _bankswitchFontdata

    LDA #<TITLE_BANK_ID

; General bankswitch call, switches to the bank in A
; void __fastcall__ bankswitch(unsigned char bankid);
; void __fastcall__ bankswitchBeforeTrampoline(unsigned char bankid);
_bankswitch:
    .export _bankswitch
    STA _LAST_BANKSWITCH
    STA _BANKSWITCH
    RTS

_bankswitchBeforeTitleTrampoline:
    .export _bankswitchBeforeTitleTrampoline
    PHA
    LDA #<TITLE_BANK_ID
    BPL bsAndPla
    ; good thing those bank IDs are never negative

_bankswitchBeforeTrampoline:
    .export _bankswitchBeforeTrampoline
    PHA
    LDA #<TRAMPOLINE_BANK_ID
    BPL bsAndPla

_bankswitchAfterTrampoline:
    .export _bankswitchAfterTrampoline
    PHA
    LDA _LAST_BANKSWITCH
bsAndPla:
    STA _BANKSWITCH
    PLA
    RTS
