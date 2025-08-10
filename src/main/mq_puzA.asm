; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Qordl Model machine language parts

.include "zeropage.inc"

    .import incax4
    .import incaxy
    .import incsp6
    .import ldaxysp
    .import push1
    .import pusha0
    .import pusha
    .import pushax
    .import pushc0
    .import pushwysp
    .import pushw0sp
    .import _memset
    .import _vwt_initialize
    .import _vwt_initialize_gutter
    .import _mw_initialize

    .export _mq_initialize
;
;
; This is hand-optimization of what the C compiler generated.
; If you need to change anything, build the C version in mq_puz.c
; and compare. It may be better to just use the C version.
;
; My changes are in CAPS
;

; ---------------------------------------------------------------
; void __near__ __fastcall__ mq_initialize (struct vfm_fontmanager_struct *fm1, struct vfm_fontmanager_struct *fm2, struct mq_struct *model)
; ---------------------------------------------------------------

.segment	"GAMEINIT"

.proc	_mq_initialize: near

;	.dbg	func, "mq_initialize", "00", static, "_mq_initialize"
;	.dbg	sym, "fm1", "00", auto, 4
;	.dbg	sym, "fm2", "00", auto, 2
;	.dbg	sym, "model", "00", auto, 0

.segment	"GAMEINIT"

;	.dbg	line, "src/main/mq_puz.c", 32
	jsr     pushax
;	.dbg	line, "src/main/mq_puz.c", 33
;   memset(model,1,WORDLS_IN_QORDL); // clear all the stillInPlay flags

    jsr     pushw0sp ; this duplicates ax onto the stack
;	lda     #$01
;	jsr     pusha0
    JSR push1        ; push 0001 onto the stack
	lda     #$04
	jsr     _memset
;	.dbg	line, "src/main/mq_puz.c", 35
;   vwt_initialize(mw_initialize(fm1,9,2,0,&(model->puzzles[0])));

	ldy     #$07
	jsr     pushwysp
;	lda     #$09
;	jsr     pusha
    LDX #9
	lda     #$02
    JSR pushax
;	jsr     pusha
;	lda     #$00
;	jsr     pusha
    JSR pushc0
	ldy     #$06
	jsr     ldaxysp
;	jsr     incax4
    LDY #0+4
    JSR incaxy_mw_vwt_init
;	jsr     _mw_initialize
;	jsr     _vwt_initialize
;	.dbg	line, "src/main/mq_puz.c", 36
;   vwt_initialize(mw_initialize(fm1,9,2,24,&(model->puzzles[1])));
	ldy     #$07
	jsr     pushwysp
;	lda     #$09
;	jsr     pusha
    LDX #9
	lda     #$02
    JSR pushax
;	jsr     pusha
	lda     #$18
	jsr     pusha
	ldy     #$06
	jsr     ldaxysp
;	jsr     incax4
;	ldy     #$26
    LDY #$26+4
    JSR incaxy_mw_vwt_init
;	jsr     incaxy
;	jsr     _mw_initialize
;	jsr     _vwt_initialize
;	.dbg	line, "src/main/mq_puz.c", 37
;   vwt_initialize(mw_initialize(fm2,9,12,0,&(model->puzzles[2])));
	ldy     #$05
	jsr     pushwysp
;	lda     #$09
;	jsr     pusha
    LDX #9
	lda     #$0C
    JSR pushax
;	jsr     pusha
;	lda     #$00
;	jsr     pusha
    JSR pushc0
	ldy     #$06
	jsr     ldaxysp
;	jsr     incax4
;	ldy     #$4C
    LDY #$4C+4
    JSR incaxy_mw_vwt_init
;	jsr     incaxy
;	jsr     _mw_initialize
;	jsr     _vwt_initialize
;	.dbg	line, "src/main/mq_puz.c", 38
;   vwt_initialize(mw_initialize(fm2,9,12,24,&(model->puzzles[3])));
	ldy     #$05
	jsr     pushwysp
;	lda     #$09
;	jsr     pusha
    LDX #9
	lda     #$0C
    JSR pushax
;	jsr     pusha
	lda     #$18
	jsr     pusha
	ldy     #$06
	jsr     ldaxysp
;	jsr     incax4
;	ldy     #$72
    LDY #$72+4
    JSR incaxy_mw_vwt_init
;	jsr     incaxy
;	jsr     _mw_initialize
;	jsr     _vwt_initialize
;	.dbg	line, "src/main/mq_puz.c", 39
    jsr     _vwt_initialize_gutter
	jmp     incsp6

;	.dbg	line

incaxy_mw_vwt_init:
	JSR     incaxy
	JSR     _mw_initialize
	JMP     _vwt_initialize

.endproc
