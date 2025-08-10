; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   The opponent that drives a QORDL game

.include "zeropage.inc"
    .import incsp6
    .import _vu_initialize
    .import _copyStackToPtr1
    .import staxptr1

;
; STRUCT OFFSETS
;
    MOQ_GAMEMODEL = 0   ; mq* (word)
    MOQ_DICT = 2        ; md_dict* (word)
    MOQ_VU_LETTERS = 4  ; void* (word) (pointer to screen memory where the vu_letters are)
    MOQ_COMMANDMODE = 6 ; byte

    .SEGMENT "GAMEINIT"

; void __fastcall__ moq_initialize(void *usedLetterView, md_dict *dict, mq *gameModel, moq *this)
;   this->gameModel = gameModel;
;   this->dict = dict;
;   this->vu_letters = usedLetterView;
;   this->commandModeFlag = 0;
;   vu_initialize(this->vu_letters);
_moq_initialize:
    .export _moq_initialize
    .proc moq_initialize

    ; ptr1 = this
    JSR staxptr1

    ; Copying the params to the data structure is a lot easier
    ; since they are in the same order
    LDY #6
    JSR _copyStackToPtr1

    ; Clear the command mode flag
;   this->commandModeFlag = 0;
    LDA #0
    LDY #MOQ_COMMANDMODE
    STA (ptr1),Y

    ; initialize the vu_letters
;   vu_initialize(this->vu_letters);
    LDY #MOQ_VU_LETTERS + 1
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR _vu_initialize

    ; and return
    JMP incsp6

    .endproc
