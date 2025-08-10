; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   The World's Most Useless Cartridge

; All this thing does is boot up,
; change the background color,
; and then JMP $E471.
; Useless.
; That triggers Self Test on an XL
; but on an Atari 400/800, it is
; ATARI COMPUTER - MEMO PAD


	.export		_main
BLKBDV = $E471
PCOLR0 = $02C6


.proc	_main: near
.CODE
    LDA #$A0
    STA PCOLR0
    JMP BLKBDV  ; start memopad
.endproc
