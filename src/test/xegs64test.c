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

void printBank(void)
{
    printf("The current bank is %d\n",*bankaddr);
}
void switchBank(void)
{
    char k;
    printf("Press a number 0-7 to bankswitch\n");
    k=cgetc();
    k = k & 0x7;
    printf("Switching to bank %d\n",k);
    *bankswitch=k;
}

int main(void)
{
    OS.coldst=1; // force cold start on warm reset
    printf("\x7DXEGS 64K Cartridge Test\n");
    for(;;)
    {
        printBank();
        switchBank();
    }
    return 0;
}

