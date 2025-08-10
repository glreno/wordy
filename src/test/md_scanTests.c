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

extern md_dict DICT_ES;
extern md_dict DICT_EA;
extern md_dict DICT_MS;
extern md_dict DICT_MA;
extern md_dict DICT_HS;
extern md_dict DICT_HA;
extern md_dict DICT_SS;
extern md_dict DICT_SA;

void lookupTest(md_dict *d,const char *expDict, const char *target)
{
    int n;
    md_word tword;
    char buf[6];
    n = md_findWord(d, (md_word*)target, &tword);
    md_wordToString(buf,&tword);
    printf("%s:[%s] is at %d %s f:%d\n",expDict, target, n,buf,tword.wordflags);
}

void dictTests(md_dict *d)
{
    lookupTest(d,"ES","ABOUT"); // top.dict first headword
    lookupTest(d,"ES","YOUTH"); // top.dict last headword
    lookupTest(d,"EA","ACIDS"); // top.dict first derived word
    lookupTest(d,"EA","ZONES"); // top.dict last derived word
    lookupTest(d,"MA","ABACI"); // main.dict first word
    lookupTest(d,"MA","ZOOMS"); // main.dict last word
    lookupTest(d,"HA","AGORA"); // rare.dict first word
    lookupTest(d,"HA","ZOMBI"); // rare.dict last word
    lookupTest(d,"HA","ARSES"); // naughty.dict first word
    lookupTest(d,"HA","WOODY"); // naughty.dict last word
    lookupTest(d,"SA","CLITS"); // bad.dict first word
    lookupTest(d,"SA","TWATS"); // bad.dict last word
    lookupTest(d,"00","!WORD");
}

void dictSpeedTests(md_dict *d)
{
    char buf[6];
    char *scr = (char*) OS.savmsc;
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

    n = md_size(d);
    printf("Timing and verifying %d finds\n",n);
    // point the timer vectors to a handy RTS
    // TODO use rtclock instead!
    OS.cdtma1=(void*)0xa000;
    OS.cdtma2=(void*)0xa000;
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        md_getWord(d,i,&tword);
        md_wordToString(buf,&tword);
        for(j=0;j<5;j++)
            scr[j]=(buf[j]-32); // convert ascii to internal and print to screen
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(d,(md_word*)buf,NULL);
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
        // TODO check that the found word matches what we searched for
        if ( j==-1 )
        {
            printf("FAILED to find %s\n",buf);
for(;;) ;
        }
    }
    after=OS.cdtmv2;
    total=start-after;
    printf("Duration: %ld jiffies, %ld sec\n",total,total/60);
    printf("Findonly: %ld jiffies, %ld sec\n",total2,total2/60);
    printf("Avg %d jiffies per word, max %d\n",total2/n,max);
    printf("%d words <= 1 jiffy\n",total1);

    printf("Timing and verifying %d not finds\n",n);
    total=0;
    total1=0;
    total2=0;
    max=0;
    n = md_size(d);
    OS.cdtmv2=start; /* timer 2: total time */
    for(i=0; i<n; i++)
    {
        md_getWord(d,i,&tword);
        md_wordToString(buf,&tword);
        buf[4]='!'; /* clear the last letter, this is NOT a word */
        for(j=0;j<5;j++)
            scr[j]=(buf[j]-32); // convert ascii to internal
        OS.cdtmv1=start; /* timer 1: individual time */
        j = md_findWord(d,(md_word*)buf,NULL);
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

void test(md_dict *d,char *name)
{
    char k;
    clrscr();
    printf("        Scan of dictionary %s\n",name);
    dictTests(d);
    printf("Press S for scan, C to continue\n");
    k=cgetc();
    if ( k != 's' )
    {
        return;
    }
    dictSpeedTests(d);
    printf("Press C to continue\n");
    while (k!='c')
    {
        k=cgetc();
    }
}

void __fastcall__ resetScreenSaver(void)
{
    OS.atract=0; // keep attract mode off
}

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    setYieldFunc(&resetScreenSaver);
    test(&DICT_ES,"ES");
    test(&DICT_EA,"EA");
    test(&DICT_MS,"MS");
    test(&DICT_MA,"MA");
    test(&DICT_HS,"HS");
    test(&DICT_HA,"HA");
    test(&DICT_SS,"SS");
    test(&DICT_SA,"SA");
    printf("\nAll done!\n");
    for(;;)
        ;
    return 1;
}
