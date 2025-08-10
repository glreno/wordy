; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

; ds_screendriverA.h

; Declarations for wordl screen driver.

; #########################################################
; #                  SCREEN DRIVER                        #
; #########################################################

; Header for a screen page.
; Offsets for the fields in the C ds_pageHeader

    ; Address of this page; technically a *ds_pageHeader
    ds_pageHeader_myAddress = 0
    ds_pageHeader_id = 2
    ds_pageHeader_screenState = 4
    ds_pageHeader_queueState = 5
    ds_pageHeader_next = 6 ; pointer to next page to queue
    ds_pageHeader_spare = 8 ; 6 bytes of unused space
    ds_pageHeader_spare0 = $08 ; int 0 of spare
    ds_pageHeader_spare1 = $0A ; int 1 of spare
    ds_pageHeader_spare2 = $0C ; int 1 of spare
    ds_pageHeader_pmpos = $0E ; 13 bytes of PM color and position data
    ds_pageHeader_pcolors = $1B ; 4 bytes of color data
    ds_pageHeader_colors = $1F ; 5 bytes of color data
    ds_pageHeader_fonts = $24 ; 156 bytes of font addresses (MSB only)
    ds_pageHeader_displaylist = $C0 ; 64 bytes to store the display list
    ds_pageHeader_freespace = $100 ; 256 bytes of unused space

; State constants for the screenState
; (queueState and next are handled by whoever is doing the animation queueing)

    DS_SCREENSTATE_INITIAL = 3
    DS_SCREENSTATE_QUEUED = 2
    DS_SCREENSTATE_ISNEXT = 1
    DS_SCREENSTATE_ONSCREEN = 0
    DS_SCREENSTATE_OFFSCREEN = $FF
