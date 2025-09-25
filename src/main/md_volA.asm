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
    .import incsp3
    .import umul8x8r16
    .import decax1
    .import popptr1
    .import push0
    .import pushax
    .import pushw0sp
    .import ldaxysp
    .import ldaxi
    .import staxptr1
    .import _md_wordListSize
    .import _md_wordListFindRecurse
    .import _md_wordListGetInternal
    .import _md_wordInternalToWord

; MD segment because it has to access bankswitched dictionary data
    .segment "MD"

; int md_wordListFind(const md_wordList *d, const md_wordInternal *s)
;     int sz = md_wordListSize(d);
;     return md_wordListFindRecurse(d,s,0,sz-1);
; Just a wrapper around the recursive call
_md_wordListFind:
    .export _md_wordListFind
    .proc md_wordListFind

    ; d is already on the stack, push s on top of that
    JSR pushax
    ; Stack: TOP-> s d
    JSR push0 ; push int 0
    ; Stack: TOP-> 0 s d

;     int sz = md_wordListSize(d);
    LDY #5
    JSR ldaxysp
    ; JSR _md_wordListSize
    ; oops, that's a C macro, it just loads d->size, which is offset 0 from ptr1
    JSR ldaxi ; store ax in ptr1, ldax from (ptr1),0

; sz is in ax; subtract 1
    JSR decax1

    ; AX:sz-1   Stack: TOP-> 0 s d
    JMP _md_wordListFindRecurse
    ; that will pop all three things off the stack and return

    .endproc

; void md_wordListCopyWord(md_word *dest, const md_wordList *wl, int index)
;
;   const md_wordInternal *w=md_wordListGetInternal(wl,index);
;   if null set dest->flags=FF and return
;   md_wordInternalToWord(dest,w);

; AX: index Stack: TOP-> wl dest
; ...which is conveniently what you need to make the first call

_md_wordListCopyWord:
    .export _md_wordListCopyWord
    .proc md_wordListCopyWord

    ; need to duplicate the wl on the top of the stack
    ; AX: index Stack: TOP-> wl dest

    PHA
    TXA
    PHA
    JSR pushw0sp     ; dup top word on stack
    PLA
    TAX
    PLA

    ; AX: index Stack: TOP-> wl wl dest
    JSR _md_wordListGetInternal
    ; AX: w   Stack: TOP-> wl dest

    JSR staxptr1    ; ptr1 is now w
    ; AX: w   ptr1:w   Stack: TOP-> wl dest

    ORA ptr1+1      ; null check
    BNE notnull

    ; it was null; set flags=FF and return
    JSR popptr1
    ; AX: w   ptr1:w   Stack: empty

	LDA #$FF
	LDY #5          ; w->flags
	STA (ptr1),y
	LDA #0
    LDY #1
	STA (ptr1),y
    RTS

notnull:
    ; AX: ?,>w   ptr1:w  Stack: TOP-> wl dest

    ; Clear the flag byte
    LDA ptr1
    ;LDX ptr1+1 ; this is already there
    ; AX: w   Stack: TOP-> wl dest
    ; which is conveniently what you need to call md_wordInternalToWord(dest,wl,w)
    JMP _md_wordInternalToWord

    .endproc




