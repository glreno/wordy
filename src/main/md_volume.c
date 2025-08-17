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
const md_wordInternal *md_volumeGetInternal(const md_volume *d, int index)
{
    // Port to assmebler, if possible, currently 0x53 bytes
    int sz = md_volumeSize(d);
    if ( index < 0 || index >= sz )
    {
        return NULL;
    }
    return &((d->list)[index]);
}
md_word RETURNBUF;
const md_word *md_volumeGet(const md_volume *d, int index)
{
    md_wordInternal *found = md_volumeGetInternal(d,index);
    strcpy(RETURNBUF.s,found->s);
    return &RETURNBUF;
}

//segment MD
void md_volumeCopyWord(const md_volume *d, int index, md_word *dest)
{
    const md_wordInternal *w=md_volumeGetInternal(d,index);
    if ( w == NULL )
    {
        // Error condition - index out of range
        dest->wordflags=0xff;
        return;
    }
    dest->wordflags=0;
    memcpy(dest,w,5);
}

//segment MD
int md_volumeFindRecurse(const md_volume *d, const md_word *s,int first,int last)
{
    const md_wordInternal *firstw, *lastw, *midw;
    int mid;
    signed char cmp;

    /* Make sure it's not first or last.*/

    firstw=md_volumeGetInternal(d,first);

    cmp = md_wordCmpInternal(s,firstw);
    if ( cmp == 0 )
    {
        /** Cool, first in the list */
        return first;
    }
    else if (cmp < 0)
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
    lastw=md_volumeGetInternal(d,last);
    cmp = md_wordCmpInternal(s,lastw);
    if ( cmp == 0 )
    {
        /** Cool, last in the list */
        return last;
    }
    else if (cmp > 0)
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
    midw=md_volumeGetInternal(d,mid);
    /* Is it before,at, or after the midpoint? */
    cmp = md_wordCmpInternal(s,midw);
    if ( cmp == 0 )
    {
        /* Found it! */
        return mid;
    }
    else if ( cmp < 0 )
    {
        return md_volumeFindRecurse(d,s,first+1,mid-1);
    }
    else
    {
        return md_volumeFindRecurse(d,s,mid+1,last-1);
    }
}

//segment MD
int md_volumeFind(const md_volume *d, const md_word *s)
{
    /* TODO check to see if lastfound points to the word already */
    int sz = md_volumeSize(d);
    return md_volumeFindRecurse(d,s,0,sz-1);
}

#pragma code-name (pop)

