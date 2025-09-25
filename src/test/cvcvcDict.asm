; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    .segment "DICT_IDX"

; md_lexicon: array of 26 md_wordList
; md_wordList:  int size, byte bankID, md_wordInternal*
; If there is nothing: 5 zero bytes
_cvcvc_lexicon:
    .export _cvcvc_lexicon

    .byte 11 ; lexicon array length

    ; Lex entry 0:B
    .import _wordCountB, _wordsB
    .word _wordCountB
    .byte 'B'
    .byte 0
    .word _wordsB

    ; Lex entry 1:C
    .import _wordCountC, _wordsC
    .word _wordCountC
    .byte 'C'
    .byte 0
    .word _wordsC

    ; Lex entry 2:D
    .import _wordCountD, _wordsD
    .word _wordCountD
    .byte 'D'
    .byte 1
    .word _wordsD

    ; Lex entry 3:F
    .import _wordCountF, _wordsF
    .word _wordCountF
    .byte 'F'
    .byte 1
    .word _wordsF

    ; Lex entry 4:G
    .import _wordCountG, _wordsG
    .word _wordCountG
    .byte 'G'
    .byte 1
    .word _wordsG

    ; Lex entry 5:H
    .import _wordCountH, _wordsH
    .word _wordCountH
    .byte 'H'
    .byte 1
    .word _wordsH

    ; Lex entry 6:J
    .import _wordCountJ, _wordsJ
    .word _wordCountJ
    .byte 'J'
    .byte 1
    .word _wordsJ

    ; Lex entry 7:K
    .import _wordCountK, _wordsK
    .word _wordCountK
    .byte 'K'
    .byte 1
    .word _wordsK

    ; Lex entry 8:L
    .import _wordCountL, _wordsL
    .word _wordCountL
    .byte 'L'
    .byte 2
    .word _wordsL

    ; Lex entry 9:M
    .import _wordCountM, _wordsM
    .word _wordCountM
    .byte 'M'
    .byte 2
    .word _wordsM

    ; Lex entry 10:N
    .import _wordCountN, _wordsN
    .word _wordCountN
    .byte 'N'
    .byte 2
    .word _wordsN


; md_dict: byte number of multilex, then a list of multilex
; md_multilex: byte multiplier, md_lexicon*
_CVDICT:
    .export _CVDICT
    .byte 1
    .byte 1
    .word _cvcvc_lexicon

