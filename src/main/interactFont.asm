; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0


; From Interact Model R rom disassembly, 0x71f to 0x7a0 (lines 1718 to 1879)
; Derived work - I rounded a few letters (B O Q),
; shifted everything over a bit, and inserted some zeroes to turn
; a five-row font into an eight-row font.

    .export _INTERACT_FONT
    .export _INTERACT_FONT_START

    .segment "FONTDATA"

_INTERACT_FONT:

    ; Note that in an Atari font, A is at position 33.
    ; So we need the actual pointer to the font to be
    ; offset a bit from where the assembler actually puts this.
    _INTERACT_FONT_START = _INTERACT_FONT - $0108

; 65 0x41 A
    .byte 0,0
    .byte %00111000
    .byte %01000100
    .byte %01111100
    .byte %01000100
    .byte %01000100
    .byte 0
; 66 0x42 B
    .byte 0,0
    .byte %01111000
    .byte %01000100
    .byte %01111000
    .byte %01000100
    .byte %01111000
    .byte 0
; 67 0x43 C
    .byte 0,0
    .byte %01111100
    .byte %01000000
    .byte %01000000
    .byte %01000000
    .byte %01111100
    .byte 0
; 68 0x44 D
    .byte 0,0
    .byte %01111000
    .byte %01000100
    .byte %01000100
    .byte %01000100
    .byte %01111000
    .byte 0
; 69 0x45 E
    .byte 0,0
    .byte %01111100
    .byte %01000000
    .byte %01110000
    .byte %01000000
    .byte %01111100
    .byte 0
; 70 0x46 F
    .byte 0,0
    .byte %01111100
    .byte %01000000
    .byte %01110000
    .byte %01000000
    .byte %01000000
    .byte 0
; 71 0x47 G
    .byte 0,0
    .byte %01111100
    .byte %01000000
    .byte %01001100
    .byte %01000100
    .byte %01111100
    .byte 0
; 72 0x48 H
    .byte 0,0
    .byte %01000100
    .byte %01000100
    .byte %01111100
    .byte %01000100
    .byte %01000100
    .byte 0
; 73 0x49 I
    .byte 0,0
    .byte %00111000
    .byte %00010000
    .byte %00010000
    .byte %00010000
    .byte %00111000
    .byte 0
; 74 0x4A J
    .byte 0,0
    .byte %00000100
    .byte %00000100
    .byte %00000100
    .byte %01000100
    .byte %01111100
    .byte 0
; 75 0x4B K
    .byte 0,0
    .byte %01000100
    .byte %01001000
    .byte %01110000
    .byte %01001000
    .byte %01000100
    .byte 0
; 76 0x4C L
    .byte 0,0
    .byte %01000000
    .byte %01000000
    .byte %01000000
    .byte %01000000
    .byte %01111100
    .byte 0
; 77 0x4D M
    .byte 0,0
    .byte %01000100
    .byte %01101100
    .byte %01010100
    .byte %01000100
    .byte %01000100
    .byte 0
; 78 0x4E N
    .byte 0,0
    .byte %01000100
    .byte %01100100
    .byte %01010100
    .byte %01001100
    .byte %01000100
    .byte 0
; 79 0x4F O
    .byte 0,0
    .byte %00111000
    .byte %01000100
    .byte %01000100
    .byte %01000100
    .byte %00111000
    .byte 0
; 80 0x50 P
    .byte 0,0
    .byte %01111100
    .byte %01000100
    .byte %01111100
    .byte %01000000
    .byte %01000000
    .byte 0
; 81 0x51 Q
    .byte 0,0
    .byte %00111000
    .byte %01000100
    .byte %01010100
    .byte %01001000
    .byte %00110100
    .byte 0
; 82 0x52 R
    .byte 0,0
    .byte %01111100
    .byte %01000100
    .byte %01111100
    .byte %01001000
    .byte %01000100
    .byte 0
; 83 0x53 S
    .byte 0,0
    .byte %01111100
    .byte %01000000
    .byte %01111100
    .byte %00000100
    .byte %01111100
    .byte 0
; 84 0x54 T
    .byte 0,0
    .byte %01111100
    .byte %00010000
    .byte %00010000
    .byte %00010000
    .byte %00010000
    .byte 0
; 85 0x55 U
    .byte 0,0
    .byte %01000100
    .byte %01000100
    .byte %01000100
    .byte %01000100
    .byte %01111100
    .byte 0
; 86 0x56 V
    .byte 0,0
    .byte %01000100
    .byte %01000100
    .byte %00101000
    .byte %00111000
    .byte %00010000
    .byte 0
; 87 0x57 W
    .byte 0,0
    .byte %01000100
    .byte %01000100
    .byte %01010100
    .byte %01101100
    .byte %01000100
    .byte 0
; 88 0x58 X
    .byte 0,0
    .byte %01000100
    .byte %00101000
    .byte %00010000
    .byte %00101000
    .byte %01000100
    .byte 0
; 89 0x59 Y
    .byte 0,0
    .byte %01000100
    .byte %00101000
    .byte %00010000
    .byte %00010000
    .byte %00010000
    .byte 0
; 90 0x5a Z
    .byte 0,0
    .byte %01111100
    .byte %00001000
    .byte %00010000
    .byte %00100000
    .byte %01111100
    .byte 0
