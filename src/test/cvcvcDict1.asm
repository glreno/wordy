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

.segment "CARTBANK0"
    .export _wordsB
_wordsB:
    entry "BABEL"   ; 0
    entry "BACON"
    entry "BAGEL"
    entry "BAKER"

    entry "BANAL"   ; 4
    entry "BARER"
    entry "BARON"
    entry "BASAL"

    entry "BASER"   ; 8
    entry "BASIC"
    entry "BASIL"
    entry "BASIN"

    entry "BATIK"   ; 12
    entry "BATON"
    entry "BEBOP"
    entry "BEFIT"

    entry "BEFOG"   ; 16
    entry "BEGAN"
    entry "BEGAT"
    entry "BEGET"

    entry "BEGIN"   ; 20
    entry "BEGOT"
    entry "BERET"
    entry "BESET"

    entry "BESOM"   ; 24
    entry "BESOT"
    entry "BEVEL"
    entry "BICEP"

    entry "BIDET"   ; 28
    entry "BIGOT"
    entry "BIKER"
    entry "BISON"

    entry "BONER"   ; 32
    entry "BORER"
    entry "BORON"
    entry "BOSOM"   ; 35
.export _wordCountB = (* - _wordsB)/3

    .export _wordsC
_wordsC:
    entry "CABAL"   ; 36
    entry "CABIN"
    entry "CADET"
    entry "CALIF"

    entry "CAMEL"   ; 40
    entry "CANAL"
    entry "CANON"
    entry "CAPER"

    entry "CAPON"   ; 44
    entry "CARAT"
    entry "CARET"
    entry "CAROL"

    entry "CAROM"   ; 48
    entry "CATER"
    entry "CAVIL"
    entry "CEDAR"

    entry "CIDER"   ; 52
    entry "CIGAR"
    entry "CIVET"
    entry "CIVIC"

    entry "CIVIL"   ; 56
    entry "COLIC"
    entry "COLON"
    entry "COLOR"

    entry "COMER"   ; 60
    entry "COMET"
    entry "COMIC"
    entry "CONIC"

    entry "CORAL"   ; 64
    entry "COVEN"
    entry "COVER"
    entry "COVET"   ; 67
.export _wordCountC = (* - _wordsC)/3

