; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

.include "../main/include/ds_screendriverA.inc"

.import _ds_flipToPageNextVBI
.importzp _DS_ZTMPI
.importzp _DS_ZTMPIL
.importzp _DS_ZTMPIH


.BSS
    .export _currentPage
_currentPage:
    .word 0

.CODE

; Handlers are called by the VBI
; Since we're in interrupt context, can't use C pointers (cc65 will stomp its internal pointer holder)
_timerHandler:
    .export _timerHandler
    .proc timerhandler
    ; Look at the current page being displayed.
    ; next is the index of the next page.
    ; If it's -1, restart the animation timer.
    LDA _currentPage
    CMP #$FF
    BNE nextexists
    RTS
nextexists:
    ; There is a next page. Unqueue it by clearing the currentPage->page.next pointer
    ; stash currentPage->page.next
    ; _DS_ZTMPI = _currentPage
    LDA _currentPage+1
    STA _DS_ZTMPIH
    LDA _currentPage
    STA _DS_ZTMPIL

    ; push page.next onto the stack
    LDY #ds_pageHeader_next
    LDA (_DS_ZTMPIL),Y
    PHA ; STACK: next(LSB)
    INY
    LDA (_DS_ZTMPIL),Y
    PHA ; STACK: next(MSB) next(LSB)

    ; currentPage->page.next=NOPAGE;
    LDY #ds_pageHeader_next
    LDA #$FF
    STA (_DS_ZTMPIL),Y
    INY
    STA (_DS_ZTMPIL),Y

    ; Store next page (currently on stack) in _currentPage
    ; and send it to the page flip function
    PLA ; POP next(MSB) - STACK: next(MSB)
    TAX ; next(MSB) in X, ready for function call
    STA _currentPage+1
    PLA ; POP next(LSB) - STACK is clear
    STA _currentPage
    
    JMP _ds_flipToPageNextVBI
    ; RTS is in that function
.endproc
