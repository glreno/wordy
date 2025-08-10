; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Bankswitch code for things that don't support bankswitching!
;
; All of these functions are just an RTS.
;


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
; General bankswitch call, switches to the bank in A
; void __fastcall__ bankswitch(unsigned char bankid);
; void __fastcall__ bankswitchBeforeTrampoline(unsigned char bankid);
_bankswitch:
    .export _bankswitch
_bankswitchBeforeTrampoline:
    .export _bankswitchBeforeTrampoline
_bankswitchAfterTrampoline:
    .export _bankswitchAfterTrampoline
_bankswitchBeforeTitleTrampoline:
    .export _bankswitchBeforeTitleTrampoline

    RTS
