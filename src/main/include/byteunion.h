/*
* Copyright (c) 2025 Gerald Reno, Jr.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*/

/* byteunion.h
 *
 * Because it's faster than saying x=(ax>>8)
 *
 */

#ifndef __BYTEUNION_H__
#define __BYTEUNION_H__

typedef union {
    struct {
        unsigned char lsb;
        unsigned char msb;
    };
    unsigned w;
} byteunion;

#endif
