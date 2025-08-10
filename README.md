![Image: aqordl screenshot](aqordl_screenshot1.png)

# Aqordl

## What is it?

A word puzzle game for the Atari 8-bit computers. With a dancing robot. And no social media.

## Why?

Because "Wordl" is awesome, and "Dordle" and "Quordle" are awesomer. "Sedecordle" is awesomest, but it's really hard to fit 16 puzzles on an Atari screen.

I have seen a couple versions of Wordl for the Atari, but not Quordl, and of course no dancing robot.

Also, I've always wanted to write a dictionary.

## How to play

You might have seen a similar game. Enter a guess, and the letters will turn green or yellow. Green letters are correct. Yellow letters are in the word, but in a different location.

The twist here is that you are guessing four words at once.

## System Requirements

- The disk version requires a 130XE.
  - It uses the bank-switched memory to hold the dictionary and animation data.
  - This is NOT compatible with a RAM disk.

- The cartridge version should run on any Atari 8-bit system with at least 16K RAM.
  - I have only tested this in emulation
  - It is a 64KB XEGS bank-switched cartridge (cartridge type 13)
  - This may or may not work on a real 400/800 system; I'm not sure the XEGS cartridge hardware is compatible
  - It works in emulation though!

- The text-only cartridge version will run in 8KB.
  - If you can find one of the unreleased 8KB Atari 400s, have the ability to burn a cartridge, and a 400 supports this cartridge hardware.

## Starting the game

- Retrogames **THE400**
  - Plug a keyboard into one of the USB ports
  - Copy the aqordl_1.00.car file on to a thumb drive, and plug it into one of the USB ports.
  - Select "Media Access" from the carousel
  - Select the aqordl_1.00 image
  - Press Home to start the game

- Emulators
  - Pretty much any settings should work for the .car images
  - The .xex and .atr images require a 130XE with 128K RAM.

- A **Real** Atari 130XE
  - This is still untested, since my 130XE has been in storage for a couple decades
  - Copy the aqordl_1.00.atr image to a blank single-density floppy. The image includes myPicoDos, and will boot directly into the game
  - Or you can load the AQORDL_1.XEX file using your favourite method of loading .exe files
  - Either way, it's going to take a long time to load
  - The game does use all 128K, so do not use with a RAM disk

- Any other real hardware
  - I have no idea how to get this .car image onto a real physical cartridge. But that's what it would take.
  - The cartridge image is 64K XEGS (cartridge type 13)

## Options

- The dictionary is chosen at startup
  - NTSC systems get the US dictionary
  - PAL systems get the UK dictionary.

- Since you don't like my colours, press any number key to pick a different one.
  - 9 is a high-contrast blue-orange scheme
  - 0 is suitable for black & white TVs

- Type "PLEASE" (which you will notice is six letters) to access debug commands
  - "PLEASE QUIET" disables audio
  - "PLEASE DANCE" plays the you-won-the-game animation
  - "PLEASE SADLY" plays the you-lost-the-game animation
  - "PLEASE JUMPY" plays the you-guessed-the-word animation


## Credit where Credit is Due

- Wordle was created by Josh Wardle 2021 and is &#174; New York Times 2022
- Quordle was created by Freddie Meyer 2022, and is a property of Merriam-Webster
- Developed using the cc65 cross-compiler, which is licensed under the zlib License

## Questionable Design Decisions

- Why a cartridge?

    It's the real native format.

    If it wasn't for the cost, all the software for these machines would be on cartridge. And since I'm mainly writing for my THE400, cost isn't an issue.

    A bank-switched cartridge gives you a massive amount of space, and I needed to store a 32KB dictionary. Plus the robot graphics.

    Once the game was working, I ported it to disk. But in order to do that I had to replicate the bank switching by using a 130XE.

    I made a 48K RAM version of the text-only game, but I had to remove so much of the dictionary that it was unplayable. The rest of the dictionary needs another 24KB. I might be able to squeeze it on to an 800XL, but my calculations say I need to discard about a quarter of the dictionary.

    But realistically: if this game had been written in the 80s then the dictionary would live on the disk. And that means a lot of work on good indexing and caching systems. Maybe I'll work on that for version 2.

- Why not use the nice Atari font?

    It's possible to make this game using Mode 1 colored text. The requirement is three colors of text. But that doesn't let you put a tile behind the letters. For that, I needed Mode 4 text. Even then, the Atari font doesn't leave enough space to draw the tile. So I needed a smaller font.

    The 5x5 font is based on my pre-Atari computer, the Interact Model R, with a couple changes to the least-legible letters.

- Why a dancing robot?

    Why not?

    A lot of 90s puzzle games included an opponent on screen to react to your moves, and I wanted to have something like that. I made a list of possible characters, but the robot was really the extent of my artistic abilities.

    The jumping and dancing were recommendations from my daughter. I picked the music while tagging the US/UK differences in the dictionary.


***Copyright &copy; 2025 Gerald Reno, Jr  All Rights Reserved***

