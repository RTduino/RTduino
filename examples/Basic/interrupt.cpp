/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-09     Meco Man     first version
 */

#include <Arduino.h>

/* When you push the button, LED will on until you release the button. */
/* Please change the LED and button pin number based on your board. */
const byte ledPin = LED_BUILTIN;
const byte interruptPin = D16;

volatile byte state = LOW;

void blink()
{
    state = !state;
}

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop()
{
    digitalWrite(ledPin, state);
}
