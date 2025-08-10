; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Screen Driver machine language parts - interrupt context

.include "atari.inc"
.importzp sp

   .constructor _DA_CTOR

; Application locations in Page Zero
    .ZEROPAGE

_DA_VOICE1_COUNTER: .byte 0
    .exportzp _DA_VOICE1_COUNTER
_DA_VOICE1_Y: .byte 0
    .exportzp _DA_VOICE1_Y
_DA_VOICE1_ENV: .word 0
    .exportzp _DA_VOICE1_ENV
_DA_VOICE2_COUNTER: .byte 0
    .exportzp _DA_VOICE2_COUNTER
_DA_VOICE2_Y: .byte 0
    .exportzp _DA_VOICE2_Y
_DA_VOICE2_ENV: .word 0
    .exportzp _DA_VOICE2_ENV
_DA_VOICE3_COUNTER: .byte 0
    .exportzp _DA_VOICE3_COUNTER
_DA_VOICE3_Y: .byte 0
    .exportzp _DA_VOICE3_Y
_DA_VOICE3_ENV: .word 0
    .exportzp _DA_VOICE3_ENV
_DA_VOICE4_COUNTER: .byte 0
    .exportzp _DA_VOICE4_COUNTER
_DA_VOICE4_Y: .byte 0
    .exportzp _DA_VOICE4_Y
_DA_VOICE4_ENV: .word 0
    .exportzp _DA_VOICE4_ENV

; Unitialized application data

    .segment "ONCE"

; CONSTRUCTOR - intialize the POKEY chip

_DA_CTOR:
    .proc DA_CTOR
    ; clear _DA control registers
    LDA #0
    LDY #12
dainit:
    STA _DA_VOICE1_COUNTER,Y
    DEY
    BPL dainit
    ; Initialize POKEY for sound
    LDY #$E
pokeyinit:
    STA $D200,Y
    DEY
    BPL pokeyinit
    LDA #3
    STA SKCTL ; D20F clear serial port mode, enable keyboard

    LDA #<_DA_B5TIMER
    STA VTIMR1
    LDA #>_DA_B5TIMER
    STA VTIMR1+1

    ; Start the timer at P=31 (note B5) which is about the frequency of a key click (997Hz)
    LDA #$A0
    STA AUDC1
    LDA #31
    STA AUDF1

    ; Enable POKEY
    LDA #$41
    STA IRQEN ; D20E enable key interrupts (not break key though) and Timer 1
    STA POKMSK ; shadow of IRQEN

    RTS
    .endproc
; end of constructor



;
;
; INTERRUPT HANDLERS
;
; These all go in LOWCODE, which is guaranteed to never be swapped out
;
;

    .segment "LOWCODE"

.macro control voiceEnv,voiceCounter,voicePlay
.scope
    ; Quick exit if not doing anything
    LDA voiceEnv+1 ; only check high byte, this will never be in page 0
    BEQ control1done

    ; Decrement the counter, and return if not zero
    DEC voiceCounter
    BNE control1done
    JSR voicePlay
control1done:
.endscope
.endmacro

.macro play voiceEnv,voiceCounter,voiceY,voiceAudc
.scope
    LDY voiceY
    ; Load the control byte. If it's zero, we're all done.
    LDA (voiceEnv),Y
    BEQ endNote
    STA voiceAudc

    ; Load the delay byte. Note that '1' means 'no delay'
    ; and '0' means 255! Maybe I should do a BEQ back to the top?
    INY
    LDA (voiceEnv),Y
    STA voiceCounter

    ; and prepare for the next round
    INY
    STY voiceY
    RTS

endNote:
    ; if we INY here, the next byte could be the freq to set for next time
    ; And the two bytes after that could be an address to load into _DA_VOICE1_ENV.
    ; Maybe. That would take too long though. Keep it simple.
    ; Reset the voiceEnv MSB, that's the flag that tells it to do nothing.
    ; playNote() will clear everything else
    ; maybe I should write a 0 to AUDC here to shut it up?
    LDA #0
    STA voiceEnv+1
    RTS
.endscope
.endmacro


play1:
.export play1
    play _DA_VOICE1_ENV,_DA_VOICE1_COUNTER,_DA_VOICE1_Y,AUDC1
play2:
.export play2
    play _DA_VOICE2_ENV,_DA_VOICE2_COUNTER,_DA_VOICE2_Y,AUDC2
play3:
.export play3
    play _DA_VOICE3_ENV,_DA_VOICE3_COUNTER,_DA_VOICE3_Y,AUDC3
play4:
.export play4
    play _DA_VOICE4_ENV,_DA_VOICE4_COUNTER,_DA_VOICE4_Y,AUDC4

_DA_B5TIMER:
    .proc DA_B5TIMER
    .export _DA_B5TIMER
    ; OS will have pushed A onto the stack
    ; we need to protect Y, do it now, then load 'our' Y
    TYA
    PHA
    control _DA_VOICE1_ENV,_DA_VOICE1_COUNTER,play1
    control _DA_VOICE2_ENV,_DA_VOICE2_COUNTER,play2
    control _DA_VOICE3_ENV,_DA_VOICE3_COUNTER,play3
    control _DA_VOICE4_ENV,_DA_VOICE4_COUNTER,play4
b5done:
    PLA
    TAY
b5fastdone:
    PLA
    CLI ; let next interrupt be handled
    RTI

    .endproc
