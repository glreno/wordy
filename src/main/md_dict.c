/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include <string.h>
#include <stdlib.h>
#include <atari.h>
#include "md_dict.h"
#include "bankswitch.h"
#include "yield.h"
#include "byteunion.h"

/*
* This file is the implementation of the dictionary model.
* It consists of a collection of smaller volumes (see md_volume.c)
*/


/** Copy a md_volume struct */
unsigned int __fastcall__ md_cpWordList(char firstletter, md_wordList *dest, const md_lexicon *src);

#pragma code-name (push,"DICT_IDX")

//segment MD
unsigned int md_size(const md_dict *d)
{
    unsigned int i;
    unsigned int sz;
    unsigned int sum;
    sz = d->size;
    sum = 0;
    for(i=0; i<sz; ++i)
    {
        sum += d->lexicons[i].mult * md_lexiconSize(d->lexicons[i].lexicon);
    }
    return sum;
}

#pragma code-name (pop)
#pragma code-name (push,"MD")

//segment MD
void md_getWord(const md_dict *d, int index, md_word *dest)
{
    int i;
    char j,k;
    unsigned int sum,prevsum,currsum,listlen;
    md_wordList tempvol;

    if ( dest==NULL )
    {
        return;
    }
    md_bankswitchIdx(); // BANK SWITCH!

    if ( index < 0 )
    {
        // do nothing if index is negative
        // If it's higher than md_size(), we'll know after stepping through the list
    }
    else
    {
        // Step through the list of volumes,
        // adding the size to the total.
        // Once size > index, get the word from that volume.
        sum = 0;
        prevsum = 0;
        for(i=0; i<d->size; ++i)
        {
            for(j=0; j<d->lexicons[i].mult; ++j)
            {
                prevsum=sum;
                sum += md_lexiconSize(d->lexicons[i].lexicon);
                if ( sum > index )
                {
                    currsum=prevsum;
                    for(k='A';k<='Z';++k)
                    {
                        listlen = md_cpWordList(k,&tempvol,d->lexicons[i].lexicon);
                        if ( (listlen > 0) && (currsum + md_wordListSize(&tempvol) > index ) )
                        {
                            bankswitch( tempvol.bank ); // BANK SWITCH!
                            md_wordListCopyWord(dest,&tempvol,index-currsum);
                            md_bankswitchIdx(); // BANK SWITCH!
                            return;
                        }
                        currsum += listlen;
                    }
                }
            }
        }
    }

    // Didn't find it.
    dest->wordflags=0xff;
}

//segment MD
int md_findWord(const md_dict *d, const md_word *goal, md_word *dest)
{
    int i;
    int found;
    md_wordList tempvol;
    md_wordInternal compressed;
    unsigned int sum = 0, listlen;
    char first = goal->s[0];

    md_bankswitchIdx(); // BANK SWITCH!
    md_wordToWordInternal( &compressed, goal );
    for(i=0; i<d->size; ++i)
    {
        // Let the secondary thread do some stuff before we scan the next volume
        yield(); // this probably bankswitched
        md_bankswitchIdx(); // BANK SWITCH!
        listlen = md_cpWordList(first,&tempvol,d->lexicons[i].lexicon);
        if ( listlen > 0 )
        {
            bankswitch( tempvol.bank ); // BANK SWITCH!
            found = md_wordListFind(&tempvol,&compressed);
            if ( found > -1 )
            {
                // Found it!
                if ( dest != NULL )
                {
                    md_wordListCopyWord(dest,&tempvol,found);
                }
                md_bankswitchIdx(); // BANK SWITCH!
                return sum+found+md_lexiconSizeBefore(first,d->lexicons[i].lexicon);
            }
        }
        md_bankswitchIdx(); // BANK SWITCH!
        sum += ( d->lexicons[i].mult * md_lexiconSize(d->lexicons[i].lexicon) );
    }
    if ( dest != NULL )
    {
        dest->wordflags=0xff;
    }
    return -1;
}
#pragma code-name (pop)

#pragma code-name (push,"DICT_IDX")

//segment DICT_IDX
void md_pickRandomWord(const md_dict *d, md_word *dest)
{
    byteunion randy;
    randy.lsb = POKEY_READ.random;
    randy.msb = POKEY_READ.random;
    md_getWord(d,randy.w % md_size(d),dest);
}

#pragma code-name (pop)
