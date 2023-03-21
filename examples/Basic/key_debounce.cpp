/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author            Notes
 * 2023-02-03     Stanley Lwin      first version
 */

#include <Arduino.h>

const byte ledPin = LED_BUILTIN;
const byte interruptPin = D16;

volatile byte state = LOW;
volatile int count = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

void ISR__blink()
{
    /*Button debounceDelay*/
    if ((millis() - lastDebounceTime) > debounceDelay || lastDebounceTime == 0)
    {
        state = !state;
        count++;
        Serial.println("count: ");
        Serial.println(count);

        lastDebounceTime = millis();
    }
}

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), ISR__blink, FALLING);

}

void loop()
{
    digitalWrite(ledPin, state);
}
