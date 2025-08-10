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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <_atarios.h>
#define DICTSIZE 4956
#define MAXCOUNT 255

extern md_dict DICT_ES;
extern md_dict DICT_EA;
extern md_dict DICT_MS;
extern md_dict DICT_MA;
extern md_dict DICT_HS;
extern md_dict DICT_HA;
extern md_dict DICT_SS;
extern md_dict DICT_SA;

short maxcount;
short counts[DICTSIZE+1];

struct wordOfInterest
{
    int index;
    char word[6];
    int x, y;
};

struct wordOfInterest wordsOfInterest[15];

/*
void lookupTest(md_dict *d,int id,const char *expDict, const char *target)
{
    int n;
    md_word tword;
    n = md_findWord(d, (md_word*)target, &tword);
    wordsOfInterest[id].index=n;
    wordsOfInterest[id].x=((id&1)?20:0);
    wordsOfInterest[id].y=15+id/2;
    md_wordToString(wordsOfInterest[id].word,&tword);
    printf("%s:[%s] is at %d %s\n",expDict, target, n,wordsOfInterest[id].word);
}

void dictTests(md_dict *d)
{
    lookupTest(d,0,"top ","ABOUT"); // top.dict first headword
    lookupTest(d,1,"top ","YOUTH"); // top.dict last headword
    lookupTest(d,2,"topd","ACIDS"); // top.dict first derived word
    lookupTest(d,3,"topd","ZONES"); // top.dict last derived word
    lookupTest(d,4,"main","ABACK"); // main.dict first word
    lookupTest(d,5,"main","ZONAL"); // main.dict last word
    lookupTest(d,6,"mind","ABACI"); // main.dict first word
    lookupTest(d,7,"mind","ZOOMS"); // main.dict last word
    lookupTest(d,8,"rare","AGORA"); // rare.dict first word
    lookupTest(d,9,"rare","ZOMBI"); // rare.dict last word
    lookupTest(d,10,"naut","ARSES"); // naughty.dict first word
    lookupTest(d,11,"naut","WOODY"); // naughty.dict last word
    lookupTest(d,12,"bad ","CLITS"); // bad.dict first word
    lookupTest(d,13,"bad ","TWATS"); // bad.dict last word

    wordsOfInterest[14].index=-1;
    wordsOfInterest[14].x=0;
    wordsOfInterest[14].y=22;
    strcpy(wordsOfInterest[14].word,"xxxxx");
}
*/

void displayCount(int index)
{
    char buf[8];
    int i;

    for(i=0;i<15;i++)
    {
        if ( wordsOfInterest[i].index==index )
        {
            itoa(counts[wordsOfInterest[i].index],buf,10);
            cputsxy(wordsOfInterest[i].x+11,wordsOfInterest[i].y,buf);
        }
    }
}

void displayWordOfInterest(int i)
{
    char buf[8];
    int addr;

    addr=(int)(&counts[wordsOfInterest[i].index]);
    itoa(addr,buf,16);
    if ( addr < 0x1000 )
    {
        cputcxy(wordsOfInterest[i].x,wordsOfInterest[i].y,'0');
        cputsxy(wordsOfInterest[i].x+1,wordsOfInterest[i].y,buf);
    }
    else
    {
        cputsxy(wordsOfInterest[i].x,wordsOfInterest[i].y,buf);
    }
    cputsxy(wordsOfInterest[i].x+5,wordsOfInterest[i].y,wordsOfInterest[i].word);
    displayCount(wordsOfInterest[i].index);
}

void setUpDisplay()
{
    int i;
    for(i=0;i<DICTSIZE+1;++i)
    {
        counts[i]=0;
    }
    maxcount=0;

    cputsxy(0,15,"                                       ");
    for(i=0;i<15;++i)
    {
        displayWordOfInterest(i);
    }
}

short count(char *word)
{
    int i;
    int found;
    short n;

    found=md_findWord(&DICT_SA,(md_word*)word,NULL);
    if ( found < 0 )
    {
        printf("FAILED TO FIND [%s]\n",word);
        return MAXCOUNT;
    }
    n = counts[found] + (short)1;
    counts[found]=n;
    if ( n > maxcount )
    {
        maxcount=n;
        wordsOfInterest[14].index=found;
        strcpy(wordsOfInterest[14].word,word);
    }
    for(i=0;i<15;++i)
    {
        if ( found == wordsOfInterest[i].index )
        {
            displayWordOfInterest(i);
        }
    }
    return n;
}

void countRandomWords(md_dict *d)
{
    short n;
    md_word w1;
    char buf1[6];

    _randomize();
    for(;;)
    {
        OS.atract=0; // keep attract mode off
        // Pick a random word
        md_pickRandomWord(d,&w1);
        md_wordToString(buf1,&w1);
        n = count(buf1);
        if ( n == (short)MAXCOUNT )
        {
            return;
        }
    }
}

void showStats(char *name,int start,int end)
{
    unsigned long total;
    unsigned long size;
    short min;
    int i;
    short n;

    total=0;
    min=MAXCOUNT;
    size=end-start+1;
    for(i=start;i<=end;++i)
    {
        n=counts[i];
        total += n;
        if ( n < min )
        {
            min = n;
        }
    }
    printf("%s: Min:%d Avg:%lu Total:%lu\n",name,min,total/size,total);
}

md_dict *chooseDict()
{
    int k;
    // Choose a dictionary
    printf("Choose dictionary to test:\n");
    printf("0: ES\n");
    printf("1: EA\n");
    printf("2: MS\n");
    printf("3: MA\n");
    printf("4: HS\n");
    printf("5: HA\n");
    printf("6: SS\n");
    printf("7: SA\n");
    for(;;)
    {
        k = cgetc();
        switch (k) {
            case '0': return &DICT_ES;
            case '1': return &DICT_EA;
            case '2': return &DICT_MS;
            case '3': return &DICT_MA;
            case '4': return &DICT_HS;
            case '5': return &DICT_HA;
            case '6': return &DICT_SS;
            case '7': return &DICT_SA;
        }
    }
}

int main(void)
{
    md_dict *dictToTest;

    if ( OS.ramtop < 0x80 ) { cputs("Min 32K"); for(;;); } // 7C20 in the cfg file

    OS.coldst=1; // force cold start on warm reset

    dictToTest = chooseDict();

    // Show the original dictionary content
    clrscr();
    printf("Index of first/last of each volume\n");
    //dictTests(&DICT_SA);
    printf("Press S to start\n");
    cgetc();
    setUpDisplay();
    // pick a whole bunch of words in the dictionary, and store the count in dictcopy.
    // Stop when one of them hits 255.
    countRandomWords(dictToTest);
    printf("\n");
    showStats(" top",0,780);
    showStats("topd",781,1386);
    showStats("main",1387,3622);
    showStats("maind",3623,4723);
    showStats("rare",4724,4882);
    showStats("naut",4883,4955);
    showStats(" bad",4956,4963);

    for(;;)
        ;
    return 1;
}
