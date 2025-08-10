/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

#include <atari.h>
#include <stdio.h>
#include <conio.h>

char *bankaddr=(char*) 0x8004;
char *bankswitch=(char*) 0xD500;

char message[] = "This is a message in the data segment\n";

#pragma code-name (push,"CARTBANK0")
#pragma rodata-name (push,"CARTBANK0")
#pragma local-strings (on)
void bank0msg()
{
    printf("This is bank zero\n");
}
#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)

#pragma code-name (push,"CARTBANK1")
#pragma rodata-name (push,"CARTBANK1")
#pragma local-strings (on)
void bank1msg()
{
    printf("This is bank one\n");
}
#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)

#pragma code-name (push,"CARTBANK2")
#pragma rodata-name (push,"CARTBANK2")
#pragma local-strings (on)
void bank2msg()
{
    printf("This is bank two\n");
}
#pragma local-strings (off)
#pragma rodata-name (pop)
#pragma code-name (pop)

void bank3msg()
{
    printf("This is bank three\n");
}

void printBank(void)
{
    printf("The current bank is %d\n",*bankaddr);
    printf(message);
    if ( *bankaddr == 0 ) bank0msg();
    if ( *bankaddr == 1 ) bank1msg();
    if ( *bankaddr == 2 ) bank2msg();
    if ( *bankaddr == 3 ) bank3msg();
}
void switchBank(void)
{
    char k;
    printf("Press a number 0-3 to bankswitch\n");
    k=cgetc();
    k = k & 0x3;
    printf("Switching to bank %d\n",k);
    *bankswitch=k;
}

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    printf("\x7DXEGS 32K Cartridge Test\n");
    for(;;)
    {
        printBank();
        switchBank();
    }
    return 0;
}

