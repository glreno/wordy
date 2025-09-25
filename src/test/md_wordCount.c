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
    const md_lexicon *v;

    for(i=0; i<nvol; i++)
    {
        // By working at the lexicon level
        // (and NOT the multilex) we can use md_lexiconSize
        // which does NOT include the md_multilex multiplier.
        v=d->lexicons[i].lexicon;
        if ( i > 0 && v == d->lexicons[i-1].lexicon )
        {
            // skip this one!
        }
        else
        {
            n+=md_lexiconSize(v);
        }
    }
    return n;
}

void test(md_dict *d,char *name)
{
    int n;
    md_bankswitchIdx(); // BANK SWITCH!
    n = md_size(d);
    printf("%s\t%d\t",name,n);
    n=dictWordCount(d);
    printf("%d\n",n);
}

void stats_for_lex(md_dict *dict,md_lexicon *lex,char *lex_name)
{
    int dict_total,lex_count,mult,i;
    long lex_multiplied,lex_pct,word_pct;
    long lex_pct_high, word_pct_high;
    dict_total = md_size(dict);
    lex_count = md_lexiconSize(lex);
    mult=0;
    for(i=0;i<dict->size;++i)
    {
        // dict->lexicons[i] is a multilex...
        if ( dict->lexicons[i].lexicon==lex )
        {
            mult = dict->lexicons[i].mult;
        }
    }
    // probability the lexicon will be chosen, as a percent*100
    // This is lex_count * mult / dict_total

    lex_multiplied=lex_count*mult;
    lex_pct=lex_multiplied*10000L/dict_total; 
    lex_pct_high = lex_pct/100;

    // probability that any word in the lexicon will be chosen, as a percent*10000
    // This is the same as lex_pct / lex_count, but it's better
    // to calculate it as mult/dict_total
    word_pct= mult * 1000000L / dict_total;
    word_pct_high = word_pct/10000;


    // Need probability for each individual word, which is pct/lex_count
    // and need to print decimals here!

    if ( mult > 0 )
    {
        // Name lex_count*mult=lex_multiplied   lex_pct
        printf("%s %4d*%d=%ld\t%2ld.%02ld%% %2ld.%04ld%%\n",
            lex_name,
            lex_count,
            mult,
            lex_multiplied,
            lex_pct_high, lex_pct-lex_pct_high*100,
            word_pct_high, word_pct-word_pct_high*10000
        );
    }
    else
    {
        printf("%s      %d\n",lex_name,mult);
    }
}

extern md_lexicon lex_top;
extern md_lexicon lex_top_uk;
extern md_lexicon lex_top_us;
extern md_lexicon lex_topD;
extern md_lexicon lex_top_ukD;
extern md_lexicon lex_top_usD;
extern md_lexicon lex_main;
extern md_lexicon lex_main_uk;
extern md_lexicon lex_main_us;
extern md_lexicon lex_mainD;
extern md_lexicon lex_main_ukD;
extern md_lexicon lex_main_usD;
extern md_lexicon lex_rare;
extern md_lexicon lex_naughty;
extern md_lexicon lex_carlin;

void stat(md_dict *d,char *name)
{
    int n;
    printf("\nPress a key to continue\n");
    cgetc();
    clrscr();
    n = md_size(d);
    printf("\t%s (%d words)\n",name,n);
    //      top        764*1=764   55.36%   0.07%
    printf("Lexicon   Size M      Lex Pr  Word pr\n");
    stats_for_lex(d,&lex_top,      "top      ");
    stats_for_lex(d,&lex_top_uk,   "top UK   ");
    stats_for_lex(d,&lex_top_us,   "top US   ");
    stats_for_lex(d,&lex_topD,     "top D    ");
    stats_for_lex(d,&lex_top_ukD,  "top UK D ");
    stats_for_lex(d,&lex_top_usD,  "top US D ");
    stats_for_lex(d,&lex_main,     "main     ");
    stats_for_lex(d,&lex_main_uk,  "main UK  ");
    stats_for_lex(d,&lex_main_us,  "main US  ");
    stats_for_lex(d,&lex_mainD,    "main D   ");
    stats_for_lex(d,&lex_main_ukD, "main UK D");
    stats_for_lex(d,&lex_main_usD, "main US D");
    stats_for_lex(d,&lex_rare,     "rare     ");
    stats_for_lex(d,&lex_naughty,  "naughty  ");
    stats_for_lex(d,&lex_carlin,   "carlin   ");
    md_bankswitchIdx(); // BANK SWITCH!
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

extern md_dict DICT_TS;

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
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

    test(&DICT_TS,"TS   ");

    stat(&DICT_EA,"EA   ");
    stat(&DICT_MA,"MA   ");
    stat(&DICT_HA,"HA   ");

    stat(&DICT_ES,"ES   ");
    stat(&DICT_MS,"MS   ");
    stat(&DICT_HS,"HS   ");
    stat(&DICT_TS,"TS   ");

    stat(&DICT_ES_UK,"ES_UK");
    stat(&DICT_MS_UK,"MS_UK");
    stat(&DICT_HS_UK,"HS_UK");

    stat(&DICT_ES_US,"ES_US");
    stat(&DICT_MS_US,"MS_US");
    stat(&DICT_HS_US,"HS_US");
    printf("\nAll done!\n");
    for(;;)
        ;
    return 1;
}
