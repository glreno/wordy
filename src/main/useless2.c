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
int main(void)
{
    OS.color2 = 0xa0;
    __asm__ ("jmp $E471");
    return 0;
}
