/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* robot0_anim.h
 *
 * Animation definitions: robot0
 *
 */

#ifndef __ROBOT0_ANIM_H__
#define __ROBOT0_ANIM_H__

extern int vor_anim_standby; // standing still and doing nothing
// { "pic":"vor_frame_stand", "next":"vor_anim_standby", "exit":"true" }

extern int vor_anim_angry_loop; // angry foot tapping
// { "pic":"vor_frame_angry2", "exit":"true" }
// { "pic":"vor_frame_angry1", "exit":"true", "next":"vor_anim_angry_loop" }

extern int vor_anim_angry_notfound; // angry foot tapping
// { "pic":"vor_frame_angry1", "exit":"true", "next":"vor_anim_angry_loop", "sfxlevel":1, "sound":"cueWordNotFound" }
extern int vor_anim_angry_badkey; // angry foot tapping
// { "pic":"vor_frame_angry1", "exit":"true", "next":"vor_anim_angry_loop", "sfxlevel":1, "sound":"cueBadKey" }

extern int vor_anim_bored; // bored foot tapping
// { "pic":"vor_frame_stand", "exit":"true" }
// { "pic":"vor_frame_bored2", "exit":"true" }
// { "pic":"vor_frame_bored3", "exit":"true" }
// { "pic":"vor_frame_bored2", "next":"vor_anim_bored", "exit":"true" }


extern int vor_anim_victory_loop; // victory dance
// Each frame is a 1/16 note; 16 frames is one measure of 4/4 time

// { "t":"0", "pic":"vor_frame_wrist2" }
// { "t":"1", "pic":"vor_frame_wrist1", "measure":1, "lyric":"one" }
// { "t":"2", "pic":"vor_frame_wrist2" }
// { "t":"3", "pic":"vor_frame_wrist2" }

// { "t":"4", "pic":"vor_frame_wrist2" }
// { "t":"5", "pic":"vor_frame_wrist1", "lyric":"two" }
// { "t":"6", "pic":"vor_frame_wrist2" }
// { "t":"7", "pic":"vor_frame_wrist2" }

// { "t":"8", "pic":"vor_frame_wrist2", "exit":"debug" }
// { "t":"9", "pic":"vor_frame_wrist1", "lyric":"three" }
// { "t":"A", "pic":"vor_frame_wrist2" }
// { "t":"B", "pic":"vor_frame_wrist2" }

// { "t":"C", "pic":"vor_frame_wrist2" }
// { "t":"D", "pic":"vor_frame_wrist1", "lyric":"four" }
// { "t":"E", "pic":"vor_frame_wrist2" }
// { "t":"F", "pic":"vor_frame_wrist2" }



// { "t":"0", "pic":"vor_frame_wrist2" }
// { "t":"1", "pic":"vor_frame_wrist1", "measure":2, "lyric":"one" }
// { "t":"2", "pic":"vor_frame_wrist2" }
// { "t":"3", "pic":"vor_frame_wrist2" }

// { "t":"4", "pic":"vor_frame_wrist2" }
// { "t":"5", "pic":"vor_frame_wrist1", "lyric":"two" }
// { "t":"6", "pic":"vor_frame_wrist2" }
// { "t":"7", "pic":"vor_frame_wrist2" }

// { "t":"8", "pic":"vor_frame_wrist2" }
// { "t":"9", "pic":"vor_frame_wrist1", "lyric":"three" }
// { "t":"A", "pic":"vor_frame_wrist2" }
// { "t":"B", "pic":"vor_frame_wrist2" }

// { "t":"C", "pic":"vor_frame_wrist2" }
// { "t":"D", "pic":"vor_frame_wrist1", "lyric":"four" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1" }



// { "t":"0", "pic":"vor_frame_happy_lefty2" }
// { "t":"1", "pic":"vor_frame_happy_leftpoint2", "measure":3, "lyric":"You" }
// { "t":"2", "pic":"vor_frame_happy_lefty2" }
// { "t":"3", "pic":"vor_frame_happy_leftkick", "lyric":"Say" }

// { "t":"4", "pic":"vor_frame_happy_leftkickback" }
// { "t":"5", "pic":"vor_frame_happy_lefty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_leftpoint1", "lyric":"Toe" }

// { "t":"8", "pic":"vor_frame_happy_lefty2", "exit":"debug" }
// { "t":"9", "pic":"vor_frame_happy_leftpoint2", "lyric":"May" }
// { "t":"A", "pic":"vor_frame_happy_lefty2" }
// { "t":"B", "pic":"vor_frame_happy_leftkick", "lyric":"Toe" }

// { "t":"C", "pic":"vor_frame_happy_leftkickback" }
// { "t":"D", "pic":"vor_frame_happy_lefty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1", "lyric":"And" }



// { "t":"0", "pic":"vor_frame_happy_lefty2" }
// { "t":"1", "pic":"vor_frame_happy_leftpoint2", "measure":4, "lyric":"I" }
// { "t":"2", "pic":"vor_frame_happy_lefty2" }
// { "t":"3", "pic":"vor_frame_happy_leftkick", "lyric":"Say" }

// { "t":"4", "pic":"vor_frame_happy_leftkickback" }
// { "t":"5", "pic":"vor_frame_happy_lefty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_leftpoint1", "lyric":"Toe" }

// { "t":"8", "pic":"vor_frame_happy_lefty2" }
// { "t":"9", "pic":"vor_frame_happy_leftpoint2", "lyric":"Mah" }
// { "t":"A", "pic":"vor_frame_happy_lefty2" }
// { "t":"B", "pic":"vor_frame_happy_leftkick", "lyric":"Toe" }

// { "t":"C", "pic":"vor_frame_happy_leftkickback" }
// { "t":"D", "pic":"vor_frame_happy_lefty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_rightpoint1" }



// { "t":"0", "pic":"vor_frame_happy_righty2" }
// { "t":"1", "pic":"vor_frame_happy_rightpoint2", "measure":5, "lyric":"I" }
// { "t":"2", "pic":"vor_frame_happy_righty2" }
// { "t":"3", "pic":"vor_frame_happy_rightkick", "lyric":"Say" }

// { "t":"4", "pic":"vor_frame_happy_rightkickback" }
// { "t":"5", "pic":"vor_frame_happy_righty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_rightpoint1", "lyric":"Po" }

// { "t":"8", "pic":"vor_frame_happy_righty2", "exit":"debug" }
// { "t":"9", "pic":"vor_frame_happy_rightpoint2", "lyric":"Tay" }
// { "t":"A", "pic":"vor_frame_happy_righty2" }
// { "t":"B", "pic":"vor_frame_happy_rightkick", "lyric":"Toe" }

// { "t":"C", "pic":"vor_frame_happy_rightkickback" }
// { "t":"D", "pic":"vor_frame_happy_righty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_rightpoint1", "lyric":"And" }



// { "t":"0", "pic":"vor_frame_happy_righty2" }
// { "t":"1", "pic":"vor_frame_happy_rightpoint2", "measure":6, "lyric":"You" }
// { "t":"2", "pic":"vor_frame_happy_righty2" }
// { "t":"3", "pic":"vor_frame_happy_rightkick", "lyric":"Say" }

// { "t":"4", "pic":"vor_frame_happy_rightkickback" }
// { "t":"5", "pic":"vor_frame_happy_righty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_rightpoint1", "lyric":"Po" }

// { "t":"8", "pic":"vor_frame_happy_righty2" }
// { "t":"9", "pic":"vor_frame_happy_rightpoint2", "lyric":"Tah" }
// { "t":"A", "pic":"vor_frame_happy_righty2" }
// { "t":"B", "pic":"vor_frame_happy_rightkick", "lyric":"Toe" }

// { "t":"C", "pic":"vor_frame_happy_rightkickback" }
// { "t":"D", "pic":"vor_frame_happy_righty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1", "lyric":"TOE" }



// { "t":"0", "pic":"vor_frame_happy_lefty2" }
// { "t":"1", "pic":"vor_frame_happy_leftpoint2", "measure":7, "lyric":"MAY" }
// { "t":"2", "pic":"vor_frame_happy_lefty2" }
// { "t":"3", "pic":"vor_frame_happy_leftkick", "lyric":"TOE" }

// { "t":"4", "pic":"vor_frame_happy_leftkickback" }
// { "t":"5", "pic":"vor_frame_happy_lefty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_rightpoint1", "lyric":"TOE" }

// { "t":"8", "pic":"vor_frame_happy_righty2", "exit":"debug" }
// { "t":"9", "pic":"vor_frame_happy_rightpoint2", "lyric":"MAH" }
// { "t":"A", "pic":"vor_frame_happy_righty2" }
// { "t":"B", "pic":"vor_frame_happy_rightkick", "lyric":"TOE" }

// { "t":"C", "pic":"vor_frame_happy_rightkickback" }
// { "t":"D", "pic":"vor_frame_happy_righty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1", "lyric":"POE" }



// { "t":"0", "pic":"vor_frame_happy_lefty2" }
// { "t":"1", "pic":"vor_frame_happy_leftpoint2", "measure":8, "lyric":"TAY" }
// { "t":"2", "pic":"vor_frame_happy_lefty2" }
// { "t":"3", "pic":"vor_frame_happy_leftkick", "lyric":"TOE" }

// { "t":"4", "pic":"vor_frame_happy_leftkickback" }
// { "t":"5", "pic":"vor_frame_happy_lefty1" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_rightpoint1", "lyric":"PO" }

// { "t":"8", "pic":"vor_frame_happy_righty2" }
// { "t":"9", "pic":"vor_frame_happy_rightpoint2", "lyric":"TAH" }
// { "t":"A", "pic":"vor_frame_happy_righty2" }
// { "t":"B", "pic":"vor_frame_happy_rightkick", "lyric":"TOE" }

// { "t":"C", "pic":"vor_frame_happy_rightkickback" }
// { "t":"D", "pic":"vor_frame_happy_righty1" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1" }



// { "t":"0", "pic":"vor_frame_happy_lefty2" }
// { "t":"1", "pic":"vor_frame_happy_leftpoint2", "measure":9, "lyric":"Lets" }
// { "t":"2", "pic":"vor_frame_happy_lefty2" }
// { "t":"3", "pic":"vor_frame_happy_leftkick" }

// { "t":"4", "pic":"vor_frame_happy_leftkickback" }
// { "t":"5", "pic":"vor_frame_happy_lefty1", "lyric":"call" }
// { "t":"6", "pic":"vor_frame_happy" }
// { "t":"7", "pic":"vor_frame_happy_rightpoint1", "lyric":"the" }

// { "t":"8", "pic":"vor_frame_happy_righty2", "exit":"debug" }
// { "t":"9", "pic":"vor_frame_happy_rightpoint2", "lyric":"whole" }
// { "t":"A", "pic":"vor_frame_happy_righty2" }
// { "t":"B", "pic":"vor_frame_happy_rightkick" }

// { "t":"C", "pic":"vor_frame_happy_rightkickback" }
// { "t":"D", "pic":"vor_frame_happy_righty1", "lyric":"thing" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy_leftpoint1" }



// { "t":"0", "pic":"vor_frame_happy" }
// { "t":"1", "pic":"vor_frame_happy_rightpoint1", "measure":10, "lyric":"off" }
// { "t":"2", "pic":"vor_frame_jump_crouch1" }
// { "t":"3", "pic":"vor_frame_jump_crouch2" }

// { "t":"4", "pic":"vor_frame_jump_up" }
// { "t":"5", "pic":"vor_frame_jump_air" }
// { "t":"6", "pic":"vor_frame_jump_air" }
// { "t":"7", "pic":"vor_frame_jump_air" }

// { "t":"8", "pic":"vor_frame_jump_down" }
// { "t":"9", "pic":"vor_frame_happy" }
// { "t":"A", "pic":"vor_frame_happy_leftpoint1" }
// { "t":"B", "pic":"vor_frame_happy" }

// { "t":"C", "pic":"vor_frame_happy_rightpoint1" }
// { "t":"D", "pic":"vor_frame_happy" }
// { "t":"E", "pic":"vor_frame_happy" }
// { "t":"F", "pic":"vor_frame_happy", "next":"vor_anim_victory_loop", "measure":0, "exit":"countdown" }

extern int vor_anim_victory; // victory dance
// { "pic":"vor_frame_wrist2", "next":"vor_anim_victory_loop" }

extern int vor_anim_loss_loop; // sad dance
// { "pic":"vor_frame_sad2", "exit":"true", "next":"vor_anim_loss_loop" }
extern int vor_anim_loss; // sad dance
// { "pic":"vor_frame_sad1", "sfxlevel":0, "sound":"cueSadTrombone" }
// { "pic":"vor_frame_sad2" }
// { "pic":"vor_frame_sad3", "next":"vor_anim_loss_loop" }

extern int vor_anim_laserwait; // fast wrist computer check for between lasers
// { "pic":"vor_frame_wrist1", "sfxlevel":3, "sound":"cueRobotsOfDeath_LOW", "exit":"true" }
// { "pic":"vor_frame_wrist2", "exit":"countdown" }
// { "pic":"vor_frame_wrist3", "sfxlevel":3, "sound":"cueRobotsOfDeath_HIGH", "exit":"true" }
// { "pic":"vor_frame_wrist2", "next":"vor_anim_laserwait", "exit":"true" }

extern int vor_anim_laserleft5; // Left laser to tile 5
// { "pic":"vor_frame_left_laser60", "next":"vor_anim_laserwait" }
extern int vor_anim_laserleft4; // Left laser to tile 4
// { "pic":"vor_frame_left_laser30" }
// { "pic":"vor_frame_left_laser60", "next":"vor_anim_laserwait" }
extern int vor_anim_laserleft3; // Left laser to tile 3
// { "pic":"vor_frame_left_laser20" }
// { "pic":"vor_frame_left_laser40" }
// { "pic":"vor_frame_left_laser60", "next":"vor_anim_laserwait" }
extern int vor_anim_laserleft2; // Left laser to tile 2
// { "pic":"vor_frame_left_laser15" }
// { "pic":"vor_frame_left_laser30" }
// { "pic":"vor_frame_left_laser45" }
// { "pic":"vor_frame_left_laser60", "next":"vor_anim_laserwait" }
extern int vor_anim_laserleft1; // Left laser to tile 1
// { "pic":"vor_frame_left_laser12" }
// { "pic":"vor_frame_left_laser24" }
// { "pic":"vor_frame_left_laser36" }
// { "pic":"vor_frame_left_laser48" }
// { "pic":"vor_frame_left_laser60", "next":"vor_anim_laserwait" }

extern int vor_anim_laserleft_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft3" }
extern int vor_anim_laserleft1_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft1" }
extern int vor_anim_laserleft2_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft2" }
extern int vor_anim_laserleft3_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft3" }
extern int vor_anim_laserleft4_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft4" }
extern int vor_anim_laserleft5_yellow; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserleft5" }
extern int vor_anim_laserleft_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft3" }
extern int vor_anim_laserleft1_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft1" }
extern int vor_anim_laserleft2_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft2" }
extern int vor_anim_laserleft3_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft3" }
extern int vor_anim_laserleft4_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft4" }
extern int vor_anim_laserleft5_green; // Left laser
// { "pic":"vor_frame_left_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserleft5" }

extern int vor_anim_laserright; // Right laser
// { "pic":"vor_frame_right_laser20" }
// { "pic":"vor_frame_right_laser40" }
// { "pic":"vor_frame_right_laser60", "next":"vor_anim_laserwait" }

extern int vor_anim_laserright_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright1_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright2_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright3_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright4_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright5_yellow; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserYellow", "next":"vor_anim_laserright" }
extern int vor_anim_laserright_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }
extern int vor_anim_laserright1_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }
extern int vor_anim_laserright2_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }
extern int vor_anim_laserright3_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }
extern int vor_anim_laserright4_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }
extern int vor_anim_laserright5_green; // Right laser
// { "pic":"vor_frame_right_laser0", "sfxlevel":1, "sound":"cueLaserGreen", "next":"vor_anim_laserright" }

extern int vor_anim_fistpump_loop; // Green word
// { "pic":"vor_frame_wrist2", "exit":"true" }
// { "pic":"vor_frame_wrist3", "sfxlevel":3, "sound":"cueRobotsOfDeath_HIGH" }
// { "pic":"vor_frame_wrist2", "exit":"true" }
// { "pic":"vor_frame_wrist1", "sfxlevel":3, "sound":"cueRobotsOfDeath_LOW", "next":"vor_anim_fistpump_loop" }
extern int vor_anim_fistpump; // Green word
// (the lead in is a long sound effect)
// { "pic":"vor_frame_wrist1", "sfxlevel":0, "sound":"cueTadaa" }
// { "pic":"vor_frame_stand" }
// { "pic":"vor_frame_jump_crouch1" }
// { "pic":"vor_frame_jump_crouch2" }
// { "pic":"vor_frame_jump_up" }
// { "pic":"vor_frame_jump_air" }
// { "pic":"vor_frame_jump_air" }
// { "pic":"vor_frame_jump_down" }
// { "pic":"vor_frame_stand", "next":"vor_anim_fistpump_loop" }

extern int vor_anim_wristcheck; // Check wrist computer
// { "pic":"vor_frame_wrist1", "sfxlevel":3, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist2", "exit":"true" }
// { "pic":"vor_frame_wrist3", "sfxlevel":3, "sound":"cueRobotsOfDeath_HIGH"}
// { "pic":"vor_frame_wrist2",  "next":"vor_anim_wristcheck", "exit":"true" }
extern int vor_anim_lookup_wristcheck; // Check wrist computer
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist3", "sfxlevel":2, "sound":"cueRobotsOfDeath_HIGH"}
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist1", "sfxlevel":2, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist1", "sfxlevel":2, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist3", "sfxlevel":2, "sound":"cueRobotsOfDeath_HIGH"}
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist1", "sfxlevel":2, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist2",  "next":"vor_anim_wristcheck" }

extern int vor_anim_bored_wristcheck; // Tap wrist computer
// { "pic":"vor_frame_wrist2",  "exit":"true" }
// { "pic":"vor_frame_wrist3",  "sfxlevel":3, "sound":"cueRobotsOfDeath_HIGH", "exit":"true" }
// { "pic":"vor_frame_wrist2",  "exit":"true" }
// { "pic":"vor_frame_wrist2",  "exit":"true" }
// { "pic":"vor_frame_wrist1",  "sfxlevel":3, "sound":"cueRobotsOfDeath_LOW", "exit":"true" }
// { "pic":"vor_frame_wrist2",  "next":"vor_anim_bored_wristcheck", "exit":"true" }

extern int vor_anim_alldone_wristcheck; // Tap wrist computer
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist3", "sfxlevel":1, "sound":"cueRobotsOfDeath_HIGH"}
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist1", "sfxlevel":1, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist1", "sfxlevel":1, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist3", "sfxlevel":1, "sound":"cueRobotsOfDeath_HIGH"}
// { "pic":"vor_frame_wrist2" }
// { "pic":"vor_frame_wrist1", "sfxlevel":1, "sound":"cueRobotsOfDeath_LOW" }
// { "pic":"vor_frame_wrist2",  "next":"vor_anim_standby" }

#endif
