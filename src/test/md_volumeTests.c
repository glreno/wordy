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

const md_wordInternal WORD[] = {
    { "super" }, // , 6 },
    { "supes" }, // , 8 },
    { "today" }, // , 7 }
};

const md_volume DICT = { 3, 0, WORD };

extern md_word cvcvcDict;
const md_volume CVDICT = { 200, 0, &cvcvcDict };

void wordTests(void)
{
    char buf0[6],buf1[6],buf2[6];
    int n;
    printf("\x7DThe first Test\n");
    //printf("Storage: Word0 flag is: %d\n",WORD[0].wordflags);
    md_wordToString(buf0,&WORD[0]);
    printf("Storage: Word0 is: [%s]\n",buf0);
    md_wordToString(buf1,&WORD[1]);
    printf("Storage: Word1 is: [%s]\n",buf1);
    md_wordToString(buf2,&WORD[2]);
    printf("Storage: Word2 is: [%s]\n",buf2);

    n = md_wordCmp(&WORD[0], &WORD[1]);
    printf("Compare: %s %d %s\n",buf0,n,buf1);

    n = md_wordCmp(&WORD[0], &WORD[2]);
    printf("Compare: %s %d %s\n",buf0,n,buf2);

    n = md_wordCmp(&WORD[1], &WORD[0]);
    printf("Compare: %s %d %s\n",buf1,n,buf0);

    n = md_wordCmp(&WORD[0], &WORD[0]);
    printf("Compare: %s %d %s\n",buf0,n,buf0);

    n = md_wordCmp(NULL, &WORD[0]);
    printf("Compare: NULL %d %s\n",n,buf0);

    n = md_wordCmp(&WORD[0], NULL);
    printf("Compare: %s %d NULL\n",buf0,n);
}

void dictTests(void)
{
    char buf0[6];
    int i,j,n;
    const md_word *w;

    printf("\x7DThe Dictionary Test\n");

    n = md_volumeFind(&DICT, (md_word*)"supes");
    printf("[supes] is at %d\n",n);

    n = md_volumeFind(&DICT, (md_word*)"gloom");
    printf("[gloom] is not found: %d\n",n);

    n = md_volumeSize(&DICT);
    printf("Size(3): Dict contains: %d\n",n);

    for(i=0; i<n; i++)
    {
        w = md_volumeGet(&DICT,i);
        md_wordToString(buf0,w);
        j = md_volumeFind(&DICT,(md_word*)buf0);
        printf("%d: [%s] found at %d\n",i,buf0,j);
    }

    printf("get(-1) should return null: %d\n",md_volumeGet(&DICT,-1));
    printf("get(len) should return null: %d\n",md_volumeGet(&DICT,n));

    n = md_volumeFind(&CVDICT, (md_word*)"BABEL");
    printf("0:[BABEL] is at %d\n",n);

    n = md_volumeFind(&CVDICT, (md_word*)"NASAL");
    printf("199:[NASAL] is at %d\n",n);

    n = md_volumeSize(&CVDICT);
    printf("Size (200): CVDict contains: %d\n",n);
}

void dictSpeedTests()
{
    char buf0[6];
    int i,j,n;
    const md_word *w;
    unsigned int after;
    unsigned int diff;
    unsigned int max = 0;
    unsigned int total = 0;
    unsigned int start = 0xffff;

    printf("Timing and verifying 200 finds\n",n);
    n = md_volumeSize(&CVDICT);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        w = md_volumeGet(&CVDICT,i);
        md_wordToString(buf0,w);
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_volumeFind(&CVDICT,(md_word*)buf0);
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
    n = md_volumeSize(&CVDICT);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        w = md_volumeGet(&CVDICT,i);
        md_wordToString(buf0,w);
        ++buf0[1]; /* increment the first vowel, this is NOT a word. */
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_volumeFind(&CVDICT,(md_word*)buf0);
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

char dest[256];
void memSpeedTests(void)
{
    unsigned int after;
    unsigned int diff = 0;
    unsigned int start = 0xffff;
    /* Should be void*. memcpy() doesn't care. But that for() loop needs src & dest to be the same type */
    char *src=(char*)0xa000;
    int i,j;

    printf("\x7DThe Timing Tests\n");

    for(i=0;i<256;++i)
    {
        OS.cdtmv1=start;
        memcpy(dest,src,256);
        after=OS.cdtmv1;
        diff=diff+start-after;
    }
    printf("memcpy 256x256: %d\n",diff);
    /* 256*256 = 64K -- got 59, so less than one second to copy all ram, .015 sec/KB */

    diff=0;
    for(i=0;i<64;++i)
    {
        OS.cdtmv1=start;
        for(j=0;j<256;++j)
        {
            dest[j]=src[j];
        }
        after=OS.cdtmv1;
        diff=diff+start-after;
    }
    printf("for loop 256x64: %d\n",diff);
    /* 256*64 = 16K -- got 183, so three seconds to copy 16K; .19 sec/KB */

    diff=0;
    for(i=0;i<256;++i)
    {
        OS.cdtmv1=start;
        __asm__ ("tya");
        __asm__ ("pha");
        __asm__ ("ldy #$ff");
copyloop1:
        __asm__ ("lda $a000,y");
        __asm__ ("sta $5000,y");
        __asm__ ("dey");
        __asm__ ("bne %g",copyloop1);
        __asm__ ("pla");
        __asm__ ("tay");
        after=OS.cdtmv1;
        diff=diff+start-after;
    }
    printf("asm loop 256x256: %d\n",diff);
    /* 256*256 = 64K -- got 51, so less than one second to copy all ram */
    /* Slightly faster than memcpy but not by enough to make it worth it */

    diff=0;
    for(i=0;i<256;i+=4)
    {
        OS.cdtmv1=start;
        __asm__ ("tya");
        __asm__ ("pha");
        __asm__ ("ldy #$ff");
copyloop2:
        __asm__ ("lda $a000,y");
        __asm__ ("sta $5000,y");
        __asm__ ("lda $a100,y");
        __asm__ ("sta $5100,y");
        __asm__ ("lda $a200,y");
        __asm__ ("sta $5200,y");
        __asm__ ("lda $a300,y");
        __asm__ ("sta $5300,y");
        __asm__ ("dey");
        __asm__ ("bne %g",copyloop2);
        __asm__ ("pla");
        __asm__ ("tay");
        after=OS.cdtmv1;
        diff=diff+start-after;
    }
    printf("asm loop 256x256/4: %d\n",diff);
    /* 256*256 = 64K -- got 37, so about .6 sec to copy all ram, .01 sec/KB */
    /* If you are copying from a page boundary, this is almost twice as fast */
}

int main(void)
{
    char k;
    OS.coldst=1; // force cold start on warm reset
    wordTests();
    printf("\nPress a key to continue\n");
    k = cgetc();
    dictTests();
    dictSpeedTests();
    printf("\nPress a key to continue\n");
    k = cgetc();
    memSpeedTests();
    printf("\nPress a key to continue\n");
    k = cgetc();
    printf("All done! (%d)\n",k);
    for(;;)
        ;
    return 1;
}
