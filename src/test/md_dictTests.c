/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* Unit tests for the md_dict.h functions */

#include "md_dict.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <_atarios.h>

const md_word WORD[] = {
    { "super", 6 },
    { "supes", 8 },
    { "today", 7 }
};

const md_volume DICT1 = { 3, 0, WORD };
const md_dict DICT = { 1, { &DICT1 } };

extern md_word cvcvcDict1;
extern md_word cvcvcDict2;
extern md_word cvcvcDict3;
const md_volume CVDICT1 = { 67, 0, &cvcvcDict1 };
const md_volume CVDICT2 = { 67, 0, &cvcvcDict2 };
const md_volume CVDICT3 = { 67, 0, &cvcvcDict3 };
const md_dict CVDICT = { 3, { &CVDICT1, &CVDICT2, &CVDICT3 } };


void dictTests(void)
{
    char buf0[6];
    int i,j,n;
    //md_word *w;
    md_word tword;

    printf("\x7DThe Dictionary Test\n");

    n = md_findWord(&DICT, (md_word*)"supes", NULL);
    printf("[supes] is at %d\n",n);

    n = md_findWord(&DICT, (md_word*)"gloom", NULL);
    printf("[gloom] is not found: %d\n",n);

    n = md_size(&DICT);
    printf("Size: Dict contains: %d\n",n);

    for(i=0; i<n; i++)
    {
        md_getWord(&DICT,i,&tword);
        md_wordToString(buf0,&tword);
        j = md_findWord(&DICT,(md_word*)buf0, NULL);
        printf("%d: [%s] found at %d f=%d\n",i,buf0,j,tword.wordflags);
    }

    md_getWord(&DICT,-1,&tword);
    printf("get(-1) should return 0xff: %d\n",tword.wordflags);
    md_getWord(&DICT,n,&tword);
    printf("get(len) should return 0xff: %d\n",tword.wordflags);

    n = md_findWord(&CVDICT, (md_word*)"BABEL", &tword);
    md_wordToString(buf0,&tword);
    printf("0:[BABEL] is at %d %s f:%d\n",n,buf0,tword.wordflags);

    n = md_findWord(&CVDICT, (md_word*)"FINAL", &tword);
    md_wordToString(buf0,&tword);
    printf("101:[FINAL] is at %d %s f:%d\n",n,buf0,tword.wordflags);

    n = md_findWord(&CVDICT, (md_word*)"NATAL", &tword);
    md_wordToString(buf0,&tword);
    printf("200:[NATAL] is at %d %s f:%d\n",n,buf0,tword.wordflags);

    n = md_findWord(&CVDICT, (md_word*)"!WORD", &tword);
    printf("[!WORD] is at %d f:%d\n",n,tword.wordflags);

    n = md_size(&CVDICT);
    printf("Size: CVDict contains: %d\n",n);
}

void dictSpeedTests()
{
    /* TODO this is just cloned from dictTests.c */
    char buf0[6];
    int i,j,n;
    //md_word *w;
    md_word tword;
    unsigned int after;
    unsigned int diff;
    unsigned int max = 0;
    unsigned int total = 0;
    unsigned int start = 0xffff;

    printf("Timing and verifying 200 finds\n",n);
    n = md_size(&CVDICT);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        md_getWord(&CVDICT,i,&tword);
        md_wordToString(buf0,&tword);
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(&CVDICT,(md_word*)buf0,NULL);
        after=OS.cdtmv1;
        diff=start-after;
        if ( diff > max )
        {
            max = diff;
        }
        if ( i!=j )
        {
            printf("FAILED to find %s\n",buf0);
        }
    }
    after=OS.cdtmv2;
    total=start-after;
    printf("Duration: %d jiffies, %d sec\n",total,total/60);
    printf("Avg %d jiffies per word, max %d\n",total/n,max);

    printf("Timing and verifying 200 not finds\n",n);
    total=0;
    max=0;
    n = md_size(&CVDICT);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        md_getWord(&CVDICT,i,&tword);
        md_wordToString(buf0,&tword);
        ++buf0[1]; /* increment the first vowel, this is NOT a word. */
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(&CVDICT,(md_word*)buf0,NULL);
        after=OS.cdtmv1;
        diff=start-after;
        if ( diff > max )
        {
            max = diff;
        }
    }
    after=OS.cdtmv2;
    total=start-after;
    printf("Duration: %d jiffies, %d sec\n",total,total/60);
    printf("Avg %d jiffies per word, max %d\n",total/n,max);
}

int main(void)
{
    if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset
    //char k;
    dictTests();
    dictSpeedTests();
    //printf("\nPress a key to continue\n");
    //k = cgetc();
    printf("\nAll done!\n");
    for(;;)
        ;
    return 1;
}
