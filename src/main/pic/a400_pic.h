/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* a400_pic.h
 *
 * Picture definitions: a400
 *
 */

#ifndef __ROBOT0_H__
#define __ROBOT0_H__

#include <stdlib.h>

// { "params":"this is a parameter block","picsegment":"FONTDATA","idxsegment":"FONTDATA","makepics":false,"makestripes":false }

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

// The "display" overlaps the shoulders, at src=8,56 size 2,1 (orig image rect 8,56 to 15,63

// Main static image

extern void *a400_main_shoulders_pic[];
// {"name":"a400_main_shoulders_pic","fn":"a401.png","src":[4,48],"size":[4,2],"dst":[1,6],"bank":""}

extern void *a400_main_knees_pic[];
// {"name":"a400_main_knees_pic","fn":"a401.png","src":[0,64],"size":[6,2],"dst":[0,0],"bank":""}


#endif
