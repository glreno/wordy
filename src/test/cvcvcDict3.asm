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

.segment "CARTBANK2"
    .export _wordsL
_wordsL:
    entry "LABEL"
    entry "LABOR"
    entry "LADEN"
    entry "LAGER"
    entry "LAMER"
    entry "LAPEL"
    entry "LASER"
    entry "LATER"
    entry "LEGAL"
    entry "LEGIT"
    entry "LEMON"
    entry "LEPER"
    entry "LEVEL"
    entry "LEVER"
    entry "LIBEL"
    entry "LICIT"
    entry "LIFER"
    entry "LIKEN"
    entry "LIKER"
    entry "LILAC"
    entry "LIMIT"
    entry "LINEN"
    entry "LINER"
    entry "LITER"
    entry "LIVEN"
    entry "LIVER"
    entry "LOCAL"
    entry "LOGIC"
    entry "LOGIN"
    entry "LOGON"
    entry "LONER"
    entry "LOSER"
    entry "LOVER"
.export _wordCountL = (* - _wordsL)/3

    .export _wordsM
_wordsM:
    entry "MADAM"
    entry "MAGIC"
    entry "MAKER"
    entry "MANIC"
    entry "MANOR"
    entry "MASON"
    entry "MAVEN"
    entry "MAVIN"
    entry "MEDAL"
    entry "MEDIC"
    entry "MELON"
    entry "MERIT"
    entry "METAL"
    entry "METER"
    entry "MILER"
    entry "MIMIC"
    entry "MINER"
    entry "MINIM"
    entry "MINOR"
    entry "MISER"
    entry "MITER"
    entry "MODAL"
    entry "MODEL"
    entry "MODEM"
    entry "MOLAR"
    entry "MORAL"
    entry "MORON"
    entry "MOTEL"
    entry "MOTIF"
    entry "MOTOR"
    entry "MOVER"
.export _wordCountM = (* - _wordsM)/3

    .export _wordsN
_wordsN:
    entry "NABOB"
    entry "NADIR"
    entry "NASAL"
    entry "NATAL"
.export _wordCountN = (* - _wordsN)/3

