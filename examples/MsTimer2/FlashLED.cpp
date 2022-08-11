/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-08-07     Meco Man     port to RTduino
 */
/*
  MsTimer2 is a small and very easy to use library to interface Timer2 with
  humans. It's called MsTimer2 because it "hardcodes" a resolution of 1
  millisecond on timer2
  For Details see: http://www.arduino.cc/playground/Main/MsTimer2
*/

#include <MsTimer2.h>

// Switch on LED on and off each half second
const int led_pin = LED_BUILTIN;

void flash()
{
    static boolean output = HIGH;

    digitalWrite(led_pin, output);
    output = !output;
}

void setup()
{
    pinMode(led_pin, OUTPUT);

    MsTimer2::set(500, flash); // 500ms period
    MsTimer2::start();
}

void loop()
{
    delay(1000);
}
