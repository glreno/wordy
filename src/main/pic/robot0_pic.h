/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* robot0.h
 *
 * Animation definitions: robot0
 *
 */

#ifndef __ROBOT0_H__
#define __ROBOT0_H__

#include <stdlib.h>

// Bank destinations:
// bank 0: Cart bank 0 (audio cues, trampolined functions)
// bank 1: Cart bank 1 (mostly-full dictionary bank)
// bank 2: Cart bank 2 (mostly-full dictionary bank)
// bank 4: Cart bank 4 (half-full dictionary bank)
// bank 5: Cart bank 5 (robot animation index and view code)

// Aligining the src coords to the dst coords:
//  0,0 0,0
//  4,8 1,1
//  8,16    2,2
//  12,24   3,3
//  16,32   4,4
//  20,40   5,5
//  24,48   6,6
//   x,56   x,7
//   x,63   x,8

// What that means below:
// "src" is the coordinates in the original image
// "dst" is the coordinates in Characters in the destination image
// "size" is in characters
// So the 'head' pic is 4x4 characters, 16x32 pixels
// In the original image, it starts at 4,16 and goes to 19,47 inclusive
// Note that in each pic the dest coordinates are there too -- so you just need to
// say ds_copyPicToFont( robot0_main_head_pic, fontbase)

// The "display" overlaps the shoulders, at src=8,56 size 2,1 (orig image rect 8,56 to 15,63

// Main static image

extern void *robot0_main_head_pic[];
// {"name":"robot0_main_head_pic","fn":"robot0.24x128.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_main_shoulders_pic[];
// {"name":"robot0_main_shoulders_pic","fn":"robot0.24x128.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_main_knees_pic[];
// {"name":"robot0_main_knees_pic","fn":"robot0.24x128.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_main_toes_pic[];
// {"name":"robot0_main_toes_pic","fn":"robot0.24x128.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Angry image

extern void *robot0_angry_head_pic[];
// {"name":"robot0_angry_head_pic","fn":"robot0.angry.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_angry_shoulders_pic[];
// {"name":"robot0_angry_shoulders_pic","fn":"robot0.angry.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_angry_knees_pic[];
// {"name":"robot0_angry_knees_pic","fn":"robot0.angry.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":1}

extern void *robot0_angry_toes_pic[];
// {"name":"robot0_angry_toes_pic","fn":"robot0.angry.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Tapping wrist computer image 1

extern void *robot0_wrist1_head_pic[];
// {"name":"robot0_wrist1_head_pic","fn":"robot0.wrist1.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_wrist1_shoulders_pic[];
// {"name":"robot0_wrist1_shoulders_pic","fn":"robot0.wrist1.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_wrist1_knees_pic[];
// {"name":"robot0_wrist1_knees_pic","fn":"robot0.wrist1.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_wrist1_toes_pic[];
// {"name":"robot0_wrist1_toes_pic","fn":"robot0.wrist1.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Tapping wrist computer image 2

extern void *robot0_wrist2_head_pic[];
// {"name":"robot0_wrist2_head_pic","fn":"robot0.wrist2.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_wrist2_shoulders_pic[];
// {"name":"robot0_wrist2_shoulders_pic","fn":"robot0.wrist2.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_wrist2_knees_pic[];
// #{"name":"robot0_wrist2_knees_pic","fn":"robot0.wrist2.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":0}

extern void *robot0_wrist2_toes_pic[];
// #{"name":"robot0_wrist2_toes_pic","fn":"robot0.wrist2.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Tapping wrist computer image 3

extern void *robot0_wrist3_head_pic[];
// {"name":"robot0_wrist3_head_pic","fn":"robot0.wrist3.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_wrist3_shoulders_pic[];
// {"name":"robot0_wrist3_shoulders_pic","fn":"robot0.wrist3.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_wrist3_knees_pic[];
// #{"name":"robot0_wrist3_knees_pic","fn":"robot0.wrist3.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":0}

extern void *robot0_wrist3_toes_pic[];
// #{"name":"robot0_wrist3_toes_pic","fn":"robot0.wrist3.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Happy dance - neutral position - fistbump

extern void *robot0_happy_head_pic[];
// #{"name":"robot0_happy_head_pic","fn":"robot0.happy.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_happy_shoulders_pic[];
// {"name":"robot0_happy_shoulders_pic","fn":"robot0.happy.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_happy_knees_pic[];
// {"name":"robot0_happy_knees_pic","fn":"robot0.happy.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_happy_toes_pic[];
// #{"name":"robot0_happy_toes_pic","fn":"robot0.happy.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}


// Left facing image

extern void *robot0_lefty_head_pic[];
// {"name":"robot0_lefty_head_pic","fn":"robot0.left.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_lefty_shoulders_pic[];
// {"name":"robot0_lefty_shoulders_pic","fn":"robot0.left.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_lefty_knees_pic[];
// {"name":"robot0_lefty_knees_pic","fn":"robot0.left.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_lefty_toes_pic[];
// {"name":"robot0_lefty_toes_pic","fn":"robot0.left.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Left facing image - Point toe - start of kick

extern void *robot0_leftpoint_head_pic[];
// #{"name":"robot0_leftpoint_head_pic","fn":"robot0.leftpoint.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_leftpoint_shoulders_pic[];
// #{"name":"robot0_leftpoint_shoulders_pic","fn":"robot0.leftpoint.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_leftpoint_knees_pic[];
// {"name":"robot0_leftpoint_knees_pic","fn":"robot0.leftpoint.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_leftpoint_toes_pic[];
// {"name":"robot0_leftpoint_toes_pic","fn":"robot0.leftpoint.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Left facing image - Kickback - end of kick

extern void *robot0_leftkickback_head_pic[];
// {"name":"robot0_leftkickback_head_pic","fn":"robot0.leftkickback.png","src":[0,16],"size":[5,4],"dst":[0,2],"bank":5}

extern void *robot0_leftkickback_shoulders_pic[];
// #{"name":"robot0_leftkickback_shoulders_pic","fn":"robot0.leftkickback.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_leftkickback_knees_pic[];
// {"name":"robot0_leftkickback_knees_pic","fn":"robot0.leftkickback.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_leftkickback_toes_pic[];
// {"name":"robot0_leftkickback_toes_pic","fn":"robot0.leftkickback.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Left facing image - Kick

extern void *robot0_leftkick_head_pic[];
// {"name":"robot0_leftkick_head_pic","fn":"robot0.leftkick.png","src":[0,16],"size":[5,4],"dst":[0,2],"bank":5}

extern void *robot0_leftkick_shoulders_pic[];
// {"name":"robot0_leftkick_shoulders_pic","fn":"robot0.leftkick.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_leftkick_knees_pic[];
// {"name":"robot0_leftkick_knees_pic","fn":"robot0.leftkick.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_leftkick_toes_pic[];
// {"name":"robot0_leftkick_toes_pic","fn":"robot0.leftkick.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}


// Right facing image

extern void *robot0_right_head_pic[];
// {"name":"robot0_right_head_pic","fn":"robot0.right.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_right_shoulders_pic[];
// {"name":"robot0_right_shoulders_pic","fn":"robot0.right.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_right_knees_pic[];
// {"name":"robot0_right_knees_pic","fn":"robot0.right.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_right_toes_pic[];
// {"name":"robot0_right_toes_pic","fn":"robot0.right.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Right facing image - Point toe - start of kick

extern void *robot0_rightpoint_head_pic[];
// #{"name":"robot0_rightpoint_head_pic","fn":"robot0.rightpoint.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_rightpoint_shoulders_pic[];
// #{"name":"robot0_rightpoint_shoulders_pic","fn":"robot0.rightpoint.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_rightpoint_knees_pic[];
// {"name":"robot0_rightpoint_knees_pic","fn":"robot0.rightpoint.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":5}

extern void *robot0_rightpoint_toes_pic[];
// {"name":"robot0_rightpoint_toes_pic","fn":"robot0.rightpoint.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Right facing image - Kickback - end of kick

extern void *robot0_rightkickback_head_pic[];
// {"name":"robot0_rightkickback_head_pic","fn":"robot0.rightkickback.png","src":[4,16],"size":[5,4],"dst":[1,2],"bank":5}

extern void *robot0_rightkickback_shoulders_pic[];
// #{"name":"robot0_rightkickback_shoulders_pic","fn":"robot0.rightkickback.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_rightkickback_knees_pic[];
// {"name":"robot0_rightkickback_knees_pic","fn":"robot0.rightkickback.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":0}

extern void *robot0_rightkickback_toes_pic[];
// {"name":"robot0_rightkickback_toes_pic","fn":"robot0.rightkickback.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Right facing image - kick

extern void *robot0_rightkick_head_pic[];
// {"name":"robot0_rightkick_head_pic","fn":"robot0.rightkick.png","src":[4,16],"size":[5,4],"dst":[1,2],"bank":5}

extern void *robot0_rightkick_shoulders_pic[];
// {"name":"robot0_rightkick_shoulders_pic","fn":"robot0.rightkick.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_rightkick_knees_pic[];
// {"name":"robot0_rightkick_knees_pic","fn":"robot0.rightkick.png","src":[4,64],"size":[4,5],"dst":[1,0],"bank":5}

extern void *robot0_rightkick_toes_pic[];
// {"name":"robot0_rightkick_toes_pic","fn":"robot0.rightkick.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Jump image 1 - arms up, knees bent

extern void *robot0_jump1_head_pic[];
// {"name":"robot0_jump1_head_pic","fn":"robot0.jump1.png","src":[0,16],"size":[6,4],"dst":[0,2],"bank":0}

extern void *robot0_jump1_shoulders_pic[];
// {"name":"robot0_jump1_shoulders_pic","fn":"robot0.jump1.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_jump1_knees_pic[];
// {"name":"robot0_jump1_knees_pic","fn":"robot0.jump1.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_jump1_toes_pic[];
// {"name":"robot0_jump1_toes_pic","fn":"robot0.jump1.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":0}

// Jump image 2 - arms up bent inward, legs straight (needed because the arms aren't there)

extern void *robot0_jump2_head_pic[];
// {"name":"robot0_jump2_head_pic","fn":"robot0.jump2.png","src":[4,16],"size":[4,4],"dst":[1,2],"bank":0}

extern void *robot0_jump2_shoulders_pic[];
// {"name":"robot0_jump2_shoulders_pic","fn":"robot0.jump2.png","src":[0,48],"size":[6,2],"dst":[0,6],"bank":0}

extern void *robot0_jump2_knees_pic[];
// {"name":"robot0_jump2_knees_pic","fn":"robot0.jump2.png","src":[0,64],"size":[6,5],"dst":[0,0],"bank":0}

extern void *robot0_jump2_toes_pic[];
// #{"name":"robot0_jump2_toes_pic","fn":"robot0.jump2.png","src":[0,104],"size":[6,2],"dst":[0,5],"bank":4}

// Jump image 3 - this is same as some jump1, with dst up a bit
// Note that a row moves from knees to shoulders

extern void *robot0_jump3_head_pic[];
// {"name":"robot0_jump3_head_pic","fn":"robot0.jump1.png","src":[0,16],"size":[6,4],"dst":[0,1],"bank":2}

extern void *robot0_jump3_shoulders_pic[];
// {"name":"robot0_jump3_shoulders_pic","fn":"robot0.jump1.png","src":[0,48],"size":[6,3],"dst":[0,5],"bank":4}

extern void *robot0_jump3_knees_pic[];
// {"name":"robot0_jump3_knees_pic","fn":"robot0.jump1.png","src":[0,72],"size":[6,4],"dst":[0,0],"bank":5}

extern void *robot0_jump3_toes_pic[];
// {"name":"robot0_jump3_toes_pic","fn":"robot0.jump1.png","src":[0,104],"size":[6,2],"dst":[0,4],"bank":0}

// Jump image 4 - some unrelated bits with dst up a bit
// i.e. jump2_knees main_toes

extern void *robot0_jump4_knees_pic[];
// {"name":"robot0_jump4_knees_pic","fn":"robot0.jump2.png","src":[0,72],"size":[6,4],"dst":[0,0],"bank":5}

extern void *robot0_jump4_toes_pic[];
// {"name":"robot0_jump4_toes_pic","fn":"robot0.24x128.png","src":[0,104],"size":[6,2],"dst":[0,4],"bank":0}

// Jump image 5 - crouching down -- everything shifted down a row
// Everything from jump5 except the head
// Note that a row moves from shoulders to knees

extern void *robot0_jump5_head_pic[];
// {"name":"robot0_jump5_head_pic","fn":"robot0.24x128.png","src":[4,16],"size":[4,4],"dst":[1,3],"bank":0}

extern void *robot0_jump5_shoulders_pic[];
// {"name":"robot0_jump5_shoulders_pic","fn":"robot0.jump5.png","src":[0,48],"size":[6,1],"dst":[0,7],"bank":2}

extern void *robot0_jump5_knees_pic[];
// {"name":"robot0_jump5_knees_pic","fn":"robot0.jump5.png","src":[0,56],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_jump5_toes_pic[];
// {"name":"robot0_jump5_toes_pic","fn":"robot0.jump5.png","src":[0,96],"size":[6,2],"dst":[0,5],"bank":0}

// Jump image 6 - crouching down -- everything shifted down a row
// Everything from jump5
// Note that a row moves from shoulders to knees

extern void *robot0_jump6_head_pic[];
// {"name":"robot0_jump6_head_pic","fn":"robot0.jump5.png","src":[4,16],"size":[4,4],"dst":[1,3],"bank":0}

extern void *robot0_jump6_shoulders_pic[];
// {"name":"robot0_jump6_shoulders_pic","fn":"robot0.jump5.png","src":[0,48],"size":[6,1],"dst":[0,7],"bank":0}

extern void *robot0_jump6_knees_pic[];
// {"name":"robot0_jump6_knees_pic","fn":"robot0.jump5.png","src":[0,56],"size":[6,5],"dst":[0,0],"bank":4}

extern void *robot0_jump6_toes_pic[];
// {"name":"robot0_jump6_toes_pic","fn":"robot0.jump5.png","src":[0,96],"size":[6,2],"dst":[0,5],"bank":0}

#endif
