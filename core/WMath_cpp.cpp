/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     port to RT-Thread
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

extern "C" {
  #include <stdlib.h>
}

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

unsigned int makeWord(unsigned int w)
{
    return w;
}

unsigned int makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}
