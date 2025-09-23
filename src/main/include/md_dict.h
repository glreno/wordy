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
* (i.e., add the first letter and decompress)
*/
typedef struct md_wordInternal
{
    char s[3];
} md_wordInternal;

/* Since md_wordStruct is exactly six characters,
* any standard char[6] string can bet cast to it
* for searching purposes -- the .wordflags will be the trailing zero:
*       n = md_volumeFind(&DICT, (md_word*)"LOSER");
*/

/** Convert a md_word to a md_wordInternal */
void __fastcall__ md_wordToWordInternal( md_wordInternal *dest, const md_word* src);


/* Compare two words. Very much like strcmp.
 * 's' is the search term, and MAY NOT BE NULL and may not have flags.
 * 'a' going to be from the dictionary, and may be null, and will have flags.
 * Flags must be ignored in the comparison.
 */
signed char md_wordCmpInternal(const md_wordInternal *a, const md_wordInternal *s);

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
typedef struct md_wordListStruct
{
    unsigned int size; /* Number of words in list, so you can get this without bank-switching */
    char firstLetter; /* first letter of EVERY WORD in the list */
    char bank; /* ID of cartridge 8K bank that contains list */
    const md_wordInternal *list;
    // ...this is the thing that will be a union with "size==1 means the last three bytes are a word"
} md_wordList;

/** Convert a md_word to a md_wordInternal */
void __fastcall__ md_wordInternalToWord( md_word *dest, const md_wordList *d, const md_wordInternal* src);


typedef struct md_lexiconStruct
{
    unsigned char array_length; // if you create this in C, set length to 26!
    md_wordList wordList[26];
} md_lexicon;

/* Size of volume. md_volumeGet(d,md_volumeSize(d)) will return null (out of range) */
unsigned int __fastcall__ md_lexiconSize(const md_lexicon *d);
unsigned int __fastcall__ md_lexiconSizeBefore(char letter,const md_lexicon *d);
#define md_wordListSize(d) ((d)->size)

/* Get a word, and copy it into the provided buffer.
* Assumes correct bank is loaded.
* If not found, sets dest->wordflags=0xff
*/
void __fastcall__ md_wordListCopyWord(md_word *dest, const md_wordList *d, int index );

/* Find a word in the volume. Returns -1 if it's not there.
* Assumes correct bank is loaded.
* This should be a nice fast binary search.
* After being found, the index will be stored in a transient lastfound, so
* the next search can find it in a single operation. TODO
* The search is on the content of s, not on a plain string.
*/
int __fastcall__ md_wordListFind(const md_wordList *d, const md_wordInternal *s);

/* ######################################################### */
/* #                DICTIONARY MODEL                       # */
/* ######################################################### */

/*
* A lexicon, whose count can be multiplied.
* Wrapper for a pointer to a single md_lexicon, combined with a multiplier.
*/
typedef struct md_multilex
{
    const unsigned char mult;       // 1 byte - number of times this volume appears
                                    // Number of words is mult * words in volume
    const md_lexicon *lexicon;       // 2 bytes - pointer to a lexicon
                                    // A lexicon is 26 wordLists
} md_multilex;

/*
* A Big Dictionary -- a collection of smaller dictionary volumes.
* The smaller dictionaries might be in different cartridge banks!
* The big dict functions (md_findVolume, md_getWord, md_findWord) will handle the bank switching.
*/
typedef struct md_dictStruct
{
    const unsigned char size; // number of structs in the array, NOT size of dictionary!
    const md_multilex lexicons[]; // array of 3-byte struct
} md_dict;

/* Sum of the size of all lexicons in the big dictionary.
 * This includes the multiplier from md_multilex! */
unsigned int __fastcall__ md_size(const md_dict *d);

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

/* Bankswitch to where the dictionary and volume indices are */
void __fastcall__ md_bankswitchIdx(void);

#endif
