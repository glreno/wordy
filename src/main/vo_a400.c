/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "a400_pic.h"
#include <string.h>
#include "ds_screendriver.h"
#include "vfm_fontmanager.h"
#include "vo_a400.h"

extern void* a400_main_shoulders_pic_stripe_1_block_6;
extern void* a400_main_knees_pic_stripe_0_block_0;

vo_a400 vo4_singleton;

#pragma code-name (push,"GAMEINIT")
#pragma rodata-name (push,"GAMEINIT")
#pragma local-strings (on)

void __fastcall__ vo4_initialize(unsigned char *font1, unsigned char* font2)
{
    char i,x,y,c;

    vo4_singleton.font=font1;
    memcpy(font1 +  1*8, &a400_main_shoulders_pic_stripe_1_block_6, 8*8);
    memcpy(font1 + 65*8, &a400_main_knees_pic_stripe_0_block_0,    12*8);
    memcpy(font2 + 65*8, &a400_main_knees_pic_stripe_0_block_0,    12*8);
    i=0;
    for(x=1;x<5;++x)
    {
        for(y=0;y<2;++y)
        {
            c=VFM_UNRESERVED_CODEPOINTS[i];
            if ( y >= 0 )
            {
                ds_putc(c,17+x,9+y);
            }
            ++i;
        }
    }
    i=21;
    for(x=0;x<6;++x)
    {
        for(y=0;y<2;++y)
        {
            c=VFM_UNRESERVED_CODEPOINTS[i];
            if ( y >= 0 )
            {
                ds_putc(c,17+x,11+y);
            }
            ++i;
        }
    }

    ds_setAnimTimerCallback(4,&vo4_animateTimerCallback);

#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)
// end of initialization code

}
