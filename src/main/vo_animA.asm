; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Base class for animated opponent views

    .include "include/ds_screendriverA.inc"
    .include "include/trampoline.inc"
    .include "zeropage.inc"
    .importzp _vo_singleton
    .import incaxy
    .import incsp1
    .import incsp4
    .import popa
    .import pusha0
    .import pushax
    .import pusha
    .import ldaxysp
    .import staxptr1
    .import umul8x8r16
    .import tosmula0
    .import _da_playCue_tramp
    .import _ds_copyPicToFont
    .import _ds_setStandardPalette
    .import _ds_setAnimTimerCallback
    .import _animExitTo
    .import _DS_NOCB
    .import _ds_flipToPageNextVBI
    .importzp _DS_ZTMPI
    .importzp _DS_ZTMPIL
    .importzp _DS_ZTMPIH
    .import _cueball;
    .import _voa_qcurr;
    .export _voa_cueQueue;
    .export _voa_currentPage

    .DATA

_voa_cueQueue: .byte 0

_voa_currentPage:
    .word $FF00

; a function in DATA segment since it needs to be writable

; Draw an animationm frame into a Page.
; The ID for the animation is stored in page.spare[0]
; void __fastcall__ voa_renderFrame(char pageid);
_voa_renderFrame:
    .export _voa_renderFrame
    .proc voa_renderFrame
    JMP _DS_NOCB
    .endproc

    _voa_renderFrameCB = _voa_renderFrame+1
    .export _voa_renderFrameCB

    .segment "ONCE"

; Called by voa_preinit to set up the animation timer
; void voa_preinit(void);
_voa_preinit:
    .proc voa_preinit
    .export _voa_preinit

    ; Clear the animation queue
    LDA #0
    STA _voa_qcurr

    ; register the animation timer handler
    ; this should be in voa_initialize, once there is one
    ; NTSC: 15=4fps, 12=5fps, 10=6fps, 7.5=8fps, 6=10fps, 5=12fps, 4=15fps, 2=30fps
    ;  PAL:          10=5fps                     5=10fps                           2=25fps
    ; You need to subtract 1 from those because we're using a zero-based timer.
    LDA $D014 ; 400/800 PAL flag, probably works on XL, it does on Altirra
    AND #$E
    BEQ pal
ntsc:
    LDA #6 ; 10fps on NTSC
    JMP pushit
pal:
    LDA #5 ; 10fps on PAL
pushit:
    JSR pusha0

    LDA #<_vo_timerHandler
    LDX #>_vo_timerHandler
    JSR _ds_setAnimTimerCallback

    RTS
    .endproc

    .segment "LOWCODE"

; Animation timer handler
; Handlers are called by the VBI
; Since we're in interrupt context, can't use C pointers (cc65 will stomp its internal pointer holder)
_vo_timerHandler:
    .export _vo_timerHandler
    .proc vo_timerhandler
    ; Look at the current page being displayed.
    ; next is the index of the next page.
    ; If it's -1, restart the animation timer.
    LDA _voa_currentPage+1
    CMP #$FF
    BNE pageexists
    RTS
    ; RTS is in there
pageexists:
    ; There is a current page. See if there is a next page.
    ; stash currentPage->page.next
    ; _DS_ZTMPI = _voa_currentPage
    LDA _voa_currentPage+1 ; hey this is redundant!
    STA _DS_ZTMPIH
    LDA _voa_currentPage
    STA _DS_ZTMPIL

    ; There is a next page. Unqueue it by clearing the currentPage->page.next pointer
    ; push page.next onto the stack
    LDY #ds_pageHeader_next
    LDA (_DS_ZTMPIL),Y
    PHA ; STACK: next(LSB)
    INY
    LDA (_DS_ZTMPIL),Y
    PHA ; STACK: next(MSB) next(LSB)
    ; Check to see if there IS a next page
    CMP #$FF
    BNE nextexists
    ; There is no next. Clear the stack
    PLA
    PLA
    RTS
nextexists:
    ; currentPage->page.next=NOPAGE;
    LDY #ds_pageHeader_next
    LDA #$FF
    STA (_DS_ZTMPIL),Y
    INY
    STA (_DS_ZTMPIL),Y

    ; Store next page (currently on stack) in _voa_currentPage
    ; and in ztmpi as a pointer, and in X as param for page flip;
    ; copy the sound chord from the page to the chord queue,
    ; where it will be played at the next yield.
    ; and send to the next page to the page flip function
    LDA #0
    STA _DS_ZTMPIL
    PLA ; POP next(MSB) - STACK: next(LSB)
    TAX
    STA _DS_ZTMPIH ; page MSB into pointer
    STA _voa_currentPage+1
    PLA ; POP next(LSB) - STACK is clear
    STA _voa_currentPage
    LDY #ds_pageHeader_spare1
    LDA (_DS_ZTMPI),Y
    BEQ skipChord

    ; and store it in the queue
    STA _voa_cueQueue

    ; Clear the byte in the page
    LDA #0
    STA (_DS_ZTMPI),Y

skipChord:
    JMP _ds_flipToPageNextVBI
    ; RTS is in that function
.endproc


; VIEW segment because very very small
    .segment "VIEW"

; Request that the animation move to a new loop at the next exit
; Check to see if animExitTo is available by checking if(!voa_animLoopQueued())
; before calling. Otherwise, this will overwrite the existing queueud animation.
; void __fastcall__ voa_startAnimationLoop(unsigned int nextLoop);
;   animExitTo = nextLoop;
_voa_startAnimationLoop:
    .export _voa_startAnimationLoop
    .proc voa_startAnimationLoop

    STA _animExitTo
    STX _animExitTo+1
    RTS

    .endproc

; Reset a page to the INTIAL/CLEAN state
; void __fastcall__ voa_cleanPage(char pageid);
; pageid is in A
trampoline _voa_cleanPage,_voa_cleanPage_tramp
    .proc voa_cleanPage
;   vo_anim *this=voa_singleton;
;   ds_page *p;
;   p=this->pages[pageid];
    ; 'this' is zp _vo_singleton
    ; 'pages' is an array of pointers starting at _vo_singleton+6
    ; The index pageid is thus: A*2+6
    CLC
    ASL A
    ADC #6
    TAY

    LDA (_vo_singleton),Y
    STA ptr1
    INY
    LDA (_vo_singleton),Y
    STA ptr1+1

;   p->page.next=NOPAGE; // mark page as available for use
    LDY #ds_pageHeader_next
    LDA #$FF
    STA (ptr1),Y
    INY
    STA (ptr1),Y

;   p->page.screenState=DS_SCREENSTATE_INITIAL; // 3 INITIAL STATE
    LDY #ds_pageHeader_screenState
    LDA #3
    STA (ptr1),Y

;   p->page.queueState=VOA_QUEUESTATE_CLEANED; // 4 CLEANED
    INY
    LDA #4
    STA (ptr1),Y

    RTS

    .endproc

; ONCE segment because it calls ds_setStandardPalette

; Select the palette, because the colors I picked suck
; void __cdecl__ voa_setPalette(char p, char npages);
; Both params are on the stack: sp+1 -> paletteid, sp+0 -> npages
titleTrampoline _voa_setPalette,_voa_setPalette_title
    .proc voa_setPalette

;   vo_anim *this=voa_singleton;
;   for(i=0;i<VOA_NPAGES;++i)
;       ds_setStandardPalette(p,this->pages[i]);
    ; that is a __fastcall__, so the page address goes on the stack, p in AX

    ; 'this' is zp _vo_singleton
    ; 'pages' is an array of pointers starting at _vo_singleton+6
    ; The index pageid is thus: i*2+6

    ; since we're supposed to run from (0*2)..(npages*2),
    ; do it in the other direction. Just double the npages that
    ; is on the stack.
    ; So calculate pages (vo_singleton+6), and put it on the stack.

    ; pages=vo_singleton+6
    ; i=npages*2
    ; push paletteid
    ; store pages[i] in AX
    ; call ds_setStandardPalette
    ; decrement i by 2
    ; loop while i >= 0


    ; pages=vo_singleton+6
    LDA _vo_singleton
    LDX _vo_singleton+1
    LDY #6
    JSR incaxy
    JSR pushax
    ; Stack is now: pages (2 bytes)(sp), i (sp+2), paletteid (sp+3)
    ; i=npages*2-1
    LDY #2
    LDA (sp),Y
    CLC
    ASL A
    SBC #0 ; carry is clear so this is subtract 1
    STA (sp),Y

loop:
    ; push paletteid
    LDY #3
    LDA (sp),Y
    JSR pusha

    ; store pages[i] in AX
    LDY #2
    JSR ldaxysp
    JSR staxptr1
    LDY #3
    LDA (sp),Y
    TAY
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y

    ; call ds_setStandardPalette
    JSR _ds_setStandardPalette

    ; decrement i by 2
    LDY #2
    LDA (sp),Y
    SEC
    SBC #2
    STA (sp),Y
    ; loop while i >= 0
    BPL loop

    JMP incsp4

    .endproc

    .BSS

drawPicPtr:
    .word 0

    .segment "VOR_VIEW"

; Array of 10-byte objects of type vo_anim_pic
.import _voa_piclist

; Draw a vo_anim_pic to the given pair of fonts
; int __cdecl__ voa_drawPic(unsigned char fontbaseBot, unsigned char fontbaseTop, unsigned char picid);
; Stack: 0=picid 1=fontbaseTop 2=fontbaseBot
_voa_drawPic:
    .export _voa_drawPic
    .proc voa_drawPic

    ; Four calls to ds_copyPicToFont(void*pic,char fontbase)
    ; fontbase is on the stack at 1 and 2
    ; The pic pointers are at _voa_piclist[picid]+0,2,4,6

    ; pop the picid off the stack, multiply it by 10, add it to _voa_piclist,
    ; and stash the value someplace
    JSR popa
    STA ptr1
    LDA #0
    STA ptr1+1
    LDA #10
    JSR umul8x8r16
    ; umul stores result in ptr1, uses ptr3 for scratch
    LDA #<_voa_piclist
    CLC
    ADC ptr1
    STA drawPicPtr
    LDA #>_voa_piclist
    ADC ptr1+1
    STA drawPicPtr+1

    ; The stack is currently fontBaseTop, fontBaseBot

    ; Now we just need to push the pic ptr onto the stack and call ds_copyPicToFont. Four times.

    LDY #1
    JSR doCopyY
    LDY #3
    JSR doCopyY

    JSR incsp1 ; pop the fontbaseTop

    LDY #5
    JSR doCopyY
    LDY #7
    JSR doCopyY

    JSR incsp1 ; pop the fontbaseBot

    ; bytes 8 and 9 need to be returned to the caller
    LDA drawPicPtr
    LDX drawPicPtr+1
    JSR staxptr1
    LDY #9
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y

    RTS

    ; Get the address from drawPicPtr,Y and push it on the stack,
    ; Then load the fontbase from what was the top of the stack,
    ; and call ds_copyPicToFont
doCopyY:
    LDA drawPicPtr
    LDX drawPicPtr+1
    JSR staxptr1
    LDA (ptr1),Y
    TAX
    DEY
    LDA (ptr1),Y
    JSR pushax
    LDY #2
    LDA (sp),Y
    JSR pusha
    JSR _ds_copyPicToFont

    RTS
    .endproc

; Process the next item in the audio cue queue, if there.
; void __fastcall__ voa_playCue();
trampoline _voa_playCue,_voa_playCue_tramp
    .proc voa_playCue
    ; if ( voa_cueQueue != 0 )
	LDA _voa_cueQueue
	BEQ alldone

    ; da_playCue(&cueball[voa_cueQueue]);
	jsr     pusha0
	lda     #$0B
	jsr     tosmula0
	clc
	adc     #<(_cueball)
	tay
	txa
	adc     #>(_cueball)
	tax
	tya
    JSR _da_playCue_tramp

    ; voa_cueQueue = 0;
	LDA #0
	STA _voa_cueQueue

alldone:
    RTS

    .endproc


