; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

; Store letters 2,3,4,5 of the 5-letter string
    .macro entry s
;   .byte .strat(s,1), .strat(s,2), .strat(s,3), .strat(s,4)

    .byte ((.strat(s,1) & $1F)<<3 ) | (( .strat(s,2)&$1F ) >>2 )
    .byte ((.strat(s,2) & $03)<<6 ) | (( .strat(s,3)&$1F ) <<1 ) | (( .strat(s,4)&$10 )>>4)
    .byte (( .strat(s,4)&$0F ) << 4)
    .endmacro

.segment "CARTBANK1"
    .export _wordsD
_wordsD:
    entry "DAVIT"
    entry "DEBAR"
    entry "DEBIT"
    entry "DECAF"
    entry "DECAL"
    entry "DECOR"
    entry "DEFER"
    entry "DEMON"
    entry "DENIM"
    entry "DEPOT"
    entry "DETER"
    entry "DEVIL"
    entry "DIGIT"
    entry "DIMER"
    entry "DINER"
    entry "DIRER"
    entry "DIVAN"
    entry "DIVER"
    entry "DIVOT"
    entry "DONOR"
.export _wordCountD = (* - _wordsD)/3

    .export _wordsF
_wordsF:
    entry "FACET"
    entry "FAKER"
    entry "FAKIR"
    entry "FATAL"
    entry "FAVOR"
    entry "FECAL"
    entry "FELON"
    entry "FERAL"
    entry "FETAL"
    entry "FEVER"
    entry "FIBER"
    entry "FILET"
    entry "FINAL"
    entry "FINER"
    entry "FIVER"
    entry "FOCAL"
.export _wordCountF = (* - _wordsF)/3

    .export _wordsG
_wordsG:
    entry "GAMER"
    entry "GAMIN"
    entry "GAVEL"
    entry "GIVEN"
    entry "GOFER"
    entry "GONER"
.export _wordCountG = (* - _wordsG)/3

    .export _wordsH
_wordsH:
    entry "HABIT"
    entry "HALER"
    entry "HALON"
    entry "HAREM"
    entry "HATER"
    entry "HAVEN"
    entry "HAVOC"
    entry "HELOT"
    entry "HERON"
    entry "HIKER"
    entry "HOGAN"
    entry "HOMER"
    entry "HONOR"
    entry "HOTEL"
    entry "HOVEL"
    entry "HOVER"
.export _wordCountH = (* - _wordsH)/3

    .export _wordsJ
_wordsJ:
    entry "JOKEY"
.export _wordCountJ = (* - _wordsJ)/3

    .export _wordsK
_wordsK:
    entry "KABOB"
    entry "KAPOK"
    entry "KARAT"
    entry "KEBAB"
    entry "KEBOB"
    entry "KOPEK"
.export _wordCountK = (* - _wordsK)/3
