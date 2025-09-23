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

// this is a private function
const md_wordInternal *md_wordListGetInternal(const md_wordList *d, int index);

md_wordInternal IWORD[3];

const md_word WORD[] = {
    { "snipe", 6 },
    { "snips", 8 },
    { "swipe", 7 }
};

md_wordList DICT = { 3, 'S', 0, IWORD };
const md_lexicon *DICTL = (const md_lexicon*) &DICT;

extern md_wordList wordListS;

void buildDict(void)
{
    md_bankswitchIdx(); // BANK SWITCH!
    md_wordToWordInternal(&IWORD[0],&WORD[0]);
    md_wordToWordInternal(&IWORD[1],&WORD[1]);
    md_wordToWordInternal(&IWORD[2],&WORD[2]);
}

void wordTests(void)
{
    char buf0[6],buf1[6],buf2[6];
    int n;
    printf("\x7DThe first Test\n");
    //printf("Storage: Word0 flag is: %d\n",WORD[0].wordflags);
    md_wordToString(buf0,&WORD[0]);
    printf("Storage: Word0 is: [%s]\n",buf0);   // snipe
    md_wordToString(buf1,&WORD[1]);
    printf("Storage: Word1 is: [%s]\n",buf1);   // snips
    md_wordToString(buf2,&WORD[2]);
    printf("Storage: Word2 is: [%s]\n",buf2);   // swipe

    // Note: md_wordCmpInternal does not compare the first letter!
    n = md_wordCmpInternal(&IWORD[0], &IWORD[1]);
    printf("Compare: %s %d %s\n",buf0,n,buf1);  // snipe -1 snips

    n = md_wordCmpInternal(&IWORD[0], &IWORD[2]);
    printf("Compare: %s %d %s\n",buf0,n,buf2);  // snipe -1 swipe

    n = md_wordCmpInternal(&IWORD[1], &IWORD[0]);
    printf("Compare: %s %d %s\n",buf1,n,buf0);  // snips +1 snipe

    n = md_wordCmpInternal(&IWORD[0], &IWORD[0]);
    printf("Compare: %s %d %s\n",buf0,n,buf0);  // snipe =0 snipe

    n = md_wordCmpInternal(NULL, &IWORD[0]);
    printf("Compare: NULL %d %s\n",n,buf0);     //  NULL +1 snipe

    n = md_wordCmpInternal(&IWORD[0], NULL);
    printf("Compare: %s %d NULL\n",buf0,n);     // snipe -1 NULL
}

void internalcopy(md_wordInternal *copy,md_wordInternal *src)
{
    copy->s[0]=src->s[0];
    copy->s[1]=src->s[1];
    copy->s[2]=src->s[2];
}
void compressionTest(char *src)
{
    int result;
    int i;
    char dest[6];
    md_word w;
    md_wordInternal compressed;
    md_wordInternal copy;
    md_wordList wl;
    wl.firstLetter='A';
    md_wordToWordInternal(&compressed,(md_word*)src);
    md_wordInternalToWord(&w,&wl,&compressed);
    md_wordToString(dest,&w);
    result=strcmp(src,dest);
    if ( result != 0 )
    {
        printf("strcmp FAILED! %s %d %s\n",src,result,dest);
        for(;;)
            ;
    }
    // Copy the word-internal and check that they are equal
    internalcopy(&copy,&compressed);
    result = md_wordCmpInternal(&copy,&compressed);
    if ( result != 0 )
    {
        md_wordInternalToWord(&w,&wl,&copy);
        md_wordToString(dest,&w);
        printf("internal FAILED! %s %d %s\n",src,result,dest);
        for(;;)
            ;
    }
}

// Set to Z for a more thourough compression test
#define END 'B'
void compressionTests(void)
{
    char a,b,c,d;
    char src[6];
    printf("\x7D Compression Test\n");
    src[0]='A';
    src[5]='\0';
    for ( a='A';a<=END;++a) {
        src[1]=a;
        for ( b='A';b<=END;++b) {
            src[2]=b;
            for ( c='A';c<=END;++c) {
                src[3]=c;
                for ( d='A';d<=END;++d) {
                    src[4]=d;
                    compressionTest(src);
                }
            }
        }
        printf(src);
        printf("\n");
    }
}

int wordListFindString( md_wordList *dict, char *target)
{
    md_wordInternal compressed;
    md_bankswitchIdx(); // BANK SWITCH!
    md_wordToWordInternal(&compressed,(md_word*)target);
    return md_wordListFind(dict, &compressed);
}
void dictTests(void)
{
    char buf0[6];
    int i,j,n;
    const md_wordInternal *wi;
    md_word w;

    printf("\x7DThe Dictionary Test\n");

    n = wordListFindString(&DICT, "snips");
    printf("[snips] is at %d\n",n);                 // 1

    n = wordListFindString(&DICT, "gloom");    // -1
    printf("[gloom] is not found: %d\n",n);

    n = md_wordListSize(&DICT);
    printf("Size(3): Dict contains: %d\n",n);       // 3

    for(i=0; i<n; i++)
    {
        wi = md_wordListGetInternal(&DICT,i);
        md_wordInternalToWord(&w,&DICT,wi);
        md_wordToString(buf0,&w);
        j = wordListFindString(&DICT,buf0);
        printf("%d: [%s] found at %d\n",i,buf0,j);  // should return 0 1 2
    }

    printf("get(-1) should return null: %d\n",md_wordListGetInternal(&DICT,-1)); // 1406???
    printf("get(len) should return null: %d\n",md_wordListGetInternal(&DICT,n)); // 1406???

    n = wordListFindString(&wordListS, "SABLE");
    printf("0:[SABLE] is at %d\n",n);               // 0

    n = wordListFindString(&wordListS, "SALVE");
    printf("6:[SALVE] is at %d\n",n);
        wi = md_wordListGetInternal(&wordListS,6);
        md_wordInternalToWord(&w,&wordListS,wi);
        md_wordToString(buf0,&w);
    printf("Index 6   is %s\n",buf0);

    n = wordListFindString(&wordListS, "SPANK");
    printf("199:[SPANK] is at %d\n",n);             // 199

    n = md_wordListSize(&wordListS);
    printf("Size (200): wordListS contains: %d\n",n);  // 200
}

void dictSpeedTests()
{
    char buf0[6];
    int i,j,n;
    const md_wordInternal *wi;
    md_word w;
    unsigned int after;
    unsigned int diff;
    unsigned int max = 0;
    unsigned int total = 0;
    unsigned int start = 0xffff;

    printf("Timing and verifying 200 finds\n",n);
    n = md_wordListSize(&wordListS);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        wi = md_wordListGetInternal(&wordListS,i);
        md_wordInternalToWord(&w,&wordListS,wi);
        md_wordToString(buf0,&w);
        OS.cdtmv1=start; /* timer 1: individual time */
        j = wordListFindString(&wordListS,buf0);
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
    n = md_wordListSize(&wordListS);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        wi = md_wordListGetInternal(&wordListS,i);
        md_wordInternalToWord(&w,&wordListS,wi);
        md_wordToString(buf0,&w);
        ++buf0[1]; /* increment the first vowel, this is NOT a word. */
        OS.cdtmv1=start; /* timer 1: individual time */
        j = wordListFindString(&wordListS,buf0);
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
    buildDict();
    wordTests();
    printf("\nPress a key to continue\n");
    k = cgetc();
    compressionTests();
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
