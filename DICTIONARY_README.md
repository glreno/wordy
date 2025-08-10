# A Dictionary of Five Letter Words

** Copyright &copy; 2025 Gerald Reno, Jr  All Rights Reserved **

# Overview

This is a dictionary designed for word puzzles limited to five letter words.  It is organized into separate files to allow for different skill levels and age groups. It is intended to include most standard English words; any that are missing are oversights.

# Organization

The words are separated into these files:

1. top.dict - the most common words. Separate for 'easy' level games
2. rare.dict - archaic words ("thine"), dialectal words ("bairn"), and outdated slang ("grody"). Separate for 'hard' level games
3. naughty.dict - words that make teenagers giggle. Separate so that they can be used for word guesses, but not available when randomly choosing a puzzle solution.
4. bad.dict - George Carlin's seven words. Plus the N word, which wasn't considered quite as bad when he created his list. Here for completeness.
5. main.dict - everything else

# Methodology

1. The initial main.dict was a search for all five-letter words in /usr/dict/words
2. The initial top.dict was the combination of the five-letter words in the gonaturalenglish.com and wordfrequency.info lists.
3. Additionally, main.dict was supplemented with the other free word lists from wordfrequency.info and from the Ubuntu spell check dictionary /etc/dictionaries-common/words.
4. Then the top.dict words were removed from main.dict.
5. I manually removed all proper nouns from both main.dict and top.dict
6. I then compared the top.dict with the top ten hundred word list from Thing Explainer, and moved several words from main.dict to top.dict
7. I created the "naughty" list by manually scanning the two lists for anything that would make teenagers giggle.
8. I created the "rare" list manually by scanning the two lists for words that were obviously archaic, dialectal, or slangy. For any words that I did not already know, I followed the judgement of wiktionary.
9. I manually tagged derived words (noun-plural, verb-conjugated) so that they can still be puzzle answers, just less likely.
10. I manually tagged US/UK spellings so that no one gets tripped up by "jails" and "gaols".


# Sources

McIlroy, M. "Development of a Spelling List", "/usr/dict/words", AT&T Bell Laboratories, 1982

Ubuntu dictionaries-common 1.28.14, "/etc/dictionaries-common/words", 2021

gonaturalenglish.com, "1000 Most Common Words in English", https://www.gonaturalenglish.com/1000-most-common-words-in-the-english-language/, 2024

www.wordfrequency.info, "Top 5000 lemmas", https://www.wordfrequency.info/samples/wordFrequency.xlsx

Carlin, George "An Incomplete List of Impolite Words", Carlin on Campus, 1984

Munro, Randall "The Ten Hundred Words People Use The Most", Thing Explainer, Hougton Mifflin 2015

# License info

/usr/dict/words dates back to the ancient history of Unix. The original is available at https://code.google.com/archive/p/unix-spell/ under the New BSD License, which probably would be a surprise to the original author. It was derived from two digitized dictionaries and the Brown corpus; details in the McIlroy paper.

Ubuntu dictionaries-common is licensed under GPL-2+.

gonaturalenglish.com and wordfrequency.info do not provide licensing
information on their web sites. 

wordfrequency.info sells much larger and more detailed word frequency info, but that was not necessary for this application.

