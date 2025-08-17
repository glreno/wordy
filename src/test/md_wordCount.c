/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Validation test for the wordl dictionary */

#include "md_dict.h"
#include "yield.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <_atarios.h>


int dictWordCount(md_dict *d)
{
    int nvol = d->size;
    int n = 0;
    int i;
    const md_volume *v;

    for(i=0; i<nvol; i++)
    {
        v=d->dictionaries[i];
        if ( i > 1 && v == d->dictionaries[0] )
        {
            return n;
        }
        n+=v->size;
    }
    return n;
}

void test(md_dict *d,char *name)
{
    int n;
    n = md_size(d);
    printf("%s\t%d\t",name,n);
    n=dictWordCount(d);
    printf("%d\n",n);
}

void __fastcall__ resetScreenSaver(void)
{
    OS.atract=0; // keep attract mode off
}

extern md_dict DICT_EA;
extern md_dict DICT_ES;
extern md_dict DICT_ES_UK;
extern md_dict DICT_ES_US;

extern md_dict DICT_MA;
extern md_dict DICT_MS;
extern md_dict DICT_MS_UK;
extern md_dict DICT_MS_US;

extern md_dict DICT_HA;
extern md_dict DICT_HS;
extern md_dict DICT_HS_UK;
extern md_dict DICT_HS_US;

extern md_dict DICT_SA;
extern md_dict DICT_SS;
extern md_dict DICT_SS_UK;
extern md_dict DICT_SS_US;

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    setYieldFunc(&resetScreenSaver);
    clrscr();
    printf("Dict\ttotal\twords\n\n");

    test(&DICT_EA,"EA   ");
    test(&DICT_ES,"ES   ");
    test(&DICT_ES_UK,"ES_UK");
    test(&DICT_ES_US,"ES_US");

    test(&DICT_MA,"MA   ");
    test(&DICT_MS,"MS   ");
    test(&DICT_MS_UK,"MS_UK");
    test(&DICT_MS_US,"MS_US");

    test(&DICT_HA,"HA   ");
    test(&DICT_HS,"HS   ");
    test(&DICT_HS_UK,"HS_UK");
    test(&DICT_HS_US,"HS_US");

    test(&DICT_SA,"SA   ");
    test(&DICT_SS,"SS   ");
    test(&DICT_SS_UK,"SS_UK");
    test(&DICT_SS_US,"SS_US");
    printf("\nAll done!\n");
    for(;;)
        ;
    return 1;
}
