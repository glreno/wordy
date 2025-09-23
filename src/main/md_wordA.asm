; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Dictionary machine language parts

.include "zeropage.inc"
.include "include/trampoline.inc"


    .import incsp2
    .import incsp3
    .import incax1
    .import popax
    .import umul8x8r16
    .import staxptr1
    .import staxptr2
    .import DICT_IDX_ID
    .import _bankswitch

; Compress a word, converting it from md_word to md_wordInternal
; void __fastcall__ md_wordToWordInternal( md_wordInternal *dest, const md_word* src)
;   memcpy(dest->s,src->s+1,4);
trampoline _md_wordToWordInternal,_md_wordToWordInternal_tramp
    .proc md_wordToWordInternal
    ; src is in AX, dest is on the stack

    ; We want to start copying from src->s+1 -- offset 1 from src
    JSR staxptr1
    ; We want to start copying to dest->s -- offset 0 from dest
    JSR popax
    JSR staxptr2

    ; 4-to-3 byte bit packing
    ; We will discard all but the lowest 5 bits of each ascii character,
    ; and then pack them in to 3 bytes. The remaining 4 bits will store the flags.

    ; So for a given five-letter word ABCDE with flags F,
    ; remembering that the first letter is discarded, we get bits:
    ;   1     2         3         4    5    <- src index
    ; bbbb bccc    ccdd ddde    eeee ffff
    ; ^byte 0 ^    ^byte 1 ^    ^byte 2 ^   <- dest index


    ; Start source byte b: (copy from ptr1 to tmp1)
    LDY #1
    JSR prepByteForCompress

    ; Start dest byte 0     bbbbbccc (do nothing, we'll be rotating bits into tmp2)

    ; rotate 5 bits from b to byte 0
    JSR rotate5bits

    ; Start source byte c: (copy from ptr1 to tmp1)
    LDY #2
    JSR prepByteForCompress

    ; rotate 3 bits from c to byte 0
    JSR rotate3bits

    ; Complete dest byte 0 (copy from tmp2 to ptr2)
    LDY #0
    LDA tmp2
    STA (ptr2),Y
    ; Start dest byte 1     ccddddde

    ; rotate 2 bits from c to byte 1
    JSR rotate2bits

    ; Start source byte d: (copy from ptr1 to tmp1)
    LDY #3
    JSR prepByteForCompress

    ; rotate 5 bits from d to byte 1
    JSR rotate5bits

    ; Start source byte e: (copy from ptr1 to tmp1)
    LDY #4
    JSR prepByteForCompress

    ; rotate 1 bits from e to byte 1
    ASL tmp1    ;e0
    ROL tmp2

    ; Complete dest byte 1 (copy from tmp2 to ptr2)
    LDY #1
    LDA tmp2
    STA (ptr2),Y
    ; Start dest byte 2     eeeeffff

    ; The rest of e is already in the top of tmp1,
    ; and the bottom of tmp1 is all zeroes.
    ; No need to copy to tmp2.

    ; Done rotating in bits! OR in the flags

    LDY #5 ; (src index 5 "flags")
    LDA (ptr1),Y
    AND #$0F
    ; OR in e[1..4]
    ORA tmp1
    ; complete dest byte 2
    LDY #2
    STA (ptr2),Y

    RTS

prepByteForCompress:
    ; Load byte and skip the top three bits
    LDA (ptr1),Y
    ASL A
    ASL A
    ASL A
    STA tmp1
    RTS

    .endproc

; MD segment because it has to access all dictionaries
    .segment "MD"

rotate5bits:
    ASL tmp1    ;b0
    ROL tmp2
rotate4bits:
    ASL tmp1    ;b1
    ROL tmp2
rotate3bits:
    ASL tmp1    ;b2
    ROL tmp2
rotate2bits:
    ASL tmp1    ;b3
    ROL tmp2
rotate1bits:
    ASL tmp1    ;b4
    ROL tmp2
    RTS

; Uncompress a word, converting it from md_wordInternal to md_word
;
;   src is going to be someplace in the dictionary banks;
;   we assume that the correct bank is already selected
;
; void __fastcall__ md_wordInternalToWord( md_word *dest, const md_wordList *wl, const md_wordInternal* src)
;   dest->wordflags=0;
;   memcpy(dest->s+1,src->s,4);
;   dest->s[0]=wl->firstLetter;
; src is in ax, wl and dest are on the stack
_md_wordInternalToWord:
    .export _md_wordInternalToWord
    .proc md_wordInternalToWord

    ; We want to start copying from src->s -- offset 0 from src
    JSR staxptr1

    ; Get the addr of the wordlist into ptr3
    ; First letter is at ptr3+2
    JSR popax
    STA ptr3
    STX ptr3+1

    ; We want to start copying to dest->s+1 -- offset 1 from dest
    JSR popax
    JSR staxptr2

    ; First letter - copy from ptr3+2 to ptr2+0
    LDY #2
    LDA (ptr3),Y
    LDY #0
    STA (ptr2),Y
    ; The rest of the copy will start at ptr2+1
    ; no need to increment it here though

    ; 4-to-3 byte bit packing
    ; We will discard all but the lowest 5 bits of each ascii character,
    ; and then pack them in to 3 bytes. The remaining 4 bits will store the flags.

    ; So for a given five-letter word ABCDE with flags F,
    ; remembering that the first letter is discarded, we get bits:
    ;   1     2         3         4    5    <- dest index
    ; bbbb bccc    ccdd ddde    eeee ffff
    ; ^byte 0 ^    ^byte 1 ^    ^byte 2 ^   <- src index


    ; Start src byte 0     bbbbbccc
    LDY #0
    LDA (ptr1),Y
    STA tmp1

    ; Start dest byte b (index 1)
    LDA #0
    STA tmp2

    ; rotate 5 bits from byte 0 to b
    JSR rotate5bits

    ; Complete dest byte b (index 1)
    ; Start dest byte c (index 2)
    LDY #1
    JSR completeAndStartByte

    ; rotate 3 bits from byte 0 to c
    JSR rotate3bits

    ; Start src byte 1     ccddddde
    LDY #1
    LDA (ptr1),Y
    STA tmp1

    ; rotate 2 bits from byte 1 to c
    JSR rotate2bits

    ; Complete dest byte c (index 2)
    ; Start dest byte d (index 3)
    LDY #2
    JSR completeAndStartByte

    ; rotate 5 bits from byte 1 to d
    JSR rotate5bits

    ; Complete dest byte d (index 3)
    ; Start dest byte e (index 4)
    LDY #3
    JSR completeAndStartByte

    ; rotate 1 bit from byte 1 to e
    ASL tmp1    ;d0
    ROL tmp2

    ; Start src byte 2     eeeeffff
    LDY #2
    LDA (ptr1),Y
    STA tmp1

    ; while we have that in A, copy
    ; the flag bits to dest index 5
    AND #$0F
    LDY #5
    STA (ptr2),Y

    ; rotate 4 bits from byte 2 to e
    JSR rotate4bits

    ; Complete dest byte e (index 4)
    LDY #4
    JSR completeAndStartByte

    RTS

completeAndStartByte:
    LDA tmp2
    ORA #$40 ; make it an ascii letter
    STA (ptr2),Y
    LDA #0
    STA tmp2
    RTS

    .endproc

; DICT_IDX segment because it must access that anyway
    .segment "DICT_IDX"

; Add 6 to ptr2
add6ptr2:
    LDA #6
; Add A to ptr2
addAptr2:
    CLC
    ADC ptr2
    STA ptr2
    LDA #0
    ADC ptr2+1
    STA ptr2+1
    RTS

; Copy a md_wordList struct from a lexicon
; void __fastcall__ md_cpWordList(char firstletter, md_wordList *dest, const md_lexicon *src)
;       memcpy(dest,src,6);
; src is in ax, dest and then firstletter are on the stack
_md_cpWordList:
    .export _md_cpWordList
    .proc md_cpWordList

    ; src in ptr2
    STA ptr2
    STX ptr2+1

    ; Get size of lexicon's wordlist array from (ptr2),0
    ; It will be a number from 1..26 ; store it in tmp1
    ; If it's 26, then it's sorted; skip to the sparse list index lookup

    LDY #0
    LDA (ptr2),Y
    STA tmp1

    ; and increment ptr2, so it points to the array
    LDA #1
    JSR addAptr2

    ; Check the size to decide how to search
    LDA tmp1
    CMP #26
    BEQ sparseListMode

searchMode:
    ; SEARCH MODE: look for a matching entry

    ; There aren't 26, so search the array for the matching letter.
    ; This DOES assume an uppercase letter!
    ; If there is no match, return 0.
    ; This is an array of six byte structures. The letter
    ; that we are searching for is byte 2. (Bytes 0,1 are the length
    ; that we will be returning)

    LDY #2
    LDA (sp),Y ; first letter
    ;LDY #2 ; coincidence
    CMP (ptr2),Y
    ; If it matches, we're done!
    BEQ checkwordlistlength

    JSR add6ptr2
    DEC tmp1
    BNE searchMode

    ; If we got this far, we didn't find it.
    ; Return zero.
    LDA #0
    JMP returnZero



sparseListMode:
    ; SPARSE LIST MODE: use first letter is an index

    ; Convert the first letter into an index
    ; and add it to src (ptr2).
    ; We are copying an array of md_wordList, size==6 bytes
    ; The index will be a number int the range (0-25)*6,
    ; which is less than one byte.
    ; But the letter is in ASCII. Could be upper or lower,
    ; don't care, just mask off the lower five bits
    ; and subtract 1
    LDY #2
    LDA (sp),Y ; first letter
    AND #$1F
    SEC
    SBC #1
    ; A is now the index
    ; JMP getwordlist_Aidx


getwordlist_Aidx:
    ; A is now the index
    STA ptr1
    LDA #0
    STA ptr1+1
    ; now multiply it by 6
    LDA #6
    JSR umul8x8r16 ; ptr1 * A, uses ptr3 as scratch; result in AX and ptr1
    ; and add that to ptr2 (the source lexicon)
    JSR addAptr2

checkwordlistlength:
    ; ptr2 now contains the address of the source wordlist
    ; Now would be a good time to bail out if size is zero
    LDY #1
    LDA (ptr2),Y
    DEY
    ORA (ptr2),Y
    BNE getwordlist_ptr2 ; not zero!
    ; Length is zero, so skip the copy and return 0
returnZero:
    TAX
    JMP incsp3

getwordlist_ptr2:
    ; copy dest addr to ptr1
    LDY #0
    LDA (sp),Y
    STA ptr1
    INY
    LDA (sp),Y
    STA ptr1+1

    ; Copy wordlist at ptr2 to ptr1
    LDX #5 ; copy 6 bytes
    LDY #0
loop:
    LDA (ptr2),Y
    STA (ptr1),Y
    INY
    DEX
    BPL loop

    ; All done with the copy
    ; Return the length, which is the first two bytes at ptr1
    LDY #1
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y

    JMP incsp3

    .endproc

; MD segment because it has to access bankswitched dictionary data
    .segment "MD"

; Bankswitch to where the dictionary and volume indices are
; void __fastcall__ md_bankswitchIdx(void);
_md_bankswitchIdx:
    .export _md_bankswitchIdx
    .proc md_bankswitchIdx

    LDA #<DICT_IDX_ID
    JMP _bankswitch

    .endproc

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

; Compare two words. Very much like strcmp except we know that
; there are exactly TWO AND A HALF bytes to compare,
; and that w2 (the search term) is NOT NULL and has no flags.
; signed char md_wordCmp(const md_wordInternal *w1, const md_wordInternal *w2);
; w2 is in AX; w1 is on the stack. Return result in A, 0 in X
_md_wordCmpInternal: ; flags are ignored so the compare is actually the same
    .proc md_wordCmp
    .export _md_wordCmpInternal
    ; There are a lot of returns in here. Get the params into ptr1 and ptr2
    ; and clear the stack NOW instead of at the end
    STA ptr2        ; ptr2 is the search term, never null, and has no flags
    STX ptr2+1
    LDY #1;
    LDA (sp),Y
    STA ptr1+1      ; ptr1 may be null
    DEY
    LDA (sp),Y
    STA ptr1
    JSR incsp2
    LDX #0
; w1==w2 is not worth checking; won't happen in real life,
; and it's two CMP operations
    ; if ( w1==w2 ) return 0; even if both are NULL
    ; (ptr1 LSB is in A right now)
;   CMP ptr2
;   BNE notptrequal
;   LDA ptr2+1
;   CMP ptr1+1
;   BNE notptrequal
;   LDA #0
;   RTS
notptrequal:
; Not worth checking, never happens in real life
    ; if ( w2==NULL) return -1;
;   LDA ptr2
;   ORA ptr2+1
;   BNE w2notnull
;   LDA #$FF
;   TAX
;   RTS
w2notnull:
    ; if ( w1==NULL) return 1;
    LDA ptr1
    ORA ptr1+1
    BNE w1notnull
    LDA #$1
    RTS
w1notnull:

    ; Treat both words as 24-bit integers, and subtract them
    ;
    ; We want to do *ptr1 - *ptr2
    ; There are three bytes, store the lower two in sreg;
    ; we only need the result to check for all-zero.

    ; There are three possible results:
    ; MSB subtract produces negative (BCC), this means 'less than' - return -1
    ; All three results produce 0, this means 'equal' - return 0
    ; Anything else means 'greater than' - return 1

    LDY #2

    ; LSB first
    ; This needs the lower four bits masked off
    ; because they're flags, not part of the word
    ; so can't just do a simple SBC right off the bat

    LDA (ptr1),Y
    AND #$F0        ; mask of the flag nybble
    SEC
    SBC (ptr2),Y
    STA sreg

    ; middle byte
    DEY
    LDA (ptr1),Y
    SBC (ptr2),Y
    STA sreg+1

    ; and the MSB
    DEY
    LDA (ptr1),Y
    SBC (ptr2),Y

    ; Negative result is 'carry clear'
    BCC lessthan

    ; Not negative, check to see if equal
    ORA sreg        ; 2 bytes, 3 cycles
    ORA sreg+1      ; 2 bytes, 3 cycles
    BEQ equals      ; 2 bytes, 2 or 3 cycles

greaterthan:
    LDA #1
    LDX #0
    RTS

lessthan:
    LDA #$FF
equals: ; if we branched here, A=0
    TAX
    RTS

    .endproc
