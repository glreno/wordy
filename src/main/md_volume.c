/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include "md_dict.h"
#include <string.h>

/*
* This file is the implementation of the 'small' dictionary volume.
* The proper Dictionary Model, md_Dict, is in md_dict.c
*/

#pragma code-name (push,"MD")

//segment MD
const md_wordInternal *md_wordListGetInternal(const md_wordList *d, int index)
{
    // Port to assmebler, if possible, currently 0x53 bytes
    int sz = md_wordListSize(d);
    if ( index < 0 || index >= sz )
    {
        return NULL;
    }
    return &((d->list)[index]);
}

//segment MD
int md_wordListFindRecurse(const md_wordList *d, const md_wordInternal *si,int first,int last)
{
    const md_wordInternal *firstw, *lastw, *midw;
    int mid;
    signed char cmp;

    /* Make sure it's not first or last.*/

    firstw=md_wordListGetInternal(d,first);

    cmp = md_wordCmpInternal(firstw,si);
    if ( cmp == 0 )
    {
        /** Cool, first in the list */
        return first;
    }
    else if (cmp > 0)
    {
        /** Word should be before first, so we are done here. */
        return -1;
    }
    if ( first >= last )
    {
        /* It's not first. If first==last, we are done. */
        return -1;
    }
    /* search for last here */
    lastw=md_wordListGetInternal(d,last);
    cmp = md_wordCmpInternal(lastw,si);
    if ( cmp == 0 )
    {
        /** Cool, last in the list */
        return last;
    }
    else if (cmp < 0)
    {
        /** Word should be after last, so we are done here. */
        return -1;
    }
    if ( first+1 == last )
    {
        /* if first+1 == last, there is nothing in between, so we are done. */
        return -1;
    }

    /* find the midpoint and check that */
    mid = ( last - first ) / 2 + first;
    midw=md_wordListGetInternal(d,mid);
    /* Is it before,at, or after the midpoint? */
    cmp = md_wordCmpInternal(midw,si);
    if ( cmp == 0 )
    {
        /* Found it! */
        return mid;
    }
    else if ( cmp > 0 )
    {
        // it might be worth checking to see if first+1==mid, in which case we are done
        // but it doesn't actually save any time in reality.
        return md_wordListFindRecurse(d,si,first+1,mid-1);
    }
    else
    {
        // it might be worth checking to see if last-1==mid, in which case we are done
        // but it doesn't actually save any time in reality.
        return md_wordListFindRecurse(d,si,mid+1,last-1);
    }
}

#pragma code-name (pop)
#pragma code-name (push,"DICT_IDX")

//segment DICT_IDX
unsigned int __fastcall__ md_lexiconSizeBefore(char before,const md_lexicon *d)
{
    int i;
    int ret=0;
    int n=d->array_length;
    for(i=0;i<n;++i)
    {
        if ( d->wordList[i].firstLetter == before )
        {
            return ret;
        }
        ret += md_wordListSize(&(d->wordList[i]));
    }
    return ret;
}
unsigned int __fastcall__ md_lexiconSize(const md_lexicon *d)
{
    int i;
    int ret=0;
    int n=d->array_length;
    for(i=0;i<n;++i)
    {
        ret += md_wordListSize(&(d->wordList[i]));
    }
    return ret;
}

