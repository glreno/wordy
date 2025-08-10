; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
;   Bankswitch code for 130XE
;

; Standard bank IDs. These will all be the same value
    .import         TITLE_BANK_ID
    .import         STARTUP_BANK_ID
    .import         FONTDATA_BANK_ID
    .import         CUEBALL_BANK_ID
    .import         TRAMPOLINE_BANK_ID

.export bankswitch130xe0
.export bankswitch130xe1
.export bankswitch130xe2
.export bankswitch130xe3
.export bankswitch130xe4

; OS LOCATIONS

; Location of 130XE bankswitch control byte, PORTB

    _BANKSWITCH = $D301
    INITAD = $02E2
    BOOTP  = 9
    COLDST = $0244
    DOSINI = $C
    MEMTOP = $02E5
    RAMTOP = $006A

    .segment "LOWBSS"
_LAST_BANKSWITCH: .byte 0



    ; LOWCODE is where you put stuff that must NEVER be bankswitched
    .segment "LOWCODE"



; A couple common bankswitches. These are all
; here because they ALL switch to the same bank
; No params, no return. This only works because
; it's the function before _bankswitch.
; void __fastcall__ bankswitchTitle();
; void __fastcall__ bankswitchStartup();
; void __fastcall__ bankswitchFontdata();

_bankswitchTitle:
    .export _bankswitchTitle
_bankswitchStartup:
    .export _bankswitchStartup
_bankswitchGameInit:
    .export _bankswitchGameInit
_bankswitchFontdata:
    .export _bankswitchFontdata

    LDA #<TITLE_BANK_ID

; General bankswitch call, switches to the bank in A
; void __fastcall__ bankswitch(unsigned char bankid);
; void __fastcall__ bankswitchBeforeTrampoline(unsigned char bankid);
_bankswitch:
    .export _bankswitch
    STA _LAST_BANKSWITCH

; The bits of PORTB are:
; 7 $80 1=enable RAM $5000-$57FF        0=enable selftest ROM
; 6 $40 ???
; 5 $20 VBE 1=ANTIC accesses main RAM   0=ANTIC access extended RAM
; 4 $10 VBE 1=CPU accesses main RAM     0=CPU access extended RAM
; 3 $08 Extended RAM bank MSB   00=Bank 0
; 2 $04 Extended RAM bank LSB   11=Bank 3
; 1 $02 1=enable RAM $A000-$BFFF        0=enable BASIC
; 0 $01 1=enable OS ROM                 0=enable RAM $C000-$CFFF and $D800-$FFFF
;
; Four our purposes, ANTIC will always be looking at main memory,
; and we will only bankswitch the CPU. Conveniently, the bits line
; up so that "Bank 4" is the equivalent of setting the CPU to main RAM.
; The other bits will ALWAYS be 111x xx11 = $E3 = 227
; Bank 0: 1110 0011 = $E3 = 227
; Bank 1: 1110 0111 = $E7 = 231
; Bank 2: 1110 1011 = $EB = 235
; Bank 3: 1110 1111 = $EF = 239
; Bank 4: 1111 0011 = $F3 = 243
;
; What this means is we need to take the ID passed in (0..4),
; multiply by FOUR, and ORA $E3


dobankswitch:
    ASL A
    ASL A
    ORA #$E3
    STA _BANKSWITCH
    RTS

_bankswitchBeforeTitleTrampoline:
    .export _bankswitchBeforeTitleTrampoline
    PHA
    LDA #<TITLE_BANK_ID
    BPL bsAndPla
    ; good thing those bank IDs are never negative

_bankswitchBeforeTrampoline:
    .export _bankswitchBeforeTrampoline
    PHA
    LDA #<TRAMPOLINE_BANK_ID
    BPL bsAndPla

_bankswitchAfterTrampoline:
    .export _bankswitchAfterTrampoline
    PHA
    LDA _LAST_BANKSWITCH
bsAndPla:
    JSR dobankswitch
    PLA
    RTS

; Hardcoded functions for people who don't like parameters

bankswitch130xe0:
    LDA #0
    JMP _bankswitch

bankswitch130xe1:
    LDA #1
    JMP _bankswitch

bankswitch130xe2:
    LDA #2
    JMP _bankswitch

bankswitch130xe3:
    LDA #3
    JMP _bankswitch

bankswitch130xe4:
    LDA #4
    JMP _bankswitch

_enableWarmStart:
    .export _enableWarmStart
    LDA #0
    STA COLDST
    LDA #1
    STA 9 ; BOOT? was in to DOS
    LDA #<warmstart
    STA DOSINI
    LDA #>warmstart
    STA DOSINI+1
    RTS

    .import start
    .import _title_show_title_screen
warmstart:
    .export warmstart
    JSR _bankswitchTitle
    ; that will disable BASIC but NOT adjust RAMTOP and MEMTOP.
    ; We KNOW FOR A FACT that this is a 130XE, so can just use constants here.
    LDA #$C0
    STA RAMTOP
    LDA #$BC
    STA MEMTOP+1
    JSR _title_show_title_screen
    JMP start

; These are the segments to insert into the .cfg file to
; tell it to load into each bank
; ....which is what those hardcoded functions are for

    .segment "XEBANK0_LOAD"
    .word INITAD
    .word INITAD+1
    .word bankswitch130xe0

    .segment "XEBANK1_LOAD"
    .word INITAD
    .word INITAD+1
    .word bankswitch130xe1

    .segment "XEBANK2_LOAD"
    .word INITAD
    .word INITAD+1
    .word bankswitch130xe2

    .segment "XEBANK3_LOAD"
    .word INITAD
    .word INITAD+1
    .word bankswitch130xe3

    .segment "XEBANK4_LOAD"
    .word INITAD
    .word INITAD+1
    .word bankswitch130xe4



