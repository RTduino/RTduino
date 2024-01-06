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
 * 2022-04-13     Meco Man     port to RTduino
 */
/*
  wiring_shift.c - shiftOut() function
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

/**
 * Reads a byte of data from a shift register.
 *
 * @param dataPin The pin connected to the data pin of the shift register.
 * @param clockPin The pin connected to the clock pin of the shift register.
 * @param bitOrder The order in which the bits are shifted in (LSBFIRST or MSBFIRST).
 * @return The byte of data read from the shift register.
 */
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

/**
 * Sends a byte of data to a shift register.
 *
 * @param dataPin The pin connected to the data pin of the shift register.
 * @param clockPin The pin connected to the clock pin of the shift register.
 * @param bitOrder The order in which the bits are shifted out (LSBFIRST or MSBFIRST).
 * @param val The byte of data to be sent to the shift register.
 */
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
    uint8_t i;

    for (i = 0; i < 8; i++)  {
        if (bitOrder == LSBFIRST) {
            digitalWrite(dataPin, val & 1);
            val >>= 1;
        } else {
            digitalWrite(dataPin, (val & 128) != 0);
            val <<= 1;
        }

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}
