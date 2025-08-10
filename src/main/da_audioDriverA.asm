; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Audio Driver machine language parts

.include "atari.inc"
.include "include/trampoline.inc"
.import incsp1
.import pusha
.import staxptr1
.include "zeropage.inc"

; Application locations in Page Zero

    .importzp _DA_VOICE1_COUNTER
    .importzp _DA_VOICE1_Y
    .importzp _DA_VOICE1_ENV
    .importzp _DA_VOICE2_COUNTER
    .importzp _DA_VOICE2_Y
    .importzp _DA_VOICE2_ENV
    .importzp _DA_VOICE3_COUNTER
    .importzp _DA_VOICE3_Y
    .importzp _DA_VOICE3_ENV
    .importzp _DA_VOICE4_COUNTER
    .importzp _DA_VOICE4_Y
    .importzp _DA_VOICE4_ENV

; Constant application data
    .RODATA
_AE_KEYCLICK: ; this is a six byte sequence including the shutup
 AE_KEYCLICK:
    .byte $A4,8
_AE_SHUT_UP:
 AE_SHUT_UP:
    .byte $A0,8, 0,0
    .export _AE_KEYCLICK
    .export  AE_KEYCLICK
    .export _AE_SHUT_UP
    .export  AE_SHUT_UP

; Application data
    .DATA
_DA_QUIET:
 DA_QUIET:
    .byte 0
    .export _DA_QUIET
    .export  DA_QUIET


;
;
; Main-thread code for playing sounds
;
;
    .segment "DA"

; Convenience call for da_playNote(1, AE_KEYCLICK)
; void __fastcall__ da_keyClick(void);
trampoline _da_keyClick,_da_keyClick_tramp
    .proc da_keyClick_tramp
    LDA #1
    JSR pusha
    LDA #<AE_KEYCLICK
    LDX #>AE_KEYCLICK
    JSR _da_playNote_tramp
    RTS
    .endproc

    .segment "DA"

; Play a note.
; Valid voices are 1,2,3,4 (to match what the OS calls them)
; envelope is a list of envelope commands, ending in 0,0
; A envelope command is two byte: and AUDC value, and a number of cycles to delay before the next command.
; The delay is a number of cycles of pitch 31 (B5).
; Delay 1 is 'next cycle'; Delay 0 is 255 cycles!
;   Whole Note:    4000
;   Half Note:     2000
;   Quarter Note:  1000
;   Eighth Note:    500
;   Sixteenth Note: 250
; Use audf2,audf3,audf4 to change pitch.
; Voice 1 is a timer. Do NOT change audf1 from 31.
; void __fastcall__ da_playNote(char voice, const char envelope[]);
; envelope ptr is in ax, voice ID is on the stack

trampoline _da_playNote,_da_playNote_tramp
    .proc da_playNote_tramp

    ; If muted, replace the envelope pointer with SHUTUP
    ; Can't just return because people are waiting for the sound to finish.
    ; But things do go a LOT faster when the sound is replaced by "shut up".
    LDY _DA_QUIET
    BEQ not_muted
    LDA #<AE_SHUT_UP
    LDX #>AE_SHUT_UP

not_muted:
    ; Which voice is this?
    PHA
    LDY #0
    LDA (sp),Y
    CMP #1
    BEQ voice1
    CMP #2
    BEQ voice2
    CMP #3
    BEQ voice3
    ; else voice4

    ; When starting a voice, set ENV+1 last -- that's the flag that starts things going
voice4:    ; Store the envelope pointer in the Page 0 pointer for Voice 4
    LDA #0
    STA _DA_VOICE4_Y
    LDA #1
    STA _DA_VOICE4_COUNTER
    PLA
    STA _DA_VOICE4_ENV
    STX _DA_VOICE4_ENV+1
    JMP incsp1
voice3:    ; Store the envelope pointer in the Page 0 pointer for Voice 3
    LDA #0
    STA _DA_VOICE3_Y
    LDA #1
    STA _DA_VOICE3_COUNTER
    PLA
    STA _DA_VOICE3_ENV
    STX _DA_VOICE3_ENV+1
    JMP incsp1
voice2:    ; Store the envelope pointer in the Page 0 pointer for Voice 2
    LDA #0
    STA _DA_VOICE2_Y
    LDA #1
    STA _DA_VOICE2_COUNTER
    PLA
    STA _DA_VOICE2_ENV
    STX _DA_VOICE2_ENV+1
    JMP incsp1
voice1:    ; Store the envelope pointer in the Page 0 pointer for Voice 1
    LDA #0
    STA _DA_VOICE1_Y
    LDA #1
    STA _DA_VOICE1_COUNTER
    PLA
    STA _DA_VOICE1_ENV
    STX _DA_VOICE1_ENV+1
    JMP incsp1

    .endproc

    .segment "DA"

; Is the voice still doing something?
; char __fastcall__ da_isVoiceBusy(char voice);
trampoline _da_isVoiceBusy,_da_isVoiceBusy_tramp
    .proc da_isVoiceBusy_tramp
    CMP #1
    BEQ voice1
    CMP #2
    BEQ voice2
    CMP #3
    BEQ voice3
    ; else voice4
voice4:
    LDA _DA_VOICE4_ENV+1
    JMP check
voice3:
    LDA _DA_VOICE3_ENV+1
    JMP check
voice2:
    LDA _DA_VOICE2_ENV+1
    JMP check
voice1:
    LDA _DA_VOICE1_ENV+1
check:
    BEQ nope
    LDA #1
    RTS
nope:
    LDA #0
    RTS
    .endproc

; Play up to four notes at once
; void __fastcall__ da_playCue( da_cue *chord );
trampoline _da_playCue,_da_playCue_tramp
    .proc da_playCue_tramp
    JSR staxptr1
    ; Voice 1: audf1, (fixed freq)   and chord->env[0] (offset 3,4)
    ; Voice 2: audf2, chord->freq[0] and chord->env[1] (offset 5,6)
    ; Voice 3: audf3, chord->freq[1] and chord->env[2] (offset 7,8)
    ; Voice 4: audf4, chord->freq[2] and chord->env[3] (offset 9,A)

    ; play voice 1 if not null
voice1:
    LDY #4 ; chord->env[0] for voice 1
    LDA (ptr1),Y
    DEY
    ORA (ptr1),Y
    BEQ voice2
    LDA #1 ; Playing voice 1
    JSR pusha
    LDY #4 ; chord->env[0] for voice 1
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR _da_playNote_tramp
voice2:
    LDY #6 ; chord->env[1] for voice 2
    LDA (ptr1),Y
    DEY
    ORA (ptr1),Y
    BEQ voice3
    LDY #0 ; chord->freq[0] for voice 2
    LDA (ptr1),Y
    STA AUDF2
    LDA #2 ; Playing voice 2
    JSR pusha
    LDY #6 ; chord->env[1] for voice 2
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR _da_playNote_tramp
voice3:
    LDY #8 ; chord->env[2] for voice 3
    LDA (ptr1),Y
    DEY
    ORA (ptr1),Y
    BEQ voice4
    LDY #1 ; chord->freq[1] for voice 3
    LDA (ptr1),Y
    STA AUDF3
    LDA #3 ; Playing voice 3
    JSR pusha
    LDY #8 ; chord->env[2] for voice 3
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR _da_playNote_tramp
voice4:
    LDY #$A ; chord->env[3] for voice 4
    LDA (ptr1),Y
    DEY
    ORA (ptr1),Y
    BEQ done
    LDY #2 ; chord->freq[2] for voice 4
    LDA (ptr1),Y
    STA AUDF4
    LDA #4 ; Playing voice 4
    JSR pusha
    LDY #$A ; chord->env[3] for voice 4
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR _da_playNote_tramp
done:
    RTS
    .endproc
