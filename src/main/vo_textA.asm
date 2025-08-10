; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   Assembler code for the ridiculously simple text-only opponent view.

    .include "include/trampoline.inc"
    .include "zeropage.inc"

    .import ldaxidx

	.importzp _vo_singleton

    .import VOT_BANK_ID
    .import _VO_YIELDBANKID

    .import _ds_setStandardPalette
    .import _ds_flipToPageNextVBI


    .segment "ONCE"

; Called by vot_initialize to set up bank switching
; (this has to be in assembler to get the VOT_BANK_ID from the linker)
; void vot_preinit(void);
_vot_preinit:
    .proc vot_preinit
    .export _vot_preinit
    LDA #<VOT_BANK_ID
    STA _VO_YIELDBANKID
    RTS
    .endproc

    .CODE


; Select the palette, because the colors I picked suck
;void __cdecl__ vot_setPalette(char p);
titleTrampoline _vot_setPalette,_vot_setPalette_title
    .proc vot_setPalette

;   vo_text *this=vot_singleton;
;   ds_setStandardPalette(p,this->page);
;   ds_flipToPageNextVBI(this->page);

    ; this is __cdecl__ so p is already on the stack
    ; just need to load vot_singleton->page (offset #$2D) into AX
    ; and call ds_setStandardPalette.
    ; (that's in the startup bank, so THIS function is titleTrampolined)
	LDA _vo_singleton
	LDX _vo_singleton+1
	LDY #$2D
	JSR ldaxidx
	JSR _ds_setStandardPalette

	LDA _vo_singleton
	LDX _vo_singleton+1
	LDY #$2D
	JSR ldaxidx
    JMP _ds_flipToPageNextVBI

    .endproc
