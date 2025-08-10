;
; Based on the cc65 runtime library copydata.s
; Ullrich von Bassewitz, 1998-12-07, 2004-12-01
;
; Copy the data segment from the LOAD to the RUN location
; Modified to copy the RODATA segment too
;

        .export         copydata
        .export         copyrodata
        .import         __DATA_LOAD__, __DATA_RUN__, __DATA_SIZE__
        .import         __RODATA_LOAD__, __RODATA_RUN__, __RODATA_SIZE__
        .importzp       ptr1, ptr2, tmp1

; STARTUP segment because this is os-level startup code
.segment "STARTUP"

copyrodata:
        lda     #<__RODATA_LOAD__         ; Source pointer
        sta     ptr1
        lda     #>__RODATA_LOAD__
        sta     ptr1+1

        lda     #<__RODATA_RUN__          ; Target pointer
        sta     ptr2
        lda     #>__RODATA_RUN__
        sta     ptr2+1

        ldx     #<~__RODATA_SIZE__
        lda     #>~__RODATA_SIZE__        ; Use -(__RODATASIZE__+1)
        jmp     startupcopy

copydata:
        lda     #<__DATA_LOAD__         ; Source pointer
        sta     ptr1
        lda     #>__DATA_LOAD__
        sta     ptr1+1

        lda     #<__DATA_RUN__          ; Target pointer
        sta     ptr2
        lda     #>__DATA_RUN__
        sta     ptr2+1

        ldx     #<~__DATA_SIZE__
        lda     #>~__DATA_SIZE__        ; Use -(__DATASIZE__+1)

; Source address in ptr1
; Dest address in ptr2
; negative count in AX

startupcopy:
        sta     tmp1
        ldy     #$00

; Copy loop

@L1:    inx
        beq     @L3

@L2:    lda     (ptr1),y
        sta     (ptr2),y
        iny
        bne     @L1
        inc     ptr1+1
        inc     ptr2+1                  ; Bump pointers
        bne     @L1                     ; Branch always (hopefully)

; Bump the high counter byte

@L3:    inc     tmp1
        bne     @L2

; Done

        rts

