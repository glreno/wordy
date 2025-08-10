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
#include <peekpoke.h>
#include <6502.h>

int main(void)
{
    struct regs r;
    POKE(710,0xa0);
    r.pc=0xe471;
    _sys(&r);
    return 0;
}
