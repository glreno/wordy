; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    .import _cputs
    .import _cputhex8
    .export _main

    .DATA

title: .byte "PAL/NTSC CHECKS",13,155,0
crlfs: .byte 13,155,0
sp: .byte $20,0

   pal: .byte "   PAL: ",0
palnts: .byte "PALNTS: ",0
ramtop: .byte "RAMTOP: ",0
 portb: .byte " PORTB: ",0
 basic: .byte " BASIC: ",0
  ffee: .byte "System Identification:",13,155,$20,0

    
    .BSS
copyofbasic: .byte 0

    .CODE
    .macro peek8 label,addr
    LDA #<label
    LDX #>label
    JSR _cputs
    LDA addr
    JSR _cputhex8
    .endmac

    .macro crlf
    LDA #<crlfs
    LDX #>crlfs
    JSR _cputs
    .endmac

_main:
    LDA #<title
    LDX #>title
    JSR _cputs
    crlf
    peek8 pal,$D014
    crlf
    peek8 palnts,$62
    crlf
    peek8 ramtop,$6A
    crlf
    peek8 portb,$D301
    crlf
    ; Enable BASIC temporarily to get its ID
    LDA #%11111101
    STA $D301
    LDA $A8E2
    STA copyofbasic
    LDA #%11111111
    STA $D301
    ; BASIC version. $60=rev B, $EA=rev C, $8A=Altirra, 00=no cart
    peek8 basic,copyofbasic
    crlf
    crlf
    peek8 ffee,$FFEE
    peek8 sp, $FFEF
    peek8 sp, $FFF0
    peek8 sp, $FFF1
    peek8 sp, $FFF2
    peek8 sp, $FFF3
    peek8 sp, $FFF4
    peek8 sp, $FFF5
    peek8 sp, $FFF6
    peek8 sp, $FFF7
    peek8 sp, $FFF8
    peek8 sp, $FFF9
    crlf
infiniteloop:
    JMP infiniteloop
