; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Dictionary machine language parts

.include "zeropage.inc"


    .import incsp2
    .import staxptr1

; MD segment because it has to access bankswitched dictionary data
    .segment "MD"

; Copy md_word.s to a buffer. dest must point to a char[] of length 6 or more.
; void md_wordToString(char *dest, const md_word *src);
; src is in ax; copy to ptr1
; dest is on stack at (sp),0; copy to ptr2
_md_wordToString:
    .proc md_wordToString
    .export _md_wordToString
    ; copy params to ptr1 and ptr2
    JSR staxptr1
    LDY #1;
    LDA (sp),Y
    STA ptr2+1
    DEY
    LDA (sp),Y
    STA ptr2
    ; Trailing 0
    LDY #5
    LDA #0
    STA (ptr2),Y
    DEY
    ; Copy the string
loop:
    LDA (ptr1),Y
    STA (ptr2),Y
    DEY
    BPL loop
    ; all done, clear the stack
    JMP incsp2
    .endproc

; MD segment because it has to access bankswitched dictionary data
    .segment "MD"

; Compare two words. Very much like strcmp.
; signed char md_wordCmp(const md_word *w1, const md_word *w2);
; w2 is in AX; w1 is on the stack. Return result in A, 0 in X
_md_wordCmpInternal: ; flags are ignored so the compare is actually the same
_md_wordCmp:
    .proc md_wordCmp
    .export _md_wordCmp
    .export _md_wordCmpInternal
    ; There are a lot of returns in here. Get the params into ptr1 and ptr2
    ; and clear the stack NOW instead of at the end
    STA ptr2
    STX ptr2+1
    LDY #1;
    LDA (sp),Y
    STA ptr1+1
    DEY
    LDA (sp),Y
    STA ptr1
    JSR incsp2
    LDX #0
    ; if ( w1==w2 ) return 0; even if both are NULL
    ; (ptr1 is in A right now)
    CMP ptr2            ; AB58
    BNE notptrequal
    LDA ptr2+1
    CMP ptr1+1
    BNE notptrequal
    LDA #0
    RTS
notptrequal:
    ; if ( w2==NULL) return -1;
    LDA ptr2
    ORA ptr2+1
    BNE w2notnull
    LDA #$FF
    TAX
    RTS
w2notnull:
    ; if ( w1==NULL) return 1;
    LDA ptr1
    ORA ptr1+1
    BNE w1notnull
    LDA #$1
    RTS
w1notnull:
    ; for each char 0..4
    ;   d=w1[i]-d2[i]
    ;   if (d!=0) return d
    ; They're equal! Return 0
    LDX #5
    LDY #0
loop:
    SEC
    LDA (ptr1),Y
    SBC (ptr2),Y
    BEQ endofloop
    BMI w2first
    ; They aren't equal, and w1 is bigger. Return the difference.
    LDX #0
    RTS
w2first:
    ; diff is negative, so w2 is bigger. Return the difference
    LDX #$FF
    RTS
endofloop:
    INY
    DEX
    BNE loop
    ; All done! Return 0, X is 0 already (is A too?)
    LDA #0
    RTS
    .endproc

