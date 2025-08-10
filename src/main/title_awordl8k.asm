; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

;
; Title definitions
;
    ; 7 char string, used for copyright messages in ROM
    .define title7char  "Awordl "

    ; 10 char string, used for copyright messages in Gr.0 text
    .define title10char "   Awordl "

    ; 20 char string, used for title messages in Gr.2 text
    ; so do not mix case!
    .define title20char "       awordl       "

    ; Two lines of 20-char text for game instructions
    .define instruction1 "  six attempts to   "
    .define instruction2 "  guess four words  "


;
; Minimum Memory Check
;

    ; set "ramtop_min" to the minimum-acceptable RAM.
    ; 16KB ends at $4000, so set to $40
    ; If not set, there will be no RAM check on startup
    ;ramtop_min=$40

    ; Error message. Must begin with a space.
    ;.define ramfailmsgtext " Min 16K"

;
; Set to 1 to RODATA from the startup bank to RAM on startup
;

    ;do_copyrodata=1

;
; Title screen template package
;
    .include "include/title_template.inc"
