; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   The opponent that drives a QORDL game

.include "zeropage.inc"
    .import popptr1

    .segment "GAMEINIT"

; Choose the correct dictionary.
; Pass in 'e','n',h','t', and pointers to the title text.
; It will check PAL itself.
; *locale will be set to S or K
; *level will be set to E/N/H/T
; Returns the address of the chosen dictionary.
; md_dict* __fastcall__ md_pickDictionary(char* locale, char *level, unsigned char choice);
_md_pickDictionary:
    .export _md_pickDictionary
    .proc md_pickDictionary

; Original C code:

;   char dictpick;
;   unsigned char idx; // byte offset into array of dictionaries
;   if ( choice == 'e' ) {
;       dictpick='E';
;       idx=0;
;   }
;   else if ( choice == 'h' ) {
;       dictpick='H';
;       idx=8;
;   }
;   else {
;       dictpick='N';
;       idx=4;
;   }
;   *level=dictpick;

; Choice is in A, and is a lower-case letter.
; We want to pick and index and push it onto the stack for later,
; and store the upper-case letter in *level (first ptr on the stack)

    CMP #$65 ; e
    BNE not_e
    LDA #0
    PHA
    LDA #$45 ; E
    BNE store_letter
not_e:
    CMP #$68 ; h
    BNE not_h
    LDA #8
    PHA
    LDA #$48 ; H
    BNE store_letter
not_h:
    CMP #$74 ; t
    BNE not_t
    LDA #$C
    PHA
    LDA #$54 ; T
    BNE store_letter
not_t:
    LDA #4
    PHA
    LDA #$4E ; N
store_letter:
    PHA
    JSR popptr1
    PLA
    LDY #0
    STA (ptr1),Y

    ; Top of cc65 stack is now pointer to 'locale'
    ; 6502 stack contains dict index
    ; load the locale pointer into ptr1, and we're done with the stack.
    JSR popptr1
    LDY #0

;   if ( (GTIA_READ.pal & 0xE) == 0xE )
;   {
;       *locale='S';
;   }
;   else
;   {
;       ++idx;
;       ++idx;
;       *locale='K';
;   }

    LDA $D014 ; 400/800 PAL flag, probably works on XL, it does on Altirra
    AND #$E
    BEQ pal
ntsc:
    LDA #$53 ; S
    STA (ptr1),Y
    PLA
    TAY
    JMP lookup
pal:
    LDA #$4B ; K
    STA (ptr1),Y
    PLA
    TAY
    INY
    INY

;   switch(idx) {
;       case 0: return &DICT_ES_US;
;       case 2: return &DICT_ES_UK;
;       case 4: return &DICT_MS_UK;
;       case 6: return &DICT_MS_US;
;       case 8: return &DICT_HS_US;
;       case 10: return &DICT_HS_UK;
;   }   

lookup:
    INY
    LDA dicttable,Y
    TAX
    DEY
    LDA dicttable,Y

    RTS

dicttable:
    .import _DICT_ES_US
    .import _DICT_ES_UK
    .import _DICT_MS_UK
    .import _DICT_MS_US
    .import _DICT_HS_US
    .import _DICT_HS_UK
    .import _DICT_HA

        ; Dictionary     byte offset
    .word _DICT_ES_US   ; 0
    .word _DICT_ES_UK
    .word _DICT_MS_UK   ; 4
    .word _DICT_MS_US
    .word _DICT_HS_US   ; 8
    .word _DICT_HS_UK
    .word _DICT_HA      ; A
    .word _DICT_HA

    .endproc
