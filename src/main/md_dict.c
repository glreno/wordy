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
* It consists of a collection of smaller dictionaries (see md_volume.c)
*/

#pragma code-name (push,"MD")

//segment MD
unsigned int md_size(const md_dict *d)
{
    unsigned int i;
    unsigned int sz;
    unsigned int sum;
    if ( d==NULL )
    {
        return 0;
    }
    sz = d->size;
    sum = 0;
    for(i=0; i<sz; ++i)
    {
        sum += md_volumeSize(d->dictionaries[i]);
    }
    return sum;
}

//segment MD
void md_getWord(const md_dict *d, int index, md_word *dest)
{
    int i;
    unsigned int sum,prevsum;
    if ( dest==NULL )
    {
        return;
    }

    if ( index < 0 )
    {
        // do nothing if index is negative
        // If it's higher than md_size(), we'll know after stepping through the list
    }
    else
    {
        // Step through the list of dictionaries,
        // adding the size to the total.
        // Once size > index, get the word from that volume.
        sum = 0;
        prevsum = 0;
        for(i=0; i<d->size; ++i)
        {
            prevsum=sum;
            sum += md_volumeSize(d->dictionaries[i]);
            if ( sum > index )
            {
                bankswitch( d->dictionaries[i]->bank ); // BANK SWITCH!
                md_volumeCopyWord(d->dictionaries[i],index-prevsum,dest);
                return;
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
    unsigned int sum = 0;

    for(i=0; i<d->size; ++i)
    {
        // Let the secondary thread do some stuff before we scan the next volume
        yield();
        bankswitch( d->dictionaries[i]->bank ); // BANK SWITCH!
        found = md_volumeFind(d->dictionaries[i],goal);
        if ( found > -1 )
        {
            // Found it!
            if ( dest != NULL )
            {
                md_volumeCopyWord(d->dictionaries[i],found,dest);
            }
            return sum+found;
        }
        sum += md_volumeSize(d->dictionaries[i]);
    }
    if ( dest != NULL )
    {
        dest->wordflags=0xff;
    }
    return -1;
}

//segment MD
void md_pickRandomWord(const md_dict *d, md_word *dest)
{
    byteunion randy;
    randy.lsb = POKEY_READ.random;
    randy.msb = POKEY_READ.random;
    md_getWord(d,randy.w % md_size(d),dest);
}

#pragma code-name (pop)
