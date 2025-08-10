; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;   A XL-and-800 compatible keyboard driver

    .include "atari.inc"

    .import _cgetc
    .import _yield

    .segment "ONCE"

; Find the OS's keymap
; If it's not already in KEYMAP ($79,$7A) store it there.
; In the 400/800 this is the addresses ROWINC and COLINC,
; which are used by the S: driver's DRAWTO.
; So DO NOT USE WITH DRAWTO!!!
;
; void __fastcall__ dk_initialize(void);
_dk_initialize:
    .proc dk_initialize
    .export _dk_initialize

    LDA KEYDEF
    ORA KEYDEF+1
    BNE done ; already a pointer there

    ; This is not an XL. See if the keydef table is where it belongs at $FEFE
    LDA #$FE
    STA KEYDEF
    STA KEYDEF+1
    LDY #0
    LDA (KEYDEF),Y
    CMP #$6C ; lower case L
    BNE fail
    INY
    LDA (KEYDEF),Y
    CMP #$6A ; lower case J
    BEQ done ; map looks good
fail:
    ; we did NOT find the keydef at FEFE (must be running in an emulator)
    ; so use the compatibility mode instead
    LDA #0
    STA KEYDEF
    STA KEYDEF+1
done:
    RTS

    .endproc

    .segment "LOWCODE"

; just like cgetc but it keeps calling yield()
; And it doesn't check for the BREAK key.
; Or support the ESC key or caps lock or Atari.
;
; char __fastcall__ dk_getc(void);
_dk_getc:
    .proc dk_getc
    .export _dk_getc

readch:
    JSR _yield
    LDA CH
    CMP #$FF
    BEQ readch

    ; We read SOMETHING. Make sure it's not an illegal ctrl+shift
    CMP #$C0
    BCS readch

    ; We have a key. Check that we have a good key def table
    LDA KEYDEF
    ORA KEYDEF+1
    BNE lookup

    ; Compatibility mode -- call cgetc() and return
    JMP _cgetc

lookup:
    ; now would be a good time to trigger a key click sound

    ; Look up the key
    LDY CH
    ; Clear CH
    LDA #$FF
    STA CH
    ; map key using the OS keydef table
    LDA (KEYDEF),Y

    ; Need to throw out: 0x80 (bad key) 0x81 (Atari) 0x82 (caps) 0x83 (shift-caps)
    ; In fact the only valid key above 0x80 that I want is 0x9B RETURN
    ; since BACKSPACE is 0x7E and I don't care about tab,insert,clear,bell,break
    BPL done
    CMP #$9B
    BNE readch

done:
    ; Return key value in A
    LDX #0
    RTS

    .endproc

