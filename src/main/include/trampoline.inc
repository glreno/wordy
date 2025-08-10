; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

; trampoline.inc

; Macros for bankswitch trampoline macros

.import _bankswitchAfterTrampoline
.import _bankswitchBeforeTrampoline
.import _bankswitchBeforeTitleTrampoline



; Macro for wrapping ASM functions in a trampoline.
;
; Replace this kind of thing:
;
;   .segment "CODE"
;   _func:
;       .export _func
;       .proc func
;       (do a bunch of stuff)
;       .endproc
;
; with this:
;
;   trampoline _func,_func_tramp
;       .proc func
;       (do a bunch of stuff)
;       .endproc
;   .segment "CODE"
;
; Note that you need to FOLLOW the function with a .segment declaration!
; The function itself will be in the segment "TRAMPOLINED", which is in cart bank 0.
; The "TRAMPOLINES" segment in the CODE bank will have a nine-byte function that
; bankswitches before and after calling the function.
; 

.macro trampoline name,nameTrampoline
    .segment "TRAMPOLINES"
name:
    .export name
    JSR _bankswitchBeforeTrampoline
    JSR nameTrampoline
    JMP _bankswitchAfterTrampoline
    .segment "TRAMPOLINED"
nameTrampoline:
    .export nameTrampoline
.endmacro


.macro titleTrampoline name,nameTrampoline
    .segment "TRAMPOLINES"
name:
    .export name
    JSR _bankswitchBeforeTitleTrampoline
    JSR nameTrampoline
    JMP _bankswitchAfterTrampoline
    .segment "ONCE"
nameTrampoline:
    .export nameTrampoline
.endmacro
