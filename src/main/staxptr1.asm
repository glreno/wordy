; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0


; A couple utilities for handling ptr1

    .importzp ptr1

    .CODE
staxptr1:
    .export staxptr1
    STA ptr1
    STX ptr1+1
    RTS

