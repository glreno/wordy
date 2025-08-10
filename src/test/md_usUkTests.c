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
#include <ctype.h>
#include <conio.h>
#include <_atarios.h>

extern md_dict DICT_ES;
extern md_dict DICT_ES_US;
extern md_dict DICT_ES_UK;
extern md_dict DICT_EA;
extern md_dict DICT_MS;
extern md_dict DICT_MS_US;
extern md_dict DICT_MS_UK;
extern md_dict DICT_MA;
extern md_dict DICT_HS;
extern md_dict DICT_HS_US;
extern md_dict DICT_HS_UK;
extern md_dict DICT_HA;

// validate by doing grep ,u *.dict
const char *WORDS_TOP_S[1]={
    "about"
};
const char *WORDS_TOP_P[1]={
    "acids"
};
const char *WORDS_MAIN_S[1]={
    "aback"
};
const char *WORDS_MAIN_P[1]={
    "abaci"
};
const char *WORDS_TOP_S_US[9]={
    "color",
    "draft",
    "favor",
    "fiber",
    "honor",
    "humor",
    "labor",
    "mommy",
    "rumor",
};
const char *WORDS_TOP_P_US[3]={
    "aging",
    "grays",
    "jails",
};
const char *WORDS_MAIN_S_US[7]={
    "armor",
    "rigor",
    "saber",
    "savor",
    "valor",
    "vapor",
    "vigor",
};
const char *WORDS_MAIN_P_US[1]={
    "plows",
};
const char *WORDS_TOP_S_UK[1]={
    "enrol",
};
const char *WORDS_TOP_P_UK[1]={
    "greys",
};
const char *WORDS_MAIN_S_UK[4]={
    "fibre",
    "lorry",
    "pully",
    "sabre",
};
const char *WORDS_MAIN_P_UK[1]={
    "gaols",
};

void lookupTest(md_dict *d,const char *expDict, const char *target)
{
    int n;
    md_word tword;
    char buf[6];
    n = md_findWord(d, (md_word*)target, &tword);
    md_wordToString(buf,&tword);
    cputs(expDict);
    if ( tword.wordflags==255 )
    {
        cputs("- ");
    }
    else
    {
        cputs("+ ");
    }
}

void dictTests(const char *target)
{
    // Note that the "A" dictionaries always contain both US and UK
    // but there are three solution dictionaries: S, SUK (brit), SUS (american)
    // and the test words should only appear in one of the three at that level
    lookupTest(&DICT_EA,"E",target);
    lookupTest(&DICT_MA,"M",target);
    lookupTest(&DICT_ES,"E",target);
    lookupTest(&DICT_MS,"M",target);
    lookupTest(&DICT_HS,"H",target);
    lookupTest(&DICT_ES_US,"E",target);
    lookupTest(&DICT_MS_US,"M",target);
    lookupTest(&DICT_HS_US,"H",target);
    lookupTest(&DICT_ES_UK,"E",target);
    lookupTest(&DICT_MS_UK,"M",target);
    lookupTest(&DICT_HS_UK,"H",target);
    cputs("\r\n");
}

void test(const char*header,const char *targets[],unsigned char len)
{
    unsigned char i,j;
    char target[6];
    cputs(header);
    cputs("\r\n      all   solution s_us     s_uk\r\n");
    for(i=0;i<len;++i)
    {
        strcpy(target,targets[i]);
        for(j=0;j<5;++j)
        {
            target[j]=toupper(target[j]);
        }
        cputs(target);
        cputs(":");
        dictTests(target);
    }
    cputs("\r\n\r\n");
}

void waitForC()
{
    char k;
    cputs("\r\nPress C to continue\r\n");
    k='\0';
    while (k!='c')
    {
        k=cgetc();
    }
    clrscr();
}

void __fastcall__ resetScreenSaver(void)
{
    OS.atract=0; // keep attract mode off
}

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    setYieldFunc(&resetScreenSaver);
    clrscr();
    test("Std Top S, expect EA and ES",WORDS_TOP_S,1);
    test("Std Top P, expect EA and MS",WORDS_TOP_P,1);
    test("UK Top S, expect EA and ES_UK",WORDS_TOP_S_UK,1);
    test("UK Top P, expect EA and MS_UK",WORDS_TOP_P_UK,1);
    waitForC();
    test("Std Main S, expect MA and HS",WORDS_MAIN_S,1);
    test("Std Main P, expect MA and HS",WORDS_MAIN_P,1);
    test("UK Main S, expect MA and HS_UK",WORDS_MAIN_S_UK,3);
    test("UK Main P, expect MA and HS_UK",WORDS_MAIN_P_UK,1);
    waitForC();
    test("US Top S, expect EA and ES_US",WORDS_TOP_S_US,9);
    test("US Top P, expect EA and MS_US",WORDS_TOP_P_US,3);
    waitForC();
    test("US Main S, expect MA and HS_US",WORDS_MAIN_S_US,7);
    test("US Main P, expect MA and HS_US",WORDS_MAIN_P_US,1);
    cputs("\r\nAll done!");
    for(;;)
        ;
    return 1;
}
