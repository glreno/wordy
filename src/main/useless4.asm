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

; This particular version has no main(), but instead
; hijacks the cc65 cartridge init stuff to call memo pad
; instead of cartstart (which is what calls main)



BLKBDV = $E471
PCOLR0 = $02C6

.CODE

; copied from cartinit.s

.export         mycartinit

mycartinit:       rts


; NOT copied from cartstart.s

mycartstart:
;.proc	mycartstart
    LDA #$A0
    STA PCOLR0
    JMP BLKBDV  ; start memopad
;.endproc

; copied from carthdr.s

.import         __CARTSIZE__, __CARTFLAGS__

;.include        "atari.inc"

.segment        "CARTHDR"

                .word   mycartstart       ; start routine
                .byte   0               ; must be zero
                .byte   <__CARTFLAGS__
                .word   mycartinit        ; init routine


