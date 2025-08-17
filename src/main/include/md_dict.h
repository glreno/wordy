/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* md_dict.h
 *
 * Declarations for the wordl dictionary
 *
 */

#ifndef __MD_DICT_H__
#define __MD_DICT_H__

#include <stdlib.h>

/* ######################################################### */
/* #                      WORD                             # */
/* ######################################################### */

/* A word in the dictionary.
* Since this is wordl, it is a five-letter word in ALL CAPS.
* The wordflags byte is used by personality modules to decide
* what kind of smart-ass remark is appropriate.
* md_findWord sets wordflags to -1 for not-found
*/
typedef struct md_wordStruct
{
    char s[5];
    char wordflags;
    /* Flag bit meanings:
    * 0x80 Word is a conjugated form
    * 0x40 Word can be used as a noun
    * 0x20 Word can be used as a verb
    * 0x10
    * 0x08
    * 0x04
    * 0x02
    * 0x01
    */
} md_word;
/** This is what is actually stored. client code always gets a md_word,
* but the ROM contains md_wordInternal, and the accessors convert it.
* (i.e., add a flags byte)
*/
typedef struct md_wordInternal
{
    char s[5];
} md_wordInternal;

/* Since md_wordStruct is exactly six characters,
* any standard char[6] string can bet cast to it
* for searching purposes -- the .wordflags will be the trailing zero:
*       n = md_volumeFind(&DICT, (md_word*)"LOSER");
*/

/* Compare two words. Very much like strcmp. */
signed char md_wordCmp(const md_word *a, const md_word *b);

/* Compare two words. Very much like strcmp. */
signed char md_wordCmpInternal(const md_wordInternal *a, const md_wordInternal *b);

/* Copy md_word.s to a buffer. dest must point to a char[] of length 6 or more. */
void md_wordToString(char *dest, const md_word *src);

/* ######################################################### */
/* #                DICTIONARY                             # */
/* ######################################################### */

/*
* A Dictionary -- a sorted list of words.
* The list of words can be in a bank-switched cartridge, but
* this structure should be in non-swappable memory.
*/
typedef struct md_volumeStruct
{
    unsigned int size; /* Number of words in list, so you can get this without bank-switching */
    char bank; /* ID of cartridge 8K bank that contains list */
    const md_wordInternal *list;
} md_volume;

/* Size of volume. md_volumeGet(d,md_volumeSize(d)) will return null (out of range) */
//unsigned int md_volumeSize(const md_volume *d);
#define md_volumeSize(d) ((d)->size)

/* Get a word.
 * DEPRECATED used only in tests. Uses a temp buffer to return.
* Assumes correct bank is loaded.
* Returns null if index<0 or index>=d.size
* Since volumes can be bank switched, the returned pointer
* will be invalid after an operation on another volumes.
* So copy what you need, quickly
*/
const md_word * __fastcall__ md_volumeGet(const md_volume *d, int index);

/* Get a word, and copy it into the provided buffer.
* Assumes correct bank is loaded.
* If not found, sets dest->wordflags=0xff
*/
void __fastcall__ md_volumeCopyWord(const md_volume *d, int index, md_word *dest);

/* Find a word in the volume. Returns -1 if it's not there.
* Assumes correct bank is loaded.
* This should be a nice fast binary search.
* After being found, the index will be stored in a transient lastfound, so
* the next search can find it in a single operation. TODO
* The search is on the string content of s; it is fine
* to search on a plain C string of length 5, you need to cast it though:
*       n = md_volumeFind(&VOL, (md_word*)"LOSER");
*/
int __fastcall__ md_volumeFind(const md_volume *d, const md_word *s);

/* ######################################################### */
/* #                DICTIONARY MODEL                       # */
/* ######################################################### */

/*
* A Big Dictionary -- a collection of smaller dictionary volumes.
* The smaller dictionaries might be in different cartridge banks!
* The big dict functions (md_findVolume, md_getWord, md_findWord) will handle the bank switching.
*/
typedef struct md_dictStruct
{
    unsigned int size;
    const md_volume *dictionaries[];
} md_dict;

/* Sum of the size of all dictionaries in the big dictionary */
unsigned int __fastcall__ md_size(const md_dict *d);

/* Find the dictionary a word is in. This is an internal function, really.
* MAY BANK SWITCH.
* If bank switching is involved, the correct bank will be left loaded
* so that you can call md_volumeGet. So this WILL work, but be sure to
* do the lookup immediately:
*   md_volume = md_findVolume(bigdict,"LOSER");
*   index = md_volumeFind(md_volume,"LOSER");
*   word = md_volumeGet(md_volume,index);
* TODO it's not clear that I need this function, since it's the same as md_findWord(d,s,NULL)
* with the extra work to bank switch
*/
//md_volume *md_findVolume(const md_dict *d, const char *s);

/* Get a word.
* MAY BANK SWITCH.
* Sets dest->wordflags to 0xff if index<0 or index>=md_size(d)
* index is index across concatenation of all the disctionaries. 
* Copies word into dest.
*/
void __fastcall__ md_getWord(const md_dict *d, int index, md_word *dest);

/* Find a word.
* MAY BANK SWITCH.
* Returns index, or -1 if not found.
* Sets dest->wordflags to 0xff if not found
* index is index across concatenation of all the disctionaries. 
* Copies word into dest if found, and dest is not null.
*/
int __fastcall__ md_findWord(const md_dict *d, const md_word *goal, md_word *dest);

/* Pick a word at random.
* MAY BANK SWITCH.
* Remember to call _randomize() before calling this the first time.
* _randomize() seeds the C pseudorandom number generator with
* a combination of VCOUNT and RTCLOK, so you need to call it
* after waiting for some kind of user input. Doesn't matter what,
* as long as you can't predict what it is.
*/
void __fastcall__ md_pickRandomWord(const md_dict *d, md_word *dest);

#endif
