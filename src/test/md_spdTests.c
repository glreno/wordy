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

extern const int _CARTBANK0_LOAD__;
extern const int _CARTBANK0_RUN__;
extern const int _CARTBANK1_LOAD__;
extern const int _CARTBANK1_RUN__;
extern const int _CARTBANK2_LOAD__;
extern const int _CARTBANK2_RUN__;
extern const int _CARTBANK3_LOAD__;
extern const int _CARTBANK3_RUN__;
extern const int _CARTBANK4_LOAD__;
extern const int _CARTBANK4_RUN__;
extern const int _CARTBANK5_LOAD__;
extern const int _CARTBANK5_RUN__;
extern const int _CARTBANK6_LOAD__;
extern const int _CARTBANK6_RUN__;
extern md_dict CVDICT;
int *pagesix=(int*)0x0600;

void lookupTest(const char *buf1)
{
    int n;
    md_word tword;
    char buf0[6];
    n = md_findWord(&CVDICT, (md_word*)buf1, &tword);
    md_wordToString(buf0,&tword);
    printf("0:[%s] is at %d %s f:%d\n",buf1, n,buf0,tword.wordflags);
}

void dictTests(void)
{
    printf("\x7DThe Performance Test\n");

    lookupTest("AAAAB"); // dict_Big0 first word
    lookupTest("AACCP"); // dict_Big0 last word
    lookupTest("AACCQ"); // dict_Big1 first word
    lookupTest("AAEFF"); // dict_Big1 last word
    lookupTest("AAEFG"); // dict_Big2 first word
    lookupTest("AAGHU"); // dict_Big2 last word
    lookupTest("AAGHV"); // dict_Big3 first word
    lookupTest("AAGZZ"); // dict_Big3 last word
    lookupTest("AAABA"); // dict_A first word
    lookupTest("AAGZA"); // dict_A last word
    lookupTest("AABAA"); // dict_AA first word
    lookupTest("AAGAA"); // dict_AA last word
    lookupTest("AAAAA"); // dict AAA only word
    lookupTest("!WORD");
}

void dictSpeedTests()
{
    char buf0[6];
    int i,j,n;
    //md_word *w;
    md_word tword;
    unsigned int after;
    unsigned int diff;
    unsigned int max = 0;
    unsigned long total = 0;
    unsigned int total1 = 0;
    unsigned long total2 = 0;
    unsigned int start = 0xffff;

    n = md_size(&CVDICT);
    printf("Timing and verifying %d finds\n",n);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        *pagesix = i;
        md_getWord(&CVDICT,i,&tword);
        md_wordToString(buf0,&tword);
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(&CVDICT,(md_word*)buf0,NULL);
        after=OS.cdtmv1;
        diff=start-after;
        total2 += diff;
        if ( diff > max )
        {
            max = diff;
        }
        if ( diff <= 1 )
        {
            ++total1;
        }
        if ( i!=j )
        {
            printf("FAILED to find %s\n",buf0);
for(;;) ;
        }
    }
    after=OS.cdtmv2;
    total=start-after;
    printf("Duration: %ld jiffies, %ld sec\n",total,total/60);
    printf("Findonly: %ld jiffies, %ld sec\n",total2,total2/60);
    printf("Avg %ld jiffies per word, max %d\n",total2/n,max);
    printf("%d words <= 1 jiffy\n",total1);

    printf("Timing and verifying %d not finds\n",n);
    total=0;
    total1=0;
    total2=0;
    max=0;
    n = md_size(&CVDICT);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        *pagesix = i;
        md_getWord(&CVDICT,i,&tword);
        md_wordToString(buf0,&tword);
        buf0[4]='!'; /* clear the last letter, this is NOT a word. */
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(&CVDICT,(md_word*)buf0,NULL);
        after=OS.cdtmv1;
        diff=start-after;
        total2 += diff;
        if ( diff > max )
        {
            max = diff;
        }
        if ( diff <= 1 )
        {
            ++total1;
        }
    }
    after=OS.cdtmv2;
    total=start-after;
    printf("Duration: %ld jiffies, %ld sec\n",total,total/60);
    printf("Findonly: %ld jiffies, %ld sec\n",total2,total2/60);
    printf("Avg %ld jiffies per word, max %d\n",total2/n,max);
    printf("%d words <= 1 jiffy\n",total1);
}

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    //char k;
    dictTests();
    printf("Bank0 = %x run %x\n",&_CARTBANK0_LOAD__,&_CARTBANK0_RUN__);
    printf("Bank1 = %x run %x\n",&_CARTBANK1_LOAD__,&_CARTBANK1_RUN__);
    printf("Bank2 = %x run %x\n",&_CARTBANK2_LOAD__,&_CARTBANK2_RUN__);
    printf("Bank3 = %x run %x\n",&_CARTBANK3_LOAD__,&_CARTBANK3_RUN__);
    printf("Bank4 = %x run %x\n",&_CARTBANK4_LOAD__,&_CARTBANK4_RUN__);
    printf("Bank5 = %x run %x\n",&_CARTBANK5_LOAD__,&_CARTBANK5_RUN__);
    printf("Bank6 = %x run %x\n",&_CARTBANK6_LOAD__,&_CARTBANK6_RUN__);
    dictSpeedTests();
    //printf("\nPress a key to continue\n");
    //k = cgetc();
    printf("\nAll done!\n");
    for(;;)
        ;
    return 1;
}
