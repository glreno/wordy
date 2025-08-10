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

    .segment "CARTBANK0"
    .byte "Bank",0,"Zero",0

    .segment "CARTBANK1"
    .byte "Bank",1,"One",0

    .segment "CARTBANK2"
    .byte "Bank",2,"Two",0

    .segment "CARTBANK3"
    .byte "Bank",3,"Three",0

    .segment "CARTBANK4"
    .byte "Bank",4,"Four",0

    .segment "CARTBANK5"
    .byte "Bank",5,"Five",0

    .segment "CARTBANK6"
    .byte "Bank",6,"Six",0

; Bank 7 is the main RODATA segment
;   .segment "CARTBANK7"
;   .byte "Bank",7,"Seven",0
