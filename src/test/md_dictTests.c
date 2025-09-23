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

md_wordInternal IWORD[3];

const md_word WORD[] = {
    { "Snipe", 6 },
    { "Snips", 8 },
    { "Swipe", 7 }
};

md_lexicon DICT1L; // empty list of 26 md_wordList
const md_dict DICT = { 1, { 1, &DICT1L }};
                    // ^    ^
                    // |    |
                    // |    +--- volume count
                    // +- array size

extern md_dict CVDICT;

void buildDict(void)
{
    md_wordToWordInternal(&IWORD[0],&WORD[0]);
    md_wordToWordInternal(&IWORD[1],&WORD[1]);
    md_wordToWordInternal(&IWORD[2],&WORD[2]);
    DICT1L.array_length=26;
    DICT1L.wordList[18].size=3;
    DICT1L.wordList[18].firstLetter='S';
    DICT1L.wordList[18].bank=0;
    DICT1L.wordList[18].list=IWORD;
}

void lookupTest(int expected,char *bufp)
{
    char buf0[6],buf1[6];
    md_word tword;
    int n = md_findWord(&CVDICT, (md_word*)bufp, &tword);
    md_wordToString(buf0,&tword);
    md_getWord(&CVDICT,n,&tword);
    md_wordToString(buf1,&tword);
    printf("%d:[%s] is at %d %s %s f:%d\n",expected,bufp,n,buf0,buf1,tword.wordflags);
}

void dictTests(void)
{
    char buf0[6];
    int i,j,n;
    //md_word *w;
    md_word tword;

    printf("\x7DThe Dictionary Test\n");

    n = md_findWord(&DICT, (md_word*)"Snips", NULL);
    printf("[Snips] is at %d\n",n);

    n = md_findWord(&DICT, (md_word*)"Gloom", NULL);
    printf("[Gloom] is not found: %d\n",n);

    md_bankswitchIdx(); // BANK SWITCH!
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

    lookupTest(0,"BABEL");
    lookupTest(35,"BOSOM");
    lookupTest(36,"CABAL");
    lookupTest(67,"COVET");
    lookupTest(68,"DAVIT");
    lookupTest(200,"NATAL");

    n = md_findWord(&CVDICT, (md_word*)"!WORD", &tword);
    printf("[!WORD] is at %d f:%d\n",n,tword.wordflags);
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
    unsigned int total = 0;
    unsigned int start = 0xffff;

    md_bankswitchIdx(); // BANK SWITCH!
    n = md_size(&CVDICT);
    printf("Size: CVDict contains: %d\n",n);
    printf("Timing and verifying %d finds\n",n);
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

    printf("Timing and verifying 201 not finds\n",n);
    total=0;
    max=0;
    md_bankswitchIdx(); // BANK SWITCH!
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
    //if ( OS.ramtop < 0xA0 ) { cputs("Min 40K"); for(;;); } // 9C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset
    //char k;
    buildDict();
    dictTests();
    dictSpeedTests();
    //printf("\nPress a key to continue\n");
    //k = cgetc();
    printf("All done!\n");
    for(;;)
        ;
    return 1;
}
