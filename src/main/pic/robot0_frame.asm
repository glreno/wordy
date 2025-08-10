; Copyright (c) 2025 Gerald Reno, Jr.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0

    .include "../../../target/robot0_pic.inc"

    .segment "VOR_IDX"

; Array of 10-byte objects of type vo_anim_pic
_voa_piclist:
    .export _voa_piclist

; ===============================
; Boring Standing Still Robot
; ===============================

; frame vor_frame_stand ; this is the default standing position
    .word _robot0_main_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, 0

; ===============================
; Angry Robot - crossed arms and tapping foot
; ===============================

; frame vor_frame_angry1 ; angry - crossed arms - no sound
    .word _robot0_angry_head_pic, _robot0_angry_shoulders_pic, _robot0_angry_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_angry2 ; angry - crossed arms and tapping foot
    .word _robot0_angry_head_pic, _robot0_angry_shoulders_pic, _robot0_angry_knees_pic, _robot0_angry_toes_pic, 0

; ===============================
; Bored Robot - tapping foot
; ===============================

; frame vor_frame_bored2
    .word _robot0_main_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_wrist1_toes_pic, 0

; frame vor_frame_bored3
    .word _robot0_main_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_angry_toes_pic, 0

; ===============================
; Wrist Computer
; ===============================

; frame vor_frame_wrist1
; right key, with low key click (7)
    .word _robot0_wrist1_head_pic, _robot0_wrist1_shoulders_pic, _robot0_wrist1_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_wrist2
; hand above key
    .word _robot0_wrist1_head_pic, _robot0_wrist2_shoulders_pic, _robot0_wrist1_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_wrist3
; left key, with high key click (8)
    .word _robot0_wrist1_head_pic, _robot0_wrist3_shoulders_pic, _robot0_wrist1_knees_pic, _robot0_main_toes_pic, 0

; ===============================
; Happy Dance
; ===============================

; frame vor_frame_happy
; happy dance - mid point - both arms down and legs straight
    .word _robot0_main_head_pic, _robot0_happy_shoulders_pic, _robot0_happy_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_happy_lefty1
; happy dance - left knees up 0 - right arm down
    .word _robot0_lefty_head_pic, _robot0_lefty_shoulders_pic, _robot0_lefty_knees_pic, _robot0_lefty_toes_pic, 0

; frame vor_frame_happy_lefty2
; happy dance - left knees up - right arm up
    .word _robot0_leftkick_head_pic, _robot0_leftkick_shoulders_pic, _robot0_lefty_knees_pic, _robot0_lefty_toes_pic, 0

; frame vor_frame_happy_leftpoint1
; happy dance - left point - right arm down
    .word _robot0_lefty_head_pic, _robot0_lefty_shoulders_pic, _robot0_leftpoint_knees_pic, _robot0_leftpoint_toes_pic, 0

; frame vor_frame_happy_leftpoint2
; happy dance - left point - right arm straight up
    .word _robot0_leftkick_head_pic, _robot0_leftkick_shoulders_pic, _robot0_leftpoint_knees_pic, _robot0_leftpoint_toes_pic, 0

; frame vor_frame_happy_leftkickback
; happy dance - left kickback
    .word _robot0_leftkickback_head_pic, _robot0_leftkick_shoulders_pic, _robot0_leftkickback_knees_pic, _robot0_leftkickback_toes_pic, 0

; frame vor_frame_happy_leftkick
; happy dance - left kick
    .word _robot0_leftkick_head_pic, _robot0_leftkick_shoulders_pic, _robot0_leftkick_knees_pic, _robot0_leftkick_toes_pic, 0

; frame vor_frame_happy_righty1
; happy dance - right knees up - left arm down
    .word _robot0_right_head_pic, _robot0_right_shoulders_pic, _robot0_right_knees_pic, _robot0_right_toes_pic, 0

; frame vor_frame_happy_righty2
; happy dance - right knees up - left arm up
    .word _robot0_rightkick_head_pic, _robot0_rightkick_shoulders_pic, _robot0_right_knees_pic, _robot0_right_toes_pic, 0

; frame vor_frame_happy_rightpoint1
; happy dance - right point - left arm down
    .word _robot0_right_head_pic, _robot0_right_shoulders_pic, _robot0_rightpoint_knees_pic, _robot0_rightpoint_toes_pic, 0

; frame vor_frame_happy_rightpoint2
; happy dance - right point - left arm straight up
    .word _robot0_rightkick_head_pic, _robot0_rightkick_shoulders_pic, _robot0_rightpoint_knees_pic, _robot0_rightpoint_toes_pic, 0

; frame vor_frame_happy_rightkickback
; happy dance - right kickback
    .word _robot0_rightkickback_head_pic, _robot0_rightkick_shoulders_pic, _robot0_rightkickback_knees_pic, _robot0_rightkickback_toes_pic, 0

; frame vor_frame_happy_rightkick
; happy dance - right kick
    .word _robot0_rightkick_head_pic, _robot0_rightkick_shoulders_pic, _robot0_rightkick_knees_pic, _robot0_rightkick_toes_pic, 0

; ===============================
; Jumpin in the Air
; ===============================

; frame vor_frame_jump_crouch1
; crouch before jump
    .word _robot0_jump5_head_pic, _robot0_jump5_shoulders_pic, _robot0_jump5_knees_pic, _robot0_jump5_toes_pic, 0

; frame vor_frame_jump_crouch2
; crouch before jump
    .word _robot0_jump6_head_pic, _robot0_jump6_shoulders_pic, _robot0_jump6_knees_pic, _robot0_jump6_toes_pic, 0

; frame vor_frame_jump_up
; jump in the air
    .word _robot0_jump2_head_pic, _robot0_jump2_shoulders_pic, _robot0_jump4_knees_pic, _robot0_jump4_toes_pic, 0

; frame vor_frame_jump_air
; Air Jordan
    .word _robot0_jump3_head_pic, _robot0_jump3_shoulders_pic, _robot0_jump3_knees_pic, _robot0_jump3_toes_pic, 0

; frame vor_frame_jump_down
; land after jump
    .word _robot0_jump1_head_pic, _robot0_jump1_shoulders_pic, _robot0_jump2_knees_pic, _robot0_main_toes_pic, 0

; ===============================
; Sad Robot
; ===============================

; frame vor_frame_sad1
; sad dance - slump 1 - with sad sound(3)
    .word _robot0_wrist1_head_pic, _robot0_angry_shoulders_pic, _robot0_angry_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_sad2
; sad dance - slump 2
    .word _robot0_wrist2_head_pic, _robot0_angry_shoulders_pic, _robot0_angry_knees_pic, _robot0_main_toes_pic, 0

; frame vor_frame_sad3
; sad dance - slump 3
    .word _robot0_wrist3_head_pic, _robot0_angry_shoulders_pic, _robot0_angry_knees_pic, _robot0_main_toes_pic, 0

; ===============================
; Firing Laser
; ===============================

; Laser frames. Return parameter indicates laser progress:
; Bit 15 indicates "this is a laser" and bit 14 "facing right" so
; 0x8000 = left laser, 0xC000 = right laser
; Mask for the progress is 0x3F00. Progress runs from 0 to 60 ($3C)
; All in all: BC00 and FC00 are left and right 100% done.
;
; The initial laser frame (0/60 complete) has sound 4 or 5 for green or yellow.

; Left Laser Frames

; frame vor_frame_left_laser0
; left laser, green or yellow start, sound(4 or 5)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $8000

; frame vor_frame_left_laser10 ; (10/60, 0x0A)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $8A00
; frame vor_frame_left_laser12 ; (12/60, 0x0C)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $8C00
; frame vor_frame_left_laser15 ; (15/60, 0x0F)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $8F00
; frame vor_frame_left_laser20 ; (20/60, 0x14)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $9400
; frame vor_frame_left_laser24 ; (24/60, 0x18)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $9800
; frame vor_frame_left_laser30 ; (30/60, 0x1E)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $9E00
; frame vor_frame_left_laser36 ; (36/60, 0x24)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $A400
; frame vor_frame_left_laser40 ; (40/60, 0x28)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $A800
; frame vor_frame_left_laser45 ; (45/60, 0x2D)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $AD00
; frame vor_frame_left_laser48 ; (48/60, 0x30)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $B000
; frame vor_frame_left_laser50 ; (50/60, 0x32)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $B200
; frame vor_frame_left_laser60 ; end (60/60, 0x3C)
    .word _robot0_lefty_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $BC00

; Right Laser Frames

; frame vor_frame_right_laser0
; right laser, green or yellow start, sound(4 or 5)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $C000

; frame vor_frame_right_laser10 ; (10/60, 0x0A)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $CA00
; frame vor_frame_right_laser12 ; (12/60, 0x0C)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $CC00
; frame vor_frame_right_laser15 ; (15/60, 0x0F)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $CF00
; frame vor_frame_right_laser20 ; (20/60, 0x14)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $D400
; frame vor_frame_right_laser24 ; (24/60, 0x18)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $D800
; frame vor_frame_right_laser30 ; (30/60, 0x1E)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $DE00
; frame vor_frame_right_laser36 ; (36/60, 0x24)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $E400
; frame vor_frame_right_laser40 ; (40/60, 0x28)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $E800
; frame vor_frame_right_laser45 ; (45/60, 0x2D)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $ED00
; frame vor_frame_right_laser48 ; (48/60, 0x30)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $F000
; frame vor_frame_right_laser50 ; (50/60, 0x32)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $F200
; frame vor_frame_right_laser60 ; end (60/60, 0x3C)
    .word _robot0_right_head_pic, _robot0_main_shoulders_pic, _robot0_main_knees_pic, _robot0_main_toes_pic, $FC00
