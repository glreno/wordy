; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Disable XL/XE built-in BASIC on boot
;

    .include "atari.inc"

    .export xlDisableBasic


; The bits of PORTB are:
; 7 $80 1=enable RAM $5000-$57FF        0=enable selftest ROM
; 6 $40 ???
; 5 $20 VBE 1=ANTIC accesses main RAM   0=ANTIC access extended RAM
; 4 $10 VBE 1=CPU accesses main RAM     0=CPU access extended RAM
; 3 $08 Extended RAM bank MSB   00=Bank 0
; 2 $04 Extended RAM bank LSB   11=Bank 3
; 1 $02 1=enable RAM $A000-$BFFF        0=enable BASIC
; 0 $01 1=enable OS ROM                 0=enable RAM $C000-$CFFF and $D800-$FFFF

; On startup on an XL/XE, PORTB will be one of:
; FD    1111 1101   (selftest disabled; antic and cpu main RAM) (bank 3, enable basic, enable OS)
; FF    1111 1111   (selftest disabled; antic and cpu main RAM) (bank 3, disable basic, enable OS)
; 7F is what happens when it goes to selftest.
; 7D is written temporarily when booting with basic, which enables both selftest and basic
;
; The goal of this module is to write $FF to $D301, and update RAMTOP RAMSIZ MEMTOP accordingly.
; Whether it succeeds or not, we don't care.
; The next step of the load process should be to check the available RAM,
; either xeCheck or the cc65 SYSCHK

    .segment "BASICCHK"

    RTS     ; for older DOSes which unconditionally run the first load chunk

xlDisableBasic:

    ; First of all, make sure this machine isn't a 600XL with no RAM to enable.
    ; This will also reject any 400/800 with 48K or less than 40K.
    ; (if this is a 40K Atari 800, we'll try to disable basic, but it won't work;
    ; this is a VERY rare case!)

    LDA RAMTOP
    CMP #$A0
    BEQ found40K
    ; This machine either has 48K RAM (and thus no BASIC)
    ; or less than 40K (and it will make no difference)
    RTS

found40K:
    LDA #$FF
    STA PORTB

    ; basic is disabled. Write and verify a bit pattern
    ; to $A000 to make sure there actually IS ram there.
    ; (This still might be a 40K Atari 800!)

    LDY #$10
chkloop:
    TYA
    STA tmp
    STA $A000,Y
    LDA $A000,Y
    CMP tmp
    BNE done ; mismatch! Not RAM!
    DEY
    BPL chkloop

    ; OK, I'm convinced there is RAM there
    ; Update the memory size. It WAS $A0,
    ; so RAMSIZ and RAMTOP just need to be set to $C0
    LDA #$C0
    STA RAMSIZ+1
    STA RAMTOP
    ; MEMTOP need to be increased bu $20 as well; it's PROBABLY $9C1F
    CLC
    LDA MEMTOP+1
    ADC #$20
    STA MEMTOP+1
    LDA #$C0

    ; Having moved MEMTOP, re-open the screen
    LDX #0      ; IOCB 0, E:
    LDA #CLOSE
    STA ICCOM,X
    JSR CIOV

    LDA #OPEN
    STA ICCOM,X
    LDA #OPNIN | OPNOT  ; input and output
    STA ICAX1,X
    LDA #0
    STA ICAX2,X
    LDA #<edevice
    STA ICBAL,X
    LDA #>edevice
    STA ICBAH,X
    LDA #<edevice_len
    STA ICBLL,X
    LDA #>edevice_len
    STA ICBLH,X
    JSR CIOV


done:
    RTS
tmp:
    .byte 0
edevice:
        .byte "E:",$9B          ; Device code for screen editor
edevice_len := 3


; Disk headers

    .segment "BASICCHKHDR"
    .import __BASICCHK_LOAD__,__BASICCHK_SIZE__
    .word __BASICCHK_LOAD__
    .word __BASICCHK_LOAD__+__BASICCHK_SIZE__-1

    .segment "BASICCHKTRL"
    .word INITAD
    .word INITAD+1
    .word xlDisableBasic
