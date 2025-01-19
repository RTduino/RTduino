/*
 * Copyright (c) 2021-2024, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     port to RTduino
 */
/*
  Part of the Wiring project - http://wiring.org.co
  Copyright (c) 2004-06 Hernando Barragan
  Modified 13 August 2006, David A. Mellis for Arduino - http://www.arduino.cc/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include <Arduino.h>

long random(long howbig)
{
    if (howbig == 0)
    {
        return 0;
    }
    return rand() % howbig;
}

long random(long howsmall, long howbig)
{
    long diff;

    if (howsmall >= howbig)
    {
        return howsmall;
    }
    diff = howbig - howsmall;

    return random(diff) + howsmall;
}

uint16_t makeWord(uint16_t w)
{
    return w;
}

uint16_t makeWord(byte h, byte l)
{
    return (h << 8) | l;
}

void randomSeed(unsigned long seed)
{
    if (seed != 0)
    {
        srand(seed);
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (long)(((double)(x - in_min) * (double)(out_max - out_min)) / (double)((in_max - in_min) + out_min));
}
