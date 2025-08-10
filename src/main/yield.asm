; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Simple two-thread cooperative multitasking

    _YIELDVEC = _yield+1
    .export _YIELDVEC


    .DATA
; Call the secondary thread
; This function is in .DATA so that the address can be changed
; void __fastcall__ yield();
_yield:
    .export _yield
    JMP yieldreturn ; _YIELDVEC points to this address


    .segment "ONCE"

; Register a function to be the seconday thread
; void __fastcall__ setYieldFunc(void *func);
; func should be address of a function that takes no params and returns void

_setYieldFunc:
    .export _setYieldFunc
    STA _YIELDVEC
    STX _YIELDVEC+1
yieldreturn:
    ; _YIELDVEC points here by default
    RTS
